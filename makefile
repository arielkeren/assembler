CC = gcc
FLAGS = -ansi -Wall -pedantic
OBJ_FLAGS = $(FLAGS) -c
EXE_FLAGS = $(FLAGS) -o
EXE_DEPS = assembler.o fileHandling.o errorHandling.o fileGeneration.o macroExpansion.o fileReading.o labelLinking.o freeingLogic.o encoder.o instructionInformation.o lineValidation.o utils.o macroTable.o wordList.o labelList.o foundLabelList.o usedLabelList.o

assembler: $(EXE_DEPS)
	$(CC) $(EXE_FLAGS) assembler $(EXE_DEPS)

assembler.o: assembler.c fileHandling.h globals.h
	$(CC) $(OBJ_FLAGS) assembler.c

fileHandling.o: fileHandling.c fileHandling.h errorHandling.h macroExpansion.h fileReading.h labelLinking.h fileGeneration.h freeingLogic.h macroTable.h wordList.h labelList.h foundLabelList.h usedLabelList.h globals.h
	$(CC) $(OBJ_FLAGS) fileHandling.c

errorHandling.o: errorHandling.c errorHandling.h globals.h
	$(CC) $(OBJ_FLAGS) errorHandling.c

fileGeneration.o: fileGeneration.c fileGeneration.h errorHandling.h labelList.h foundLabelList.h usedLabelList.h utils.h globals.h
	$(CC) $(OBJ_FLAGS) fileGeneration.c

macroExpansion.o: macroExpansion.c macroExpansion.h errorHandling.h macroTable.h lineValidation.h utils.h globals.h
	$(CC) $(OBJ_FLAGS) macroExpansion.c

fileReading.o: fileReading.c fileReading.h errorHandling.h encoder.h lineValidation.h instructionInformation.h macroTable.h labelList.h foundLabelList.h usedLabelList.h utils.h globals.h
	$(CC) $(OBJ_FLAGS) fileReading.c

labelLinking.o: labelLinking.c labelLinking.h errorHandling.h encoder.h labelList.h foundLabelList.h globals.h
	$(CC) $(OBJ_FLAGS) labelLinking.c

freeingLogic.o: freeingLogic.c freeingLogic.h wordList.h macroTable.h labelList.h foundLabelList.h usedLabelList.h globals.h
	$(CC) $(OBJ_FLAGS) freeingLogic.c

encoder.o: encoder.c encoder.h instructionInformation.h wordList.h utils.h globals.h
	$(CC) $(OBJ_FLAGS) encoder.c

instructionInformation.o: instructionInformation.c instructionInformation.h globals.h
	$(CC) $(OBJ_FLAGS) instructionInformation.c

lineValidation.o: lineValidation.c lineValidation.h errorHandling.h instructionInformation.h utils.h globals.h
	$(CC) $(OBJ_FLAGS) lineValidation.c

utils.o: utils.c utils.h errorHandling.h freeingLogic.h globals.h
	$(CC) $(OBJ_FLAGS) utils.c

macroTable.o: macroTable.c macroTable.h utils.h globals.h
	$(CC) $(OBJ_FLAGS) macroTable.c

wordList.o: wordList.c wordList.h utils.h globals.h
	$(CC) $(OBJ_FLAGS) wordList.c

labelList.o: labelList.c labelList.h utils.h globals.h
	$(CC) $(OBJ_FLAGS) labelList.c

foundLabelList.o: foundLabelList.c foundLabelList.h utils.h globals.h
	$(CC) $(OBJ_FLAGS) foundLabelList.c

usedLabelList.o: usedLabelList.c usedLabelList.h utils.h globals.h
	$(CC) $(OBJ_FLAGS) usedLabelList.c
