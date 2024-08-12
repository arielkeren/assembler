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

/**
 * Prints a message to stdout according to the arguments given.
 *
 * Assumes that the given message is not NULL and is null-terminated.
 * Assumes that the given file name is not NULL and is null-terminated.
 *
 * @param message The message to print.
 * @param fileName The name of the file in which the error occurred.
 * @param lineNumber The line number in the file in which the error occurred.
 * @param isError Whether the message is an error or a warning.
 * @param isMacro Whether the message is a macro error or not.
 */
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

/**
 * Prints an error in the .am file to stdout according to the arguments given.
 *
 * Assumes that the given message is not NULL and is null-terminated.
 * Assumes that the given file name is not NULL and is null-terminated.
 *
 * @param message The message to print.
 * @param fileName The name of the file in which the error occurred.
 * @param lineNumber The line number in the file in which the error occurred.
 */
void printError(char message[], char fileName[], LineNumber lineNumber) {
    /* Print an error that was found in the .am file. */
    printMessage(message, fileName, lineNumber, TRUE, FALSE);
}

/**
 * Prints a macro error in the .as file to stdout according to the arguments
 * given.
 *
 * Assumes that the given message is not NULL and is null-terminated.
 * Assumes that the given file name is not NULL and is null-terminated.
 *
 * @param message The message to print.
 * @param fileName The name of the file in which the error occurred.
 * @param lineNumber The line number in the file in which the error occurred.
 */
void printMacroError(char message[], char fileName[], LineNumber lineNumber) {
    /* Print an error that was found in the .as file. */
    printMessage(message, fileName, lineNumber, TRUE, TRUE);
}

/**
 * Prints a warning in the .am file to stdout according to the arguments given.
 *
 * Assumes that the given message is not NULL and is null-terminated.
 * Assumes that the given file name is not NULL and is null-terminated.
 *
 * @param message The message to print.
 * @param fileName The name of the file in which the error occurred.
 * @param lineNumber The line number in the file in which the error occurred.
 */
void printWarning(char message[], char fileName[], LineNumber lineNumber) {
    /* Print a warning that was found in the .am file. */
    printMessage(message, fileName, lineNumber, FALSE, FALSE);
}

/**
 * Prints an error in the specified file to stdout according to the arguments
 * given.
 * Adds "Macro" or "Label" with a space after it to the start of the message,
 * depending on the value of isMacro.
 * So, the given message should not start with an uppercase letter.
 *
 * Assumes that the given message is not NULL and is null-terminated.
 * Assumes that the given file name is not NULL and is null-terminated.
 *
 * @param message The message to print (should not start with an uppercase).
 * @param fileName The name of the file in which the error occurred.
 * @param lineNumber The line number in the file in which the error occurred.
 * @param isMacro Whether the invalid name is of a macro or of a label.
 */
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

/**
 * Prints a file error to stdout with the given file name.
 * A file error can occur if some file could not be opened for some reason.
 *
 * Assumes that the given file name is not NULL and is null-terminated.
 *
 * @param fileName The name of the file which could not be opened.
 */
void printFileError(char fileName[]) {
    printf("\n--- File Error ---\n");
    printf("Could not open the file by the name of: %s.\n", fileName);
    printf(
        "Moving on to the next file, or exiting if there are no more "
        "files...\n");
}

/**
 * Prints a allocation error to stdout.
 * An allocation error can occur if some memory could not be allocated for some
 * reason.
 */
void printAllocationError() {
    printf("\n--- Allocation Error ---\n");
    printf("Failed to allocate enough memory.\n");
    printf("Exiting the program...\n");
}

/**
 * Prints a no files error to stdout.
 * A no files error can occur if no files have been provided as command line
 * arguments.
 */
void printNoFilesError() {
    printf("\n--- No Files Error ---\n");
    printf("No files provided to compile.\n");
    printf(
        "Add the .as files' names to compile (without the .as extensions) as "
        "command line arguments.\n");
    printf("Exiting the program...\n");
}
