/*
 * timer.h
 *
 *  Created on: Sep 16, 2012
 *      Author: hujin
 */

#ifndef TIMER_H_
#define TIMER_H_

enum {
	TIMER_STOPPED,
	TIMER_RUNNING,
};

typedef struct _ctimer {
	int status;
	ulong ttime;
	ulong lttime;
	ulong _lms;
}ctimer_t;



ctimer_t * ctimer_new();
void ctimer_free(ctimer_t * timer);
ctimer_t * ctimer_init(ctimer_t * timer);

void ctimer_start(ctimer_t * timer);
void ctimer_stop(ctimer_t * timer);
void ctimer_clean(ctimer_t * timer);

ulong ctimer_last_runtime(ctimer_t * timer);
ulong ctimer_total_runtime(ctimer_t * timer);

#endif /* TIMER_H_ */
