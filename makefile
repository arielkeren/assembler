assembler: main.o macroTable.o macroExpansion.o utils.o firstPass.o
	gcc -ansi -Wall -pedantic -o assembler main.o macroTable.o macroExpansion.o utils.o firstPass.o

main.o: main.c
	gcc -ansi -Wall -pedantic -c main.c

macroTable.o: macroTable.c macroTable.h utils.h
	gcc -ansi -Wall -pedantic -c macroTable.c

macroExpansion.o: macroExpansion.c macroExpansion.h macroTable.h utils.h
	gcc -ansi -Wall -pedantic -c macroExpansion.c

utils.o: utils.c utils.h
	gcc -ansi -Wall -pedantic -c utils.c

firstPass.o: firstPass.c firstPass.h utils.h
	gcc -ansi -Wall -pedantic -c firstPass.c
