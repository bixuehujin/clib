/*
 * cerror.h
 *
 *  Created on: Sep 10, 2012
 *      Author: hujin
 */

#ifndef CERROR_H_
#define CERROR_H_

typedef struct _cerror{
	int code;
	char * msg;
}cerror_t;

cerror_t * cerror_new();
void cerror_free(cerror_t * error);

#endif /* CERROR_H_ */
