#ifndef UTILS_H
#define UTILS_H

#include <stdio.h>

#include "globals.h"

void *allocate(size_t size);
FILE *openFile(char fileName[], char extension[], char mode[]);
char *skipWhitespace(char line[]);
char *skipCharacters(char line[]);
char *getNextToken(char line[]);
void removeEnding(char string[], char ending);
boolean checkIfLabel(char token[]);
boolean checkIfFollowedByComma(char line[]);
boolean checkForConsecutiveCommas(char line[]);
char *addExtension(char fileName[], char extension[]);
void printMessage(char message[], char fileName[], unsigned lineNumber, boolean isError, boolean isMacro);
void printError(char message[], char fileName[], unsigned lineNumber);
void printMacroError(char message[], char fileName[], unsigned lineNumber);
void printWarning(char message[], char fileName[], unsigned lineNumber);

#endif
