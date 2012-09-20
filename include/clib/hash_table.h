/*
 * hash_table.h
 *
 *  Created on: 2012-5-8
 *      Author: hujin
 */

#ifndef HASH_TABLE_H_
#define HASH_TABLE_H_
#include "clib.h"

typedef void (*hash_table_dtor_func_t)(pointer data);
typedef void (*hash_table_apply_func_t)(pointer data);

typedef struct _hash_bucket{
	ulong idx;
	size_t key_size;
	pointer data;
	struct _hash_bucket * next;
	struct _hash_bucket * elem_next;
	struct _hash_bucket * elem_prev;
	char key[0];
}hash_bucket_t;

typedef struct _hash_table{
	uint size;
	uint bucket_size;
	uint table_mask;
	hash_bucket_t ** buckets;
	hash_bucket_t * head;
	hash_bucket_t * tail;
	hash_bucket_t * curr; /* internal pointer for iterator */
	hash_table_dtor_func_t dtor;
}hash_table_t;


#define hash_table_insert(ht, key, pdata) \
	hash_table_sized_insert(ht, key, strlen(key) + 1, pdata)

#define hash_table_find(ht, key) \
	hash_table_sized_find(ht, key, strlen(key) + 1)
#define hash_table_quick_find(ht, key, idx) \
	hash_table_sized_quick_find(ht, key, strlen(key) + 1, idx)


#define hash_table_remove(ht, key) \
	hash_table_sized_remove(ht, key, strlen(key) + 1)
#define hash_table_quick_remove(ht, key, idx) \
	hash_table_sized_quick_remove(ht, key, strlen(key) + 1, idx)


#define hash_table_key_exist(ht, key) \
	hash_table_sized_key_exist(ht, strlen(key) + 1, idx)
#define hash_table_quick_key_exist(ht, key, idx) \
	hash_table_sized_quick_key_exist(ht, key, strlen(key) + 1, idx)


hash_table_t * hash_table_new(uint size, hash_table_dtor_func_t dtor);
hash_table_t * hash_table_init(hash_table_t * ht, uint size, hash_table_dtor_func_t dtor);

void hash_table_clear(hash_table_t * ht);
void hash_table_free(hash_table_t * ht);
void hash_table_destroy(hash_table_t *ht);

void hash_table_rehash(hash_table_t * ht);

bool hash_table_sized_insert(hash_table_t * ht, string key, int key_len, pointer data);

pointer hash_table_sized_find(hash_table_t * ht, string key, size_t key_size);
pointer hash_table_sized_quick_find(hash_table_t * ht, string key, size_t key_size, ulong idx);

bool hash_table_sized_quick_remove(hash_table_t * ht, string key, size_t key_size, ulong idx);
bool hash_table_sized_remove(hash_table_t *ht, string key, size_t key_size);

bool hash_table_sized_quick_key_exist(hash_table_t * ht, string key, size_t key_size, ulong idx);
bool hash_table_sized_key_exist(hash_table_t *ht, string key, size_t key_size);

void hash_table_apply(hash_table_t * ht, hash_table_apply_func_t func, bool reverse);
void hash_table_rehash(hash_table_t * ht);

/*****************************************/
/*            iterators                  */
/*****************************************/


void hash_table_rewind(hash_table_t *ht);
bool hash_table_current(hash_table_t * ht);
char * hash_table_current_key(hash_table_t * ht);
pointer hash_table_current_data(hash_table_t * ht);
void hash_table_next(hash_table_t * ht);
void hash_table_prev(hash_table_t * ht);

#endif /* HASH_TABLE_H_ */
