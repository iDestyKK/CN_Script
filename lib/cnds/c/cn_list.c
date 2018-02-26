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
 *     Changelog of library is located at the bottom of "cn_list.h"
 *
 * Author:
 *     Clara Van Nguyen
 *
 * For documentation and details on every function in this library, visit the following URL:
 * http://web.eecs.utk.edu/~ssmit285/lib/cn_list/index.html
 */

#include "cn_list.h"

//Functions
//Initializer
CN_LIST new_cn_list(cnl_uint size) {
    //Initializes the new list. It needs the size of the data type we are storing in it.
    CN_LIST list = (CN_LIST) malloc(sizeof(struct cn_list));
    list->size      = 0;
    list->head      = NULL;
    list->elem_size = size;

    return list;
}

//Add
void cn_list_insert(CN_LIST list, cnl_uint pos, void* ptr) {
    CNL_NODE* data = __cn_list_create_node(ptr, list->elem_size);
    if (pos > list->size && list->size != 0)
        return; //You failed.

    if (list->size == 0) {
        list->head = data;
    } else {
        //Go to the node in the list
        cnl_uint i = 0;
        CNL_NODE* trav = list->head,
                * prev = NULL;

        if (pos == list->size) {
            //Constant time push_back. Because why not.
            if (list->head->prev == NULL) {
                //Must be a new list of size 1...
                list->head->next       = data;
                data->prev             = list->head;
            } else {
                list->head->prev->next = data;
                data->prev             = list->head->prev;
            }
            data->next       = list->head;
            list->head->prev = data; //Allow loop back.
            //printf("lol%d", *(int *)ptr);
        }
        else {
            for (; i < pos; i++)
                prev = trav,
                trav = trav->next;

            if (pos == 0)
                data->prev = list->head->prev;

            if (prev != NULL) {
                prev->next       = data;
                data->prev       = prev;
                //list->head->prev = data;
            }

            trav->prev = data;
            data->next = trav;

            if (pos == 0) {
                list->head->prev = data;
                list->head       = data;
            }
        }
    }

    list->size++;
}

void cn_list_push_front(CN_LIST list, void* ptr) {
    cn_list_insert(list, 0, ptr);
}

void cn_list_push_back(CN_LIST list, void* ptr) {
    cn_list_insert(list, list->size, ptr);
}

//Set
void cn_list_resize(CN_LIST list, cnl_uint size) {
    if (size == 0)
        cn_list_clear(list);
    else
    if (size != list->size) {
        cnl_uint i, j;
        if (size < list->size) {
            j = (list->size - size);

            for (i = 0; i < j; i++)
                cn_list_delete(list, cn_list_size(list) - 1);
        } else
        if (size > list->size) {
            void* data = (void *) calloc(list->elem_size, 1); //Blank entry of just 0's
            j = (size - list->size);

            for (i = 0; i < j; i++)
                cn_list_push_back(list, data);

            free(data);
        }
    }
    //Otherwise, size == list->size
    return;
}

void cn_list_set(CN_LIST list, cnl_uint pos, void* ptr) {
    if (pos >= list->size)
        return; //You failed.
    memcpy(cn_list_at(list, pos), ptr, list->elem_size);
}

void cn_list_delete(CN_LIST list, cnl_uint pos) {
    if (pos >= list->size)
        return; //You failed.

    if (list->size == 1) {
        __cn_list_free_node(list->head);
        list->head = NULL;
    } else {
        if (pos == 0) {
            //In the front of the list. Constant time.
            CNL_NODE* tmp = list->head;
            if (list->size > 2) {
                list->head->prev->next = list->head->next;
                list->head->next->prev = list->head->prev;
                list->head             = list->head->next;
            } else {
                list->head       = list->head->next;
                list->head->prev = NULL; //No wraparound on lists with a single entry.
                list->head->next = NULL;
            }
            if (list->size == 1) {
                list->head = NULL;
            }
            __cn_list_free_node(tmp);
        } else
        if (pos == list->size - 1) {
            //In the end of the list. Constant time.
            CNL_NODE* tmp = list->head->prev;
            if (list->size > 2) {
                list->head->prev = tmp->prev;
                tmp->prev->next  = list->head;
            } else {
                list->head->prev = NULL;
                list->head->next = NULL;
            }
            __cn_list_free_node(tmp);

            if (list->size == 2)
                list->head->prev = NULL;
        } else {
            //In the middle of the list. O(N) time.
            cnl_uint i = 0;
            CNL_NODE* trav = list->head,
                    * prev = NULL;

            for (; i < pos; i++)
                prev = trav,
                trav = trav->next;

            prev->next       = trav->next;
            trav->next->prev = trav->prev;
            __cn_list_free_node(trav);
        }
    }
    list->size--;
}

