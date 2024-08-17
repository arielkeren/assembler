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

#include "errorHandling.h" /* Printing an error if no files have been provided. */
#include "fileGeneration.h" /* Generating the .ob, .ent and .ext files. */
#include "fileReading.h"    /* Reading the .am file. */
#include "foundLabelList.h" /* Freeing the found label list. */
#include "freeingLogic.h" /* Setting the pointers to each linked list in case of an allocation failure. */
#include "globals.h"      /* Constants and typedefs. */
#include "labelLinking.h" /* Linking all the used labels with their definitions. */
#include "labelList.h"    /* Freeing the 2 label lists. */
#include "macroExpansion.h" /* Expanding the macros in the .as file to generate the .am file. */
#include "macroTable.h"    /* Freeing the macro table. */
#include "usedLabelList.h" /* Freeing the used label list. */
#include "wordList.h" /* Creating dummy nodes and freeing the 2 word lists. */

void handleNoFiles(int fileCount) {
    if (fileCount == NO_FILES) {
        printNoFilesError();
        exit(ERROR);
    }
}

void compileAllFiles(char *fileNames[], int fileCount) {
    Word *code = NULL; /* The words in the code part. */
    Word *data = NULL; /* The words in the data part. */

    Macro *macros = NULL;           /* The macro table. */
    Label *entryLabels = NULL;      /* The entry labels list. */
    Label *externLabels = NULL;     /* The extern labels list. */
    UsedLabel *usedLabels = NULL;   /* The used labels list. */
    FoundLabel *foundLabels = NULL; /* The found labels list. */

    /* In case of an allocation failure, make it possible to free the lists. */
    setToFree(&code, &data, &macros, &entryLabels, &externLabels, &usedLabels,
              &foundLabels);

    /* Compile each file separately. */
    while (fileCount > NO_FILES) {
        /* Create dummy nodes, so that the words will be ordered. */
        code = createWord();
        data = createWord();

        /* Order does not matter, so set the other lists to NULL. */
        macros = NULL;
        entryLabels = NULL;
        externLabels = NULL;
        usedLabels = NULL;
        foundLabels = NULL;

        /* Start compiling the current file. */
        compileFile(*fileNames, code, data, macros, entryLabels, externLabels,
                    usedLabels, foundLabels);

        /* Move on to the next file. */
        fileCount--;
        fileNames++;
    }
}

void compileFile(char fileName[], Word *code, Word *data, Macro *macros,
                 Label *entryLabels, Label *externLabels, UsedLabel *usedLabels,
                 FoundLabel *foundLabels) {
    WordCount instructionCount;  /* The number of words in the code part. */
    WordCount dataCount;         /* The number of words in the data part. */
    Boolean shouldGenerateFiles; /* Whether or not to generate output files. */

    /* Initialize the word counts. */
    instructionCount = INITIAL_VALUE;
    dataCount = INITIAL_VALUE;

    /* Try expanding the macros in the .as file. */
    if (!expandMacros(fileName, &macros)) {
        /* Move on to the next file. */
        freeMacroTable(macros);
        return;
    }

    /* Read the .am file and put everything in the lists. */
    shouldGenerateFiles =
        readFile(fileName, macros, code, data, &entryLabels, &externLabels,
                 &usedLabels, &foundLabels, &instructionCount, &dataCount);

    /* The macro table is no longer needed. */
    freeMacroTable(macros);

    /* Try linking all the used labels with their definitions. */
    if (!linkLabels(fileName, externLabels, usedLabels, foundLabels,
                    instructionCount)) {
        shouldGenerateFiles = FALSE;
    }

    /* Try generating the .ent file. */
    if (!generateEntFile(fileName, entryLabels, foundLabels, instructionCount,
                         shouldGenerateFiles)) {
        shouldGenerateFiles = FALSE;
    }

    /* The entry labels are no longer needed. */
    freeLabelList(entryLabels);

    /* Try generating the .ext file. */
    if (!generateExtFile(fileName, externLabels, usedLabels, foundLabels,
                         shouldGenerateFiles)) {
        shouldGenerateFiles = FALSE;
    }

    /* The extern, used and found labels are no longer needed. */
    freeLabelList(externLabels);
    freeUsedLabelList(usedLabels);
    freeFoundLabelList(foundLabels);

    /* Check if the .ob file should be generated. */
    if (shouldGenerateFiles) {
        /* Generate the .ob file (skip the dummy nodes). */
        generateObFile(fileName, code->next, data->next, instructionCount,
                       dataCount);
    }

    /* The word lists are no longer needed. */
    freeWordList(code);
    freeWordList(data);
}
