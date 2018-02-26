/*
 * CN_Script: The C subset that makes C easier to program
 *
 * Description:
 *     CN_Script is a subset of the C language which integrates tools that make
 *     coding in C a bit more managable, including data structures and other
 *     tools to enforce good coding behaviour. The CN_Script language will
 *     "transpile" into valid C Code (assuming you didn't mess up at all), and
 *     can be compiled with a C compiler afterwards.
 *
 * Author:
 *     Clara Nguyen
 */

#ifndef __CNS_H__
#define __CNS_H__

#include "src/lib/handy/cnds/cn_vec.h"

/* Macros */
#define CNS_REPEAT_VAR "__CN_SCRIPT_REPEAT_"
#define MAX_PATH 4096

/* Cool structs I guess */
typedef struct var_pair {
	char* name;
	char* type;
} VAR_PAIR;

/* Globals */
char*  CUR_FILE;
int    REPEAT_TOTAL;
int    IN_OBJ;
int    EXEC_LEN;
char*  CUR_OBJ;
char*  CUR_TYPE;
char   EXEC_PATH[MAX_PATH];
CN_VEC VAR_PAIRS;

/* Functions */
void import(char*, char*, char*);
void fend();
void append_pair(char*, char*);
void clear_pairs();

/* Functions you shouldn't really use... */
char* malloc_concat(char*, char*);
char* realloc_concat(char*, char*);
char* realloc_rconcat(char*, char*);
void freeifnull(void*);

#endif
