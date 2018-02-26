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
%token <str> ID CON ARR_STR ARG_STR QUOTE_STR STR IMPORT NAME FNAME TYPE CIMPORT_STR

/* Passed by other means */
%token INT UINT FLOAT DOUBLE CHAR UCHAR STRING_LITERAL
%token FUNC_TOP FUNC LEFT_PAREN RIGHT_PAREN FEND ANGLE_LT ANGLE_GT OBJECT
%token IF ELSE ELSEIF FOR DO WHILE UNTIL REPEAT LOOP INCREMENT DECREMENT CONT_IF BRK_IF
%token EQ ADDEQ SUBEQ MULEQ DIVEQ MODEQ LSEQ RSEQ ANDEQ OREQ XOREQ LEQ GEQ NOTEQ AND OR XOR DEREFERENCE
%type <str> fargs arg stmt cnsexpr expr exprs decl_var more_var temp obj_decl type
%type <str> INT
%type <str> DOUBLE
%type <str> STRING_LITERAL

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
         | external object                     { }
         | external func                       { }
         ;

import   : IMPORT CIMPORT_STR                  { import($1, "<>"  , $2); }
         | IMPORT STRING_LITERAL               { import($1, "\"\"", $2); }
         ;

object   : obj_decl '{' obj_body '}' ';' { if (IN_OBJ != 2) printf("} %s;", $1); IN_OBJ = 0; free(CUR_OBJ); }
         ;

obj_decl : OBJECT '<' temp '>' NAME            { printf("typedef struct {");
                                                 $$ = $5; IN_OBJ = 1; CUR_OBJ = strdup($5); }
         | OBJECT              NAME            { printf("typedef struct {");
                                                 $$ = $2; IN_OBJ = 1; CUR_OBJ = strdup($2); }
         ;

obj_body :                                     { }
         | obj_body stmt ';'                   { printf("%s;", $2); }
         | obj_body func                       { }
         ;
    
func     :                                     { }
         | func func_decl '{' fbody '}'        { fend(); }
         ;

func_decl: FUNC '<' NAME '>' '<' temp '>' NAME fargs
                                               { if (IN_OBJ == 1) {
											         IN_OBJ = 2;
													 printf("}");
												 }
												 if (IN_OBJ == 2) {
													 printf(
														 "//%s\n%s %s_%s(%s* this%s%s) {",
														 $6,
														 $3,
														 CUR_OBJ,
														 $8,
														 CUR_OBJ,
														 ($9 == NULL) ? "" : ", ",
														 ($9 == NULL) ? "" : $9
													 );
												 }
												 else {
													 printf(
														 "//%s\n%s %s(%s) {",
														 $6,
														 $3,
														 $8,
														 ($9 == NULL) ? "" : $9
													 );
												 }
                                                 freeifnull($9);
												 freeifnull($6); }
         | FUNC '<' NAME '>' NAME fargs        { if (IN_OBJ == 1) {
												     IN_OBJ = 2;
													 printf("} %s;", CUR_OBJ);
												 }
												 if (IN_OBJ == 2) {
													 printf(
														"%s %s_%s(%s* this%s%s) {" ,
														$3,
														CUR_OBJ,
														$5,
														CUR_OBJ,
														($6 == NULL) ? "" : ", ",
														($6 == NULL) ? "" : $6
													 );
												 }
												 else {
													 printf(
														"%s %s(%s) {" ,
														$3,
														$5,
														($6 == NULL) ? "" : $6
													 );
												 }
                                                 freeifnull($6); }
         | FUNC NAME fargs                     { if (IN_OBJ == 1) {
		                                             IN_OBJ = 2;
													 printf("} %s;", CUR_OBJ); 
												 }
												 if (IN_OBJ == 2) {
													 printf(
														 "%s_%s(%s* this%s%s) {",
														 CUR_OBJ,
														 $2,
														 CUR_OBJ,
														 ($3 == NULL) ? "" : ", ",
														 ($3 == NULL) ? "" : $3
													 );
												 }
												 else {
													 printf(
														 "%s(%s) {",
														 $2,
														 ($3 == NULL) ? "" : $3
													 );
		                                         }
                                                 freeifnull($3); }
         ;

