/*
 * sort.c
 *
 *  Created on: Sep 14, 2012
 *      Author: hujin
 */

#include <clib/sort.h>


void sort_swap(void *a, void * b, size_t siz) {

	register int * char_a;
	register int * char_b;

	register int * int_a;
	register int * int_b;

	int t_i;
	char t_c;
	size_t i;

	int_a = (int *)a;
	int_b = (int *)b;

	for(i = 0; i < siz; i += sizeof(int)) {
		t_i = *int_a;
		*(int_a ++) = *int_b;
		*(int_b ++) = t_i;
	}

	char_a = (char *)int_a;
	char_b = (char *)int_b;

	for(i = i - sizeof(int); i < siz; ++i) {
		t_c = *char_a;
		*char_a ++ = *char_b;
		*char_b ++ = t_c;
	}
}


