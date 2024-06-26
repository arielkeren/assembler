#include "fileCreation.h"

#include <stdio.h>
#include <string.h>

#include "globals.h"
#include "utils.h"

void generateObFile(char fileName[], word *code, word *data, unsigned instructionCount, unsigned dataCount) {
    FILE *file;

    file = openFile(fileName, "ob", "w");
    fprintf(file, "%u %u\n", instructionCount, dataCount);

    insertWordList(file, code, 100);
    insertWordList(file, data, instructionCount + 100);
}

void generateEntFile(char fileName[], label *entryLabels) {
    FILE *file;

    if (entryLabels == NULL) {
        return;
    }

    file = openFile(fileName, "ent", "w");
    insertEntryLabels(file, entryLabels);

    fclose(file);
}

void generateExtFile(char fileName[], externLabel *externLabels) {
    FILE *file;

    if (externLabels == NULL) {
        return;
    }

    file = openFile(fileName, "ext", "w");
    insertExternLabels(file, externLabels);

    fclose(file);
}

void insertWordList(FILE *file, word *wordList, unsigned startingIndex) {
    while (wordList != NULL) {
        fprintf(file, "%04u %05o\n", startingIndex, wordList->data1 + ((unsigned)wordList->data2 << 8));
        startingIndex++;
        wordList = wordList->next;
    }
}

void insertEntryLabels(FILE *file, label *entryLabels) {
    size_t longest;

    longest = getLongestEntryLabel(entryLabels);

    while (entryLabels != NULL) {
        insertLabel(file, entryLabels->name, entryLabels->address, longest);
        entryLabels = entryLabels->next;
    }
}

void insertExternLabels(FILE *file, externLabel *externLabels) {
    size_t longest;

    longest = getLongestExternLabel(externLabels);

    while (externLabels != NULL) {
        insertUses(file, externLabels, longest);
        externLabels = externLabels->next;
    }
}

void insertUses(FILE *file, externLabel *labelToInsert, size_t longest) {
    use *currentUse;

    currentUse = labelToInsert->useList;

    while (currentUse != NULL) {
        insertLabel(file, labelToInsert->name, currentUse->address, longest);
        currentUse = currentUse->next;
    }
}

void insertLabel(FILE *file, char labelName[], unsigned address, size_t longest) {
    fprintf(file, "%-*s %04u\n", longest, labelName, address);
}

size_t getLongestEntryLabel(label *entryLabels) {
    size_t longest;
    size_t currentLength;

    longest = 0;

    while (entryLabels != NULL) {
        currentLength = strlen(entryLabels->name);
        if (currentLength > longest) {
            longest = currentLength;
        }

        entryLabels = entryLabels->next;
    }

    return longest;
}

size_t getLongestExternLabel(externLabel *externLabels) {
    size_t longest;
    size_t currentLength;

    longest = 0;

    while (externLabels != NULL) {
        currentLength = strlen(externLabels->name);
        if (currentLength > longest) {
            longest = currentLength;
        }

        externLabels = externLabels->next;
    }

    return longest;
}
