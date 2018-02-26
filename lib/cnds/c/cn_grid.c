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
 *     Changelog of this library is located at the bottom of
 *     "cn_grid.h"
 *
 * Author:
 *     Clara Van Nguyen
 *
 * For documentation and details on every function in this library,
 * visit the following URL:
 * http://web.eecs.utk.edu/~ssmit285/lib/cn_grid/c/index.html
 */

#include "cn_grid.h"

//Initialisers
/*
	These functions will initialise a CN_Grid for you. You just have
	to declare a "CN_GRID" (which is a pointer to cn_grid), and set
	it equal to this function.
*/

CN_GRID new_cn_grid(cng_uint size) {
	CN_GRID grid = (CN_GRID) malloc(sizeof(struct cn_grid));
	
	//Default Parameters
	grid->width     = 0;
	grid->height    = 0;
	grid->owid      = 0;
	grid->ohei      = 0;
	grid->data      = NULL;
	grid->elem_size = size;

	return grid;
}

CN_GRID new_cn_grid_size(cng_uint size, cng_uint width, cng_uint height) {
	CN_GRID grid = (CN_GRID) malloc(sizeof(struct cn_grid));
	
	//Set Parameters
	grid->width     = width;
	grid->height    = height;
	grid->elem_size = size;
	
	//Set Original Values
	grid->owid = grid->width;
	grid->ohei = grid->height;

	//Deal with pointers
	grid->data = (void*) calloc(width * height, size);

	return grid;
}

//Resize
/*
	These functions deal with resising the grid. Resising a grid is
	a computationally expensive operation as it calls realloc and
	has to move memory around accordingly with memmove. Then it has
	to recalculate pointers. Try to avoid using these if you can
	help it.
*/

void cn_grid_resize_x(CN_GRID grid, cng_uint _w) {
	grid->width = _w;
	__cn_grid_resize_proc(grid);
}

void cn_grid_resize_y(CN_GRID grid, cng_uint _h) {
	grid->height = _h;
	__cn_grid_resize_proc(grid);
}

void cn_grid_resize(CN_GRID grid, cng_uint _w, cng_uint _h) {
	grid->width  = _w;
	grid->height = _h;
	__cn_grid_resize_proc(grid);
}

//Get
cng_uint cn_grid_size_x(CN_GRID grid) {
	return grid->width;
}

cng_uint cn_grid_size_y(CN_GRID grid) {
	return grid->height;
}

cng_uint cn_grid_size(CN_GRID grid) {
	return grid->width * grid->height;
}

void* cn_grid_at(CN_GRID grid, cng_uint _x, cng_uint _y) {
	return (void*)(((char*)grid->data) + (grid->elem_size * (_y + (grid->height * _x))));
}


//Set


//Modify
void cn_grid_clear(CN_GRID grid) {
	if (grid->data != NULL)
		free(grid->data);
}

void cn_grid_swap(CN_GRID grid, cng_uint x1, cng_uint y1, cng_uint x2, cng_uint y2) {
	//XOR Swap Implementation
	char* pos1 = cn_grid_at(grid, x1, y1);
	char* pos2 = cn_grid_at(grid, x2, y2);

	if (pos1 != pos2) {
		//XOR Swap's flaw is in if you try swapping one value with itself.
		//It will ZERO out. So, we add this check in to prevent that.

		cng_uint i = 0;
		for (; i < grid->elem_size; i++) {
			*pos1   ^= *pos2;
			*pos2   ^= *pos1;
			*pos1++ ^= *pos2++;
		}
	}
}
void cn_grid_copy(CN_GRID src, CN_GRID dest) {
	//Absolutely nasty implementation, but whatever...
	//Copies contents of src to dest.

	//Clear Grid and then copy over all variables, except data
	cn_grid_clear(dest);
	memcpy(dest, src, sizeof(struct cn_grid));
	dest->data = NULL;

	//Resize the grid
	cn_grid_resize(dest, src->width, src->height);

	//Copy every entity over, byte-by-byte
	memcpy(dest->data, src->data, src->elem_size * src->width * src->height);
}


//Iteration


//Memory Save/Read Functions


//Deinitialiser
void cn_grid_free(CN_GRID grid) {
	cn_grid_clear(grid);
	free(grid);
}


//Functions you shouldn't use if you are sane. ;)
/*
	For real, do not use these unless you have some absolute need
	to do so. They are made as "background" functions to aid in
	other functions offered to you. Besides, usually double
	underscores means "stay the hell away from this"...
*/

void __cn_grid_resize_proc(CN_GRID grid) {
	if (grid->data == NULL) {
		//Easy case, nothing was set prior
		grid->data = (void*) calloc(grid->width * grid->height, grid->elem_size);
	} else {
		//Horizontal First. It's the easiest.
		if (grid->width != grid->owid) {
			grid->data = (void*) realloc(grid->data, grid->elem_size * grid->width * grid->ohei);
			if (grid->width > grid->owid) {
				char* val = (char *) grid->data + (grid->elem_size * (grid->ohei * grid->owid)); //cn_grid_at(grid, grid->owid, 0);
				memset(val, 0, grid->elem_size * (grid->ohei * (grid->width - grid->owid)));
			}
		}

		//Now vertical... this is where it gets tricky.
		if (grid->height != grid->ohei) {
			char* pos1,
			    * pos2;
			cng_uint i = 1;

			if (grid->height < grid->ohei) {
				//Size is going down. "Slide" data first.
				pos1 = (char *) grid->data + (grid->elem_size * grid->ohei  );
				pos2 = (char *) grid->data + (grid->elem_size * grid->height);
				
				for (; i <= grid->height; i++) {
					memmove(pos2, pos1, grid->elem_size * grid->height);
					pos1 += (grid->elem_size * grid->ohei  );
					pos2 += (grid->elem_size * grid->height);
				}
				
				//Now resize...
				grid->data = (void *) realloc(grid->data, grid->elem_size * grid->width * grid->height);
			}
			else
			if (grid->height > grid->ohei) {
				//Size is going up. Resize first.
				grid->data = (void *) realloc(grid->data, grid->elem_size * grid->width * grid->height);

				//Then "Slide" Data over.
				pos1 = (char *) grid->data + (grid->elem_size * grid->width * (grid->ohei   - 1)) + (grid->elem_size * (grid->height - grid->ohei));
				pos2 = (char *) grid->data + (grid->elem_size * grid->width * (grid->height - 1));

				for (; i < grid->height; i++) {
					memmove(pos2, pos1, grid->elem_size * grid->ohei);
					memset (pos2 + (grid->elem_size * grid->ohei), 0, grid->elem_size * (grid->height - grid->ohei));
					pos1 -= (grid->elem_size * grid->ohei);
					pos2 -= (grid->elem_size * grid->height);
				}
				memset(pos2 + (grid->elem_size * grid->ohei), 0, grid->elem_size * (grid->height - grid->ohei));
			}
		}
	}
	//Set Original Values
	grid->owid = grid->width;
	grid->ohei = grid->height;
}
