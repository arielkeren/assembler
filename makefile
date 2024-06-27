assembler: main.o fileHandling.o fileCreation.o macroExpansion.o macroTable.o firstPass.o secondPass.o lineValidation.o utils.o globals.o wordList.o labelList.o externLabelList.o usedLabelList.o foundLabelList.o
	gcc -ansi -Wall -pedantic -o assembler main.o fileHandling.o fileCreation.o macroExpansion.o macroTable.o firstPass.o secondPass.o lineValidation.o wordList.o labelList.o externLabelList.o usedLabelList.o foundLabelList.o utils.o globals.o

main.o: main.c fileHandling.h globals.h
	gcc -ansi -Wall -pedantic -c main.c

fileHandling.o: fileHandling.c fileHandling.h fileCreation.h macroExpansion.h firstPass.h secondPass.h wordList.h labelList.h usedLabelList.h globals.h
	gcc -ansi -Wall -pedantic -c fileHandling.c

fileCreation.o: fileCreation.c fileCreation.h utils.h globals.h
	gcc -ansi -Wall -pedantic -c fileCreation.c

macroExpansion.o: macroExpansion.c macroExpansion.h macroTable.h utils.h globals.h
	gcc -ansi -Wall -pedantic -c macroExpansion.c

macroTable.o: macroTable.c macroTable.h utils.h globals.h
	gcc -ansi -Wall -pedantic -c macroTable.c

firstPass.o: firstPass.c firstPass.h wordList.h labelList.h usedLabelList.h foundLabelList.h utils.h globals.h
	gcc -ansi -Wall -pedantic -c firstPass.c

secondPass.o: secondPass.c secondPass.h wordList.h labelList.h foundLabelList.h utils.h globals.h
	gcc -ansi -Wall -pedantic -c secondPass.c

lineValidation.o: lineValidation.c lineValidation.h wordList.h utils.h globals.h
	gcc -ansi -Wall -pedantic -c lineValidation.c

utils.o: utils.c utils.h globals.h
	gcc -ansi -Wall -pedantic -c utils.c

globals.o: globals.c globals.h
	gcc -ansi -Wall -pedantic -c globals.c

wordList.o: wordList.c wordList.h utils.h globals.h
	gcc -ansi -Wall -pedantic -c wordList.c

labelList.o: labelList.c labelList.h utils.h globals.h
	gcc -ansi -Wall -pedantic -c labelList.c

externLabelList.o: externLabelList.c externLabelList.h utils.h globals.h
	gcc -ansi -Wall -pedantic -c externLabelList.c

usedLabelList.o: usedLabelList.c usedLabelList.h utils.h globals.h
	gcc -ansi -Wall -pedantic -c usedLabelList.c

foundLabelList.o: foundLabelList.c foundLabelList.h utils.h globals.h
	gcc -ansi -Wall -pedantic -c foundLabelList.c
