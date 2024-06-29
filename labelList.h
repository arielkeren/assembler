#ifndef LABEL_LIST_H
#define LABEL_LIST_H

#include "globals.h"

void addLabel(label **labelList, char labelName[], unsigned lineNumber);
boolean containsLabel(label *labelList, char labelName[]);
unsigned char getLongestLabel(label *labelList);
void freeLabelList(label *labelList);
void freeLabel(label *labelToFree);

#endif