fargs    : '(' ')'                             { $$ = NULL; }
         | '(' arg ')'                         { $$ = $2; }
         ;

arg      : NAME NAME ',' arg                   { $$ = malloc_concat ($1, " " );
                                                 $$ = realloc_concat($$, $2  );
                                                 $$ = realloc_concat($$, ", ");
                                                 $$ = realloc_concat($$, $4  );
												 append_pair($2, $1);
                                                 free($4); }
         | NAME NAME                           { $$ = malloc_concat ($1, " " );
                                                 $$ = realloc_concat($$, $2  );
												 append_pair($2, $1); }
         ;

fbody    :                                     { }
         | fbody stmt ';'                      { printf("%s;", $2); }
		 | fbody CONT_IF '(' cnsexpr ')' ';'   { printf("if (%s) continue;", $4);
		                                         free($4); }
		 | fbody BRK_IF '(' cnsexpr ')' ';'    { printf("if (%s) break;", $4);
		                                         free($4); }
		 | fbody if_cond '{' fbody '}'         { printf("}"); }
         | fbody loop '{' fbody '}'            { printf("}"); }
         ;

if_cond  : ELSE IF '(' cnsexpr ')'             { printf("else if (%s) {", $4); }
		 | ELSEIF  '(' cnsexpr ')'             { printf("else if (%s) {", $3); }
		 | IF '(' cnsexpr ')'                  { printf("if (%s) {", $3); }
		 | ELSE                                { printf("else {"); }
		 ;

stmt     : cnsexpr                             { $$ = $1; }
         | decl_var                            { $$ = $1; }
         | NAME                                { $$ = $1; }
         ;

cnsexpr  : expr                                { $$ = $1; }
         ;

exprs    : expr                                { $$ = $1; }
         | exprs ',' expr                      { $$ = malloc_concat ($1, ", ");
                                                 $$ = realloc_concat($$, $3);
                                                 free($1); free($3); }
         ;


