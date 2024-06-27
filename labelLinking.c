#include "labelLinking.h"

#include <stdio.h>
#include <string.h>

#include "encoder.h"
#include "externLabelList.h"
#include "foundLabelList.h"
#include "globals.h"
#include "labelList.h"

void linkLabels(label *entryLabels, externLabel *externLabels, usedLabel *usedLabels, foundLabel *foundLabels, unsigned instructionCount) {
    insertLabelAddresses(usedLabels, externLabels, foundLabels, instructionCount);
    fillEntryLabels(entryLabels, foundLabels, instructionCount);
    fillExternLabels(externLabels, usedLabels);
}

void insertLabelAddresses(usedLabel *usedLabels, externLabel *externLabels, foundLabel *foundLabels, unsigned instructionCount) {
    foundLabel *matchingFoundLabel;

    while (usedLabels != NULL) {
        matchingFoundLabel = getFoundLabel(foundLabels, usedLabels->name);

        if (matchingFoundLabel == NULL) {
            if (containsExternLabel(externLabels, usedLabels->name)) {
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

void fillExternLabels(externLabel *externLabels, usedLabel *usedLabels) {
    usedLabel *currentUsedLabel;

    while (externLabels != NULL) {
        currentUsedLabel = usedLabels;

        while (currentUsedLabel != NULL) {
            if (strcmp(externLabels->name, currentUsedLabel->name) == 0) {
                addUse(externLabels, currentUsedLabel->address);
            }

            currentUsedLabel = currentUsedLabel->next;
        }

        externLabels = externLabels->next;
    }
}
