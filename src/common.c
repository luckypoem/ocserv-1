/*
 * Copyright (C) 2013 Nikos Mavrogiannopoulos
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 */

#include <config.h>
#include <stdint.h>
#include <errno.h>
#include <unistd.h>
#include <string.h>
#include <vpn.h>
#include <sys/socket.h>

ssize_t force_write(int sockfd, const void *buf, size_t len)
{
int left = len;
int ret;
const uint8_t * p = buf;

	while(left > 0) {
		ret = write(sockfd, p, left);
		if (ret == -1) {
			if (errno != EAGAIN && errno != EINTR)
				return ret;
		}
		
		if (ret > 0) {
			left -= ret;
			p += ret;
		}
	}
	
	return len;
}

ssize_t force_read(int sockfd, void *buf, size_t len)
{
int left = len;
int ret;
uint8_t * p = buf;

	while(left > 0) {
		ret = read(sockfd, p, left);
		if (ret == -1) {
			if (errno != EAGAIN && errno != EINTR)
				return ret;
		}
		
		if (ret > 0) {
			left -= ret;
			p += ret;
		}
	}
	
	return len;
}

int ip_cmp(const struct sockaddr_storage *s1, const struct sockaddr_storage *s2, size_t n)
{
	if (((struct sockaddr*)s1)->sa_family == AF_INET) {
		return memcmp(SA_IN_P(s1), SA_IN_P(s2), sizeof(struct in_addr));
	} else { /* inet6 */
		return memcmp(SA_IN6_P(s1), SA_IN6_P(s2), sizeof(struct in6_addr));
	}
}
