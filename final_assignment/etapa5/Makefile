#
# UFRGS - Compiladores B - Marcelo Johann - 2021/ERE - Etapa 3
#
# Makefile for single compiler call
# All source files must be included from code embedded in scanner.l
# In our case, you probably need #include "hash.c" at the beginning
# and #include "main.c" in the last part of the scanner.l
#

etapa5: main.o lex.yy.o y.tab.o hash.o ast.o decompiler.o semantic.o
	gcc main.o lex.yy.o y.tab.o hash.o ast.o decompiler.o semantic.o -o etapa5

main.o: main.c
	gcc -c main.c

semantic.o: semantic.c
	gcc -c semantic.c

hash.o: hash.c
	gcc -c hash.c

ast.o: ast.c
	gcc -c ast.c

decompiler.o: decompiler.c
	gcc -c decompiler.c

y.tab.o: y.tab.c
	gcc -c y.tab.c

lex.yy.o: lex.yy.c
	gcc -c lex.yy.c

y.tab.c: parser.y
	yacc -d -v parser.y
lex.yy.c: scanner.l y.tab.c
	lex scanner.l

clean:
	rm lex.yy.c y.tab.c y.tab.h y.output *.o etapa5
