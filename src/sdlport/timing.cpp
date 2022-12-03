/*
 *  Abuse - dark 2D side-scrolling platform game
 *  Copyright (c) 2001 Anthony Kruize <trandor@labyrinth.net.au>
 *  Copyright (c) 2005-2011 Sam Hocevar <sam@hocevar.net>
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software Foundation,
 *  Inc., 51 Franklin Street, Fifth Floor, Boston MA 02110-1301, USA.
 */

#if defined HAVE_CONFIG_H
#   include "config.h"
#endif

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdint.h>

#ifdef _MSC_VER
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#else
#include <sys/time.h>
#endif

#include "timing.h"

#ifdef __APPLE__
// OSX 10.1 has nanosleep but no header for it!
extern "C" {
int nanosleep(const struct timespec *rqtp, struct timespec *rmtp);
}
#endif

// Constructor
//
time_marker::time_marker()
{
    get_time();
}

//
// get_time()
// Get the current time
//
#ifdef _MSC_VER
void time_marker::get_time()
{
    SYSTEMTIME sys_time;
    FILETIME file_time;

    GetSystemTime(&sys_time);

    SystemTimeToFileTime(&sys_time, &file_time);

    uint64_t nsec10 = (static_cast<uint64_t>(file_time.dwHighDateTime) << 32) + file_time.dwLowDateTime;
    uint64_t usec = nsec10 / 10u;

    seconds = static_cast<long>(usec / 1000000u);
    micro_seconds = usec % 1000000u;
}
#else
void time_marker::get_time()
{
    struct timeval tv = { 0, 0 };
    gettimeofday( &tv, NULL );
    seconds = tv.tv_sec;
    micro_seconds = tv.tv_usec;
}
#endif

//
// diff_time()
// Find the time difference
//
double time_marker::diff_time( time_marker *other )
{
    return (double)(seconds - other->seconds) + (double)(micro_seconds - other->micro_seconds) / 1000000;
}

