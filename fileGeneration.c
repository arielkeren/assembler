/*
 * fileGeneration.c
 *
 * Contains functions to generate the .ob, .ent and .ext files.
 *
 * Name: Ariel Keren
 * Course: C Lab, 20465
 * Semester: 2024B
 * Date: 30/07/2024
 */

#include "fileGeneration.h"

#include <stdio.h>  /* fprintf, fclose. */
#include <string.h> /* strcmp. */

#include "errorHandling.h"  /* Printing errors. */
#include "foundLabelList.h" /* Searching through the found label list. */
#include "globals.h"        /* Constants and typedefs. */
#include "labelList.h" /* Getting the longest label's length in each label list. */
#include "utils.h"     /* Creating the output files. */

void generateObFile(char fileName[], Word *code, Word *data,
                    WordCount instructionCount, WordCount dataCount) {
    FILE *file; /* The .ob file to write to. */

    /* Open the .ob file. */
    file = openFile(fileName, "ob", "w");

    /* Check if the there was a problem opening the file. */
    if (file == NULL) {
        /* Do not generate the file. */
        return;
    }

    /* Write the instruction and data count to the file. */
    fprintf(file, "%hu %hu", instructionCount, dataCount);

    /* Start at address 100 and write the code part to the file. */
    insertWords(file, code, STARTING_MEMORY_ADDRESS);
    /* Write at the end of the code part and write the data part. */
    insertWords(file, data,
                (Address)instructionCount + STARTING_MEMORY_ADDRESS);

    /* Close the no longer used file. */
    fclose(file);
}

Boolean generateEntFile(char fileName[], Label *entryLabels,
                        FoundLabel *foundLabels, WordCount instructionCount,
                        Boolean shouldGenerate) {
    FoundLabel *matchingFoundLabel; /* The entry label's possible defintion. */
    FILE *file;                     /* The .ent file. */
    Length longest;                 /* The longest label's length. */
    Boolean isFirst; /* Whether this is the first label inserted. */

    /* Initialize the necessary variables. */
    isFirst = TRUE;
    file = NULL;
    longest = getLongestLabel(entryLabels);

    /* Loop over the entry labels. */
    while (entryLabels != NULL) {
        /* Get the entry label's possible definition. */
        matchingFoundLabel = getFoundLabel(foundLabels, entryLabels->name);

        /* Check if there is no defintion. */
        if (matchingFoundLabel == NULL) {
            /* It is invalid for an entry label to have no definition. */
            printError("Label marked as .entry, but definition not found.",
                       fileName, entryLabels->lineNumber);
            shouldGenerate = FALSE;

            /* Move on to the next label. */
            entryLabels = entryLabels->next;
            continue;
        }

        /* Check if there was some kind of error. */
        if (!shouldGenerate) {
            /* Move on to the next label. */
            entryLabels = entryLabels->next;
            continue;
        }

        /* Check if the file has not been opened yet. */
        if (file == NULL) {
            /* Try opening the .ent file. */
            file = openFile(fileName, "ent", "w");

            /* Check if there was a problem opening the file. */
            if (file == NULL) {
                /* An error occured. */
                return FALSE;
            }
        }

        /* Insert the label's name and definition into the .ent file. */
        insertLabelDefinition(file, entryLabels->name, matchingFoundLabel,
                              instructionCount, longest, isFirst);

        /* The first label has already been inserted. */
        isFirst = FALSE;
        /* Move on to the next label. */
        entryLabels = entryLabels->next;
    }

    /* Check if a file was opened. */
    if (file != NULL) {
        /* Close the file, as it is no longer needed. */
        fclose(file);
    }

    /* Return if the files after this one should be generated. */
    return shouldGenerate;
}

