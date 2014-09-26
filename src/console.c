/* vi:set ts=4 sts=4 sw=4 noet:
 *
 * Practical Music Search
 * Copyright (c) 2006-2014 Kim Tore Jensen
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

#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdarg.h>
#include <sys/time.h>
#include "pms.h"

extern WINDOW * window_main;

static logline_t ** lines = NULL;
static unsigned int line_cursor = 0;
static unsigned int line_limit = 0;
static unsigned int first_line = 0;
static int full = 0;

pthread_mutex_t console_mutex = PTHREAD_MUTEX_INITIALIZER;

window_t * console_window = NULL;

void console_init(unsigned int max_lines) {
	if (lines != NULL) {
		lines = realloc(lines, max_lines * sizeof(logline_t *));
	} else {
		lines = malloc(max_lines * sizeof(logline_t *));
	}
	if (lines == NULL) {
		fatal(PMS_EXIT_MEMORY, "out of memory\n");
	}
	if (console_window == NULL) {
		console_window = malloc(sizeof(window_t));
		if (console_window == NULL) {
			fatal(PMS_EXIT_MEMORY, "out of memory\n");
		}
		// TODO: move into resize func
		console_window->height = LINES - 2;
		console_window->position = 0;
		console_window->num_lines = 0;
	}
	line_limit = max_lines;
	memset(lines, 0, max_lines * sizeof(logline_t *));
}

logline_t * console_get_line(unsigned int n) {
	n = (n + first_line) % line_limit;
	return lines[n];
}

void console(const char * format, ...) {
	logline_t *	line;
	time_t		t;
	int			changed;
	va_list		ap;
	char *		buffer;
	char *		existing;

	pthread_mutex_lock(&console_mutex);

	line = malloc(sizeof(logline_t));
	line->str = malloc(512);
	line->ts = malloc(9);

	if (line->str == NULL || line->ts == NULL) {
		fatal(PMS_EXIT_MEMORY, "Out of memory");
	}

	va_start(ap, format);
	vsnprintf(line->str, 512, format, ap);
	va_end(ap);

	if (lines[line_cursor] != NULL) {
		free_logline(lines[line_cursor]);
	}

	if (full) {
		first_line += 1;
		if (first_line >= line_limit) {
			first_line = 0;
		}
	} else {
		++console_window->num_lines;
	}

	t = time(NULL);
	localtime_r(&t, &line->timestamp);
	strftime(line->ts, 9, "%H:%M:%S", &line->timestamp);

	lines[line_cursor] = line;

	if (++line_cursor >= line_limit) {
		line_cursor = 0;
		full = 1;
	}

	/* Scroll window if at bottom. */
	if (console_window->num_lines < console_window->height || console_window->position + console_window->height + 1 >= console_window->num_lines) {
		console_scroll(1);
	}

	pthread_mutex_unlock(&console_mutex);
}

void console_draw_lines(long start, long end) {

	long s;
	long ptr;
	logline_t * line;

    pms_curses_lock();

	ptr = console_window->position + start;
	for (s = start; s <= end; s++) {
		if (ptr >= console_window->num_lines) {
			break;
		}
		line = console_get_line(ptr);
		mvwprintw(window_main, s, 0, "%s: %s", line->ts, line->str);
		++ptr;
	}

	wrefresh(window_main);

    pms_curses_unlock();
}

int console_scroll(long delta) {

	int changed = window_scroll(console_window, delta);

	if (changed > 0) {
		console_draw_lines(console_window->height - changed, console_window->height);
	} else if (changed < 0) {
		console_draw_lines(0, -changed - 1);
	} else {
		console_draw_lines(console_window->num_lines - 1, console_window->num_lines - 1);
	}

	return changed;
}

void free_logline(logline_t * line) {
	free(line->str);
	free(line->ts);
}
