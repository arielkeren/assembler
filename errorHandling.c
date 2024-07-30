/*
 * errorHandling.c
 *
 * Contains functions to print the different errors and warnings that could occur.
 *
 * Name: Ariel Keren
 * Course: C Lab, 20465
 * Semester: 2024B
 * Date: 30/07/2024
 */

#include "errorHandling.h"

#include <stdio.h> /* printf. */

#include "globals.h" /* Constants and typedefs. */

void printMessage(char message[], char fileName[], LineNumber lineNumber, Boolean isError, Boolean isMacro) {
    static unsigned long errorCount = INITIAL_VALUE;
    static unsigned long warningCount = INITIAL_VALUE;

    if (isError) {
        printf("\n--- Error #%lu ---\n", ++errorCount);
    } else {
        printf("\n--- Warning #%lu ---\n", ++warningCount);
    }

    printf("File: %s.a%c\n", fileName, isMacro ? 's' : 'm');
    printf("Line: %hu\n", lineNumber);
    printf("%s\n", message);
}

void printError(char message[], char fileName[], LineNumber lineNumber) {
    printMessage(message, fileName, lineNumber, TRUE, FALSE);
}

void printMacroError(char message[], char fileName[], LineNumber lineNumber) {
    printMessage(message, fileName, lineNumber, TRUE, TRUE);
}

void printWarning(char message[], char fileName[], LineNumber lineNumber) {
    printMessage(message, fileName, lineNumber, FALSE, FALSE);
}

void printCriticalError(char message[]) {
    printf("\n--- Critical Error ---\n");
    printf("%s\n", message);
}
