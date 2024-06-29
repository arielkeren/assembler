#include "labelLinking.h"

#include <stdio.h>
#include <string.h>

#include "encoder.h"
#include "foundLabelList.h"
#include "globals.h"
#include "labelList.h"
#include "utils.h"

void linkLabels(label *externLabels, usedLabel *usedLabels, foundLabel *foundLabels, unsigned instructionCount) {
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

void generateEntFile(char fileName[], label *entryLabels, foundLabel *foundLabels, unsigned instructionCount) {
    FILE *file;
    unsigned char longest;

    if (entryLabels == NULL) {
        return;
    }

    file = openFile(fileName, "ent", "w");
    longest = getLongestLabel(entryLabels);
    insertEntryLabels(file, entryLabels, foundLabels, instructionCount, longest);

    fclose(file);
}

void generateExtFile(char fileName[], label *externLabels, usedLabel *usedLabels, foundLabel *foundLabels) {
    FILE *file;
    unsigned char longest;

    if (externLabels == NULL) {
        return;
    }

    file = openFile(fileName, "ext", "w");
    longest = getLongestLabel(externLabels);
    insertExternLabels(file, externLabels, usedLabels, foundLabels, longest);

    fclose(file);
}

void insertEntryLabels(FILE *file, label *entryLabels, foundLabel *foundLabels, unsigned instructionCount, unsigned char longest) {
    foundLabel *matchingFoundLabel;

    while (entryLabels != NULL) {
        matchingFoundLabel = getFoundLabel(foundLabels, entryLabels->name);

        if (matchingFoundLabel == NULL) {
            printError("Label marked as .entry, but definition not found.", entryLabels->lineNumber);
        } else {
            if (matchingFoundLabel->isData) {
                insertLabel(file, entryLabels->name, matchingFoundLabel->address + instructionCount + 100, longest);
            } else {
                insertLabel(file, entryLabels->name, matchingFoundLabel->address + 100, longest);
            }
        }

        entryLabels = entryLabels->next;
    }
}

void insertExternLabels(FILE *file, label *externLabels, usedLabel *usedLabels, foundLabel *foundLabels, unsigned char longest) {
    usedLabel *currentUsedLabel;

    while (externLabels != NULL) {
        if (getFoundLabel(foundLabels, externLabels->name) != NULL) {
            printError("Label marked as .extern, but also defined.", externLabels->lineNumber);
        }

        currentUsedLabel = usedLabels;

        while (currentUsedLabel != NULL) {
            if (strcmp(externLabels->name, currentUsedLabel->name) == 0) {
                insertLabel(file, externLabels->name, currentUsedLabel->address, longest);
            }

            currentUsedLabel = currentUsedLabel->next;
        }

        externLabels = externLabels->next;
    }
}

void insertLabel(FILE *file, char labelName[], unsigned address, unsigned char longest) {
    fprintf(file, "%-*s %04u\n", longest, labelName, address);
}
