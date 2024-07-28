#ifndef LABEL_LINKING_H
#define LABEL_LINKING_H

#include <stdio.h>

#include "globals.h"

Boolean linkLabels(char fileName[], Label *externLabels, UsedLabel *usedLabels, FoundLabel *foundLabels, unsigned instructionCount);

#endif
