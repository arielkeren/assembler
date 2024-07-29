#ifndef FREEING_LOGIC_H
#define FREEING_LOGIC_H

#include "globals.h"

void freeAll();
void setToFree(Word **code, Word **data, Macro **macros, Label **entryLabels, Label **externLabels, UsedLabel **usedLabels, FoundLabel **foundLabels);
void freeingLogic(Boolean shouldFree, Word **code, Word **data, Macro **macros, Label **entryLabels, Label **externLabels, UsedLabel **usedLabels, FoundLabel **foundLabels);
void freeLists(ToFree *toFree);

#endif
