#
# UFRGS - Compiladores B - Marcelo Johann - 2021/ERE - Etapa 2
#
# Makefile for single compiler call
# All source files must be included from code embedded in scanner.l
# In our case, you probably need #include "hash.c" at the beginning
# and #include "main.c" in the last part of the scanner.l
#

etapa2: y.tab.c lex.yy.c 
	gcc -o etapa2 lex.yy.c hash.c
y.tab.c: parser.y
	yacc -d parser.y
lex.yy.c: scanner.l y.tab.c
	lex scanner.l

clean:
	rm lex.yy.c y.tab.c y.tab.h etapa2
