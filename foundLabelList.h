#ifndef FOUND_LABEL_LIST_H
#define FOUND_LABEL_LIST_H

#include "globals.h"

void addFoundLabel(foundLabel **foundLabelList, char labelName[]);
void markAsData(foundLabel *foundLabelToModify);
void setAddress(foundLabel *foundLabelToModify, address newAddress);
foundLabel *getFoundLabel(foundLabel *foundLabelList, char labelName[]);
void freeFoundLabelList(foundLabel *foundLabelList);
void freeFoundLabel(foundLabel *foundLabelToFree);

#endif
