/*
 * fileHandling.c
 *
 * Contains functions to handle the files given as command line arguments.
 *
 * Name: Ariel Keren
 * Course: C Lab, 20465
 * Semester: 2024B
 * Date: 30/07/2024
 */

#include "fileHandling.h"

#include <stdlib.h> /* exit. */

#include "errorHandling.h"  /* Printing an error if no files have been provided. */
#include "fileGeneration.h" /* Generating the .ob, .ent and .ext files. */
#include "fileReading.h"    /* Reading the .am file. */
#include "foundLabelList.h" /* Freeing the found label list. */
#include "freeingLogic.h"   /* Setting the pointers to each linked list in case of an allocation failure. */
#include "globals.h"        /* Constants and typedefs. */
#include "labelLinking.h"   /* Linking all the used labels with their definitions. */
#include "labelList.h"      /* Freeing the 2 label lists. */
#include "macroExpansion.h" /* Expanding the macros in the .as file to generate the .am file. */
#include "macroTable.h"     /* Freeing the macro table. */
#include "usedLabelList.h"  /* Freeing the used label list. */
#include "wordList.h"       /* Creating dummy nodes and freeing the 2 word lists. */

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
