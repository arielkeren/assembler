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
#include "utils.h"
#include "wordList.h"

void checkNoFiles(int fileCount) {
    if (fileCount == NO_FILES) {
        printCriticalError("No files provided to compile. Add the file names (without the extensions) to compile as command line arguments. Exiting the program...");
        exit(ERROR);
    }
}

void compileFiles(char *fileNames[], int fileCount) {
    Word *code;
    Word *data;
    Macro *macros;
    Label *entryLabels;
    Label *externLabels;
    UsedLabel *usedLabels;
    FoundLabel *foundLabels;
    Boolean shouldGenerateFiles;
    unsigned instructionCount;
    unsigned dataCount;

    while (fileCount > NO_FILES) {
        code = createWord();
        data = createWord();
        macros = NULL;
        entryLabels = NULL;
        externLabels = NULL;
        usedLabels = NULL;
        foundLabels = NULL;
        shouldGenerateFiles = TRUE;
        instructionCount = INITIAL_VALUE;
        dataCount = INITIAL_VALUE;

        if (!expandMacros(*fileNames, &macros)) {
            freeMacroTable(macros);
            fileCount--;
            fileNames++;
            continue;
        }

        shouldGenerateFiles = readFile(*fileNames, macros, code, data, &entryLabels, &externLabels, &usedLabels, &foundLabels, &instructionCount, &dataCount);
        freeMacroTable(macros);

        shouldGenerateFiles = linkLabels(*fileNames, externLabels, usedLabels, foundLabels, instructionCount) && shouldGenerateFiles;

        shouldGenerateFiles = generateEntFile(*fileNames, entryLabels, foundLabels, instructionCount, shouldGenerateFiles) && shouldGenerateFiles;
        freeLabelList(entryLabels);

        shouldGenerateFiles = generateExtFile(*fileNames, externLabels, usedLabels, foundLabels, shouldGenerateFiles) && shouldGenerateFiles;
        freeLabelList(externLabels);
        freeUsedLabelList(usedLabels);
        freeFoundLabelList(foundLabels);

        if (shouldGenerateFiles) {
            generateObFile(*fileNames, code->next, data->next, instructionCount, dataCount);
        }

        freeWordList(code);
        freeWordList(data);

        fileCount--;
        fileNames++;
    }
}
