/*
 * labelLinking.c
 *
 * Contains a function to link all the used labels with their definitions.
 *
 * Name: Ariel Keren
 * Course: C Lab, 20465
 * Semester: 2024B
 * Date: 30/07/2024
 */

#include "labelLinking.h"

#include "encoder.h"        /* Encoding label addresses into words. */
#include "errorHandling.h"  /* Printing errors, if no definition is found for a label. */
#include "foundLabelList.h" /* Searching through the found label list. */
#include "globals.h"        /* Constants and typedefs. */
#include "labelList.h"      /* Searching through the list of extern labels. */

/**
 * Links all the instances of used labels with their definitions.
 * Modifies the words in the code and data parts to include the addresses of the labels.
 * Also, checks and returns whether or not all of the used labels are defined or declared as extern.
 *
 * Assumes that the given file name is not NULL and is null-terminated.
 * Assumes that the instruction count is the final instruction count of the file to compile.
 *
 * @param fileName The name of the current file that is being compiled.
 * @param externLabels The list of extern labels.
 * @param usedLabels The list of used labels.
 * @param foundLabels The list of found labels.
 * @param instructionCount The final instruction count.
 * @return TRUE if no errors occurred (all labels are defined or declared as extern), FALSE otherwise.
 */
Boolean linkLabels(char fileName[], Label *externLabels, UsedLabel *usedLabels, FoundLabel *foundLabels, WordCount instructionCount) {
    Boolean isSuccessful;
    FoundLabel *matchingFoundLabel;

    isSuccessful = TRUE;

    while (usedLabels != NULL) {
        matchingFoundLabel = getFoundLabel(foundLabels, usedLabels->name);

        if (matchingFoundLabel == NULL) {
            if (containsLabel(externLabels, usedLabels->name)) {
                encodeAddressingMode(usedLabels->wordPointer, 'E');
            } else {
                printError("Definition of label not found.", fileName, usedLabels->lineNumber);
                isSuccessful = FALSE;
            }
        } else {
            encodeAddressingMode(usedLabels->wordPointer, 'R');

            if (matchingFoundLabel->isData) {
                encodeLabel(usedLabels->wordPointer, matchingFoundLabel->address + (Address)instructionCount + STARTING_MEMORY_ADDRESS);
            } else {
                encodeLabel(usedLabels->wordPointer, matchingFoundLabel->address + STARTING_MEMORY_ADDRESS);
            }
        }

        usedLabels = usedLabels->next;
    }

    return isSuccessful;
}
