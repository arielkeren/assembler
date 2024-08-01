/*
 * macroTable.h
 *
 * Contains the function prototypes for the functions in macroTable.c.
 *
 * Name: Ariel Keren
 * Course: C Lab, 20465
 * Semester: 2024B
 * Date: 30/07/2024
 */

#ifndef MACRO_TABLE_H
#define MACRO_TABLE_H

#include "globals.h" /* Typedefs. */

void addMacro(Macro **macroTable, char macroName[]);
void addMacroContent(Macro *macro, char content[]);
char *getMacroContent(Macro *macroTable, char macroName[]);
void freeMacroTable(Macro *macroTable);
void freeMacro(Macro *macro);

#endif
