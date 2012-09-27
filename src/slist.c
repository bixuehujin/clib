/*
 * slist.c
 *
 *  Created on: 2012-4-28
 *      Author: hujin
 */

#include <stdio.h>
#include <assert.h>
#include <malloc.h>
#include <string.h>
#include "clib/helper.h"
#include "clib/slist.h"


slist_t * _slist_new(int size, slist_dtor_func_t dtor) {
	slist_t * ret = m_new0(slist_t, 1);
	assert(ret != NULL);
	if(ret == NULL) return NULL;

	ret->dtor = dtor;
	ret->length = 0;
	ret->size = size;

	return ret;
}


bool slist_append(slist_t * list, pointer data) {
	slist_node_t * tmp = malloc(sizeof(slist_node_t) + list->size);
	if(!tmp) return false;

	tmp->next = NULL;
	memcpy(tmp->data, data, list->size);

	if(list->tail) {
		list->tail->next = tmp;
	}else{
		list->head = tmp;
		list->tail = tmp;
	}
	list->tail = tmp;
	list->length ++;

	return true;
}


bool slist_prepend(slist_t * list, pointer data) {
	slist_node_t * tmp = malloc(sizeof(slist_node_t) + list->size);
	if(!tmp) return false;

	tmp->next = list->head;
	memcpy(tmp->data, data, list->size);

	if(!list->tail) {
		list->tail = tmp;
	}

	list->head = tmp;
	list->length ++;
	return true;
}


void slist_apply(slist_t * list, slist_apply_func_t apply_func) {
	slist_node_t * elem;
	for(elem = list->head; elem; elem = elem->next) {
		apply_func(elem->data);
	}
}


void slist_clear(slist_t * list) {
	slist_node_t * curr = list->head, *next;
	while(curr) {
		next = curr->next;
		if(list->dtor) {
			list->dtor(curr->data);
		}
		free(curr);
		curr = next;
	}
	list->head = list->tail = NULL;
	list->length = 0;
}


void slist_reverse_from_node(slist_node_t ** node) {

	slist_node_t * left, *curr;
	left = *node;
	*node = NULL;//初始化一个空的单链表
	while(left) {
		curr = left;
		left = left->next;//从剩余的节点中取出最前面的一个节点

		curr->next = *node;//将取出节点的下一个节点设为新建立的单链表
		*node = curr;//对新建单链表进行重新赋值，使其成为添加了当前节点的最新单链表
	}
}


void slist_reverse(slist_t * list) {
	slist_reverse_from_node(&list->head);
}


void slist_string_dtor_func(pointer data) {
	string * d = (string *)data;
	free(*d);
}


void slist_print_node(slist_node_t *node, slist_apply_func_t func) {
	if(node) {
		func(node->data);
	}
}




pointer slist_head(slist_t * list) {
	if(list->head) {
		return (pointer) list->head->data;
	}else {
		return NULL;
	}

}


pointer slist_tail(slist_t * list) {
	return (pointer) list->tail->data;
}


void slist_remove_head(slist_t * list) {
	slist_node_t * tmp = list->head;
	if(!tmp) {
		return;
	}
	list->head = list->head->next;
	if(list->dtor) {
		list->dtor(tmp);
	}
}
