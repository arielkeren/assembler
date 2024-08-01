/*
 * macroTable.c
 *
 * Contains functions to interface with a macro table, to search through it and finally to free it.
 * The macro table contains all the macro definitions in the .as file, along with their contents.
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
 * Searches for the given macro name in the given macro table.
 * If found, returns its content.
 * If not found, returns NULL.
 *
 * @param macroTable The macro table.
 * @param macroName The macro name to search for.
 * @return The macro's content, or NULL if not found.
 */
char *getMacroContent(Macro *macroTable, char macroName[]) {
    while (macroTable != NULL) {
        if (strcmp(macroTable->name, macroName) == EQUAL_STRINGS) {
            return macroTable->content;
        }

        macroTable = macroTable->next;
    }

    return NULL;
}

/**
 * Adds a new macro to the given macro table with the given name.
 *
 * @param macroTable The macro table to add the macro to.
 * @param macroName The name of the new macro.
 */
void addMacro(Macro **macroTable, char macroName[]) {
    Macro *newMacro;

    newMacro = allocate(sizeof(Macro));
    newMacro->name = macroName;
    newMacro->content = "";
    newMacro->next = *macroTable;
    *macroTable = newMacro;
}

/**
 * Appends the given content to the given macro's content.
 *
 * @param macro The macro to add the content to.
 * @param content The content to add.
 */
void addMacroContent(Macro *macro, char content[]) {
    char *newContent;

    newContent = allocate(sizeof(char) * (strlen(macro->content) + strlen(content) + NULL_BYTE));

    strcpy(newContent, macro->content);
    free(macro->content);
    strcat(newContent, content);

    macro->content = newContent;
}

/**
 * Frees a macro table.
 *
 * @param macroTable The macro table to free.
 */
void freeMacroTable(Macro *macroTable) {
    Macro *next;

    while (macroTable != NULL) {
        next = macroTable->next;
        freeMacro(macroTable);
        macroTable = next;
    }
}

/**
 * Frees a single macro.
 *
 * @param macro The macro to free.
 */
void freeMacro(Macro *macro) {
    free(macro->name);
    free(macro->content);
    free(macro);
}
