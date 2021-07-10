/*
 * Macros for timer using gettimeofday()
 */

#ifndef TIMER_H
#define TIMER_H

#include <stdio.h>
#include <sys/time.h>

struct timeval tv1, tv2;

#define TIMER_CLEAR     (tv1.tv_sec = tv1.tv_usec = tv2.tv_sec = tv2.tv_usec = 0)
#define TIMER_START     gettimeofday(&tv1, (struct timezone*)0)
#define TIMER_ELAPSED(s)						\
{									\
    printf("Time elapsed %s: %f (sec)\n", (s), (0.000001*(tv2.tv_usec-tv1.tv_usec)+((tv2.tv_sec-tv1.tv_sec))));							\
}
#define TIMER_STOP      gettimeofday(&tv2, (struct timezone*)0)

#endif /* TIMER_H */
