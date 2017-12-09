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
%token <str> ID CON ARR_STR ARG_STR QUOTE_STR STR IMPORT

/* Passed by other means */
%token INT UINT FLOAT DOUBLE CHAR UCHAR
%token FUNC_TOP FUNC LEFT_PAREN RIGHT_PAREN FEND
%type <str> fargs

/* Precedence */
/*
	%left
	%right
*/

/* The real deal */
%%
prog     : external               {}
	     ;

external :                        {}
		 | external import        {}
		 ;

import   : IMPORT ARR_STR         { import($1, "<>"  , $2); }
         | IMPORT QUOTE_STR       { import($1, "\"\"", $2); }
	     | func
	     ;
	
func     :                        {}
	     | func func_decl FEND    { fend(); }
	     ;

func_decl: FUNC ARR_STR STR fargs { printf("%s %s%s {", $2, $3, $4); }
		 | FUNC STR fargs         { printf("%s%s {", $2, $3); }
		 ;

fargs    : ARG_STR                { $$ = $1; }
	     ;
%%

/* Parser */
#include <stdio.h>
#include <stdlib.h>

extern int yylineno;
extern FILE* yyin;

main(int argc, char** argv) {
	FILE* fp;
	if (argc == 1) {
		fprintf(stderr, "Usage: %s file\n", argv[0]);
	}
	else {
		fp = fopen(argv[1], "r");
		yyin = fp;
		if (yyparse()) {
			yyerror("syntax error");
		}
	}
	exit(0);
}

yyerror(char msg[]) {
	fprintf(stderr, " %s.  Line %d\n", msg, yylineno);
}
