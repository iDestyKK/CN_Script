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
	#include <string.h>

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
%token <str> ID CON ARR_STR ARG_STR QUOTE_STR STR IMPORT NAME FNAME TYPE

/* Passed by other means */
%token INT UINT FLOAT DOUBLE CHAR UCHAR
%token FUNC_TOP FUNC LEFT_PAREN RIGHT_PAREN FEND ANGLE_LT ANGLE_GT
%type <str> fargs arg stmt decl_var more_var temp

/* Precedence */
/*
	%left
	%right
*/

/* The real deal */
%%
prog     : external                            { }
         ;

external :                                     { }
         | external import                     { }
         ;

import   : IMPORT '<' NAME '>'                 { import($1, "<>"  , $3); }
         | IMPORT '"' NAME '"'                 { import($1, "\"\"", $3); }
         | func
         ;
    
func     :                                     { }
         | func func_decl fbody FEND           { fend(); }
         ;

func_decl: FUNC '<' NAME '>' NAME fargs ':'    { printf("%s %s(%s) {", $3, $5, ($6 == NULL) ? "" : $6);
                                                 freeifnull($6); }
         | FUNC NAME fargs ':'                 { printf("%s(%s) {", $2, ($3 == NULL) ? "" : $3);
                                                 freeifnull($3); }
         ;

fargs    : '(' ')'                             { $$ = NULL; }
         | '(' arg ')'                         { $$ = $2; }
         ;

arg      : NAME NAME ',' arg                   { $$ = malloc_concat ($1, " " );
                                                 $$ = realloc_concat($$, $2  );
                                                 $$ = realloc_concat($$, ", ");
                                                 $$ = realloc_concat($$, $4  );
                                                 free($4); }
         | NAME NAME                           { $$ = malloc_concat ($1, " " );
                                                 $$ = realloc_concat($$, $2  ); }
         ;

fbody    :                                     { }
         | fbody stmt ';'                      { printf("%s;", $2); }
         ;

stmt     : decl_var                            { $$ = $1; }
         | NAME                                { $$ = $1; }
         ;

decl_var : NAME NAME ',' more_var              { $$ = malloc_concat ($1, " " );
                                                 $$ = realloc_concat($$, $2  );
                                                 $$ = realloc_concat($$, ", ");
                                                 $$ = realloc_concat($$, $4  );
                                                 free($4); }
         | NAME '<' temp '>' NAME ',' more_var { $$ = malloc_concat ($1, "_" );
                                                 $$ = realloc_concat($$, $3  );
                                                 $$ = realloc_concat($$, " " );
                                                 $$ = realloc_concat($$, $5  );
                                                 $$ = realloc_concat($$, ", ");
                                                 $$ = realloc_concat($$, $7  );
                                                 free($7); }
         | NAME NAME                           { $$ = malloc_concat ($1, " " );
                                                 $$ = realloc_concat($$, $2  ); }
         | NAME '<' temp '>' NAME              { $$ = malloc_concat ($1, "_" );
                                                 $$ = realloc_concat($$, $3  );
                                                 $$ = realloc_concat($$, " " );
                                                 $$ = realloc_concat($$, $5  ); }
         ;

more_var : NAME ',' more_var                   { $$ = malloc_concat ($1, ", ");
                                                 $$ = realloc_concat($$, $3  );
                                                 free($3); }
         | NAME                                { $$ = malloc_concat ($1, ""  ); }
         ;

temp     : NAME ',' temp                       { $$ = malloc_concat ($1, "_" );
                                                 $$ = realloc_concat($$, $3  );
                                                 free($3); }
         | NAME                                { $$ = malloc_concat ($1, ""  ); }
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
		CUR_FILE = argv[1];
		if (yyparse()) {
			yyerror("syntax error");
		}
	}
	exit(0);
}

yyerror(char msg[]) {
	fprintf(stderr, " %s.  Line %d\n", msg, yylineno);
}
