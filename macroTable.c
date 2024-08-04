/*
 * macroTable.c
 *
 * Contains functions to interface with a macro table, to search through it and
 * finally to free it.
 * The macro table contains all the macro definitions in the .as file, along
 * with their contents.
 *
 * Name: Ariel Keren
 * Course: C Lab, 20465
 * Semester: 2024B
 * Date: 30/07/2024
 */

#include "macroTable.h"

#include <stdlib.h> /* free. */
#include <string.h> /* strcmp, strlen, strcpy, strcat. */

#include "globals.h" /* Constants and typedefs. */
#include "utils.h"   /* Allocating memory. */

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
void addMacro(Macro **macros, char macroName[]) {
    Macro *newMacro;

    newMacro = allocate(sizeof(Macro));
    newMacro->name = macroName;
    newMacro->content = "";
    newMacro->next = *macros;
    *macros = newMacro;
}

/**
 * Appends the given content to the given macro's content.
 *
 * Assumes that the given macro pointer is not NULL.
 * Assumes that the given content is not NULL and is null-terminated.
 *
 * @param macro The macro to add the content to.
 * @param content The content to add.
 */
void addMacroContent(Macro *macro, char content[]) {
    char *newContent;

    newContent = allocate(
        sizeof(char) * (strlen(macro->content) + strlen(content) + NULL_BYTE));

    strcpy(newContent, macro->content);
    free(macro->content);
    strcat(newContent, content);

    macro->content = newContent;
}

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
char *getMacroContent(Macro *macros, char macroName[]) {
    while (macros != NULL) {
        if (strcmp(macros->name, macroName) == EQUAL_STRINGS) {
            return macros->content;
        }

        macros = macros->next;
    }

    return NULL;
}

/**
 * Frees a macro table.
 *
 * Assumes that the given macro table has not been freed yet.
 *
 * @param macros The macro table to free.
 */
void freeMacroTable(Macro *macros) {
    Macro *next;

    while (macros != NULL) {
        next = macros->next;
        freeMacro(macros);
        macros = next;
    }
}

/**
 * Frees a single macro.
 *
 * Assumes that the given macro is not NULL and has not been freed yet.
 *
 * @param macro The macro to free.
 */
void freeMacro(Macro *macro) {
    free(macro->name);
    free(macro->content);
    free(macro);
}
