#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "myinput.h"

/* Big fat warning:
 * inputeventloadmap() leaks memory
 * or better, is may consume memory that is never free'd.
 * This is no problem if you use it only once at startup time.
 * If you plan to use it regularly in your program, then this must
 * be rewritten.
 * You have been warned!
 */
int inputeventloadmap(const char *file)
{
	FILE *fp;
	char *line = NULL, *tok, *endp;
	size_t linesize = 0;
	int ret, type, code;

	fp = fopen(file, "r");
	if (!fp)
		goto fail_open;
	while (1) {
		ret = getline(&line, &linesize, fp);
		if (ret == 0 || (ret < 0 && feof(fp)))
			break;
		else if (ret < 0)
			goto failed;
		if (*line == '#')
			/* ignore comments */
			continue;
		tok = strtok(line, "\t \r\n\v\f");
		/* try to parse as number */
		code = strtoul(tok, &endp, 0);
		type = EV_KEY;
		if (endp > tok) {
			if (*endp == ':') {
				type = code;
				code = strtoul(endp+1, NULL, 0);
			}
		} else {
			code = strtoinputevent(tok, &type);
			if (code < 0) {
				fprintf(stderr, "input event code '%s' unknown", tok);
				continue;
			}
		}
		inputeventnewname(type, code, strdup(strtok(NULL, "\t \r\n\v\f") ?: ""));
	}
	fclose(fp);
	return 0;

failed:
	if (line)
		free(line);
	fclose(fp);
fail_open:
	return -1;
}
