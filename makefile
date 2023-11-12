assambler: assambler.o preprocessor.o pass1and2.o output.o helpers.o list.o
	gcc -ansi -Wall -pedantic assambler.o preprocessor.o pass1and2.o output.o helpers.o list.o -o assambler -lm
assambler.o: assambler.c preprocessor.h pass1and2.h output.h memory.h
	gcc -c -ansi -Wall -pedantic assambler.c
preprocessor.o: preprocessor.c preprocessor.h
	gcc -c -ansi -Wall -pedantic preprocessor.c
pass1and2.o: pass1and2.c pass1and2.h
	gcc -c -ansi -Wall -pedantic pass1and2.c
output.o: output.c output.h
	gcc -c -ansi -Wall -pedantic output.c 
helpers.o: helpers.c helpers.h
	gcc -c -ansi -Wall -pedantic helpers.c
list.o: list.c list.h
	gcc -c -ansi -Wall -pedantic list.c
