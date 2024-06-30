#ifndef MACRO_TABLE_H
#define MACRO_TABLE_H

#include "globals.h"

char *getMacroContent(macro *macroTable, char macroName[]);
void addMacro(macro **macroTable, char macroName[]);
void addMacroContent(macro *macroToModify, char content[]);
void freeMacroTable(macro *macroTable);
void freeMacro(macro *macro);

#endif
