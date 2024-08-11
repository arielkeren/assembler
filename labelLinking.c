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

/**
 * Links all the instances of used labels with their definitions.
 * Modifies the words in the code and data parts to include the addresses of the
 * labels.
 * Also, checks and returns whether or not all of the used labels are defined or
 * declared as extern.
 *
 * Assumes that the given file name is not NULL and is null-terminated.
 * Assumes that the instruction count is the final instruction count of the file
 * to compile.
 *
 * @param fileName The name of the current file that is being compiled.
 * @param externLabels The list of extern labels.
 * @param usedLabels The list of used labels.
 * @param foundLabels The list of found labels.
 * @param instructionCount The final instruction count.
 * @return TRUE if no errors occurred (all labels are defined or declared as
 * extern), FALSE otherwise.
 */
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

/**
 * Handles an undefined label.
 * Returns whether the label has been declared as extern.
 * If so, sets the addressing mode of the target word to E.
 * If not, prints an error.
 *
 * Assumes that the given file name is not NULL and is null-terminated.
 * Assumes that the given used label is not NULL.
 *
 * @param fileName The name of the current file that is being compiled.
 * @param externLabels The list of extern labels.
 * @param usedLabel The undefined used label.
 * @return TRUE if the label has been declared as extern, FALSE otherwise.
 */
Boolean handleUndefinedLabel(char fileName[], Label *externLabels,
                             UsedLabel *usedLabel) {
    /* Check if the label is declared as extern. */
    if (containsLabel(externLabels, usedLabel->name)) {
        /* Mark the target word as extern. */
        encodeAddressingMode(usedLabel->wordPointer, 'E');
        /* This label is valid. */
        return TRUE;
    }

    /* Print an error, as the label has no defintion. */
    printError("Definition of label not found.", fileName,
               usedLabel->lineNumber);
    /* This label is invalid. */
    return FALSE;
}

/**
 * Handles a defined label.
 * Sets the addressing mode of the target word to R.
 * Encodes the label's definition's address into the target word, based on
 * whether the label is defined in the data part or in the code part.
 *
 * Assumes that the given used label is not NULL.
 * Assumes that the given found label is not NULL.
 *
 * @param usedLabel The defined used label.
 * @param foundLabel The matching found label.
 * @param instructionCount The final instruction count.
 */
void handleDefinedLabel(UsedLabel *usedLabel, FoundLabel *foundLabel,
                        WordCount instructionCount) {
    /* Mark the target word as a label that is defined in the same file. */
    encodeAddressingMode(usedLabel->wordPointer, 'R');

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
