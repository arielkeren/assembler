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

#include <stdio.h>

#include "globals.h"

Boolean expandMacros(char fileName[], Macro **macros);
Boolean expandFileMacros(FILE *inputFile, FILE *outputFile, Macro **macros, char fileName[]);
Boolean expandLineMacros(FILE *inputFile, FILE *outputFile, Macro **macros, char fileName[], char line[], LineNumber lineNumber, Boolean *isInsideMacro);
Boolean isEndOfMacro(char line[]);

#endif
