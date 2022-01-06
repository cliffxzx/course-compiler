CFLAGS = -Wall

main: scanner.yy.c parser.tab.c
	gcc scanner.yy.c parser.tab.c -ll -o main ${CFLAGS}
scanner.yy.c: scanner.l
	flex -o scanner.yy.c scanner.l
parser.tab.c: parser.y
	yacc parser.y -do parser.tab.c
clean:
	@rm -rf main
	@trash scanner.yy.c parser.tab.c parser.tab.h