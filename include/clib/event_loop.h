/*
 * event_loop.h
 *
 *  Created on: Oct 6, 2012
 *      Author: hujin
 */

#ifndef EVENT_LOOP_H_
#define EVENT_LOOP_H_

#include <stdio.h>
#include <stdlib.h>
#include "clib/helper.h"

BEGIN_DECLS

typedef struct _event_source  event_source_t;
typedef struct _event_context event_context_t;
typedef struct _event_loop    event_loop_t;

typedef bool (*event_source_func)(pointer data);

typedef struct _event_source_funcs {
	bool (*prepare) (event_source_t * source, int timeout);
	bool (*check) (event_source_t * source);
	bool (*dispatch)(event_source_t * source, event_source_func callback);
	void (*finalize)(event_source_t * source);
}event_source_funcs;


struct _event_source {
	uint ref_count;
	char * name;
	event_source_funcs * source_funcs;
	struct _event_source * prev;
	struct _event_source * next;
	event_context_t * context;
};

struct _event_context{
	event_source_t * source_head;
	event_source_t * source_tail;
	uint nsources;
};

struct _event_loop{
	bool is_running;
	uint ref_count;
	event_context_t * context;
};


/* event loop functions */
event_loop_t * event_loop_new();
void event_loop_unref(event_loop_t * loop);
void event_loop_ref(event_loop_t * loop);
event_context_t * event_loop_get_context(event_loop_t * loop);


/* event context functions */
event_context_t * event_context_new();
void event_context_unref(event_context_t * context);
void event_context_ref(event_context_t * context);


/* event source functions */
event_source_t * event_source_new(size_t source_size);
void event_source_unref(event_source_t * source);
void event_source_ref(event_source_t * source);
void event_source_attach(event_source_t * source, event_context_t * context);
void event_source_detach(event_source_t * source, event_context_t * context);

END_DECLS

#endif /* EVENT_LOOP_H_ */
