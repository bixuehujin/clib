/*
 * test.c
 *
 *  Created on: 2012-9-7
 *      Author: hujin
 */



#include <clib.h>

struct swap_struct {
	int a;
	char b;
};

int main(int argc, char **argv) {
	struct swap_struct x = {12, 'c'};
	struct swap_struct y = {21, 'd'};

	printf("x:%d%c y:%d%c\n", x.a, x.b, y.a, y.b);
	sort_swap(&x, &y, sizeof(char));
	printf("x:%d%c y:%d%c\n", x.a, x.b, y.a, y.b);

	return 0;
}

