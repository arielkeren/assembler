#include "fileHandling.h"

#include <stdio.h>
#include <stdlib.h>

#include "fileGeneration.h"
#include "fileReading.h"
#include "foundLabelList.h"
#include "globals.h"
#include "labelLinking.h"
#include "labelList.h"
#include "macroExpansion.h"
#include "macroTable.h"
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
    macro *macros;
    label *entryLabels;
    label *externLabels;
    usedLabel *usedLabels;
    foundLabel *foundLabels;
    boolean shouldGenerateFiles;
    unsigned instructionCount;
    unsigned dataCount;

    while (fileCount > 0) {
        code = NULL;
        data = NULL;
        macros = NULL;
        entryLabels = NULL;
        externLabels = NULL;
        usedLabels = NULL;
        foundLabels = NULL;
        shouldGenerateFiles = TRUE;
        instructionCount = 0;
        dataCount = 0;

        if (!expandMacros(*fileNames, &macros)) {
            freeMacroTable(macros);
            fileCount--;
            fileNames++;
            continue;
        }

        shouldGenerateFiles = readFile(*fileNames, macros, &code, &data, &entryLabels, &externLabels, &usedLabels, &foundLabels, &instructionCount, &dataCount);
        freeMacroTable(macros);

        shouldGenerateFiles = shouldGenerateFiles && linkLabels(externLabels, usedLabels, foundLabels, instructionCount);

        shouldGenerateFiles = shouldGenerateFiles && generateEntFile(*fileNames, entryLabels, foundLabels, instructionCount, shouldGenerateFiles);
        freeLabelList(entryLabels);

        shouldGenerateFiles = shouldGenerateFiles && generateExtFile(*fileNames, externLabels, usedLabels, foundLabels, shouldGenerateFiles);
        freeLabelList(externLabels);
        freeUsedLabelList(usedLabels);
        freeFoundLabelList(foundLabels);

        if (shouldGenerateFiles) {
            generateObFile(*fileNames, code, data, instructionCount, dataCount);
        }

        freeWordList(code);
        freeWordList(data);

        fileCount--;
        fileNames++;
    }
}
