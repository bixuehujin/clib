/*
 * hash_table.c
 *
 *  Created on: 2012-5-8
 *      Author: hujin
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <assert.h>
#include "clib/hash_table.h"



ulong hash_func(string key, uint key_len) {
	ulong hash = 31;
	int i;
	for(i=0;key[i] != 0; i ++) {
		hash = hash + key[i] * 33;
	}
	return hash;
}


hash_table_t * hash_table_new(int size, hash_table_dtor_func_t dtor) {
	hash_table_t * ht = malloc(sizeof(hash_table_t));
	if(!ht) return NULL;

	ht->dtor = dtor;

	ht->size = 0;
	ht->bucket_size = size;
	ht->curr = NULL;
	ht->head = NULL;
	ht->tail = NULL;
	ht->bucket = calloc(sizeof(hash_bucket_t *) * ht->bucket_size, 1);
	return ht;
}


bool hash_table_sized_insert(hash_table_t * ht, string key, int key_size, pointer data){
	ulong idx = hash_func(key, key_size);
	ulong bidx = idx % (ht->bucket_size - 1);
	hash_bucket_t * nb = malloc(sizeof(hash_bucket_t) + key_size);
	if(!nb) return false;

	nb->data = data;
	nb->key_size = key_size;
	nb->next = NULL;
	nb->elem_next = NULL;
	nb->elem_prev = ht->tail;
	nb->idx = idx;
	if(ht->size == 0) {
		ht->head = ht->tail = nb;
	}else {
		ht->tail->elem_next = nb;
		ht->tail = nb;
	}

	memcpy(nb->key, key, key_size);


	hash_bucket_t * bucket = ht->bucket[bidx];
	if(!bucket) {
		ht->bucket[bidx] = nb;
	}else{// 冲突解决
		while(bucket->next) {
			bucket = bucket->next;
		}
		bucket->next = nb;
	}

	ht->size ++;
	return true;
}


pointer hash_table_sized_quick_find(hash_table_t * ht, string key, size_t key_size, ulong idx) {
	ulong  bidx = idx % (ht->bucket_size - 1);
	hash_bucket_t * bucket = ht->bucket[bidx];
	if(!bucket) {
		return NULL;
	}else {
		while(bucket) {
			if(strcmp(bucket->key, key) == 0) {
				break;
			}
			bucket = bucket->next;
		}
		return bucket->data;
	}
}


pointer hash_table_sized_find(hash_table_t * ht, string key, size_t key_size) {
	ulong idx = hash_func(key, key_size);
	return hash_table_sized_quick_find(ht, key, key_size, idx);
}


void hash_table_apply(hash_table_t * ht, hash_table_apply_func_t func, bool reverse) {
	hash_bucket_t * bucket = reverse ? ht->tail : ht->head;
	while(bucket) {
		if(func) {
			func(bucket->data);
		}
		bucket = reverse ? bucket->elem_prev : bucket->elem_next;
	}
}


void hash_table_clear(hash_table_t * ht) {
	hash_bucket_t * bucket = ht->head, *tmp;
	ulong idx;
	while(bucket) {
		if(ht->dtor) {
			ht->dtor(bucket->data);
		}
		idx = bucket->idx;
		tmp = bucket->elem_next;
		free(bucket);
		ht->bucket[idx % ht->bucket_size - 1] = NULL;
		bucket = tmp;
	}
	ht->head = ht->tail = NULL;
	ht->curr = NULL;
	ht->size = 0;
}


void hash_table_free(hash_table_t * ht) {
	assert(ht != NULL);
	hash_table_clear(ht);
	free(ht->bucket);
	free(ht);
}
