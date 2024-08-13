/*
 * errorHandling.c
 *
 * Contains functions to print the different errors and warnings that could
 * occur.
 *
 * Name: Ariel Keren
 * Course: C Lab, 20465
 * Semester: 2024B
 * Date: 30/07/2024
 */

#include "errorHandling.h"

#include <stdio.h> /* printf. */

#include "globals.h" /* Constants and typedefs. */

void printMessage(char message[], char fileName[], LineNumber lineNumber,
                  Boolean isError, Boolean isMacro) {
    static unsigned long errorCount = INITIAL_VALUE;   /* Errors so far. */
    static unsigned long warningCount = INITIAL_VALUE; /* Warnings so far. */

    /* Specify the type of the message. */
    if (isError) {
        printf("\n--- Error #%lu ---\n", ++errorCount);
    } else {
        printf("\n--- Warning #%lu ---\n", ++warningCount);
    }

    /* Specify the file's name and extension. */
    printf("File: %s.a%c\n", fileName, isMacro ? 's' : 'm');
    /* Specify the line number. */
    printf("Line: %lu\n", lineNumber);
    /* Finally, print the message. */
    printf("%s\n", message);
}

void printError(char message[], char fileName[], LineNumber lineNumber) {
    /* Print an error that was found in the .am file. */
    printMessage(message, fileName, lineNumber, TRUE, FALSE);
}

void printMacroError(char message[], char fileName[], LineNumber lineNumber) {
    /* Print an error that was found in the .as file. */
    printMessage(message, fileName, lineNumber, TRUE, TRUE);
}

void printWarning(char message[], char fileName[], LineNumber lineNumber) {
    /* Print a warning that was found in the .am file. */
    printMessage(message, fileName, lineNumber, FALSE, FALSE);
}

void printNameError(char message[], char fileName[], LineNumber lineNumber,
                    Boolean isMacro) {
    static unsigned long nameErrorCount = INITIAL_VALUE; /* Errors so far. */

    /* Specify the type of the message. */
    printf("\n--- Name Error #%lu ---\n", ++nameErrorCount);
    /* Specify the file's name and extension. */
    printf("File: %s.a%c\n", fileName, isMacro ? 's' : 'm');
    /* Specify the line number. */
    printf("Line: %lu\n", lineNumber);
    /* Finally, print the message. */
    printf("%s %s\n", isMacro ? "Macro" : "Label", message);
}

void printFileError(char fileName[]) {
    printf("\n--- File Error ---\n");
    printf("Could not open the file by the name of: %s.\n", fileName);
    printf(
        "Moving on to the next file, or exiting if there are no more "
        "files...\n");
}

void printAllocationError() {
    printf("\n--- Allocation Error ---\n");
    printf("Failed to allocate enough memory.\n");
    printf("Exiting the program...\n");
}

void printNoFilesError() {
    printf("\n--- No Files Error ---\n");
    printf("No files provided to compile.\n");
    printf(
        "Add the .as files' names to compile (without the .as extensions) as "
        "command line arguments.\n");
    printf("Exiting the program...\n");
}
