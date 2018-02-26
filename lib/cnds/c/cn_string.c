/*
 * CN_String Library (C Version)
 *
 * Version 0.1.1 (Last Updated: 2017/01/23)
 *
 * Description:
 *     Implements a wrapper around char pointer-based memory references to make
 *     utilising strings in C easier and nicer to the programmer. The string is
 *     implemented as continuous memory struct that keeps a char* for the data
 *     and an integer for string length.
 *
 *     Changelog of this library is located at the bottom of "cn_string.h".
 *
 * Author:
 *     Clara Van Nguyen
 *
 * For documentation and details on every function in this library,
 * visit the following URL:
 * http://web.eecs.utk.edu/~ssmit285/lib/cn_string/index.html
 */

#include "cn_string.h"

CN_STRING cn_string_init()  {
	return cn_string_from_cstr((const char*)NULL);
}

CN_STRING cn_string_from_cstr(const char* data) {
	CN_STRING str = (CN_STRING) malloc(sizeof(struct cn_string));
	//Set up Parametres
	if (data == NULL) {
		str->data = (cn_byte *) calloc(1, 1);
		str->len  = 0;
	} else {
		str->len  = strlen(data);
		str->data = (cn_byte *) calloc(str->len + 1, 1);
		memcpy(str->data, data, str->len);
	}

	return str;
}

char* cn_string_str(CN_STRING str) {
	return str->data;
}

cn_uint cn_string_len(CN_STRING str) {
	return str->len;
}

cn_byte cn_string_at(CN_STRING str, cn_uint pos) {
	return str->data[pos];
}

void cn_string_set(CN_STRING dest, CN_STRING src) {
	cn_string_clear(dest);
	dest->len  = src->len;
	dest->data = (cn_byte *) calloc(1, src->len + 1);
	memcpy(dest->data, src->data, src->len);
}

void cn_string_set_from_cstr(CN_STRING dest, char* src) {
	cn_string_clear(dest);
	dest->len  = strlen(src);
	dest->data = (cn_byte *) calloc(1, dest->len + 1);
	memcpy(dest->data, src, dest->len);
}

CN_STRING cn_string_substr(CN_STRING str, cn_uint pos, cn_uint len) {
	char* arr = cn_string_substr_as_cstr(str, pos, len);
	
	//Cheat
	CN_STRING res = cn_string_init();
	res->data = arr;
	res->len  = len;

	return res;
}

char* cn_string_substr_as_cstr(CN_STRING str, cn_uint pos, cn_uint len) {
	char* arr = (char *) malloc(len + 1);
	arr[len] = 0; //Null-Terminate
	memcpy(arr, str->data + pos, len);

	return arr;
}

void cn_string_insert(CN_STRING dest, CN_STRING src, cn_uint pos) {
	dest->data = realloc(dest->data, dest->len + src->len + 1);
	memmove(dest->data + src->len + pos, dest->data + pos, dest->len - pos + 1);
	memcpy (dest->data + pos, src->data, src->len);
	dest->len += src->len;
}

void cn_string_insert_from_cstr(CN_STRING dest, char* src, cn_uint pos) {
	cn_uint len = strlen(src);
	dest->data = realloc(dest->data, dest->len + len + 1);
	memmove(dest->data + len + pos, dest->data + pos, dest->len - pos + 1);
	memcpy (dest->data + pos, src, len);
	dest->len += len;
}

void cn_string_concat(CN_STRING dest, CN_STRING add) {
	//Faster than cn_string_concat_from_cstr since it lacks strlen()
	dest->data = (cn_byte *) realloc(dest->data, dest->len + add->len + 1);
	dest->data[dest->len + add->len] = 0;
	memcpy(dest->data + dest->len, add->data, add->len);
	dest->len += add->len;
}

void cn_string_concat_from_cstr(CN_STRING dest, const char* add) {
	cn_uint len = strlen(add);
	dest->data = (cn_byte *) realloc(dest->data, dest->len + len + 1);
	dest->data[dest->len + len] = 0;
	memcpy(dest->data + dest->len, add, len);
	dest->len += len;
}

void cn_string_update(CN_STRING str) {
	//Used if the string's data gets updated but not the length.
	//Also clears the string if length is 0 but not null'd out.
	if (str->data != NULL) {
		if (str->len == 0) {
			free(str->data);
			str->data = (cn_byte *) calloc(1, 1);
		}
		else
		if (str->data[str->len] != 0) {
			str->len = strlen(str->data);
		}
	}
	if (str->data == NULL) {
		str->data = (cn_byte *) calloc(1, 1);
	}
}

void cn_string_clear(CN_STRING str) {
	if (str->data[0] != 0) {
		free(str->data);
		str->data = (cn_byte *) calloc(1, 1);
		str->len = 0;
	}
	else
	if (str->data[0] == 0)
		str->len = 0;
}

void cn_string_free(CN_STRING str) {
	if (str->data != NULL)
		free(str->data);
	free(str);
}
