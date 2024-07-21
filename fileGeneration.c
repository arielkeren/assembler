#include "fileGeneration.h"

#include <stdio.h>
#include <string.h>

#include "foundLabelList.h"
#include "globals.h"
#include "labelList.h"
#include "utils.h"

void generateObFile(char fileName[], word *code, word *data, unsigned instructionCount, unsigned dataCount) {
    FILE *file;

    file = openFile(fileName, "ob", "w");

    if (file == NULL) {
        return;
    }

    fprintf(file, "%u %u\n", instructionCount, dataCount);

    insertWordList(file, code, STARTING_MEMORY_ADDRESS);
    insertWordList(file, data, instructionCount + STARTING_MEMORY_ADDRESS);

    fclose(file);
}

boolean generateEntFile(char fileName[], label *entryLabels, foundLabel *foundLabels, unsigned instructionCount, boolean shouldGenerate) {
    foundLabel *matchingFoundLabel;
    FILE *file;
    unsigned char longest;

    file = NULL;
    longest = getLongestLabel(entryLabels);

    while (entryLabels != NULL) {
        matchingFoundLabel = getFoundLabel(foundLabels, entryLabels->name);

        if (matchingFoundLabel == NULL) {
            printError("Label marked as .entry, but definition not found.", fileName, entryLabels->lineNumber);
            shouldGenerate = FALSE;
            entryLabels = entryLabels->next;
            continue;
        }

        if (!shouldGenerate) {
            entryLabels = entryLabels->next;
            continue;
        }

        if (file == NULL) {
            file = openFile(fileName, "ent", "w");

            if (file == NULL) {
                return FALSE;
            }
        }

        if (matchingFoundLabel->isData) {
            insertLabel(file, entryLabels->name, matchingFoundLabel->address + instructionCount + STARTING_MEMORY_ADDRESS, longest);
        } else {
            insertLabel(file, entryLabels->name, matchingFoundLabel->address + STARTING_MEMORY_ADDRESS, longest);
        }

        entryLabels = entryLabels->next;
    }

    if (file != NULL) {
        fclose(file);
    }

    return shouldGenerate;
}
boolean generateExtFile(char fileName[], label *externLabels, usedLabel *usedLabels, foundLabel *foundLabels, boolean shouldGenerate) {
    usedLabel *currentUsedLabel;
    FILE *file;
    unsigned char longest;

    file = NULL;
    longest = getLongestLabel(externLabels);

    while (externLabels != NULL) {
        if (getFoundLabel(foundLabels, externLabels->name) != NULL) {
            printError("Label marked as .extern, but also defined.", fileName, externLabels->lineNumber);
            shouldGenerate = FALSE;
            externLabels = externLabels->next;
            continue;
        }

        if (!shouldGenerate) {
            externLabels = externLabels->next;
            continue;
        }

        currentUsedLabel = usedLabels;

        while (currentUsedLabel != NULL) {
            if (strcmp(externLabels->name, currentUsedLabel->name) == EQUAL_STRINGS) {
                if (file == NULL) {
                    file = openFile(fileName, "ext", "w");

                    if (file == NULL) {
                        return FALSE;
                    }
                }

                insertLabel(file, externLabels->name, currentUsedLabel->address, longest);
            }

            currentUsedLabel = currentUsedLabel->next;
        }

        externLabels = externLabels->next;
    }

    if (file != NULL) {
        fclose(file);
    }

    return shouldGenerate;
}

void insertWordList(FILE *file, word *wordList, unsigned startingAddress) {
    while (wordList != NULL) {
        fprintf(file, "%04u %05o\n", startingAddress, wordList->data1 + ((unsigned)wordList->data2 << (sizeof(wordList->data1) * BITS_PER_BYTE)));
        startingAddress++;
        wordList = wordList->next;
    }
}

void insertLabel(FILE *file, char labelName[], unsigned address, unsigned char longest) {
    fprintf(file, "%-*s %04u\n", longest, labelName, address);
}
