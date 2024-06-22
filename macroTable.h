#ifndef MACRO_TABLE_H
#define MACRO_TABLE_H

#include "globals.h"

char *getMacroContent(macro *macroTable, char *macroName);
char *addMacro(macro **macroTable, char *macroName);
void addContent(macro *macroTable, char *macroName, char *content);
void freeMacroTable(macro *macroTable);
void freeMacro(macro *macro);

#endif
