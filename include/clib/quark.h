/*
 * quark.h
 *
 *  Created on: Sep 10, 2012
 *      Author: hujin
 */

#ifndef QUARK_H_
#define QUARK_H_

#include <sys/types.h>
#include <clib/helper.h>

typedef ulong quark_t;

struct _quark_intern {
	size_t len;
	size_t _alloc;
	pointer * quarks;
};

#define QUARK_DEAULT_BUCKETS	100

typedef struct _quark_intern quark_intern_t;
extern  quark_intern_t clib_quark;

#define quark_is_inited() (clib_quark.quarks)

quark_t quark_from_mem(const pointer * mem, size_t size);
quark_t quark_form_string(const char * str);
bool quark_delete(quark_t quark);
pointer quark_get(quark_t quark);
char *  quark_get_string(quark_t quark);
void quark_destroy();


#endif /* QUARK_H_ */
