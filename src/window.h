/* vi:set ts=4 sts=4 sw=4 noet:
 *
 * Practical Music Search
 * Copyright (c) 2006-2013 Kim Tore Jensen
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

/**
 * Shared properties in a window.
 */
typedef struct {

	/* Scroll position - top item */
	long position;

	/* Cursor position */
	long cursor;

	/* Height in lines */
	int height;

    /* Number of lines in list */
    int num_lines;

    /* Ncurses WINDOW */
    WINDOW * window;

} window_t;

/**
 * Scroll console up or down. A positive integer means move content that amount
 * of lines upwards, negative integers move content downwards.
 *
 * @return amount of lines that need to be drawn. If amount is positive, draw
 * that many lines from top. If amount is negative, draw that many lines from
 * bottom.
 */
int window_scroll(window_t * window, long delta);
