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
 * Handles a potential macro definition in the given line.
 * Returns whether or not the definition is valid.
 * If the definition is valid, adds the macro.
 * If the definition is invalid, prints an error message.
 *
 * Assumes that the given pointer to the macros is not NULL.
 * Assumes that the given file name is not NULL and is null-terminated.
 * Assumes that the given line is not NULL and is null-terminated.
 *
 * @param macros The macro table.
 * @param fileName The name of the source file.
 * @param line The current line.
 * @param lineNumber The current line's line number.
 * @return TRUE if the definition is valid, FALSE otherwise.
 */
Boolean handleMacroDefinition(Macro **macros, char fileName[], char line[],
                              LineNumber lineNumber);

/**
 * Handles lines that are part of a macro definition.
 * Sets the isInsideMacro flag to FALSE if the given line is the macro's end.
 * Returns whether or not the line is valid.
 * If so and if it is not the end, adds the line to the macro's content.
 *
 * Assumes that the given macro pointer is not NULL.
 * Assumes that the given file name is not NULL and is null-terminated.
 * Assumes that the given line is not NULL and is null-terminated.
 * Assumes that the given isInsideMacro pointer is not NULL.
 *
 * @param macro The macro that this line is a part of.
 * @param fileName The name of the source file.
 * @param line The current line.
 * @param lineNumber The current line's line number.
 * @param isInsideMacro The pointer to the isInsideMacro flag.
 */
Boolean handleInsideMacro(Macro *macro, char fileName[], char line[],
                          LineNumber lineNumber, Boolean *isInsideMacro);

/**
 * If the given line contains ONLY an already-defined macro's name, expands
 * the macro's content into the given output file. Otherwise, writes the
 * given line into the given output file.
 *
 * Assumes that the given output file pointer is not NULL.
 * Assumes that the given line is not NULL and is null-terminated.
 * Assumes that the given token is not NULL and is null-terminated.
 *
 * @param outputFile The .am file pointer.
 * @param macros The macro table.
 * @param line The current line.
 * @param token The first token in the line (to check for a macro).
 */
void writeLine(FILE *outputFile, Macro *macros, char line[], char token[]);

#endif
