/*
    Copyright 2020 VUKOZ

    This file is part of 3D Forest.

    3D Forest is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    3D Forest is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with 3D Forest.  If not, see <https://www.gnu.org/licenses/>.
*/

/** @file Time.cpp */

#include <cstdio>

#if defined(_MSC_VER)
    #include <windows.h>
#else
    #include <sys/time.h>
#endif /* _MSC_VER */

#include <time.h>

#include <Time.hpp>

double getRealTime()
{
#if defined(_MSC_VER)
    FILETIME tp;
    GetSystemTimeAsFileTime(&tp);
    uint64_t t = (static_cast<uint64_t>(tp.dwHighDateTime) << 32) + tp.dwLowDateTime;
    return (static_cast<double>(t) * 1e-7) - 11644473600.; // sec 1 Jan 1970
#else
    struct timeval tp;

    (void)::gettimeofday(&tp, NULL);

    return static_cast<double>(tp.tv_sec) +
           (1e-6 * static_cast<double>(tp.tv_usec));
#endif /* _MSC_VER */
}

uint64_t getRealTime64()
{
#if defined(_MSC_VER)
    LARGE_INTEGER fq, t;
    QueryPerformanceFrequency(&fq);
    QueryPerformanceCounter(&t);
    return (1000000UL * t.QuadPart) / fq.QuadPart;
#else
    struct timeval tp;

    (void)::gettimeofday(&tp, NULL);

    return (static_cast<uint64_t>(tp.tv_sec) << 32) +
           static_cast<uint64_t>(tp.tv_usec);
#endif /* _MSC_VER */
}

void msleep(long milliseconds)
{
#if defined(_MSC_VER)
    Sleep(milliseconds);
#else
    struct timespec request;

    request.tv_sec = milliseconds / 1000L;
    request.tv_nsec = (milliseconds % 1000L) * 1000000L;

    (void)::nanosleep(&request, &request);
#endif /* _MSC_VER */
}
