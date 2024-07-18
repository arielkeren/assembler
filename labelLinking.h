#ifndef LABEL_LINKING_H
#define LABEL_LINKING_H

#include <stdio.h>

#include "globals.h"

boolean linkLabels(char fileName[], label *externLabels, usedLabel *usedLabels, foundLabel *foundLabels, unsigned instructionCount);

#endif
