#include "fileCreation.h"

#include <stdio.h>
#include <string.h>

#include "globals.h"
#include "utils.h"

void makeObjectFile(char fileName[], word *code, word *data, unsigned instructionCount, unsigned dataCount) {
    FILE *file;

    file = openFile(fileName, "ob", "w");
    fprintf(file, "%d %d\n", instructionCount, dataCount);

    insertWordList(file, code, 100);
    insertWordList(file, code, instructionCount + 100);
}

void makeLabelFile(char fileName[], char extension[], label *labelList) {
    size_t longest;
    FILE *file;

    if (labelList == NULL) {
        return;
    }

    file = openFile(fileName, extension, "w");

    longest = getLongestLabel(labelList);
    insertLine(file, labelList, longest);

    fclose(file);
}

void insertWordList(FILE *file, word *wordList, unsigned startingIndex) {
    while (wordList != NULL) {
        fprintf(file, "%04d %05o\n", startingIndex, wordList->data1 + ((unsigned)wordList->data2 << 8));
        startingIndex++;
        wordList = wordList->next;
    }
}

void insertLine(FILE *file, label *labelToInsert, size_t longest) {
    fprintf(file, "%s%*s: %d\n", labelToInsert->name, longest - strlen(labelToInsert->name), " ", labelToInsert->address);
}

size_t getLongestLabel(label *labelList) {
    size_t longest;
    size_t currentLength;

    longest = 0;

    while (labelList != NULL) {
        currentLength = strlen(labelList->name);
        if (currentLength > longest) {
            longest = currentLength;
        }

        labelList = labelList->next;
    }

    return longest;
}