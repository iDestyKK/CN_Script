/*
 * CN_List Library
 *
 * Version 1.0.0 (Last Updated 2016-06-01)
 *
 * Description:
 *     Circular Doubly-Linked Lists for C library. Implements the data structure
 *     with a main struct and node struct with pointers to get around in memory.
 *     Any datatype can be stored in a CN_List, just like C++ Lists.
 *
 *     Changelog of library is located at the bottom of this file.
 *
 * Author:
 *     Clara Van Nguyen
 *
 * For documentation and details on every function in this library, visit the following URL:
 * http://web.eecs.utk.edu/~ssmit285/lib/cn_list/index.html
 */

#ifndef __CN_LIST__
#define __CN_LIST__

#include <stdlib.h>
#include <string.h>

//Custom Types
typedef unsigned int       cnl_uint;
typedef unsigned long long cnl_u64;
typedef unsigned char      cnl_byte;

//Structs
typedef struct cnl_node {
    struct cnl_node* next,
                   * prev;
    void           * data;
} CNL_NODE;

typedef struct cn_list {
    CNL_NODE* head;
    cnl_uint  size,
              elem_size;
} *CN_LIST;

typedef CN_LIST      LIST;     //For you C++ people...
#define CNL_ITERATOR CNL_NODE* //My point still stands...

//Functions
//Initializer
CN_LIST   new_cn_list          (cnl_uint);

//Add
void      cn_list_insert       (CN_LIST, cnl_uint, void*);
void      cn_list_push_front   (CN_LIST, void*);
void      cn_list_push_back    (CN_LIST, void*);

//Set
void      cn_list_resize       (CN_LIST, cnl_uint);
void      cn_list_set          (CN_LIST, cnl_uint, void*);
void      cn_list_delete       (CN_LIST, cnl_uint);

//Modify
void      cn_list_clear       (CN_LIST);
void      cn_list_swap        (CN_LIST, cnl_uint, cnl_uint);
void      cn_list_reverse     (CN_LIST);
void      cn_list_pop_front   (CN_LIST);
void      cn_list_pop_back    (CN_LIST);
//void      cn_list_sort        (CN_LIST, void (*)(void*, void*));

//Get
void*     cn_list_at           (CN_LIST, cnl_uint);
CNL_NODE* cn_list_node_at      (CN_LIST, cnl_uint);
cnl_uint  cn_list_size         (CN_LIST);
cnl_uint  cn_list_element_size (CN_LIST);
cnl_byte  cn_list_empty        (CN_LIST);

//Iteration
CNL_NODE* cn_list_begin        (CN_LIST);
CNL_NODE* cn_list_end          (CN_LIST);
CNL_NODE* cn_list_rbegin       (CN_LIST);
CNL_NODE* cn_list_rend         (CN_LIST);
CNL_NODE* cn_list_prev         (CN_LIST, CNL_NODE*);
CNL_NODE* cn_list_next         (CN_LIST, CNL_NODE*);

//Deinitializer
void      cn_list_free         (CN_LIST);

//Functions you won't use if you are sane
CNL_NODE* __cn_list_create_node(void*, cnl_uint);
void      __cn_list_free_node  (CNL_NODE*);

//Macros
#define cn_list_init(type) \
    new_cn_list(sizeof(type)) //For those who hate typing sizeof(...)

#define cn_list_deref(ptr, type) \
    (*(type*)ptr) //lol cheat

#define cn_list_get(CN_LIST, type, pos) \
    cn_list_deref(cn_list_at(CN_LIST, pos), type) //CHEAT EVEN MORE

#define cn_list_traverse(CN_LIST, iterator) \
    for (iterator = cn_list_begin(CN_LIST); iterator != cn_list_end(CN_LIST); iterator = cn_list_next(CN_LIST, iterator))

#define cn_list_rtraverse(CN_LIST, iterator) \
    for (iterator = cn_list_rbegin(CN_LIST); iterator != cn_list_rend(CN_LIST); iterator = cn_list_prev(list, iterator))

#define cn_list_front(CN_LIST, type) \
    cn_list_get(CN_LIST, type, 0)

#define cn_list_back(CN_LIST, type) \
    cn_list_get(CN_LIST, type, cn_list_size(CN_LIST) - 1)

#endif

/***************************************\
    * ** ***    CHANGELOG    *** ** *
\***************************************/

/*
    2016-06-02 (1.0.0)
      - Started writing CN_List. Initial (And probably the only) Release.
*/
