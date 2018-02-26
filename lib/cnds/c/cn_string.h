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
 *     Changelog of this library is located at the bottom of this file.
 *
 * Author:
 *     Clara Van Nguyen
 *
 * For documentation and details on every function in this library,
 * visit the following URL:
 * http://web.eecs.utk.edu/~ssmit285/lib/cn_string/index.html
 */

#ifndef __CN_STRING_C_HAN__
#define __CN_STRING_C_HAN__

//C Includes
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//Custom Types
#ifndef __HANDYTYPES_C_HAN__
	typedef unsigned int  cn_uint;
	typedef unsigned char cn_byte;
#endif

//Structs
typedef struct cn_string {
	cn_byte* data;
	cn_uint len;
} *CN_STRING;

typedef CN_STRING STRING; //For you C++ people... (Which is weird here...)

CN_STRING cn_string_init            ();
CN_STRING cn_string_from_cstr       (const char*);

char*     cn_string_str             (CN_STRING);
cn_uint   cn_string_len             (CN_STRING);
cn_byte   cn_string_at              (CN_STRING, cn_uint);
void      cn_string_set             (CN_STRING, CN_STRING);
void      cn_string_set_from_cstr   (CN_STRING, char*);
CN_STRING cn_string_substr          (CN_STRING, cn_uint  , cn_uint);
char*     cn_string_substr_as_cstr  (CN_STRING, cn_uint  , cn_uint);
void      cn_string_insert          (CN_STRING, CN_STRING, cn_uint);
void      cn_string_insert_from_cstr(CN_STRING, char*    , cn_uint);

void      cn_string_concat          (CN_STRING, CN_STRING);
void      cn_string_concat_from_cstr(CN_STRING, const char*);

void      cn_string_update          (CN_STRING);
void      cn_string_clear           (CN_STRING);

void      cn_string_free            (CN_STRING);

//Shortcut Macros
#define CN_STR(str)\
	cn_string_str(str)

#endif

/***************************************\
    * ** ***    CHANGELOG    *** ** *
\***************************************/

/*
    2017/01/23 (0.1.1)
      - Added "cn_string_set()". I'm not sure why I didn't add that before. :/

    2017/01/20 (0.1.0)
      - Added many functions and made string guaranteed to be allocated if you
        use the functions the library supplies.

    2016/12/31 (0.0.1)
      - Started writing CN_String. Initial (And probably the only) Release.
*/
