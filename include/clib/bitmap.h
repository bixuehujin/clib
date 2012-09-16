/*
 * bitmap.h
 *
 *  Created on: 2012-4-26
 *      Author: hujin
 */

#ifndef BITMAP_H_
#define BITMAP_H_

typedef struct _bitmap{
	unsigned char * bitmap;
	int size;
	int base;
}bitmap_t;

bitmap_t * bitmap_new(int size, int start);
int bitmap_set(bitmap_t * map, int index);
int bitmpa_get(bitmap_t * map, int index);
int bitmap_data(bitmap_t * map, int index);
void bitmap_free(bitmap_t * map);

#endif /* BITMAP_H_ */
