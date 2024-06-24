#ifndef FOUND_LABEL_LIST_H
#define FOUND_LABEL_LIST_H

#include "globals.h"

void addFoundLabel(foundLabel **foundLabelList, char labelName[]);
void freeFoundLabelList(foundLabel *foundLabelList);
void freeFoundLabel(foundLabel *foundLabelToFree);

#endif
