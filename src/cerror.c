/*
 * cerror.c
 *
 *  Created on: Sep 19, 2012
 *      Author: hujin
 */

#include <stdio.h>
#include <assert.h>
#include "clib/quark.h"
#include "clib/cerror.h"

/**
 * create a new error object .
 * msg : should allocated using malloc family functions.
 */
cerror_t * cerror_new(const char * domain, int code, char * msg) {
	cerror_t * ret = malloc(sizeof(cerror_t));
	ret->domain = quark_form_string(domain);
	ret->code = code;
	if(msg) {
		ret->msg = msg;
	}else {
		ret->msg = NULL;
	}
	return ret;
}


/**
 * set error info to an error object .
 * code : the error code that will be set, if the code < 0, then the code will be ignore.
 * msg : the error message
 */
inline void cerror_set_error(cerror_t * error, int code, char * msg) {
	if(*error->msg) {
		free(*error->msg);
	}
	*error->msg = msg;
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
