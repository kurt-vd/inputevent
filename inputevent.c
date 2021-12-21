/*
 * Copyright 2016 Kurt Van Dijck <dev.kurt@vandijck-laurijssen.be>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <math.h>
#include <signal.h>
#include <stdarg.h>
#include <time.h>

#include <unistd.h>
#include <getopt.h>
#include <fcntl.h>
#include <poll.h>

#include "myinput.h"
#include "lib/libt.h"

#define NAME "inputevent"

/* non-daemon syslog */
static void elog(int exitcode, int errnum, const char *fmt, ...)
{
	va_list va;
	static char str[1024];
	int ret;

	ret = sprintf(str, "%s: ", NAME);
	va_start(va, fmt);
	ret += vsprintf(str+ret, fmt, va);
	va_end(va);
	if (errnum) {
		if (str[ret-1] == '\n')
			--ret;

		ret += sprintf(str+ret, ": %s\n", strerror(errnum));
	}
	fputs(str, stderr);
	fflush(stderr);
	if (exitcode)
		exit(exitcode);
}

static const char help_msg[] =
	NAME ": inspect input event devices\n"
	"usage: " NAME " [OPTIONS ...] DEVICE [CMD ARGS]\n"
	"     :	" NAME " [OPTIONS ...] -i DEVICE\n"
	"\n"
	"Options\n"
	" -V, --version		Show version\n"
	" -?, --help		Show this help message\n"
	" -i, --info		Show info & exit\n"
	" -0, --initial		Fetch initial state\n"
	"			Twice will exit after the initial state\n"
	" -z, --zero		Print zero's on initial state\n"
	" -g, --grab		Grab device\n"
	" -l, --long[=TIME]	Detect additionally long or short keypresses\n"
	"			This is for EV_KEY events only.\n"
	"			TIME defaults to 0.25 sec\n"
	" -m, --map=FILE	Map names from FILE\n"
	"			FILE contains 'OLD NEW' pairs\n"
	" -n, --numeric		Show numeric codes\n"
	;

static struct option long_opts[] = {
	{ "help", no_argument, NULL, '?', },
	{ "version", no_argument, NULL, 'V', },

	{ "info", no_argument, NULL, 'i', },
	{ "initial", no_argument, NULL, '0', },
	{ "zero", no_argument, NULL, 'z', },
	{ "grab", no_argument, NULL, 'g', },
	{ "long", optional_argument, NULL, 'l', },
	{ "map", required_argument, NULL, 'm', },
	{ "numeric", no_argument, NULL, 'n', },
	{ },
};
static const char optstring[] = "+?Vi0zgl::m:n";

static int options = 0;
	#define OPT_INFO	1
	#define OPT_GRAB	2
	#define OPT_LONG	4
	#define OPT_INITIAL	0x08
	#define OPT_QUIT	0x10
	#define OPT_ZERO	0x20
	#define OPT_NUM		0x40

/* time cache */
static double dtlong = 0.25;
static struct timeval tlong = { .tv_sec = 0, .tv_usec = 250000, };
static struct timeval *keytimes;

static void keytimeout(void *ptr)
{
	long keycode = (long)ptr;
	struct timeval tv;

	timeradd(&keytimes[keycode], &tlong, &tv);
	printf("%lu.%06lu %s long\n",
		tv.tv_sec, tv.tv_usec,
		inputeventtostr(EV_KEY, keycode, options & OPT_NUM));
	/* flush to stdout since we're about to enter the (poll) wait loop */
	fflush(stdout);
}

