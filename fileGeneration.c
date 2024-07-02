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
    fprintf(file, "%u %u\n", instructionCount, dataCount);

    insertWordList(file, code, 100);
    insertWordList(file, data, instructionCount + 100);
}

boolean generateEntFile(char fileName[], label *entryLabels, foundLabel *foundLabels, unsigned instructionCount, boolean shouldGenerate) {
    boolean isSuccessful;
    FILE *file;

    if (entryLabels == NULL) {
        return TRUE;
    }

    isSuccessful = TRUE;

    if (shouldGenerate) {
        file = openFile(fileName, "ent", "w");

        if (file == NULL) {
            isSuccessful = FALSE;
        }
    }

    isSuccessful = isSuccessful && insertEntryLabels(shouldGenerate ? file : NULL, entryLabels, foundLabels, instructionCount, getLongestLabel(entryLabels));

    fclose(file);
    return isSuccessful;
}

boolean generateExtFile(char fileName[], label *externLabels, usedLabel *usedLabels, foundLabel *foundLabels, boolean shouldGenerate) {
    boolean isSuccessful;
    FILE *file;

    if (externLabels == NULL) {
        return TRUE;
    }

    isSuccessful = TRUE;
    file = NULL;

    if (shouldGenerate) {
        file = openFile(fileName, "ext", "w");

        if (file == NULL) {
            isSuccessful = FALSE;
        }
    }

    isSuccessful = isSuccessful && insertExternLabels(shouldGenerate ? file : NULL, externLabels, usedLabels, foundLabels, getLongestLabel(externLabels));

    if (file != NULL) {
        fclose(file);
    }

    return isSuccessful;
}

void insertWordList(FILE *file, word *wordList, unsigned startingAddress) {
    while (wordList != NULL) {
        fprintf(file, "%04u %05o\n", startingAddress, wordList->data1 + ((unsigned)wordList->data2 << 8));
        startingAddress++;
        wordList = wordList->next;
    }
}

boolean insertEntryLabels(FILE *file, label *entryLabels, foundLabel *foundLabels, unsigned instructionCount, unsigned char longest) {
    boolean isSuccessful;
    foundLabel *matchingFoundLabel;

    isSuccessful = TRUE;

    while (entryLabels != NULL) {
        matchingFoundLabel = getFoundLabel(foundLabels, entryLabels->name);

        if (matchingFoundLabel == NULL) {
            printError("Label marked as .entry, but definition not found.", entryLabels->lineNumber);
            isSuccessful = FALSE;
            file = NULL;
            entryLabels = entryLabels->next;
            continue;
        }

        if (file == NULL) {
            entryLabels = entryLabels->next;
            continue;
        }

        if (matchingFoundLabel->isData) {
            insertLabel(file, entryLabels->name, matchingFoundLabel->address + instructionCount + 100, longest);
        } else {
            insertLabel(file, entryLabels->name, matchingFoundLabel->address + 100, longest);
        }

        entryLabels = entryLabels->next;
    }

    return isSuccessful;
}

boolean insertExternLabels(FILE *file, label *externLabels, usedLabel *usedLabels, foundLabel *foundLabels, unsigned char longest) {
    boolean isSuccessful;
    usedLabel *currentUsedLabel;

    isSuccessful = TRUE;

    while (externLabels != NULL) {
        if (getFoundLabel(foundLabels, externLabels->name) != NULL) {
            printError("Label marked as .extern, but also defined.", externLabels->lineNumber);
            isSuccessful = FALSE;
            file = NULL;
            externLabels = externLabels->next;
            continue;
        }

        if (file == NULL) {
            externLabels = externLabels->next;
            continue;
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

    return isSuccessful;
}

void insertLabel(FILE *file, char labelName[], unsigned address, unsigned char longest) {
    fprintf(file, "%-*s %04u\n", longest, labelName, address);
}
