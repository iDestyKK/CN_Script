#
# CN_Script Makefile
# 
# Author:
#     Clara Nguyen
#

CC     = gcc
CFLAGS = --std=gnu89
PARSER = bison
PFLAGS = -d
LEX    = flex
LFLAGS = -t

all: cns

cns: cns.tab.c cns.yy.c cns.c
	$(CC) $(CFLAGS) -o $@ $^

cns.tab.c: cns.y
	$(PARSER) $(PFLAGS) $^
	#mv cns.tab.c cns.yacc.c
	#mv cns.tab.h cns.tab.h

cns.yy.c: cns.lex
	$(LEX) $(LFLAGS) $^ > cns.yy.c
	#mv lex.yy.c cns.yy.c

clean:
	$(RM) *.c *.o *.h
