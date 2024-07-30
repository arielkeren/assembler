/*
 * errorHandling.h
 *
 * Contains the function prototypes for the functions in errorHandling.c.
 *
 * Name: Ariel Keren
 * Course: C Lab, 20465
 * Semester: 2024B
 * Date: 30/07/2024
 */

#ifndef ERROR_HANDLING_H
#define ERROR_HANDLING_H

#include "globals.h"

void printMessage(char message[], char fileName[], LineNumber lineNumber, Boolean isError, Boolean isMacro);
void printError(char message[], char fileName[], LineNumber lineNumber);
void printMacroError(char message[], char fileName[], LineNumber lineNumber);
void printWarning(char message[], char fileName[], LineNumber lineNumber);
void printCriticalError(char message[]);

#endif
