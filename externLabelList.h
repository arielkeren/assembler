#ifndef EXTERN_LABEL_LIST_H
#define EXTERN_LABEL_LIST_H

#include "globals.h"

void addExternLabel(externLabel **labelList, char labelName[]);
void addUse(externLabel *labelToModify, unsigned address);
boolean containsExternLabel(externLabel *labelList, char labelName[]);
void freeExternLabelList(externLabel *labelList);
void freeExternLabel(externLabel *labelToFree);
void freeUseList(use *useList);

#endif
