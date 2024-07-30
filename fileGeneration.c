#include "fileGeneration.h"

#include <stdio.h>
#include <string.h>

#include "errorHandling.h"
#include "foundLabelList.h"
#include "globals.h"
#include "labelList.h"
#include "utils.h"

void generateObFile(char fileName[], Word *code, Word *data, WordCount instructionCount, WordCount dataCount) {
    FILE *file;

    file = openFile(fileName, "ob", "w");

    if (file == NULL) {
        return;
    }

    fprintf(file, "%u %u\n", instructionCount, dataCount);

    insertWordList(file, code, STARTING_MEMORY_ADDRESS);
    insertWordList(file, data, (Address)instructionCount + STARTING_MEMORY_ADDRESS);

    fclose(file);
}

Boolean generateEntFile(char fileName[], Label *entryLabels, FoundLabel *foundLabels, WordCount instructionCount, Boolean shouldGenerate) {
    FoundLabel *matchingFoundLabel;
    FILE *file;
    Length longest;

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
            insertLabel(file, entryLabels->name, matchingFoundLabel->address + (Address)instructionCount + STARTING_MEMORY_ADDRESS, longest);
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
Boolean generateExtFile(char fileName[], Label *externLabels, UsedLabel *usedLabels, FoundLabel *foundLabels, Boolean shouldGenerate) {
    UsedLabel *currentUsedLabel;
    FILE *file;
    Length longest;

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

void insertWordList(FILE *file, Word *wordList, Address startingAddress) {
    while (wordList != NULL) {
        fprintf(file, "%04hu %05o\n", startingAddress, (unsigned short)wordList->data1 + ((unsigned short)wordList->data2 << (sizeof(wordList->data1) * BITS_PER_BYTE)));
        startingAddress++;
        wordList = wordList->next;
    }
}

void insertLabel(FILE *file, char labelName[], Address address, Length longest) {
    fprintf(file, "%-*s %04hu\n", longest, labelName, address);
}
