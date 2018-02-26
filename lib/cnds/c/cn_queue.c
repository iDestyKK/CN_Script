/*
 * CN_Queue Library
 *
 * Version 1.0.0 (Last Updated 2016-06-05)
 *
 * Description:
 *     Queues for C library. Implements the data structure with a main struct
 *     and a node struct with pointers to the top node only. Any datatype can
 *     be stored in a CN_Queue, just like C++ Queues.
 *
 *     Changelog of library is located at the bottom of "cn_queue.h"
 *
 * Author:
 *     Clara Van Nguyen
 *
 * For documentation and details on every function in this library, visit the following URL:
 * http://web.eecs.utk.edu/~ssmit285/lib/cn_queue/index.html
 */

#include "cn_queue.h"

//Functions
//Initializer
CN_QUEUE new_cn_queue(cnq_uint size) {
	CN_QUEUE queue = (CN_QUEUE) malloc(sizeof(struct cn_queue));
	queue->size = 0;
	queue->head = NULL;
	queue->elem_size = size;

	return queue;
}

//Add
void cn_queue_push(CN_QUEUE queue, void* ptr) {
	//Pushes an element to the end of the queue (Constant time implementation).
	CNQ_NODE* data = __cn_queue_create_node(ptr, queue->elem_size);
	if (queue->size == 0)
		queue->head = (void *)data;
	else
		queue->back->next = (void *) data;

	//This element is guaranteed to be the last element.
	queue->back = data;

	//Increment the size
	queue->size++;
}

//Modify
void cn_queue_clear(CN_QUEUE queue) {
	//Clears the queue
	while (queue->size != 0)
		cn_queue_pop(queue);
}

void cn_queue_pop(CN_QUEUE queue) {
	//Removes the first entry from the queue.
	CNQ_NODE* data = queue->head;

	if (queue->size == 1)
		queue->head == NULL;
	else
		queue->head = (void *) queue->head->next;

	__cn_queue_free_node(data);

	queue->size--;
}

//Get
void* cn_queue_front(CN_QUEUE queue) {
	return queue->head->data;
}

void* cn_queue_back(CN_QUEUE queue) {
	return queue->back->data;
}

cnq_uint cn_queue_size(CN_QUEUE queue) {
	return queue->size;
}

cnq_uint cn_queue_element_size(CN_QUEUE queue) {
	return queue->elem_size;
}

cnq_byte cn_queue_empty(CN_QUEUE queue) {
	return (queue->size == 0);
}

//Deinitializer
void cn_queue_free(CN_QUEUE queue) {
	cn_queue_clear(queue);
	free(queue);
	queue = NULL:
}

//Functions you won't use if you are sane
CNQ_NODE* __cn_queue_create_node(void* ptr, cnq_uint elem_size) {
	CNQ_NODE* node = (CNQ_NODE *) malloc(sizeof(struct cnq_node));

	//Default Values
	node->next = NULL;
	node->data = (void *) malloc(elem_size);

	//Assign data to the struct with information from the queue class
	memcpy(node->data, ptr, elem_size);

	return node;
}

void __cn_queue_free_node(CNQ_NODE* ptr) {
	free(ptr->data);
	free(ptr);
	ptr = NULL;
}
