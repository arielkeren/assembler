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

void insertWordList(FILE *file, word *wordList, unsigned startingAddress) {
    while (wordList != NULL) {
        fprintf(file, "%04u %05o\n", startingAddress, wordList->data1 + ((unsigned)wordList->data2 << 8));
        startingAddress++;
        wordList = wordList->next;
    }
}
