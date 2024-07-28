#ifndef FOUND_LABEL_LIST_H
#define FOUND_LABEL_LIST_H

#include "globals.h"

void addFoundLabel(FoundLabel **labels, char labelName[]);
void markAsData(FoundLabel *label);
void setAddress(FoundLabel *label, Address newAddress);
FoundLabel *getFoundLabel(FoundLabel *labels, char labelName[]);
void freeFoundLabelList(FoundLabel *labels);
void freeFoundLabel(FoundLabel *label);

#endif
