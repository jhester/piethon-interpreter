CCC = g++
LEX = flex
YACC = bison

CFLAGS= -O0 -Wall -fprofile-arcs -ftest-coverage
LEXFLAGS = -Wno-unused

LEXDEBUG = -d
YACCDEBUG = -vtd --graph

OBJS = main.o ast.o parse.tab.o lex.yy.o

run: $(OBJS)
	$(CCC) $(CFLAGS) -o run $(OBJS)

main.o: main.cpp parse.y scan.l parse.tab.c lex.yy.c
	$(CCC) $(CFLAGS) -c main.cpp

parse.tab.c: parse.y ast.h
	$(YACC) $(YACCDEBUG) parse.y

ast.o: ast.cpp ast.h
	$(CCC) $(CFLAGS) -c ast.cpp

parse.tab.o: parse.tab.c
	$(CCC) $(CFLAGS) -c parse.tab.c

lex.yy.c: scan.l parse.tab.o
	$(LEX) $(LEXDEBUG) scan.l

lex.yy.o: lex.yy.c
	$(CCC) $(CFLAGS) $(LEXFLAGS) -c lex.yy.c
	
clean:
	rm -f main run *.o parse.tab.c lex.yy.c
	rm -f parse.tab.h
	rm -f parse.output
	rm -f *.gcda *.gcno *.bb *.bbg *.da *.gcov
	rm -f ast.gv ast.pdf

view:
	dot -Tpdf AST.gv -o ast.pdf
	open ast.pdf