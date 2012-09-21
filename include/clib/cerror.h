/*
 * cerror.h
 *
 *  Created on: Sep 10, 2012
 *      Author: hujin
 */

#ifndef CERROR_H_
#define CERROR_H_

#include <sys/types.h>

typedef struct _cerror{
	ulong domain;
	int code;
	char * msg;
}cerror_t;

cerror_t * cerror_new(const char * domain, int code, char * format, ...);
void cerror_set_error(cerror_t * error, int code, char * format, ...);
void cerror_print(cerror_t * error);
void cerror_fprint(cerror_t * error, int fd);
void cerror_free(cerror_t ** error);

#endif /* CERROR_H_ */
