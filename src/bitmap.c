/*
 * bitmap.c
 *
 *  Created on: 2012-4-26
 *      Author: hujin
 */
#include <stdio.h>
#include <malloc.h>
#include "clib/bitmap.h"

/**
 * 创建一个 bitmap 对象
 * @param int size 创建 bitmap 的大小
 * @param int start bitmap 存储起始元素
 * @return bitmap_t
 */
bitmap_t * bitmap_new(int size, int start) {
	bitmap_t * ret = malloc(sizeof(bitmap_t));
	if(!ret) return NULL;
	ret->base = start;
	ret->size =  (size >> 3) + 1;
	ret->bitmap = malloc(ret->size * sizeof(char));
	if(!ret->bitmap) {
		free(ret);
		return NULL;
	}
	return ret;
}


int bitmap_set(bitmap_t * map, int index) {
	int quo = (index - map->base) / 8;
	if(quo > map->size) {
		return 0;
	}
	int remainer = (index - map->base) % 8;
	map->bitmap[quo] |= 1 << remainer;
	return 1;
}


int bitmpa_get(bitmap_t * map, int index) {
	int quo = index / 8;
	if(quo > map->size) return 0;
	unsigned char ret = map->bitmap[quo] & (1 << (index % 8));
	return ret > 0 ? 1 : 0;
}


int bitmap_data(bitmap_t * map, int index) {
	return map->base + index;
}


void bitmap_free(bitmap_t * map) {
	free(map->bitmap);
	free(map);
}
