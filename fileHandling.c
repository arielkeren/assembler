#include "fileHandling.h"

#include <stdio.h>
#include <stdlib.h>

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
    label *externLabels;
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
        printf("Expanded macros.\n");
        firstPass(*fileNames, &code, &data, &entryLabels, &externLabels, &usedLabels, &foundLabels, &instructionCount, &dataCount);
        printf("Instruction count: %d", instructionCount);
        printf("Data count: %d", dataCount);
        printf("First pass.\n");
        secondPass(entryLabels, externLabels, usedLabels, foundLabels, instructionCount);
        printf("Second pass.\n");

        makeLabelFile(*fileNames, ".ent", entryLabels);
        makeLabelFile(*fileNames, ".ext", externLabels);
        makeObjectFile(*fileNames, code, data, instructionCount, dataCount);

        fileCount--;
        fileNames++;
    }

    freeWordList(code);
    freeWordList(data);
    freeLabelList(entryLabels);
    freeLabelList(externLabels);
    freeUsedLabelList(usedLabels);
    freeFoundLabelList(foundLabels);
}
