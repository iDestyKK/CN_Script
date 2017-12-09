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

/* Globals */
char* CUR_FILE;

/* Functions */
void import(char*, char*, char*);
void fend();

#endif
