/*
 * CN_Grid Library (C Version)
 *
 * Version 0.0.1 (Last Updated 2016-09-17)
 *
 * Description:
 *     Implements a custom "grid-like" data type for C users.
 *     Uses functions a user is already familiar with using in STL.
 *     Implemented with a malloc call size m*n with pointers set
 *     to allow using double array operators. Also, lookup times
 *     are constant time.
 *
 *     Changelog of this library is located at the bottom of this
 *     file.
 *
 * Author:
 *     Clara Van Nguyen
 *
 * For documentation and details on every function in this library,
 * visit the following URL:
 * http://web.eecs.utk.edu/~ssmit285/lib/cn_grid/c/index.html
 */

#ifndef __CN_GRID_C_HAN__
#define __CN_GRID_C_HAN__

//C Includes
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//Custom Types
typedef unsigned int       cng_uint;
typedef unsigned long long cng_u64;
typedef unsigned char      cng_byte;

//Structs
typedef struct cn_grid {
	void**   data;
	cng_uint elem_size,
	         width, height,
	         owid, ohei;
} *CN_GRID;

typedef CN_GRID GRID; //For you C++ people... (Which is weird here...)

//Functions
//Initialiser
/*
	These functions will initialise a CN_Grid for you. You just have
	to declare a "CN_GRID" (which is a pointer to cn_grid), and set
	it equal to this function.
*/

CN_GRID  new_cn_grid          (cng_uint);
CN_GRID  new_cn_grid_size     (cng_uint, cng_uint, cng_uint);

//Resize
/*
	These functions deal with resising the grid. Resising a grid is
	a computationally expensive operation as it calls realloc and
	has to move memory around accordingly with memmove. Then it has
	to recalculate pointers. Try to avoid using these if you can
	help it.
*/

void     cn_grid_resize_x     (CN_GRID, cng_uint);
void     cn_grid_resize_y     (CN_GRID, cng_uint);
void     cn_grid_resize       (CN_GRID, cng_uint, cng_uint);

//Get
cng_uint cn_grid_size_x       (CN_GRID);
cng_uint cn_grid_size_y       (CN_GRID);
cng_uint cn_grid_size         (CN_GRID);
void*    cn_grid_at           (CN_GRID, cng_uint, cng_uint);

//Set

//Modify
void     cn_grid_clear        (CN_GRID);
void     cn_grid_swap         (CN_GRID, cng_uint, cng_uint, cng_uint, cng_uint);
void     cn_grid_copy         (CN_GRID, CN_GRID);

//Iteration

//Memory Save/Read Functions

//Deinitialiser
void     cn_grid_free         (CN_GRID);

//Functions you shouldn't use if you are sane. ;)
/*
	For real, do not use these unless you have some absolute need
	to do so. They are made as "background" functions to aid in
	other functions offered to you. Besides, usually double
	underscores means "stay the hell away from this"...
*/

void     __cn_grid_resize_proc(CN_GRID);

//Macros
#define cn_grid_init(type) \
	new_cn_grid(sizeof(type)) //For those who hate typing sizeof(...)

#define cn_grid_init_size(type, w, h) \
	new_cn_grid_size(sizeof(type), w, h) //For those who hate typing sizeof(...)

#define cn_grid_deref(ptr, type) \
	(*(type*)ptr) //lol cheat

#define cn_grid_get(CN_GRID, type, x, y) \
	cn_grid_deref(cn_grid_at(CN_GRID, x, y), type) //CHEAT EVEN MORE


#endif

/***************************************\
    * ** ***    CHANGELOG    *** ** *
\***************************************/

/*
	2016-09-17 (0.0.1)
	  - Started writing CN_Grid (C Version).
*/
