#ifndef LABEL_LIST_H
#define LABEL_LIST_H

#include "globals.h"

void addLabel(Label **labels, char labelName[], unsigned lineNumber);
Boolean containsLabel(Label *labels, char labelName[]);
unsigned char getLongestLabel(Label *labels);
void freeLabelList(Label *labels);
void freeLabel(Label *label);

#endif
