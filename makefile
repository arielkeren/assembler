assembler: main.o macroTable.o macroExpansion.o
	gcc -ansi -Wall -pedantic -o assembler main.o macroTable.o macroExpansion.o

main.o: main.c
	gcc -ansi -Wall -pedantic -c main.c

macroTable.o: macroTable.c macroTable.h
	gcc -ansi -Wall -pedantic -c macroTable.c

macroExpansion.o: macroExpansion.c macroExpansion.h macroTable.h
	gcc -ansi -Wall -pedantic -c macroExpansion.c
