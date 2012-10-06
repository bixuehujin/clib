/*
 * event_loop.c
 *
 *  Created on: Oct 6, 2012
 *      Author: hujin
 */

#include <assert.h>
#include "clib/event_loop.h"

event_context_t * event_context_new() {
	event_context_t * ret = malloc(sizeof(event_context_t));
	if(!ret) return NULL;

	ret->source_head = NULL;
	ret->source_tail = NULL;
	ret->nsources = 0;


	return ret;
}

event_loop_t * event_loop_new() {
	event_loop_t * ret = malloc(sizeof(event_loop_t));
	if(!ret) return NULL;
	ret->is_running = false;
	ret->ref_count = 1;
	ret->context = event_context_new();
	if(!ret->context) {
		free(ret);
		return NULL;
	}

	return ret;
}


void event_loop_unref(event_loop_t * loop) {
	assert(loop != NULL);
	assert(loop->ref_count > 0);
	if(loop->ref_count > 0) {
		loop->ref_count --;
		if(loop->ref_count == 0) {
			free(loop->context);
			free(loop);
		}
	}
}


void event_loop_ref(event_loop_t * loop) {
	assert(loop != NULL);
	assert(loop->ref_count > 0);
	loop->ref_count ++;
}

