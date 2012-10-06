/*
 * helper.h
 *
 *  Created on: 2012-9-7
 *      Author: hujin
 */

#ifndef HELPER_H_
#define HELPER_H_

#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <stdint.h>

enum {
	false,
	true
};

typedef uint8_t bool;
typedef void * pointer;
typedef char * string;


#define m_new(type, c)  calloc(sizeof(type), c)
#define m_new0(type, c) _m_newn(sizeof(type), c, 0)
#define m_newc(type, c, chr) _m_newn(sizeof(type), c, chr)


static inline void * _m_newn(size_t size, size_t count, char c) {
	void * ret = calloc(size, count);
	if(!ret) return NULL;
	memset(ret, c, size);
	return ret;
}


#define realloca(ptr, osize, nsize) ({									\
		void * nptr = alloca(nsize); 									\
		memcpy(nptr, ptr, osize); 										\
		nptr; 															\
	})


#ifdef	__cplusplus
# define BEGIN_DECLS	extern "C" {
# define END_DECLS	}
#else
# define BEGIN_DECLS
# define END_DECLS
#endif


#endif /* HELPER_H_ */
