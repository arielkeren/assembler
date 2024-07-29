#include "macroTable.h"

#include <stdlib.h>
#include <string.h>

#include "globals.h"
#include "utils.h"

char *getMacroContent(Macro *macroTable, char macroName[]) {
    while (macroTable != NULL) {
        if (strcmp(macroTable->name, macroName) == EQUAL_STRINGS) {
            return macroTable->content;
        }

        macroTable = macroTable->next;
    }

    return (char *)NULL;
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

    newContent = allocate(sizeof(char) * (strlen(macro->content) + strlen(content) + (size_t)NULL_BYTE));

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
