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
 *     Changelog of library is located at the bottom of this file.
 *
 * Author:
 *     Clara Van Nguyen
 *
 * For documentation and details on every function in this library, visit the following URL:
 * http://web.eecs.utk.edu/~ssmit285/lib/cn_queue/index.html
 */

#ifndef __CN_QUEUE__
#define __CN_QUEUE__

#include <stdlib.h>
#include <string.h>

//Custom Types
typedef unsigned int       cnq_uint;
typedef unsigned long long cnq_u64;
typedef unsigned char      cnq_byte;

//Structs
typedef struct cnq_node {
	struct cnl_node* next;
	void           * data;
} CNQ_NODE;

typedef struct cn_queue {
	//Nodes holding data
	CNQ_NODE* head,
		    * back;

	//Data needed in the structure
	cnq_uint  size,
	          elem_size;
} *CN_QUEUE;

typedef CN_QUEUE     QUEUE;    //For you C++ people...
#define CNQ_ITERATOR CNQ_NODE* //My point still stands...

//Functions
//Initializer
CN_QUEUE  new_cn_queue          (cnq_uint);

//Add
void      cn_queue_push         (CN_QUEUE, void*);

//Modify
void      cn_queue_clear        (CN_QUEUE);
void      cn_queue_pop          (CN_QUEUE);

//Get
void*     cn_queue_front        (CN_QUEUE);
void*     cn_queue_back         (CN_QUEUE);
cnq_uint  cn_queue_size         (CN_QUEUE);
cnq_uint  cn_queue_element_size (CN_QUEUE);
cnq_byte  cn_queue_empty        (CN_QUEUE);

//Deinitializer
void      cn_queue_free         (CN_QUEUE);

//Functions you won't use if you are sane
CNQ_NODE* __cn_queue_create_node(void*, cnq_uint);
void      __cn_queue_free_node  (CNQ_NODE*);

//Macros
#define cn_queue_init(type) \
	new_cn_queue(sizeof(type)) //For those who hate typing sizeof(...)i

#define cn_queue_deref(iterator, type) \
	*(type *)iterator

#define cn_queue_deref_front(CN_QUEUE, type) \
	*(type *)CN_QUEUE->head->data

#define cn_queue_deref_back(CN_QUEUE, type) \
	*(type *)CN_QUEUE->back->data

#endif

/***************************************\
    * ** ***    CHANGELOG    *** ** *
\***************************************/

/*
    2016-06-05 (1.0.0)
      - Started writing CN_Queue. Initial (And probably the only) Release.
*/
