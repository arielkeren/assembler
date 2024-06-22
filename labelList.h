#ifndef LABEL_LIST_H
#define LABEL_LIST_H

#include "globals.h"

void addLabel(label **labels, char labelName[]);
void freeLabelList(label *labelList);
void freeLabel(label *labelToFree);

#endif
