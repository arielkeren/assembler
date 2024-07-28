#include "labelLinking.h"

#include "encoder.h"
#include "foundLabelList.h"
#include "globals.h"
#include "labelList.h"
#include "utils.h"

Boolean linkLabels(char fileName[], Label *externLabels, UsedLabel *usedLabels, FoundLabel *foundLabels, unsigned instructionCount) {
    Boolean isSuccessful;
    FoundLabel *matchingFoundLabel;

    isSuccessful = TRUE;

    while (usedLabels != NULL) {
        matchingFoundLabel = getFoundLabel(foundLabels, usedLabels->name);

        if (matchingFoundLabel == NULL) {
            if (containsLabel(externLabels, usedLabels->name)) {
                encodeMetadata(usedLabels->wordPointer, 'E');
            } else {
                printError("Definition of label not found.", fileName, usedLabels->lineNumber);
                isSuccessful = FALSE;
            }
        } else {
            encodeMetadata(usedLabels->wordPointer, 'R');

            if (matchingFoundLabel->isData) {
                encodeLabel(usedLabels->wordPointer, matchingFoundLabel->address + instructionCount + STARTING_MEMORY_ADDRESS);
            } else {
                encodeLabel(usedLabels->wordPointer, matchingFoundLabel->address + STARTING_MEMORY_ADDRESS);
            }
        }

        usedLabels = usedLabels->next;
    }

    return isSuccessful;
}
