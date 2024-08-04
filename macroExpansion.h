/*
 * macroExpansion.h
 *
 * Contains the function prototypes for the functions in macroExpansion.c.
 *
 * Name: Ariel Keren
 * Course: C Lab, 20465
 * Semester: 2024B
 * Date: 30/07/2024
 */

#ifndef MACRO_EXPANSION_H
#define MACRO_EXPANSION_H

#include <stdio.h> /* FILE. */

#include "globals.h" /* Typedefs. */

/**
 * Expands the macros in the .as file.
 * Opens the .as for reading and the .am for writing.
 * Generates the expanded .am file, which is used in later stages of the
 * compilation.
 * Returns whether or not the macro expansion was successful.
 *
 * Assumes that the given file name is not NULL and is null-terminated.
 * Assumes that the given pointer to the macros is not NULL.
 *
 * @param fileName The name of the source file.
 * @param macros The macro table.
 * @return TRUE if the macro expansion was successful, FALSE otherwise.
 */
Boolean expandMacros(char fileName[], Macro **macros);

/**
 * Expands the macros in the .as file line by line.
 * Inserts each line into the .am file if they don't contain macros.
 * Remembers definitions of macros and expands them when seen.
 * Returns whether or not the macro expansion was successful.
 *
 * Assumes that the given input file pointer is not NULL.
 * Assumes that the given output file pointer is not NULL.
 * Assumes that the given pointer to the macros is not NULL.
 * Assumes that the given file name is not NULL and is null-terminated.
 *
 * @param inputFile The .as file.
 * @param outputFile The .am file.
 * @param macros The macro table.
 * @param fileName The name of the source file.
 * @return TRUE if the macro expansion was successful, FALSE otherwise.
 */
Boolean expandFileMacros(FILE *inputFile, FILE *outputFile, Macro **macros,
                         char fileName[]);

/**
 * Handles each line of the .as file.
 * Determines whether the line contains a macro defintion.
 * If so, fills in the macro's content.
 * If there is a reference to an already-defined macro, inserts its content into
 * the .am file.
 * Otherwise, just inserts the current line into the .am file.
 * Returns whether or not the handling the line was successful.
 *
 * Assumes that the given input file pointer is not NULL.
 * Assumes that the given output file pointer is not NULL.
 * Assumes that the given pointer to the macros is not NULL.
 * Assumes that the given file name is not NULL and is null-terminated.
 * Assumes that the given line is not NULL and is null-terminated.
 * Assumes that the given pointer to whether or not this line is inside a macro
 * is not NULL.
 *
 * @param inputFile The .as file.
 * @param outputFile The .am file.
 * @param macros The macro table.
 * @param fileName The name of the source file.
 * @param line The current line.
 * @param lineNumber The current line's line number.
 * @param isInsideMacro Whether or not the current line is part of some macro's
 * definition.
 * @return TRUE if the handling of the line was successful, FALSE otherwise.
 */
Boolean expandLineMacros(FILE *inputFile, FILE *outputFile, Macro **macros,
                         char fileName[], char line[], LineNumber lineNumber,
                         Boolean *isInsideMacro);

/**
 * Checks and returns whether the given line is the end of some macro's
 * definition.
 *
 * Assumes that the given line is not NULL and is null-terminated.
 *
 * @param line The line to check.
 * @return TRUE if the line is the end of some macro's definition, FALSE
 * otherwise.
 */
Boolean isEndOfMacro(char line[]);

#endif
