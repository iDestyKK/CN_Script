/*
 * Yacc grammar for CN_Script.
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

%{
	/* C Includes */
	#include <stdio.h>
	#include <stdlib.h>

	/* Custom Includes */
	#include "cns.h"
%}

%union {
	int integer;
	double decimal;
	char* str;
	char* ptr;
}

/* Passed via string */
%token <str> ID CON STR

/* Passed by other means */
%token INT UINT FLOAT DOUBLE CHAR UCHAR
%token FUNC_TOP

/* Precedence */
/*
	%left
	%right
*/

/* The real deal */
%%
prog : external {}
	 ;

external: CIMPORT STR { print_extern($1, $2); }
	
func: fhead ":" fbody "fend" { print_func($1, $2, $3); }
	;

fhead: FUNC_TOP { $$ = $1; }
%%

/* Parser */
#include <stdio.h>
#include <stdlib.h>

extern int yylineno;

main() {
	if (yyparse()) {
		yyerror("syntax error");
	}
	exit(0);
}

yyerror(char msg[]) {
	fprintf(stderr, " %s.  Line %d\n", msg, yylineno);
}