//Modify
void cn_list_clear(CN_LIST list) {
    while (list->head != NULL)
        cn_list_pop_back(list);
}

void cn_list_swap(CN_LIST list, cnl_uint pos1, cnl_uint pos2) {
    if (pos1 != pos2 && pos1 < list->size && pos2 < list->size) {
        cnl_uint i = 0;
        cnl_byte *b1, *b2;

        //XOR Swap > Malloc & Free... by 0.1 seconds :)
        b1 = (cnl_byte *) cn_list_at(list, pos1);
        b2 = (cnl_byte *) cn_list_at(list, pos2);

        for (; i < list->elem_size; i++) {
            *b1   ^= *b2;
            *b2   ^= *b1;
            *b1++ ^= *b2++;
        }
    }
}

void cn_list_reverse(CN_LIST list) {
    cnl_uint i  = 0,
             sr = cn_list_size(list),
             s  = sr / 2; //Round down
    sr--;
    for (; i < s; i++) {
        cn_list_swap(list, i, sr - i);
    }
}

void cn_list_pop_front(CN_LIST list) {
    cn_list_delete(list, 0);
}

void cn_list_pop_back(CN_LIST list) {
    cn_list_delete(list, cn_list_size(list) - 1);
}

/*void cn_list_sort(CN_LIST list, void (*func)(void*, void*)) {
    //TODO: Add Sort Functionality (Merge Sort? QSort?)
}*/

//Get
void* cn_list_at(CN_LIST list, cnl_uint pos) {
    if (list->size == 0 || pos >= list->size)
        return NULL; //Enjoy your segfault.
    CNL_NODE* ii = list->head;
    cnl_uint  i  = 0;
    for (; i < pos; i++)
        ii = ii->next;
    return ii->data;
}

CNL_NODE* cn_list_node_at(CN_LIST list, cnl_uint pos) {
    if (list->size == 0 || pos >= list->size)
        return NULL; //Enjoy your segfault.
    CNL_NODE* ii = list->head;
    cnl_uint  i  = 0;
    for (; i < pos; i++)
        ii = ii->next;
    return ii;
}

cnl_uint cn_list_size(CN_LIST list) {
    return list->size;
}

cnl_uint cn_list_element_size(CN_LIST list) {
    return list->elem_size;
}

cnl_byte cn_list_empty(CN_LIST list) {
    return (list->size == 0);
}

//Iteration
CNL_NODE* cn_list_begin (CN_LIST list) { return list->head; }
CNL_NODE* cn_list_end   (CN_LIST list) { return NULL; }
CNL_NODE* cn_list_rbegin(CN_LIST list) { return list->head->prev; }
CNL_NODE* cn_list_rend  (CN_LIST list) { return NULL; }

CNL_NODE* cn_list_prev(CN_LIST list, CNL_NODE* node) {
    if (node->prev == list->head->prev)
        return NULL;
    return node->prev;
}

CNL_NODE* cn_list_next(CN_LIST list, CNL_NODE* node) {
    if (node->next == list->head)
        return NULL;
    return node->next;
}

//Deinitializer
void cn_list_free(CN_LIST list) {
    cn_list_clear(list);
    free(list);
	list = NULL;
}

//Functions you won't use if you are sane
CNL_NODE* __cn_list_create_node(void* ptr, cnl_uint elem_size) {
    CNL_NODE* node = (CNL_NODE *) malloc(sizeof(struct cnl_node));

    //Default Values
    node->next = NULL;
    node->prev = NULL;
    node->data = (void *) malloc(elem_size);

    //Assign data to the struct with information from the list class
    memcpy(node->data, ptr, elem_size);

    return node;
}

void __cn_list_free_node(CNL_NODE* ptr) {
    free(ptr->data);
    free(ptr);
}
