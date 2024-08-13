/*
 * labelLinking.c
 *
 * Contains functions to link all the used labels with their definitions.
 *
 * Name: Ariel Keren
 * Course: C Lab, 20465
 * Semester: 2024B
 * Date: 30/07/2024
 */

#include "labelLinking.h"

#include "encoder.h" /* Encoding label addresses into words. */
#include "errorHandling.h" /* Printing errors, if no definition is found for a label. */
#include "foundLabelList.h" /* Searching through the found label list. */
#include "globals.h"        /* Constants and typedefs. */
#include "labelList.h"      /* Searching through the list of extern labels. */

Boolean linkLabels(char fileName[], Label *externLabels, UsedLabel *usedLabels,
                   FoundLabel *foundLabels, WordCount instructionCount) {
    Boolean isSuccessful; /* Whether all the used labels are defined somehow. */
    FoundLabel *matchingFoundLabel; /* The current used label's defintion. */

    isSuccessful = TRUE;

    /* Loop over the used labels and link their definitions to the words. */
    while (usedLabels != NULL) {
        /* Get the possible definition of the current used label. */
        matchingFoundLabel = getFoundLabel(foundLabels, usedLabels->name);

        /* Check if the used label is undefined. */
        if (matchingFoundLabel == NULL) {
            /* The label could still be declared as extern. */
            isSuccessful =
                handleUndefinedLabel(fileName, externLabels, usedLabels) &&
                isSuccessful;
        } else {
            /* Encode the label's definition's address into the target word. */
            handleDefinedLabel(usedLabels, matchingFoundLabel,
                               instructionCount);
        }

        /* Move on to the next used label. */
        usedLabels = usedLabels->next;
    }

    return isSuccessful;
}

Boolean handleUndefinedLabel(char fileName[], Label *externLabels,
                             UsedLabel *usedLabel) {
    /* Check if the label is declared as extern. */
    if (containsLabel(externLabels, usedLabel->name)) {
        /* Mark the target word as extern. */
        encodeMetadata(usedLabel->wordPointer, 'E');
        /* This label is valid. */
        return TRUE;
    }

    /* Print an error, as the label has no defintion. */
    printError("Definition of label not found.", fileName,
               usedLabel->lineNumber);
    /* This label is invalid. */
    return FALSE;
}

void handleDefinedLabel(UsedLabel *usedLabel, FoundLabel *foundLabel,
                        WordCount instructionCount) {
    /* Mark the target word as a label that is defined in the same file. */
    encodeMetadata(usedLabel->wordPointer, 'R');

    /* Check if the label is defined in the data part. */
    if (foundLabel->isData) {
        /* Add to the label's address the instruction count and encode. */
        encodeLabel(usedLabel->wordPointer, foundLabel->address +
                                                (Address)instructionCount +
                                                STARTING_MEMORY_ADDRESS);
        return;
    }

    /* Encode the label in the code part. */
    encodeLabel(usedLabel->wordPointer,
                foundLabel->address + STARTING_MEMORY_ADDRESS);
}
