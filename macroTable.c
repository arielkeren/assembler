#include "macroTable.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "globals.h"
#include "utils.h"

char *getMacroContent(macro *macroTable, char *macroName) {
    while (macroTable != NULL) {
        if (strcmp(macroTable->name, macroName) == 0) {
            return macroTable->content;
        }

        macroTable = macroTable->next;
    }

    return NULL;
}

char *addMacro(macro **macroTable, char *macroName) {
    macro *newMacro;

    newMacro = allocate(sizeof(macro));
    newMacro->name = allocate(sizeof(char) * (strlen(macroName) + 1));

    strcpy(newMacro->name, macroName);

    newMacro->content = "";
    newMacro->next = *macroTable;
    *macroTable = newMacro;

    return newMacro->name;
}

void addContent(macro *macroTable, char *macroName, char *content) {
    char *newContent;

    while (macroTable != NULL) {
        if (strcmp(macroTable->name, macroName) == 0) {
            newContent = allocate(sizeof(char) * (strlen(macroTable->content) + strlen(content) + 1));

            strcpy(newContent, macroTable->content);
            free(macroTable->content);
            strcat(newContent, content);

            macroTable->content = newContent;
            return;
        }

        macroTable = macroTable->next;
    }
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
