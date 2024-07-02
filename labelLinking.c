#include "labelLinking.h"

#include <stdio.h>

#include "encoder.h"
#include "foundLabelList.h"
#include "globals.h"
#include "labelList.h"

boolean linkLabels(label *externLabels, usedLabel *usedLabels, foundLabel *foundLabels, unsigned instructionCount) {
    boolean isSuccessful;
    foundLabel *matchingFoundLabel;

    isSuccessful = TRUE;

    while (usedLabels != NULL) {
        matchingFoundLabel = getFoundLabel(foundLabels, usedLabels->name);

        if (matchingFoundLabel == NULL) {
            if (containsLabel(externLabels, usedLabels->name)) {
                encodeMetadata(usedLabels->wordPointer, 'E');
            } else {
                printf("ERROR: Definition of label \"%s\" not found.\n", usedLabels->name);
                isSuccessful = FALSE;
            }
        } else {
            encodeMetadata(usedLabels->wordPointer, 'R');

            if (matchingFoundLabel->isData) {
                encodeLabel(usedLabels->wordPointer, matchingFoundLabel->address + instructionCount + 100);
            } else {
                encodeLabel(usedLabels->wordPointer, matchingFoundLabel->address + 100);
            }
        }

        usedLabels = usedLabels->next;
    }

    return isSuccessful;
}
