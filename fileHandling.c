#include <stdio.h>

#include "globals.h"

void checkNoFiles(int fileCount) {
    if (fileCount == 0) {
        printf("No files to compile.\n");
        exit(1);
    }
}

void compileFiles(char *fileNames[], int fileCount) {
    word *code;
    word *data;
    label *entryLabels;
    label *externLabels;
    usedLabel *usedLabels;
    unsigned instructionCount;
    unsigned dataCount;

    code = NULL;
    data = NULL;
    entryLabels = NULL;
    externLabels = NULL;
    usedLabels = NULL;
    instructionCount = 0;
    dataCount = 0;

    while (fileCount > 0) {
        expandMacros(*fileNames);
        firstPass(*fileNames, &code, &data, &entryLabels, &externLabels, &usedLabels, &instructionCount, &dataCount);
        secondPass(*fileNames, code, entryLabels, externLabels, usedLabels, instructionCount);

        makeEntryFile(*fileNames, entryLabels);
        makeExternFile(*fileNames, externLabels);
        makeObjectFile(*fileNames, code, data, instructionCount, dataCount);

        fileCount--;
        fileNames++;
    }
}
