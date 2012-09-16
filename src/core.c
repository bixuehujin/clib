/*
 * core.c
 *
 *  Created on: Sep 10, 2012
 *      Author: hujin
 */

#include "clib/helper.h"
#include "clib/quark.h"

bool clib_global_inited = false;


static void exit_cb(int status, pointer data) {
	if(quark_is_inited()) {
		quark_destroy();
	}
}


void clib_init() {
	clib_global_inited = true;
	on_exit(exit_cb, NULL);
}


bool clib_is_inited() {
	return clib_global_inited;
}


