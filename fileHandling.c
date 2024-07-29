#include "fileHandling.h"

#include <stdio.h>
#include <stdlib.h>

#include "errorHandling.h"
#include "fileGeneration.h"
#include "fileReading.h"
#include "foundLabelList.h"
#include "freeingLogic.h"
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
    Word *code = NULL;
    Word *data = NULL;

    Macro *macros = NULL;
    Label *entryLabels = NULL;
    Label *externLabels = NULL;
    UsedLabel *usedLabels = NULL;
    FoundLabel *foundLabels = NULL;

    WordCount instructionCount;
    WordCount dataCount;
    Boolean shouldGenerateFiles;

    setToFree(&code, &data, &macros, &entryLabels, &externLabels, &usedLabels, &foundLabels);

    while (fileCount > NO_FILES) {
        code = createWord();
        data = createWord();

        macros = NULL;
        entryLabels = NULL;
        externLabels = NULL;
        usedLabels = NULL;
        foundLabels = NULL;

        instructionCount = INITIAL_VALUE;
        dataCount = INITIAL_VALUE;
        shouldGenerateFiles = TRUE;

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
