#ifndef MACRO_TABLE_H
#define MACRO_TABLE_H

typedef struct macroStruct {
    char *name;
    char *content;
    struct macroStruct *next;
} macro;

char *getMacroContent(macro *macroTable, char *macroName);
char *addMacro(macro **macroTable, char *macroName);
void addContent(macro *macroTable, char *macroName, char *content);
void freeMacroTable(macro *macroTable);
void freeMacro(macro *macro);

#endif
