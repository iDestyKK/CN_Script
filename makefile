#
# CN_Script Makefile
# 
# Author:
#     Clara Nguyen
#

CC     = gcc
CFLAGS = -g --std=gnu89
PARSER = bison
PFLAGS = -d
LEX    = flex
LFLAGS = -t

all: cns

cns: cns.tab.c cns.yy.c sem.c
	$(CC) $(CFLAGS) -o $@ $^

cns.tab.c: cns.y
	$(PARSER) $(PFLAGS) $^

cns.yy.c: cns.lex
	$(LEX) $(LFLAGS) $^ > cns.yy.c

clean:
	$(RM) *.tab.* *.yy.c *.o cns
