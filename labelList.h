#ifndef LABEL_LIST_H
#define LABEL_LIST_H

#include "globals.h"

void addLabel(label **labelList, char labelName[]);
void putAddress(label *labelList, unsigned address);
boolean containsLabel(label *labelList, char labelName[]);
void freeLabelList(label *labelList);
void freeLabel(label *labelToFree);

#endif
