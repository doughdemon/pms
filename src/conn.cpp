/* vi:set noet ts=8 sts=8 sw=8:
 *
 * PMS  <<Practical Music Search>>
 * Copyright (C) 2006-2015  Kim Tore Jensen
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
 *
 *
 * conn.cpp
 * 	connection handle to MPD
 */

#include <mpd/client.h>

#include "conn.h"
#include "pms.h"

extern Pms *	pms;


Connection::Connection(string n_hostname, long n_port, long n_timeout)
{
	this->host = n_hostname;
	this->port = static_cast<unsigned int>(n_port);
	this->timeout = static_cast<int>(n_timeout);
	this->handle = NULL;
	this->fd = -1;
}

Connection::~Connection()
{
	this->disconnect();
}

/*
 * Returns connection state
 */
bool
Connection::connected()
{
	if (handle == NULL) {
		return false;
	}

	return (mpd_connection_get_error(handle) == MPD_ERROR_SUCCESS);
}

/**
 * Clear any non-fatal error
 */
bool
Connection::clear_error()
{
	return mpd_connection_clear_error(handle);
}

int Connection::connect()
{
	enum mpd_error err;

	disconnect();

	pms->log(MSG_DEBUG, 0, "Connecting to %s:%d, handle=%p...\n", host.c_str(), port, handle);

	if ((handle = mpd_connection_new(host.c_str(), port, timeout)) == NULL) {
		fprintf(stderr, "Out of memory\n");
		abort();
	}

	if ((err = mpd_connection_get_error(handle)) == MPD_ERROR_SUCCESS) {
		fd = mpd_connection_get_fd(handle);
	}

	pms->log(MSG_DEBUG, 0, "New connection handle is %p, error %d\n", handle, err);

	return err;
}

/**
 * Disconnect from the MPD server.
 *
 * Returns true if a connection was dropped, false otherwise.
 */
bool
Connection::disconnect()
{
	if (handle == NULL) {
		return false;
	}

	pms->log(MSG_DEBUG, 0, "Closing connection to MPD server.\n");
	mpd_connection_free(handle);
	handle = NULL;
	fd = -1;
	pms->comm->set_is_idle(false);

	return false;
}

/**
 * Return the file descriptor to the MPD socket.
 */
int
Connection::get_mpd_file_descriptor()
{
	return fd;
}
