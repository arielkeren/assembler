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
char *addExtension(char fileName[], char extension[]);
boolean contains(char string[], char *stringArray[], size_t arraySize);
size_t indexOf(char string[], char *stringArray[], size_t arraySize);

#endif
