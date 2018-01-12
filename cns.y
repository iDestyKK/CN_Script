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
%token INT UINT FLOAT DOUBLE CHAR UCHAR STRING_LITERAL
%token FUNC_TOP FUNC LEFT_PAREN RIGHT_PAREN FEND ANGLE_LT ANGLE_GT OBJECT
%token IF ELSE FOR DO WHILE UNTIL REPEAT LOOP
%token EQ ADDEQ SUBEQ MULEQ DIVEQ MODEQ LSEQ RSEQ ANDEQ OREQ XOREQ LEQ GEQ NOTEQ AND OR XOR
%type <str> fargs arg stmt cnsexpr expr exprs decl_var more_var temp obj_decl
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

import   : IMPORT '<' NAME '>'                 { import($1, "<>"  , $3); }
         | IMPORT '"' NAME '"'                 { import($1, "\"\"", $3); }
         ;

object   : obj_decl '{' obj_body '}' ';' { printf("} %s;", $1); }
         ;

obj_decl : OBJECT '<' temp '>' NAME            { printf("typedef struct {");
                                                 $$ = $5; }
         | OBJECT              NAME            { printf("typedef struct {");
                                                 $$ = $2; }
         ;

obj_body :                                     { }
         | obj_body stmt ';'                   { printf("%s;", $2); }
         ;
    
func     :                                     { }
         | func func_decl '{' fbody '}'        { fend(); }
         ;

func_decl: FUNC '<' NAME '>' '<' temp '>' NAME fargs
                                               { printf("//%s\n%s %s(%s) {", $6, $3, $8, ($9 == NULL) ? "" : $9);
                                                 freeifnull($9); freeifnull($6); }
         | FUNC '<' NAME '>' NAME fargs        { printf("%s %s(%s) {", $3, $5, ($6 == NULL) ? "" : $6);
                                                 freeifnull($6); }
         | FUNC NAME fargs                     { printf("%s(%s) {", $2, ($3 == NULL) ? "" : $3);
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
         | fbody loop '{' fbody '}'            { printf("}"); }
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
         | expr '&' expr                       { $$ = malloc_concat ($1, " & ");
                                                 $$ = realloc_concat($$, $3);
                                                 free($1); free($3); }
         | expr '=' expr                       { $$ = malloc_concat ($1, " = ");
                                                 $$ = realloc_concat($$, $3);
                                                 free($1); free($3); }
         | NAME OREQ expr                      { $$ = malloc_concat ($1, " |= ");
                                                 $$ = realloc_concat($$, $3);
                                                 free($3); }
         | NAME XOREQ expr                     { $$ = malloc_concat ($1, " ^= ");
                                                 $$ = realloc_concat($$, $3);
                                                 free($3); }
         | NAME ANDEQ expr                     { $$ = malloc_concat ($1, " &= ");
                                                 $$ = realloc_concat($$, $3);
                                                 free($3); }
         | NAME LSEQ expr                      { $$ = malloc_concat ($1, " <<= ");
                                                 $$ = realloc_concat($$, $3);
                                                 free($3); }
         | NAME RSEQ expr                      { $$ = malloc_concat ($1, " >>= ");
                                                 $$ = realloc_concat($$, $3);
                                                 free($3); }
         | NAME ADDEQ expr                     { $$ = malloc_concat ($1, " += ");
                                                 $$ = realloc_concat($$, $3);
                                                 free($3); }
         | NAME SUBEQ expr                     { $$ = malloc_concat ($1, " -= ");
                                                 $$ = realloc_concat($$, $3);
                                                 free($3); }
         | NAME MULEQ expr                     { $$ = malloc_concat ($1, " *= ");
                                                 $$ = realloc_concat($$, $3);
                                                 free($3); }
         | NAME DIVEQ expr                     { $$ = malloc_concat ($1, " /= ");
                                                 $$ = realloc_concat($$, $3);
                                                 free($3); }
         | NAME MODEQ expr                     { $$ = malloc_concat ($1, " %= ");
                                                 $$ = realloc_concat($$, $3);
                                                 free($3); }
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
         ;

/* TODO: Implement For, Repeat, Do-While, and Do-Until loops */
loop     : FOR '(' ')'                         { printf("for () {"); }
         | WHILE '(' expr ')'                  { printf("while (%s) {" , $3); free($3); }
         | UNTIL '(' expr ')'                  { printf("while (!(%s)) {" , $3); free($3); }
         | REPEAT '(' expr ')'                 { printf("repeat (%s) {", $3); free($3); }
         | LOOP                                { printf("while (1) {"); }
         | DO                                  { printf("do {"); }
         ;

decl_var : NAME expr ',' more_var              { $$ = malloc_concat ($1, " " );
                                                 $$ = realloc_concat($$, $2  );
                                                 $$ = realloc_concat($$, ", ");
                                                 $$ = realloc_concat($$, $4  );
                                                 free($4); }
         | NAME '<' temp '>' expr ',' more_var { $$ = malloc_concat ($1, "_" );
                                                 $$ = realloc_concat($$, $3  );
                                                 $$ = realloc_concat($$, " " );
                                                 $$ = realloc_concat($$, $5  );
                                                 $$ = realloc_concat($$, ", ");
                                                 $$ = realloc_concat($$, $7  );
                                                 free($7); }
         | NAME expr                           { $$ = malloc_concat ($1, " " );
                                                 $$ = realloc_concat($$, $2  ); }
         | NAME '<' temp '>' expr              { $$ = malloc_concat ($1, "_" );
                                                 $$ = realloc_concat($$, $3  );
                                                 $$ = realloc_concat($$, " " );
                                                 $$ = realloc_concat($$, $5  ); }
         ;

more_var : expr ',' more_var                   { $$ = malloc_concat ($1, ", ");
                                                 $$ = realloc_concat($$, $3  );
                                                 free($3); }
         | expr                                { $$ = malloc_concat ($1, ""  ); }
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
