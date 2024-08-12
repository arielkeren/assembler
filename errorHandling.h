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

#include "globals.h" /* Typedefs. */

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
                  Boolean isError, Boolean isMacro);

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
void printError(char message[], char fileName[], LineNumber lineNumber);

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
void printMacroError(char message[], char fileName[], LineNumber lineNumber);

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
void printWarning(char message[], char fileName[], LineNumber lineNumber);

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
                    Boolean isMacro);

/**
 * Prints a file error to stdout with the given file name.
 * A file error can occur if some file could not be opened for some reason.
 *
 * Assumes that the given file name is not NULL and is null-terminated.
 *
 * @param fileName The name of the file which could not be opened.
 */
void printFileError(char fileName[]);

/**
 * Prints a allocation error to stdout.
 * An allocation error can occur if some memory could not be allocated for some
 * reason.
 */
void printAllocationError();

/**
 * Prints a no files error to stdout.
 * A no files error can occur if no files have been provided as command line
 * arguments.
 */
void printNoFilesError();

#endif
