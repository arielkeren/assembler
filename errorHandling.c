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

/**
 * Prints a message to stdout according to the arguments given.
 *
 * @param message The message to print.
 * @param fileName The name of the file in which the error occurred.
 * @param lineNumber The line number in the file in which the error occurred.
 * @param isError Whether the message is an error or a warning.
 * @param isMacro Whether the message is a macro error or not.
 */
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

/**
 * Prints an error in the .am file to stdout according to the arguments given.
 *
 * @param message The message to print.
 * @param fileName The name of the file in which the error occurred.
 * @param lineNumber The line number in the file in which the error occurred.
 */
void printError(char message[], char fileName[], LineNumber lineNumber) {
    printMessage(message, fileName, lineNumber, TRUE, FALSE);
}

/**
 * Prints a macro error in the .as file to stdout according to the arguments given.
 *
 * @param message The message to print.
 * @param fileName The name of the file in which the error occurred.
 * @param lineNumber The line number in the file in which the error occurred.
 */
void printMacroError(char message[], char fileName[], LineNumber lineNumber) {
    printMessage(message, fileName, lineNumber, TRUE, TRUE);
}

/**
 * Prints a warning in the .am file to stdout according to the arguments given.
 *
 * @param message The message to print.
 * @param fileName The name of the file in which the error occurred.
 * @param lineNumber The line number in the file in which the error occurred.
 */
void printWarning(char message[], char fileName[], LineNumber lineNumber) {
    printMessage(message, fileName, lineNumber, FALSE, FALSE);
}

/**
 * Prints a critical error to stdout with the given message.
 *
 * @param message The message to print.
 */
void printCriticalError(char message[]) {
    printf("\n--- Critical Error ---\n");
    printf("%s\n", message);
}
