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
#include "labelList.h"      /* Getting the longest label's length in each label list. */
#include "utils.h"          /* Opening the .ob file. */

/**
 * Generates the <fileName>.ob file in the following format:
 * Addresses are decimal and padded to be 4 digits long with zeros to the left.
 * Words are octal and padded to be 5 digits long with zeros to the left.
 * NOTE: The .ob file does not include a title before each part.
 *
 * <Instruction count> <Data count>
 * --- Code Part --- (Taken from the code list)
 * <Address> <Word>
 * <Address> <Word>
 * ...
 * --- Data Part --- (Taken from the data list)
 * <Address> <Word>
 * <Address> <Word>
 * ...
 *
 * Assumes that the given file name is not NULL and is null-terminated.
 *
 * @param fileName The name of the .ob file to generate.
 * @param code List that represents the words in the code part.
 * @param data List that represents the words in the data part.
 * @param instructionCount The number of words in the code part.
 * @param dataCount The number of words in the data part.
 */
void generateObFile(char fileName[], Word *code, Word *data, WordCount instructionCount, WordCount dataCount) {
    FILE *file;

    file = openFile(fileName, "ob", "w");

    if (file == NULL) {
        return;
    }

    fprintf(file, "%u %u\n", instructionCount, dataCount);

    insertWords(file, code, STARTING_MEMORY_ADDRESS);
    insertWords(file, data, (Address)instructionCount + STARTING_MEMORY_ADDRESS);

    fclose(file);
}

/**
 * Returns whether or not more files should be generated after this one (an error occured).
 * The labels are padded to the number of characters of the longest label with spaces to the right.
 * The addresses are decimal and padded to be 4 digits long with zeros to the left.
 * Generates the <fileName>.ent file in the following format:
 * <Label> <Address>
 * <Label> <Address>
 * ...
 *
 * Assumes that the given file name is not NULL and is null-terminated.
 *
 * @param fileName The name of the .ent file to generate.
 * @param entryLabels The list of labels marked as entry.
 * @param foundLabels The list of found labels to check for missing definitions.
 * @param instructionCount The number of words in the code part.
 * @param shouldGenerate Whether the .ent file should be generated. If not, only checks for missing definitions.
 * @return Whether or not more files should be generated after the .ent file.
 */
Boolean generateEntFile(char fileName[], Label *entryLabels, FoundLabel *foundLabels, WordCount instructionCount, Boolean shouldGenerate) {
    FoundLabel *matchingFoundLabel;
    FILE *file;
    Length longest;

    file = NULL;
    longest = getLongestLabel(entryLabels);

    while (entryLabels != NULL) {
        matchingFoundLabel = getFoundLabel(foundLabels, entryLabels->name);

        if (matchingFoundLabel == NULL) {
            printError("Label marked as .entry, but definition not found.", fileName, entryLabels->lineNumber);
            shouldGenerate = FALSE;
            entryLabels = entryLabels->next;
            continue;
        }

        if (!shouldGenerate) {
            entryLabels = entryLabels->next;
            continue;
        }

        if (file == NULL) {
            file = openFile(fileName, "ent", "w");

            if (file == NULL) {
                return FALSE;
            }
        }

        if (matchingFoundLabel->isData) {
            insertLabel(file, entryLabels->name, matchingFoundLabel->address + (Address)instructionCount + STARTING_MEMORY_ADDRESS, longest);
        } else {
            insertLabel(file, entryLabels->name, matchingFoundLabel->address + STARTING_MEMORY_ADDRESS, longest);
        }

        entryLabels = entryLabels->next;
    }

    if (file != NULL) {
        fclose(file);
    }

    return shouldGenerate;
}

/**
 * Returns whether or not more files should be generated after this one (an error occured).
 * The labels are padded to the number of characters of the longest label with spaces to the right.
 * The addresses are decimal and padded to be 4 digits long with zeros to the left.
 * Generates the <fileName>.ext file in the following format:
 * <Label> <Address>
 * <Label> <Address>
 * ...
 *
 * Assumes that the given file name is not NULL and is null-terminated.
 *
 * @param fileName The name of the .ext file to generate.
 * @param externLabels The list of labels marked as extern.
 * @param usedLabels The list of used labels to check for every instance of the labels.
 * @param foundLabels The list of found labels to check for labels that are extern and also defined (invalid).
 * @param shouldGenerate Whether the .ext file should be generated. If not, only checks for errors.
 * @return Whether or not more files should be generated after the .ext file.
 */
Boolean generateExtFile(char fileName[], Label *externLabels, UsedLabel *usedLabels, FoundLabel *foundLabels, Boolean shouldGenerate) {
    UsedLabel *currentUsedLabel;
    FILE *file;
    Length longest;

    file = NULL;
    longest = getLongestLabel(externLabels);

    while (externLabels != NULL) {
        if (getFoundLabel(foundLabels, externLabels->name) != NULL) {
            printError("Label marked as .extern, but also defined.", fileName, externLabels->lineNumber);
            shouldGenerate = FALSE;
            externLabels = externLabels->next;
            continue;
        }

        if (!shouldGenerate) {
            externLabels = externLabels->next;
            continue;
        }

        currentUsedLabel = usedLabels;

        while (currentUsedLabel != NULL) {
            if (strcmp(externLabels->name, currentUsedLabel->name) == EQUAL_STRINGS) {
                if (file == NULL) {
                    file = openFile(fileName, "ext", "w");

                    if (file == NULL) {
                        return FALSE;
                    }
                }

                insertLabel(file, externLabels->name, currentUsedLabel->address, longest);
            }

            currentUsedLabel = currentUsedLabel->next;
        }

        externLabels = externLabels->next;
    }

    if (file != NULL) {
        fclose(file);
    }

    return shouldGenerate;
}

/**
 * Inserts the given list of words into the given file.
 * The address of the first word will be the given starting address.
 * The following addresses will be automatically incremented by one each time.
 *
 * Assumes that the given file pointer is not NULL.
 * Assumes that the starting address is valid, so that all the words will fit in the made-up memory until address 4095.
 *
 * @param file The file to insert the words into.
 * @param words The list of words to insert.
 * @param startingAddress The address of the first word in the list.
 */
void insertWords(FILE *file, Word *words, Address startingAddress) {
    while (words != NULL) {
        fprintf(file, "%04hu %05o\n", startingAddress, (unsigned short)words->data1 + ((unsigned short)words->data2 << (sizeof(words->data1) * BITS_PER_BYTE)));
        startingAddress++;
        words = words->next;
    }
}

/**
 * Inserts the given label into the given file, along with an address.
 *
 * Assumes that the given file pointer is not NULL.
 * Assumes that the given label name string is not NULL and is null-terminated.
 * Assumes that the given address is between 100 and 4095, inclusive (to fit in the made-up memory).
 * Assumes that the given longest label's length is the number of characters in the longest label.
 *
 * @param file The file to insert the label into.
 * @param labelName The name of the label to insert.
 * @param address The address associated with the label.
 * @param longest The number of characters in the longest label.
 */
void insertLabel(FILE *file, char labelName[], Address address, Length longest) {
    fprintf(file, "%-*s %04hu\n", longest, labelName, address);
}
