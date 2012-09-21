/*
 * sstring.c
 *
 *  Created on: 2012-9-7
 *      Author: hujin
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#include <assert.h>
#include "clib/helper.h"
#include "clib/sstring.h"


#define sstring_alloc(pss) 													\
		if((pss)->ptr == NULL) { 											\
			(pss)->ptr = (pss)->_use_stack ? 								\
				alloca((pss)->_alloc) : calloc((pss)->_alloc, 1);			\
		}


#define sstring_realloc(pss, nsize) ({										\
		typeof(nsize) __nsize = (nsize); 									\
		void * ptr = (pss)->_use_stack ? 									\
			realloca((pss)->ptr, (pss)->_alloc, nsize) : 					\
				realloc((pss)->ptr, __nsize); 								\
		(pss)->_alloc = __nsize; 											\
		ptr;																\
	})


sstring_t * sstring_new(size_t size) {
	sstring_t * ss = m_new(sstring_t, 1);
	assert(ss != NULL);
	ss->len = 0;
	ss->_alloc = size;
	ss->_use_stack = 0;
	ss->ptr = NULL;
	return ss;
}


sstring_t * sstring_new_stack(size_t size) {
	sstring_t * ss = alloca(sizeof(sstring_t));
	assert(ss != NULL);
	ss->len = 0;
	ss->_alloc = size;
	ss->_use_stack = 1;
	return ss;
}


void sstring_init(sstring_t * ss, size_t size) {
	(*ss).len = 0;
	(*ss)._alloc = size;
	(*ss).ptr = NULL;
	(*ss)._use_stack = 0;
}


void sstring_init_stack(sstring_t * ss, size_t size) {
	(*ss).len = 0;
	(*ss)._alloc = size;
	(*ss).ptr = NULL;
	(*ss)._use_stack = 1;
}


void sstring_destroy(sstring_t *ss) {
	assert(ss);
	if(ss->ptr && !ss->_use_stack) {
		free(ss->ptr);
	}
}


void sstring_free(sstring_t * ss) {
	assert(ss);
	if(ss->ptr && !ss->_use_stack) {
		free(ss->ptr);
	}
	free(ss);
}


/**
 *
 */
sstring_t * sstring_appendl(sstring_t *ss, const char * str, size_t len) {
	sstring_alloc(ss);

	size_t ss_len = ss->len;
	char * ptr = ss->ptr;
	char * nptr = ptr;
	char * p;

	if(ss->_alloc < ss_len + len + 1) {
		int c = (len + ss_len) / ss->_alloc + 1;

		nptr = sstring_realloc(ss, ss->_alloc * c);
		if(!nptr) return NULL;

		ss->ptr = nptr;
	}

	p = nptr + ss_len;

	memcpy(p, str, len);

	ss->len += len;
	ss->ptr[ss->len] = 0;

	return ss;
}


sstring_t * sstring_append(sstring_t * ss, const char * str) {
	size_t len = strlen(str);
	return sstring_appendl(ss, str, len);
}


sstring_t * sstring_appends(sstring_t * ss, sstring_t ass) {
	return sstring_appendl(ss, ass.ptr, ass.len);
}


/**
 * sstring format append. just like sprintf(), using vsnprintf() internal .
 */
sstring_t * sstring_fappend(sstring_t * ss, const char * format, ...) {
	sstring_alloc(ss);

	int n;
	size_t ss_len = ss->len;
	int buffsize = ss->_alloc - ss_len;

	va_list va;
	va_start(va, format);
	n = vsnprintf(ss->ptr + ss_len, buffsize, format, va);
	va_end(va);

	if(n < 0) return NULL;

	if(n >= buffsize) { //memory truncated , do realloc.
		int c = (n + ss_len) / ss->_alloc + 1;
		char * nptr = NULL;

		nptr = sstring_realloc(ss, ss->_alloc * c);

		if(!nptr) return NULL;

		ss->ptr = nptr;
	}
	buffsize = ss->_alloc - ss_len;

	va_start(va, format);
	n = vsnprintf(ss->ptr + ss_len, buffsize, format, va);
	va_end(va);
	if(n < 0) return NULL;

	ss->len += n;
	return ss;
}


sstring_t * sstring_vappend(sstring_t * ss, const char * format, va_list va) {
	sstring_alloc(ss);
	va_list orignal_va;
	va_copy(orignal_va, va);

	int n;
	size_t ss_len = ss->len;
	int buffsize = ss->_alloc - ss_len;

	n = vsnprintf(ss->ptr + ss_len, buffsize, format, va);

	if(n < 0) return NULL;

	if(n >= buffsize) { //memory truncated , do realloc.
		int c = (n + ss_len) / ss->_alloc + 1;
		char * nptr = NULL;

		nptr = sstring_realloc(ss, ss->_alloc * c);

		if(!nptr) return NULL;

		ss->ptr = nptr;
	}
	buffsize = ss->_alloc - ss_len;



	n = vsnprintf(ss->ptr + ss_len, buffsize, format, orignal_va);
	if(n < 0) return NULL;

	ss->len += n;
	return ss;
}


sstring_t * sstring_appendc(sstring_t * ss, char c) {
	assert(ss != NULL);
	sstring_alloc(ss);

	if(ss->len + 1 >= ss->_alloc) {
		sstring_realloc(ss, ss->_alloc * 2);
	}
	ss->ptr[ss->len] = c;
	ss->ptr[++ss->len] = 0;
	return ss;
}


void sstring_clear(sstring_t * ss) {
	assert(ss != NULL);
	ss->len = 0;
	if (ss->ptr) {
		ss->ptr[0] = '\0';
	}
}
