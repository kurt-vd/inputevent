#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <signal.h>
#include <stdarg.h>

#include <unistd.h>
#include <getopt.h>
#include <fcntl.h>
#include <linux/input.h>

#define NAME "inputevent"

extern int strtoinputevent(const char *str, int *ptype);
extern const char *inputeventtostr(int type, int code);

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
	" -g, --grab		Grab device\n"
	;

static struct option long_opts[] = {
	{ "help", no_argument, NULL, '?', },
	{ "version", no_argument, NULL, 'V', },

	{ "info", no_argument, NULL, 'i', },
	{ "grab", no_argument, NULL, 'g', },
	{ },
};
static const char optstring[] = "+?Vig";

static const char *valuetostr(struct input_event *ev)
{
	static char buf[64];

	sprintf(buf, "%i", ev->value);
	return buf;
}

int main(int argc, char *argv[])
{
	int opt, j, ret;
	int fd;
	int options = 0;
		#define OPT_INFO	1
		#define OPT_GRAB	2
	char *device;
	struct input_event evs[16];

	/* parse program options */
	while ((opt = getopt_long(argc, argv, optstring, long_opts, NULL)) != -1)
	switch (opt) {
	case 'V':
		fprintf(stderr, "%s: %s\n", NAME, VERSION);
		return 0;
	case 'i':
		options |= OPT_INFO;
		break;
	case 'g':
		options |= OPT_GRAB;
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

	while (1) {
		ret = read(fd, evs, sizeof(evs));
		if (ret < 0)
			elog(1, errno, "read input");
		for (j = 0; j < ret/sizeof(*evs); ++j) {
			printf("%lu.%06lu %s %s\n",
				evs[j].time.tv_sec, evs[j].time.tv_usec,
				inputeventtostr(evs[j].type, evs[j].code),
				valuetostr(&evs[j]));
		}
		fflush(stdout);
	}
	return 0;
}
