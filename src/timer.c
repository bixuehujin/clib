/*
 * timer.c
 *
 *  Created on: Sep 16, 2012
 *      Author: hujin
 */

#include <stdio.h>
#include <assert.h>
#include <sys/time.h>
#include <sys/types.h>

#include "clib/timer.h"


ctimer_t * ctimer_new() {
	ctimer_t *ret = malloc(sizeof(ctimer_t));
	assert(ret != NULL);
	ret->status = TIMER_STOPPED;
	ret->ttime = 0;
	ret->lttime = 0;
	return ret;
}


void ctimer_free(ctimer_t * timer) {
	assert(timer != NULL);
	free(timer);
}


ctimer_t * ctimer_init(ctimer_t * timer) {
	timer->status = TIMER_STOPPED;
	timer->lttime = 0;
	timer->ttime = 0;
	return timer;
}


void ctimer_start(ctimer_t * timer) {
	if(timer->status != TIMER_RUNNING) {
		struct timeval tm;
		gettimeofday(&tm, NULL);
		timer->_lms = tm.tv_sec * 1000 + tm.tv_usec / 1000;

		timer->status = TIMER_RUNNING;
	}
}


void ctimer_stop(ctimer_t * timer) {
	if(timer->status == TIMER_STOPPED) {
		return;
	}

	ulong now;
	struct timeval tm ;
	gettimeofday(&tm, NULL);
	now =  tm.tv_sec * 1000 + tm.tv_usec / 1000;

	timer->lttime = now - timer->_lms;
	timer->ttime += timer->lttime;
}


void ctimer_clean(ctimer_t * timer) {
	timer->_lms = 0;
	timer->lttime = 0;
	timer->status = TIMER_STOPPED;
	timer->ttime = 0;
}


ulong ctimer_last_runtime(ctimer_t * timer) {
	return timer->lttime;
}


ulong ctimer_total_runtime(ctimer_t * timer) {
	return timer->ttime;
}

