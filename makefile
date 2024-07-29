assembler: assembler.o fileHandling.o errorHandling.o fileGeneration.o macroExpansion.o fileReading.o labelLinking.o freeingLogic.o encoder.o instructionInformation.o lineValidation.o utils.o macroTable.o wordList.o labelList.o foundLabelList.o usedLabelList.o
	gcc -ansi -Wall -pedantic -o assembler assembler.o fileHandling.o errorHandling.o fileGeneration.o macroExpansion.o fileReading.o labelLinking.o freeingLogic.o encoder.o instructionInformation.o lineValidation.o utils.o macroTable.o wordList.o labelList.o foundLabelList.o usedLabelList.o

assembler.o: assembler.c fileHandling.h globals.h
	gcc -ansi -Wall -pedantic -c assembler.c

fileHandling.o: fileHandling.c fileHandling.h errorHandling.h macroExpansion.h fileReading.h labelLinking.h fileGeneration.h freeingLogic.h macroTable.h wordList.h labelList.h foundLabelList.h usedLabelList.h utils.h globals.h
	gcc -ansi -Wall -pedantic -c fileHandling.c

errorHandling.o: errorHandling.c errorHandling.h globals.h
	gcc -ansi -Wall -pedantic -c errorHandling.c

fileGeneration.o: fileGeneration.c fileGeneration.h errorHandling.h labelList.h foundLabelList.h utils.h globals.h
	gcc -ansi -Wall -pedantic -c fileGeneration.c

macroExpansion.o: macroExpansion.c macroExpansion.h errorHandling.h macroTable.h lineValidation.h utils.h globals.h
	gcc -ansi -Wall -pedantic -c macroExpansion.c

fileReading.o: fileReading.c fileReading.h errorHandling.h encoder.h lineValidation.h instructionInformation.h macroTable.h labelList.h foundLabelList.h usedLabelList.h utils.h globals.h
	gcc -ansi -Wall -pedantic -c fileReading.c

labelLinking.o: labelLinking.c labelLinking.h errorHandling.h encoder.h labelList.h foundLabelList.h utils.h globals.h
	gcc -ansi -Wall -pedantic -c labelLinking.c

freeingLogic.o: freeingLogic.c freeingLogic.h wordList.h macroTable.h labelList.h foundLabelList.h usedLabelList.h globals.h
	gcc -ansi -Wall -pedantic -c freeingLogic.c

encoder.o: encoder.c encoder.h instructionInformation.h wordList.h utils.h globals.h
	gcc -ansi -Wall -pedantic -c encoder.c

instructionInformation.o: instructionInformation.c instructionInformation.h globals.h
	gcc -ansi -Wall -pedantic -c instructionInformation.c

lineValidation.o: lineValidation.c lineValidation.h errorHandling.h instructionInformation.h utils.h globals.h
	gcc -ansi -Wall -pedantic -c lineValidation.c

utils.o: utils.c utils.h errorHandling.h freeingLogic.h globals.h
	gcc -ansi -Wall -pedantic -c utils.c

macroTable.o: macroTable.c macroTable.h utils.h globals.h
	gcc -ansi -Wall -pedantic -c macroTable.c

wordList.o: wordList.c wordList.h utils.h globals.h
	gcc -ansi -Wall -pedantic -c wordList.c

labelList.o: labelList.c labelList.h utils.h globals.h
	gcc -ansi -Wall -pedantic -c labelList.c

foundLabelList.o: foundLabelList.c foundLabelList.h utils.h globals.h
	gcc -ansi -Wall -pedantic -c foundLabelList.c

usedLabelList.o: usedLabelList.c usedLabelList.h utils.h globals.h
	gcc -ansi -Wall -pedantic -c usedLabelList.c
