#ifndef MACRO_TABLE_H
#define MACRO_TABLE_H

#include "globals.h"

char *getMacroContent(Macro *macroTable, char macroName[]);
void addMacro(Macro **macroTable, char macroName[]);
void addMacroContent(Macro *macro, char content[]);
void freeMacroTable(Macro *macroTable);
void freeMacro(Macro *macro);

#endif
