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
#ifndef _GNU_SOURCE
#define _GNU_SOURCE
#endif
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <stdarg.h>
#include <time.h>

#include <unistd.h>
#include <getopt.h>

#define NAME "timeofday"

static const char help_msg[] =
	NAME ": compares a time-of-day with 2 fixed time-of-day's\n"
	"usage: " NAME " LOW HIGH [NOW]\n"
	"\n"
	"Arguments\n"
	" NOW		When NOW is provided, it is the time-of-day under test\n"
	"		Otherwise, the current time-of-day of the system is used\n"
	" LOW, HIGH	The low and high reference points to compare NOW against\n"
	"		When NOW is between LOW & HIGH, the program returns success\n"
	"		When NOW is outside LOW & HIGH, the program 'fails'\n"
	"		When LOW > HIGH, the behaviour is inverted\n"
	"\n"
	"Options\n"
	" -V, --version		Show version\n"
	" -?, --help		Show this help message\n"
	"\n"
	"Examples\n"
	" $ timeofday 7h00 16h30 8h00\n"
	" $ timeofday 16h30 7h00 6h00\n"
	" $ timeofday 16h30 7h00 18h00\n"
	"	all return success\n"
	" $ timeofday 7h00 16h30 6h00\n"
	" $ timeofday 7h00 16h30 18h00\n"
	"	all fail\n"
	;

static struct option long_opts[] = {
	{ "help", no_argument, NULL, '?', },
	{ "version", no_argument, NULL, 'V', },
	{ },
};
static const char optstring[] = "+?V";

static int strtotod(const char *str)
{
	char *endp;
	int result = 0;

	result = strtoul(str, &endp, 10) * 60;
	if (*endp)
		result += strtoul(endp+1, NULL, 10);
	return result;
}

static int nowtod(void)
{
	struct tm tm;
	time_t t;

	time(&t);
	tm = *localtime(&t);
	return tm.tm_hour * 60 + tm.tm_min;
}

int main(int argc, char *argv[])
{
	int opt;
	int lo, hi, now;

	/* parse program options */
	while ((opt = getopt_long(argc, argv, optstring, long_opts, NULL)) != -1)
	switch (opt) {
	case 'V':
		fprintf(stderr, "%s: %s\n", NAME, VERSION);
		return 0;
	default:
		fprintf(stderr, "%s: option '%c' unrecognised\n", NAME, opt);
	case '?':
		fputs(help_msg, stderr);
		exit(1);
		break;
	}

	if ((argc - optind) < 2) {
		fputs(help_msg, stderr);
		exit(1);
	}

	lo = strtotod(argv[optind++]);
	hi = strtotod(argv[optind++]);
	if (optind < argc)
		now = strtotod(argv[optind]);
	else
		now = nowtod();

	if (lo < hi)
		return !((now >= lo) && (now < hi));
	else
		return !((now >= lo) || (now < hi));
}