struct arm_input_event {
	uint32_t secs;
	uint32_t usecs;
	uint16_t type;
	uint16_t code;
	int32_t value;
};
int main(int argc, char *argv[])
{
	int opt, j, ret;
	int fd;
	char *device;
	struct pollfd pollfd = { .events = POLLIN, };
	struct arm_input_event evs[16];

	/* parse program options */
	while ((opt = getopt_long(argc, argv, optstring, long_opts, NULL)) != -1)
	switch (opt) {
	case 'V':
		fprintf(stderr, "%s: %s\n", NAME, VERSION);
		return 0;
	case 'i':
		options |= OPT_INFO;
		break;
	case '0':
		if (options & OPT_INITIAL)
			options |= OPT_QUIT;
		options |= OPT_INITIAL;
		break;
	case 'z':
		options |= OPT_ZERO;
		break;
	case 'g':
		options |= OPT_GRAB;
		break;
	case 'l':
		options |= OPT_LONG;
		if (optarg) {
			double dtime = strtod(optarg, NULL);
			if (dtime > 0.001) {
				tlong.tv_sec = lround(floor(dtime));
				tlong.tv_usec = lround(dtime * 1e6) % 1000000;
				dtlong = dtime;
			}
		}
		break;
	case 'm':
		if (inputeventloadmap(optarg) < 0)
			elog(1, errno, "load map from '%s'", optarg);
		break;
	case 'n':
		options |= OPT_NUM;
		break;
	default:
		fprintf(stderr, "%s: option '%c' unrecognised\n", NAME, opt);
	case '?':
		fputs(help_msg, stderr);
		exit(1);
	}

	device = argv[optind];
	if (!device) {
		fprintf(stderr, "No device given\n");
		fputs(help_msg, stderr);
		exit(1);
	}

	if (options & OPT_LONG) {
		keytimes = malloc(sizeof(*keytimes) * KEY_CNT);
		if (!keytimes)
			elog(1, errno, "malloc keycache\n");
	}

	fd = open(device, O_RDONLY);
	if (fd < 0)
		elog(1, errno, "open %s\n", device);

	/* show info when requested */
	if (options & OPT_INFO) {
		char name[64];
		char phys[64];
		struct input_id id;

		if (ioctl(fd, EVIOCGID, &id) < 0)
			elog(1, errno, "ioctl %s EVIOCGID\n", device);
		if (ioctl(fd, EVIOCGNAME(sizeof(name)), name) < 0)
			elog(1, errno, "ioctl %s EVIOCGNAME\n", device);
		if (ioctl(fd, EVIOCGPHYS(sizeof(phys)), phys) < 0)
			elog(1, errno, "ioctl %s EVIOCGPHYS\n", device);

		printf("### %s\n", device);
		printf("name: %s\n", name);
		printf("phys: %s\n", phys);
		printf("bus: %u\n", id.bustype);
		printf("vendor: 0x%04x\n", id.vendor);
		printf("product: 0x%04x\n", id.product);
		printf("version: %u\n", id.version);
		return 0;
	}

	if (options & OPT_GRAB) {
		int value = 1;

		if (ioctl(fd, EVIOCGRAB, &value) < 0)
			elog(1, errno, "Grab %s failed\n", device);
	}

	if (argv[++optind]) {
		/* fork child process */
		int pp[2];

		if (pipe(pp) < 0)
			elog(1, errno, "pipe() failed\n");
		ret = fork();
		if (ret < 0)
			elog(1, errno, "fork() failed\n");
		else if (!ret) {
			dup2(pp[0], STDIN_FILENO);
			close(pp[0]);
			close(pp[1]);
			close(fd);
			execvp(argv[optind], argv+optind);
			elog(1, errno, "execvp %s ...", argv[optind]);
		}
		dup2(pp[1], STDOUT_FILENO);
		close(pp[0]);
		close(pp[1]);
	}

	if (options & OPT_INITIAL) {
		char state[KEY_CNT/8+1]; /* current state */
		char wired[KEY_CNT/8+1]; /* possible state */
		int j;

#define getbit(x, vec)	((vec[(x)/8] >> ((x) % 8)) & 1)

		if (ioctl(fd, EVIOCGBIT(EV_KEY, sizeof(wired)), wired) < 0)
			elog(1, errno, "ioctl %s EVIOCGBIT\n", device);
		if (ioctl(fd, EVIOCGKEY(sizeof(state)), state) < 0)
			elog(1, errno, "ioctl %s EVIOCGKEY\n", device);

		for (j = 0; j < KEY_CNT; ++j) {
			if (getbit(j, wired) && (getbit(j, state) || (options & OPT_ZERO)))
				printf("i %s %i\n", inputeventtostr(EV_KEY, j, options & OPT_NUM),
						getbit(j, state));
		}

		/* similar for EV_SW */
		if (ioctl(fd, EVIOCGBIT(EV_SW, sizeof(wired)), wired) < 0)
			elog(1, errno, "ioctl %s EVIOCGBIT\n", device);
		if (ioctl(fd, EVIOCGSW(sizeof(state)), state) < 0)
			elog(1, errno, "ioctl %s EVIOCGKEY\n", device);

		for (j = 0; j < SW_CNT; ++j) {
			if (getbit(j, wired) && (getbit(j, state) || (options & OPT_ZERO)))
				printf("i %s %i\n", inputeventtostr(EV_SW, j, options & OPT_NUM),
						getbit(j, state));
		}
		if (options & OPT_QUIT)
			return 0;
		fflush(stdout);
	}

	/* prepare main loop */
	pollfd.fd = fd;

	while (1) {
		libt_flush();
		ret = poll(&pollfd, 1, libt_get_waittime());
		if (ret < 0)
			elog(1, errno, "poll");
		if (!ret)
			continue;

		ret = read(fd, evs, sizeof(evs));
		if (ret < 0)
			elog(1, errno, "read input");
		for (j = 0; j < ret/sizeof(*evs); ++j) {
			if ((options & OPT_LONG) && (evs[j].type == EV_KEY) &&
					!evs[j].value && !libt_timeout_exist(keytimeout, (void *)(long)evs[j].code))
				/* long press detection timeout has already passed */
				continue;
			if ((evs[j].type == EV_KEY) && keytimes && (evs[j].code < KEY_CNT)) {
				/* do longpress detection */
				if (evs[j].value == 1) {
					keytimes[evs[j].code].tv_sec = evs[j].secs;
					keytimes[evs[j].code].tv_usec = evs[j].usecs;
					libt_add_timeout(dtlong, keytimeout, (void *)(long)evs[j].code);
				} else if (!evs[j].value && libt_timeout_exist(keytimeout, (void *)(long)evs[j].code))
					/* no long press detected yet */
					libt_remove_timeout(keytimeout, (void *)(long)evs[j].code);
				else
					/* long press detected, or autorepeat */
					continue;
			}
			printf("%lu.%06lu %s %i\n",
				evs[j].secs, evs[j].usecs,
				inputeventtostr(evs[j].type, evs[j].code, options & OPT_NUM),
				evs[j].value);
		}
		fflush(stdout);
	}
	return 0;
}
