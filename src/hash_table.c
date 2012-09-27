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



static inline ulong hash_func(string key, uint key_len) {
	ulong hash = 31;
	int i;
	for(i=0;key[i] != 0; i ++) {
		hash = hash + key[i] * 33;
	}
	return hash;
}


static inline uint get_table_size(uint size) {
	uint i = 3;
	uint ret;

	if (size >= 0x80000000) {
		/* prevent overflow */
		ret = 0x80000000;
	} else {
		while ((1U << i) < size) {
			i++;
		}
		ret = 1 << i;
	}
	return ret;
}


hash_table_t * hash_table_new(uint size, hash_table_dtor_func_t dtor) {
	hash_table_t * ht = malloc(sizeof(hash_table_t));
	if(!ht) return NULL;

	ht->dtor = dtor;

	ht->size = 0;
	ht->bucket_size = get_table_size(size);
	ht->table_mask = ht->bucket_size - 1;
	ht->curr = NULL;
	ht->head = NULL;
	ht->tail = NULL;
	ht->buckets = calloc(sizeof(hash_bucket_t *), ht->bucket_size);
	return ht;
}


hash_table_t * hash_table_init(hash_table_t * ht, uint size, hash_table_dtor_func_t dtor) {
	ht->dtor = dtor;

	ht->size = 0;
	ht->bucket_size = get_table_size(size);
	ht->table_mask = ht->bucket_size - 1;
	ht->curr = NULL;
	ht->head = NULL;
	ht->tail = NULL;
	ht->buckets = calloc(sizeof(hash_table_t *), ht->bucket_size);
	return ht;
}


void hash_table_rehash(hash_table_t * ht) {
	ht->bucket_size += ht->bucket_size;
	ht->table_mask = ht->bucket_size - 1;
	ht->buckets = realloc(ht->buckets, ht->bucket_size);
	memset(ht->buckets, 0, ht->bucket_size * sizeof(hash_bucket_t *));

	hash_bucket_t * bucket = ht->head;
	hash_bucket_t ** buckets = ht->buckets;
	hash_bucket_t * tmp;
	ulong bidx;

	while(bucket) {
		bidx = bucket->idx & ht->table_mask;
		if(buckets[bidx]) {
			tmp = buckets[bidx];
			while(tmp->next) {
				tmp = tmp->next;
			}
			tmp->next = bucket;
		}else {
			buckets[bidx] = bucket;
		}
		bucket = bucket->elem_next;
	}
}


bool hash_table_sized_insert(hash_table_t * ht, string key, int key_size, pointer data){
	ulong idx = hash_func(key, key_size);
	ulong bidx = idx & ht->table_mask;
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


	hash_bucket_t * bucket = ht->buckets[bidx];
	if(!bucket) {
		ht->buckets[bidx] = nb;
	}else{// 冲突解决
		while(bucket->next) {
			bucket = bucket->next;
		}
		bucket->next = nb;
	}

	/* make  ht->curr point to the first element of hash table, for iterator use . */
	if(ht->size == 0) {
		ht->curr = nb;
	}

	ht->size ++;

	/* rehash the table when necessary */
	if(ht->size >= ht->bucket_size) {
		hash_table_rehash(ht);
	}

	return true;
}


pointer hash_table_sized_quick_find(hash_table_t * ht, string key, size_t key_size, ulong idx) {
	ulong  bidx = idx & ht->table_mask;
	hash_bucket_t * bucket = ht->buckets[bidx];

	while(bucket) {

		printf("key:%s\n", bucket->key);
		if(strcmp(bucket->key, key) == 0) {
			return bucket->data;
		}
		bucket = bucket->next;
	}
	return NULL;
}


bool hash_table_sized_quick_remove(hash_table_t * ht, string key, size_t key_size, ulong idx) {
	ulong bidx = idx & ht->table_mask;
	hash_bucket_t * bucket = ht->buckets[bidx], *tmp;

	if(!bucket) {
		return false;
	}
	while(bucket) {
		if(strcmp(bucket->key, key) == 0) {
			goto found;
		}
	}

	return false;

found:

	if(ht->head == bucket) {//found bucket is the head of the doubly linked list .
		ht->head = bucket->elem_next;
	}else if(ht->tail == bucket){
		ht->tail = bucket->elem_prev;
	}else {
		tmp = bucket;
		bucket->elem_prev->elem_next = tmp->elem_next;
		bucket->elem_next->elem_prev = tmp->elem_prev;
	}

	// remove the bucket from conflict list .
	if(ht->buckets[bidx] == bucket) {
		ht->buckets[bidx] = bucket->next;
	}else {
		tmp = ht->buckets[bidx];
		while(tmp->next && tmp->next != bucket) {
			break;
			tmp = tmp->next;
		}
		tmp->next = bucket->next;
	}

	/* if the deleting bucket is ht->curr, move the ht->curr forward . */
	if(ht->curr == bucket) {
		ht->curr = ht->curr->elem_next;
	}

	free(bucket);
	return true;
}


bool hash_table_sized_remove(hash_table_t *ht, string key, size_t key_size) {
	ulong idx = hash_func(key, key_size);
	return hash_table_sized_quick_remove(ht, key, key_size, idx);
}


bool hash_table_sized_quick_key_exist(hash_table_t * ht, string key, size_t key_size, ulong idx) {
	ulong bidx = idx & ht->table_mask;
	hash_bucket_t * bucket = ht->buckets[bidx];
	if(!bucket) {
		return false;
	}

	while(bucket) {
		if(strcmp(bucket->key, key) == 0) {
			return true;
		}
		bucket = bucket->next;
	}
	return false;
}


bool hash_table_sized_key_exist(hash_table_t *ht, string key, size_t key_size) {
	ulong idx = hash_func(key, key_size);
	return hash_table_sized_quick_key_exist(ht, key, key_size, idx);
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
		ht->buckets[idx & ht->table_mask] = NULL;
		bucket = tmp;
	}
	ht->head = ht->tail = NULL;
	ht->curr = NULL;
	ht->size = 0;
}


void hash_table_free(hash_table_t * ht) {
	assert(ht != NULL);
	hash_table_clear(ht);
	free(ht->buckets);
	free(ht);
}


void hash_table_destroy(hash_table_t *ht) {
	assert(ht != NULL);
	hash_table_clear(ht);
	free(ht->buckets);
}


/*****************************************/
/*  iterator APIs for hash table         */
/*****************************************/


/**
 * reset internal current pointer .
 */
inline void hash_table_rewind(hash_table_t *ht) {
	ht->curr = ht->head;
}


/**
 * whether has current data
 */
inline bool hash_table_current(hash_table_t * ht) {
	return ht->curr ? true : false;
}


inline char * hash_table_current_key(hash_table_t * ht) {
	return ht->curr->key;
}


inline  pointer hash_table_current_data(hash_table_t * ht) {
	return ht->curr->data;
}


/**
 * move internal pointer forward
 */
inline void hash_table_next(hash_table_t * ht) {
	if(ht->curr) {
		ht->curr = ht->curr->elem_next;
	}
}


/**
 * move internal pointer backword.
 */
inline void hash_table_prev(hash_table_t * ht) {
	if(ht->curr) {
		ht->curr = ht->curr->elem_prev;
	}
}
