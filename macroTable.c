#include "macroTable.h"

#include <stdlib.h>
#include <string.h>

#include "globals.h"
#include "utils.h"

char *getMacroContent(macro *macroTable, char macroName[]) {
    while (macroTable != NULL) {
        if (strcmp(macroTable->name, macroName) == EQUAL_STRINGS) {
            return macroTable->content;
        }

        macroTable = macroTable->next;
    }

    return NULL;
}

void addMacro(macro **macroTable, char macroName[]) {
    macro *newMacro;

    newMacro = allocate(sizeof(macro));
    newMacro->name = macroName;
    newMacro->content = "";
    newMacro->next = *macroTable;
    *macroTable = newMacro;
}

void addMacroContent(macro *macroToModify, char content[]) {
    char *newContent;

    newContent = allocate(sizeof(char) * (strlen(macroToModify->content) + strlen(content) + NULL_BYTE));

    strcpy(newContent, macroToModify->content);
    free(macroToModify->content);
    strcat(newContent, content);

    macroToModify->content = newContent;
}

void freeMacroTable(macro *macroTable) {
    macro *next;

    while (macroTable != NULL) {
        next = macroTable->next;
        freeMacro(macroTable);
        macroTable = next;
    }
}

void freeMacro(macro *macro) {
    free(macro->name);
    free(macro->content);
    free(macro);
}
