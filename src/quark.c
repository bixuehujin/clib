/*
 * quark.c
 *
 *  Created on: Sep 10, 2012
 *      Author: hujin
 */

#include "clib/quark.h"


struct _quark_intern clib_quark = {
		0,
		0,
		NULL
};


static void quark_init() {
	clib_quark.quarks = calloc(sizeof(pointer), QUARK_DEAULT_BUCKETS);
	clib_quark._alloc = QUARK_DEAULT_BUCKETS;
	clib_quark.len = 0;
}


static void quark_realloc() {
	size_t nsize = clib_quark._alloc + 2;
	clib_quark.quarks = realloc(clib_quark.quarks, nsize * sizeof(pointer));
	clib_quark._alloc = nsize;
}


quark_t quark_from_mem(const pointer * mem, size_t size) {
	if(!quark_is_inited()) {
		quark_init();
	}
	pointer data;
	size_t len = clib_quark.len;
	int first_avaliable = -1;
	int i;

	for(i = 0; i < len; i ++ ) {
		data = clib_quark.quarks[i];
		if(data) {
			if(memcmp(mem, data, size) == 0) {
				return i;
			}
		}else {
			if(first_avaliable == -1) {
				first_avaliable = i;
			}
		}
	}

	if(first_avaliable == -1) {
		first_avaliable = i;
	}

	if(first_avaliable >= clib_quark._alloc) {
		quark_realloc();
	}

	pointer ndata = malloc(size);
	memcpy(ndata, mem, size);
	clib_quark.quarks[first_avaliable] = ndata;


	clib_quark.len ++;

	return first_avaliable;

}


quark_t quark_form_string(const char * str) {
	return quark_from_mem(str, strlen(str) + 1);
}


bool quark_delete(quark_t quark) {
	if(quark >= clib_quark.len) {
		return false;
	}
	pointer data= clib_quark.quarks[quark];
	if(data) {
		free(data);
		clib_quark.quarks[quark] = NULL;

		if(quark + 1 == clib_quark.len ) {
			clib_quark.len --;
		}
		return true;
 	}else {
		return false;
	}
}


pointer quark_get(quark_t quark) {
	if(quark >= clib_quark.len) {
		return NULL;
	}
	pointer data = clib_quark.quarks[quark];

	return data ? data : NULL;
}


char *  quark_get_string(quark_t quark) {
	return (char *)quark_get(quark);
}


void quark_destroy() {
	size_t size = clib_quark.len;
	int i;
	pointer data;

	for(i = 0; i < size; i ++) {
		data = clib_quark.quarks[i];
		if(data) {
			free(data);
		}
	}

	if(clib_quark.quarks) {
		free(clib_quark.quarks);
	}
	clib_quark.len = 0;
	clib_quark._alloc = 0;
}
