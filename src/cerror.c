/*
 * cerror.c
 *
 *  Created on: Sep 19, 2012
 *      Author: hujin
 */

#include <stdio.h>
#include <assert.h>
#include <stdarg.h>
#include "clib/quark.h"
#include "clib/cerror.h"
#include "clib/sstring.h"

/**
 * create a new error object .
 */
cerror_t * cerror_new(const char * domain, int code, char * format, ...) {
	cerror_t * ret = malloc(sizeof(cerror_t));
	ret->domain = quark_form_string(domain);
	ret->code = code;
	sstring_t ss = sstring_for_init;
	va_list va;

	if(format) {
		va_start(va, format);
		sstring_vappend(&ss, format, va);
		va_end(va);
		ret->msg = ss.ptr;
	}else {
		ret->msg = NULL;
	}
	return ret;
}


/**
 * set error info to an error object .
 * code : the error code that will be set, if the code <= 0, then the code will be ignore.
 * format : format of the message
 * ... : additional arguments
 */
void cerror_set_error(cerror_t * error, int code, char * format, ...) {
	if(code > 0) {
		error->code = code;
	}

	if(*error->msg) {
		free(*error->msg);
	}
	sstring_t ss = sstring_for_init;
	va_list va;
	va_start(&va, format);
	sstring_vappend(&ss, format, va);
	va_end(&va);
	*error->msg = ss.ptr;
}


void cerror_print(cerror_t * error) {
	char * quark = quark_get_string(error->domain);
	fprintf(stderr, "[%s: %d] %s\n", quark, error->code, error->msg);
}


void cerror_free(cerror_t ** error) {
	cerror_t * err = *error;
	assert(error != NULL);
	free(err->msg);
	free(err);
	*error = NULL;
}