expr     :
           NAME '=' expr                       { $$ = malloc_concat ($1, " = ");
                                                 $$ = realloc_concat($$, $3);
                                                 free($3); }
         | '(' exprs ')' '=' NAME '(' ')'      { $$ = malloc_concat ($5, "()");
                                                 free($5); }
         | '(' exprs ')' '=' NAME '(' exprs ')'{ $$ = malloc_concat ($5, "(");
                                                 $$ = realloc_concat($$, $7);
                                                 $$ = realloc_concat($$, ")");
                                                 free($5); free($7); }
	     /* TODO: Find out why this is broken */
		 | '(' type ')' NAME                   { $$ = malloc_concat ("(", $2);
		                                         $$ = realloc_concat($$, ")");
												 $$ = realloc_concat($$, $4);
												 free($2); free($4); }
         | expr OR expr                        { $$ = malloc_concat ($1, " || ");
                                                 $$ = realloc_concat($$, $3);
                                                 free($1); free($3); }
         | expr XOR expr                       { $$ = malloc_concat ($1, " ^^ ");
                                                 $$ = realloc_concat($$, $3);
                                                 free($1); free($3); }
         | expr AND expr                       { $$ = malloc_concat ($1, " && ");
                                                 $$ = realloc_concat($$, $3);
                                                 free($1); free($3); }
         | expr '+' expr                       { $$ = malloc_concat ($1, " + ");
                                                 $$ = realloc_concat($$, $3);
                                                 free($1); free($3); }
         | '-' expr                            { $$ = malloc_concat ("-", $2);
                                                 free($2); }
         | expr '-' expr                       { $$ = malloc_concat ($1, " - ");
                                                 $$ = realloc_concat($$, $3);
                                                 free($1); free($3); }
         | expr '*' expr                       { $$ = malloc_concat ($1, " * ");
                                                 $$ = realloc_concat($$, $3);
                                                 free($1); free($3); }
         | expr '/' expr                       { $$ = malloc_concat ($1, " / ");
                                                 $$ = realloc_concat($$, $3);
                                                 free($1); free($3); }
         | expr '%' expr                       { $$ = malloc_concat ($1, " % ");
                                                 $$ = realloc_concat($$, $3);
                                                 free($1); free($3); }
         | expr '|' expr                       { $$ = malloc_concat ($1, " | ");
                                                 $$ = realloc_concat($$, $3);
                                                 free($1); free($3); }
         | expr '^' expr                       { $$ = malloc_concat ($1, " ^ ");
                                                 $$ = realloc_concat($$, $3);
                                                 free($1); free($3); }
         | '&' expr                            { $$ = malloc_concat ("&", $2);
                                                 freeifnull($2); }
         | expr '&' expr                       { $$ = malloc_concat ($1, " & ");
                                                 $$ = realloc_concat($$, $3);
                                                 freeifnull($1); freeifnull($3); }
         | expr '=' expr                       { $$ = malloc_concat ($1, " = ");
                                                 $$ = realloc_concat($$, $3);
                                                 free($1); free($3); }
         | expr DEREFERENCE expr               { $$ = malloc_concat ($1, "->");
                                                 $$ = realloc_concat($$, $3);
                                                 free($1); free($3); }
         | expr OREQ expr                      { $$ = malloc_concat ($1, " |= ");
                                                 $$ = realloc_concat($$, $3);
                                                 free($3); }
         | expr XOREQ expr                     { $$ = malloc_concat ($1, " ^= ");
                                                 $$ = realloc_concat($$, $3);
                                                 free($3); }
         | expr ANDEQ expr                     { $$ = malloc_concat ($1, " &= ");
                                                 $$ = realloc_concat($$, $3);
                                                 free($3); }
         | expr LSEQ expr                      { $$ = malloc_concat ($1, " <<= ");
                                                 $$ = realloc_concat($$, $3);
                                                 free($3); }
         | expr RSEQ expr                      { $$ = malloc_concat ($1, " >>= ");
                                                 $$ = realloc_concat($$, $3);
                                                 free($3); }
         | expr ADDEQ expr                     { $$ = malloc_concat ($1, " += ");
                                                 $$ = realloc_concat($$, $3);
                                                 free($3); }
         | expr SUBEQ expr                     { $$ = malloc_concat ($1, " -= ");
                                                 $$ = realloc_concat($$, $3);
                                                 free($3); }
         | expr MULEQ expr                     { $$ = malloc_concat ($1, " *= ");
                                                 $$ = realloc_concat($$, $3);
                                                 free($3); }
         | expr DIVEQ expr                     { $$ = malloc_concat ($1, " /= ");
                                                 $$ = realloc_concat($$, $3);
                                                 free($3); }
         | expr MODEQ expr                     { $$ = malloc_concat ($1, " %= ");
                                                 $$ = realloc_concat($$, $3);
                                                 free($3); }
		 | NAME INCREMENT                      { $$ = malloc_concat ($1, "++"); }
		 | NAME DECREMENT                      { $$ = malloc_concat ($1, "--"); }
		 | INCREMENT NAME                      { $$ = malloc_concat ("++", $2); }
		 | DECREMENT NAME                      { $$ = malloc_concat ("--", $2); }
         | expr EQ expr                        { $$ = malloc_concat ($1, " == ");
                                                 $$ = realloc_concat($$, $3);
                                                 free($1); free($3); }
         | expr NOTEQ expr                     { $$ = malloc_concat ($1, " != ");
                                                 $$ = realloc_concat($$, $3);
                                                 free($1); free($3); }
         | expr LEQ expr                       { $$ = malloc_concat ($1, " <= ");
                                                 $$ = realloc_concat($$, $3);
                                                 free($1); free($3); }
         | expr GEQ expr                       { $$ = malloc_concat ($1, " >= ");
                                                 $$ = realloc_concat($$, $3);
                                                 free($1); free($3); }
         | expr '<' expr                       { $$ = malloc_concat ($1, " < ");
                                                 $$ = realloc_concat($$, $3);
                                                 free($1); free($3); }
         | expr '>' expr                       { $$ = malloc_concat ($1, " > ");
                                                 $$ = realloc_concat($$, $3);
                                                 free($1); free($3); }
         | expr '<' '<' expr                   { $$ = malloc_concat ($1, " << ");
                                                 $$ = realloc_concat($$, $4);
                                                 free($1); free($4); }
         | expr '>' '>' expr                   { $$ = malloc_concat ($1, " >> ");
                                                 $$ = realloc_concat($$, $4);
                                                 free($1); free($4); }
		 | expr '.' NAME '(' ')'               { unsigned int i = 0;
												 VAR_PAIR* p;
												 for (; i < cn_vec_size(VAR_PAIRS); i++) {
													 p = (VAR_PAIR*) cn_vec_at(VAR_PAIRS, i);
													 if (strcmp(p->name, $1) == 0)
														break;
												 }
												 if (i == cn_vec_size(VAR_PAIRS))
													exit(1);

												 $$ = malloc_concat (p->type, "_");
												 $$ = realloc_concat($$, $3);
												 $$ = realloc_concat($$, "(&");
												 $$ = realloc_concat($$, $1);
												 $$ = realloc_concat($$, ")");
												 free($1);
												 free($3);
		                                       }
		 | expr '.' NAME '(' exprs ')'         { unsigned int i = 0;
												 VAR_PAIR* p;
												 for (; i < cn_vec_size(VAR_PAIRS); i++) {
													 p = (VAR_PAIR*) cn_vec_at(VAR_PAIRS, i);
													 if (strcmp(p->name, $1) == 0)
														break;
												 }
												 if (i == cn_vec_size(VAR_PAIRS))
													exit(1);

												 $$ = malloc_concat (p->type, "_");
												 $$ = realloc_concat($$, $3);
												 $$ = realloc_concat($$, "(&");
												 $$ = realloc_concat($$, $1);
												 $$ = realloc_concat($$, ", ");
												 $$ = realloc_concat($$, $5);
												 $$ = realloc_concat($$, ")");
												 free($1);
												 free($3);
												 free($5);
		                                       }
         | expr '.' expr                       { $$ = malloc_concat ($1, ".");
                                                 $$ = realloc_concat($$, $3);
                                                 free($1); free($3); }
		 | expr '[' expr ']'                   { $$ = malloc_concat ($1, "[");
		                                         $$ = realloc_concat($$, $3);
												 $$ = realloc_concat($$, "]");
												 free($1); free($3);
		                                       }
         | NAME '(' ')'                        { $$ = malloc_concat ($1, "()");
                                                 free($1); }
         | NAME '(' exprs ')'                  { $$ = malloc_concat ($1, "(");
                                                 $$ = realloc_concat($$, $3);
                                                 $$ = realloc_concat($$, ")");
                                                 free($1); free($3); }
         | '(' expr ')'                        { $$ = malloc_concat ("(", $2);
                                                 $$ = realloc_concat($$, ")");
                                                 free($2); }
         | '!' expr                            { $$ = malloc_concat ("!", $2);
                                                 free($2); }
         | INT                                 { $$ = $1; }
         | DOUBLE                              { $$ = $1; }
         | STRING_LITERAL                      { $$ = $1; }
         | NAME                                { $$ = $1; }
		 |                                     { $$ = ""; }
         ;

