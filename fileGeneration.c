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

/**
 * Returns whether or not more files should be generated after this one (an
 * error occured).
 *
 * The labels are padded to the number of characters of the longest label with
 * spaces to the right.
 *
 * The addresses are decimal and padded to be 4 digits long with zeros to the
 * left.
 *
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
 * @param shouldGenerate Whether the .ent file should be generated. If not, only
 * checks for missing definitions.
 * @return Whether or not more files should be generated after the .ent file.
 */
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

/**
 * Returns whether or not more files should be generated after this one (an
 * error occured).
 * The labels are padded to the number of characters of the longest label with
 * spaces to the right.
 * The addresses are decimal and padded to be 4 digits long with zeros to the
 * left.
 * Generates the <fileName>.ext file in the following format:
 * <Label> <Address>
 * <Label> <Address>
 * ...
 *
 * Assumes that the given file name is not NULL and is null-terminated.
 *
 * @param fileName The name of the .ext file to generate.
 * @param externLabels The list of labels marked as extern.
 * @param usedLabels The list of used labels to check for every instance of the
 * labels.
 * @param foundLabels The list of found labels to check for labels that are
 * extern and also defined (invalid).
 * @param shouldGenerate Whether the .ext file should be generated. If not, only
 * checks for errors.
 * @return Whether or not more files should be generated after the .ext file.
 */
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
        if (!insertUses(&file, fileName, externLabels->name, usedLabels,
                        longest, &isFirst)) {
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

/**
 * Inserts the given list of words into the given file.
 * The address of the first word will be the given starting address.
 * The following addresses will be automatically incremented by one each time.
 *
 * Assumes that the given file pointer is not NULL.
 * Assumes that the starting address is valid, so that all the words will fit in
 * the made-up memory until address 4095.
 *
 * @param file The file to insert the words into.
 * @param words The list of words to insert.
 * @param startingAddress The address of the first word in the list.
 */
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

/**
 * Inserts all the uses of the given extern label into the given file.
 *
 * Assumes that the given pointer to the file pointer is not NULL.
 * Assumes that the given file name is not NULL and is null-terminated.
 * Assumes that the given label name string is not NULL and is null-terminated.
 * Assumes that the given isFirst boolean pointer is not NULL.
 *
 * @param file The file to insert the uses into.
 * @param fileName The name of the file to insert the uses into.
 * @param labelName The name of the extern label.
 * @param usedLabels The list of used labels to scan.
 * @param longest The number of characters in the longest label.
 * @param isFirst Whether this is the first label inserted.
 */
Boolean insertUses(FILE **file, char fileName[], char labelName[],
                   UsedLabel *usedLabels, Length longest, Boolean *isFirst) {
    /* Loop over the list of used labels. */
    while (usedLabels != NULL) {
        /* Compare the names to check if this is a use of the label. */
        if (strcmp(labelName, usedLabels->name) == EQUAL_STRINGS) {
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
            insertLabel(*file, labelName, usedLabels->address, longest,
                        *isFirst);
            /* The first line has already been inserted. */
            *isFirst = FALSE;
        }

        /* Move on to the next label. */
        usedLabels = usedLabels->next;
    }

    /* There have not been any errors. */
    return TRUE;
}

/**
 * Inserts the given label definition into the given file.
 *
 * Assumes that the given file pointer is not NULL.
 * Assumes that the given label name string is not NULL and is null-terminated.
 * Assumes that the given found label is not NULL.
 *
 * @param file The file to insert the label definition into.
 * @param labelName The name of the label to insert.
 * @param definition The found label that defines the target label.
 * @param instructionCount The final instruction count.
 * @param longest The number of characters in the longest label.
 * @param isFirst Whether this is the first label inserted.
 */
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

/**
 * Inserts the given label into the given file, along with an address.
 *
 * Assumes that the given file pointer is not NULL.
 * Assumes that the given label name string is not NULL and is null-terminated.
 * Assumes that the given address is between 100 and 4095, inclusive (to fit in
 * the made-up memory).
 * Assumes that the given longest label's length is the number of characters in
 * the longest label.
 *
 * @param file The file to insert the label into.
 * @param labelName The name of the label to insert.
 * @param address The address associated with the label.
 * @param longest The number of characters in the longest label.
 * @param isFirst Whether this is the first label to be inserted into some file.
 */
void insertLabel(FILE *file, char labelName[], Address address, Length longest,
                 Boolean isFirst) {
    /* Insert the label, along with its address. Add newlines accordingly. */
    fprintf(file, "%s%-*s %04hu", isFirst ? "" : "\n", longest, labelName,
            address);
}