Boolean generateExtFile(char fileName[], Label *externLabels,
                        UsedLabel *usedLabels, FoundLabel *foundLabels,
                        Boolean shouldGenerate) {
    FILE *file;      /* The .ext file. */
    Length longest;  /* The length of the longest label. */
    Boolean isFirst; /* Whether this is the first line inserted. */

    /* Initialize the necessary variables. */
    file = NULL;
    longest = getLongestLabel(externLabels);

    /* Loop over the extern labels. */
    while (externLabels != NULL) {
        /* Check if there is a definition of the current extern label. */
        if (getFoundLabel(foundLabels, externLabels->name) != NULL) {
            /* It is invalid for an extern label to have a definition. */
            printError("Label marked as .extern, but also defined.", fileName,
                       externLabels->lineNumber);
            shouldGenerate = FALSE;

            /* Move on to the next label. */
            externLabels = externLabels->next;
            continue;
        }

        /* Check if there was some kind of error. */
        if (!shouldGenerate) {
            /* Move on to the next label. */
            externLabels = externLabels->next;
            continue;
        }

        /* Insert all of the label's uses throughout the code. */
        if (!insertUses(&file, fileName, externLabels, usedLabels, longest,
                        &isFirst)) {
            /* There was a problem opening the .ext file. */
            return FALSE;
        }

        /* Move on to the next label. */
        externLabels = externLabels->next;
    }

    /* Check if the file was opened. */
    if (file != NULL) {
        /* Close the file, as it is no longer used. */
        fclose(file);
    }

    /* Return if the files after this one should be generated. */
    return shouldGenerate;
}

void insertWords(FILE *file, Word *words, Address startingAddress) {
    unsigned short shiftedData2; /* The shifted data2 of the current word. */

    /* Loop over the list of words. */
    while (words != NULL) {
        /* Data2 needs to be shifted, as it represents the last 7 bits. */
        shiftedData2 = (unsigned short)words->data2
                       << (sizeof(words->data1) * BITS_PER_BYTE);

        /* Insert the word, along with its address, into the file. */
        fprintf(file, "\n%04hu %05o", startingAddress,
                (unsigned short)words->data1 + shiftedData2);

        /* Increment the address. */
        startingAddress++;
        /* Move on to the next word. */
        words = words->next;
    }
}

Boolean insertUses(FILE **file, char fileName[], Label *externLabel,
                   UsedLabel *usedLabels, Length longest, Boolean *isFirst) {
    Boolean isUsed; /* Whether the extern label is used somewhere. */

    isUsed = FALSE;

    /* Loop over the list of used labels. */
    while (usedLabels != NULL) {
        /* Compare the names to check if this is a use of the label. */
        if (strcmp(externLabel->name, usedLabels->name) == EQUAL_STRINGS) {
            /* The extern label is used. */
            isUsed = TRUE;

            /* Check if the .ext file has not been opened yet. */
            if (*file == NULL) {
                /* Try opening the .ext file. */
                *file = openFile(fileName, "ext", "w");

                /* Check if there was a problem opening the file. */
                if (*file == NULL) {
                    /* An error occurred. */
                    return FALSE;
                }
            }

            /* Insert the label, along with the address of its use. */
            insertLabel(*file, externLabel->name, usedLabels->address, longest,
                        *isFirst);
            /* The first line has already been inserted. */
            *isFirst = FALSE;
        }

        /* Move on to the next label. */
        usedLabels = usedLabels->next;
    }

    /* Check if the extern label is not used. */
    if (!isUsed) {
        /* Print a warning. */
        printWarning("Unused extern label.", fileName, externLabel->lineNumber);
    }

    /* There have not been any errors. */
    return TRUE;
}

void insertLabelDefinition(FILE *file, char labelName[], FoundLabel *definition,
                           WordCount instructionCount, Length longest,
                           Boolean isFirst) {
    /* Check if this is a data label. */
    if (definition->isData) {
        /* Insert the label, along with its address after the code part. */
        insertLabel(file, labelName,
                    definition->address + (Address)instructionCount +
                        STARTING_MEMORY_ADDRESS,
                    longest, isFirst);
        return;
    }

    /* Insert the label, along with its address. */
    insertLabel(file, labelName, definition->address + STARTING_MEMORY_ADDRESS,
                longest, isFirst);
}

void insertLabel(FILE *file, char labelName[], Address address, Length longest,
                 Boolean isFirst) {
    /* Insert the label, along with its address. Add newlines accordingly. */
    fprintf(file, "%s%-*s %04hu", isFirst ? "" : "\n", longest, labelName,
            address);
}
