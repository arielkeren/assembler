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
Length getStringLength(char string[]);
Boolean checkStringEnding(char string[]);
Boolean checkIfLabel(char token[]);
Boolean checkIfFollowedByComma(char line[]);
Boolean checkForConsecutiveCommas(char line[]);
char *addExtension(char fileName[], char extension[]);
unsigned char convertDigitToNumber(char digit);

#endif
