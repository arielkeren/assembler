/*
 * utils.h
 *
 * Contains the function prototypes for the functions in utils.c.
 *
 * Name: Ariel Keren
 * Course: C Lab, 20465
 * Semester: 2024B
 * Date: 30/07/2024
 */

#ifndef UTILS_H
#define UTILS_H

#include <stdio.h> /* FILE, size_t. */

#include "globals.h" /* Typedefs. */

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