/* TODO: Implement For, Repeat, Do-While, and Do-Until loops */
loop     : FOR '(' expr ';' expr ';' expr ')'  { printf("for (%s; %s; %s) {", $3, $5, $7); }
         | WHILE '(' expr ')'                  { printf("while (%s) {" , $3); free($3); }
         | UNTIL '(' expr ')'                  { printf("while (!(%s)) {" , $3); free($3); }
         | REPEAT '(' expr ')'                 { printf(
													"int %s%d = 0; for (; %s%d < %s; %s%d++) {",
													CNS_REPEAT_VAR, REPEAT_TOTAL,
													CNS_REPEAT_VAR, REPEAT_TOTAL, $3,
													CNS_REPEAT_VAR, REPEAT_TOTAL
												 ); REPEAT_TOTAL++; free($3); }
         | LOOP                                { printf("while (1) {"); }
         | DO                                  { printf("do {"); }
         ;

type     : OBJECT NAME                         { $$ = malloc_concat ("struct", " ");
		                                         $$ = realloc_concat($$, $2 );
												 free($2);
												 if (CUR_TYPE != NULL) {
													free(CUR_TYPE);
													CUR_TYPE = NULL;
												 }
												 CUR_TYPE = strdup($$); }
         | NAME                                { $$ = $1;
		                                         if (CUR_TYPE != NULL) {
												     free(CUR_TYPE);
													 CUR_TYPE = NULL;
												 } 
												 CUR_TYPE = strdup($1); }
		 ;

