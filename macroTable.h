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

/**
 * Adds a new macro to the given macro table with the given name.
 * IMPORTANT: The caller must free the new macro.
 *
 * Assumes that the given pointer to the macros is not NULL.
 * Assumes that the given macro name is not NULL and is null-terminated.
 *
 * @param macros The macro table to add the macro to.
 * @param macroName The name of the new macro.
 */
void addMacro(Macro **macros, char macroName[]);

/**
 * Appends the given content to the given macro's content.
 *
 * Assumes that the given macro pointer is not NULL.
 * Assumes that the given content is not NULL and is null-terminated.
 *
 * @param macro The macro to add the content to.
 * @param content The content to add.
 */
void addMacroContent(Macro *macro, char content[]);

/**
 * Searches for the given macro name in the given macro table.
 * If found, returns its content.
 * If not found, returns NULL.
 *
 * Assumes that the given macro name is not NULL and is null-terminated.
 *
 * @param macros The macro table.
 * @param macroName The macro name to search for.
 * @return The macro's content, or NULL if not found.
 */
char *getMacroContent(Macro *macros, char macroName[]);

/**
 * Frees a macro table.
 *
 * Assumes that the given macro table has not been freed yet.
 *
 * @param macros The macro table to free.
 */
void freeMacroTable(Macro *macros);

/**
 * Frees a single macro.
 *
 * Assumes that the given macro is not NULL and has not been freed yet.
 *
 * @param macro The macro to free.
 */
void freeMacro(Macro *macro);

#endif
