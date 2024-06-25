#include "fileHandling.h"

#include <stdio.h>
#include <stdlib.h>

#include "externLabelList.h"
#include "fileCreation.h"
#include "firstPass.h"
#include "foundLabelList.h"
#include "globals.h"
#include "labelList.h"
#include "macroExpansion.h"
#include "secondPass.h"
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
    externLabel *externLabels;
    usedLabel *usedLabels;
    foundLabel *foundLabels;
    unsigned instructionCount;
    unsigned dataCount;

    code = NULL;
    data = NULL;
    entryLabels = NULL;
    externLabels = NULL;
    usedLabels = NULL;
    foundLabels = NULL;
    instructionCount = 0;
    dataCount = 0;

    while (fileCount > 0) {
        expandMacros(*fileNames);
        firstPass(*fileNames, &code, &data, &entryLabels, &externLabels, &usedLabels, &foundLabels, &instructionCount, &dataCount);
        secondPass(entryLabels, externLabels, usedLabels, foundLabels, instructionCount);

        generateEntFile(*fileNames, entryLabels);
        generateExtFile(*fileNames, externLabels);
        generateObFile(*fileNames, code, data, instructionCount, dataCount);

        fileCount--;
        fileNames++;
    }

    freeWordList(code);
    freeWordList(data);
    freeLabelList(entryLabels);
    freeExternLabelList(externLabels);
    freeUsedLabelList(usedLabels);
    freeFoundLabelList(foundLabels);
}
