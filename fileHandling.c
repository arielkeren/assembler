#include "fileHandling.h"

#include <stdio.h>
#include <stdlib.h>

#include "fileCreation.h"
#include "fileReading.h"
#include "foundLabelList.h"
#include "globals.h"
#include "labelLinking.h"
#include "labelList.h"
#include "macroExpansion.h"
#include "usedLabelList.h"
#include "wordList.h"

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
    foundLabel *foundLabels;
    unsigned instructionCount;
    unsigned dataCount;

    while (fileCount > 0) {
        code = NULL;
        data = NULL;
        entryLabels = NULL;
        externLabels = NULL;
        usedLabels = NULL;
        foundLabels = NULL;
        instructionCount = 0;
        dataCount = 0;

        expandMacros(*fileNames);
        readFile(*fileNames, &code, &data, &entryLabels, &externLabels, &usedLabels, &foundLabels, &instructionCount, &dataCount);
        linkLabels(externLabels, usedLabels, foundLabels, instructionCount);

        generateEntFile(*fileNames, entryLabels, foundLabels, instructionCount);
        generateExtFile(*fileNames, externLabels, usedLabels, foundLabels);
        generateObFile(*fileNames, code, data, instructionCount, dataCount);

        freeWordList(code);
        freeWordList(data);
        freeLabelList(entryLabels);
        freeLabelList(externLabels);
        freeUsedLabelList(usedLabels);
        freeFoundLabelList(foundLabels);

        fileCount--;
        fileNames++;
    }
}
