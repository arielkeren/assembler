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

char *getMacroContent(Macro *macroTable, char macroName[]) {
    while (macroTable != NULL) {
        if (strcmp(macroTable->name, macroName) == EQUAL_STRINGS) {
            return macroTable->content;
        }

        macroTable = macroTable->next;
    }

    return NULL;
}

void addMacro(Macro **macroTable, char macroName[]) {
    Macro *newMacro;

    newMacro = allocate(sizeof(Macro));
    newMacro->name = macroName;
    newMacro->content = "";
    newMacro->next = *macroTable;
    *macroTable = newMacro;
}

void addMacroContent(Macro *macro, char content[]) {
    char *newContent;

    newContent = allocate(sizeof(char) * (strlen(macro->content) + strlen(content) + NULL_BYTE));

    strcpy(newContent, macro->content);
    free(macro->content);
    strcat(newContent, content);

    macro->content = newContent;
}

void freeMacroTable(Macro *macroTable) {
    Macro *next;

    while (macroTable != NULL) {
        next = macroTable->next;
        freeMacro(macroTable);
        macroTable = next;
    }
}

void freeMacro(Macro *macro) {
    free(macro->name);
    free(macro->content);
    free(macro);
}
