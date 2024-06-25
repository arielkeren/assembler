#include "secondPass.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "foundLabelList.h"
#include "globals.h"
#include "labelList.h"
#include "utils.h"
#include "wordList.h"

void secondPass(label *entryLabels, label *externLabels, usedLabel *usedLabels, foundLabel *foundLabels, unsigned instructionCount) {
    insertLabelAddresses(usedLabels, externLabels, foundLabels, instructionCount);
    fillEntryLabels(entryLabels, foundLabels, instructionCount);
}

void insertLabelAddresses(usedLabel *usedLabels, label *externLabels, foundLabel *foundLabels, unsigned instructionCount) {
    foundLabel *matchingFoundLabel;

    while (usedLabels != NULL) {
        matchingFoundLabel = getFoundLabel(foundLabels, usedLabels->name);

        if (matchingFoundLabel == NULL) {
            if (containsLabel(externLabels, usedLabels->name)) {
                encodeMetadata(usedLabels->wordPointer, 'E');
            } else {
                printf("ERROR: Definition of label \"%s\" not found.\n", usedLabels->name);
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
}

void fillEntryLabels(label *entryLabels, foundLabel *foundLabels, unsigned instructionCount) {
    foundLabel *matchingFoundLabel;

    while (entryLabels != NULL) {
        matchingFoundLabel = getFoundLabel(foundLabels, entryLabels->name);

        if (matchingFoundLabel == NULL) {
            printf("ERROR: Label \"%s\" marked as .entry, but definition not found.\n", entryLabels->name);
        } else {
            if (matchingFoundLabel->isData) {
                putAddress(entryLabels, matchingFoundLabel->address + instructionCount + 100);
            } else {
                putAddress(entryLabels, matchingFoundLabel->address + 100);
            }
        }

        entryLabels = entryLabels->next;
    }
}