decl_var : type expr ',' more_var              { $$ = malloc_concat ($1, " " );
                                                 $$ = realloc_concat($$, $2  );
                                                 $$ = realloc_concat($$, ", ");
                                                 $$ = realloc_concat($$, $4  );
                                                 freeifnull($1); free($4);
												 append_pair($2, CUR_TYPE); }
         | type '<' temp '>' expr ',' more_var { $$ = malloc_concat ($1, "_" );
                                                 $$ = realloc_concat($$, $3  );
                                                 $$ = realloc_concat($$, " " );
                                                 $$ = realloc_concat($$, $5  );
                                                 $$ = realloc_concat($$, ", ");
                                                 $$ = realloc_concat($$, $7  );
                                                 freeifnull($1); free($7);
												 append_pair($5, CUR_TYPE); }
         | type expr                           { $$ = malloc_concat ($1, " " );
                                                 $$ = realloc_concat($$, $2  );
												 freeifnull($1); 
												 append_pair($2, CUR_TYPE); }
         | type '<' temp '>' expr              { $$ = malloc_concat ($1, "_" );
                                                 $$ = realloc_concat($$, $3  );
                                                 $$ = realloc_concat($$, " " );
                                                 $$ = realloc_concat($$, $5  );
												 freeifnull($1);
												 append_pair($5, CUR_TYPE); }
         ;

more_var : expr ',' more_var                   { $$ = malloc_concat ($1, ", ");
                                                 $$ = realloc_concat($$, $3  );
                                                 free($3);
												 append_pair($1, CUR_TYPE); }
         | expr                                { $$ = malloc_concat ($1, ""  );
		                                         append_pair($1, CUR_TYPE); }
         ;

temp     : NAME ',' temp                       { $$ = malloc_concat ($1, "_" );
                                                 $$ = realloc_concat($$, $3  );
                                                 free($3); }
         | NAME                                { $$ = malloc_concat ($1, ""  ); }
%%

/* Parser */
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

extern int yylineno;
extern FILE* yyin;

main(int argc, char** argv) {
	REPEAT_TOTAL = 0;
	IN_OBJ = 0;
	VAR_PAIRS = cn_vec_init(VAR_PAIR);
	EXEC_LEN = readlink("/proc/self/exe", EXEC_PATH, MAX_PATH);
	if (EXEC_LEN < MAX_PATH && EXEC_LEN != -1)
		EXEC_PATH[EXEC_LEN] = '\0';

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
