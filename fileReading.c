/*
 * fileReading.c
 *
 * Contains functions to read the .am file, which was generated by the macro expansion stage.
 * Gets everything ready to link the labels and then to generate the necessary files.
 *
 * Name: Ariel Keren
 * Course: C Lab, 20465
 * Semester: 2024B
 * Date: 30/07/2024
 */

#include "fileReading.h"

#include <stdlib.h> /* free. */
#include <string.h> /* strcmp. */

#include "encoder.h"                /* Encoding into words. */
#include "errorHandling.h"          /* Printing errors and warnings. */
#include "foundLabelList.h"         /* Searching through the found label list. */
#include "globals.h"                /* Constants and typedefs. */
#include "instructionInformation.h" /* Information about the different instructions. */
#include "labelList.h"              /* Searching through the entry and extern label lists. */
#include "lineValidation.h"         /* Validating lines before adding new words. */
#include "macroTable.h"             /* Searching through the macro table. */
#include "usedLabelList.h"          /* Adding labels that are used as operands. */
#include "utils.h"                  /* Opening the .am file and parsing lines. */
#include "wordList.h"               /* Adding new words. */

/**
 * Opens the .am file and starts reading it line by line.
 * Modifies the given arguments to contain the information read from the .am file.
 * Returns whether or not no errors occurred.
 *
 * @param fileName The name of the file to read.
 * @param macros The macro table.
 * @param code The code word list.
 * @param data The data word list.
 * @param entryLabels The list of entry labels.
 * @param externLabels The list of extern labels.
 * @param usedLabels The list of used labels.
 * @param foundLabels The list of found labels.
 * @param instructionCount The current instruction count to modify.
 * @param dataCount The current data count to modify.
 * @return TRUE if no errors occurred, FALSE otherwise or if the file could not be opened.
 */
Boolean readFile(char fileName[], Macro *macros, Word *code, Word *data, Label **entryLabels, Label **externLabels, UsedLabel **usedLabels, FoundLabel **foundLabels, WordCount *instructionCount, WordCount *dataCount) {
    Boolean isSuccessful;
    FILE *file;

    file = openFile(fileName, "am", "r");

    if (file == NULL) {
        return FALSE;
    }

    isSuccessful = readLines(fileName, file, macros, code, data, entryLabels, externLabels, usedLabels, foundLabels, instructionCount, dataCount);

    fclose(file);
    return isSuccessful;
}

/**
 * Reads the lines of the .am file line by line.
 * Modifies the given arguments to contain the information read from the .am file.
 * Returns whether or not no errors occurred.
 *
 * @param fileName The name of the file to read.
 * @param file The file to read.
 * @param macros The macro table.
 * @param code The code word list.
 * @param data The data word list.
 * @param entryLabels The list of entry labels.
 * @param externLabels The list of extern labels.
 * @param usedLabels The list of used labels.
 * @param foundLabels The list of found labels.
 * @param instructionCount The current instruction count to modify.
 * @param dataCount The current data count to modify.
 * @return TRUE if no errors occurred, FALSE otherwise.
 */
Boolean readLines(char fileName[], FILE *file, Macro *macros, Word *code, Word *data, Label **entryLabels, Label **externLabels, UsedLabel **usedLabels, FoundLabel **foundLabels, WordCount *instructionCount, WordCount *dataCount) {
    Boolean isSuccessful;
    char line[MAX_LINE_LENGTH + NEWLINE_BYTE + NULL_BYTE];
    LineNumber lineNumber;

    isSuccessful = TRUE;
    lineNumber = INITIAL_VALUE;

    while (fgets(line, sizeof(line), file) != NULL) {
        lineNumber++;

        if (!validateLine(line, fileName, lineNumber)) {
            isSuccessful = FALSE;
            handleLabel(fileName, skipWhitespace(line), lineNumber, macros, foundLabels, *instructionCount, *dataCount);
            continue;
        }

        isSuccessful = handleLine(fileName, line, lineNumber, macros, &code, &data, entryLabels, externLabels, usedLabels, foundLabels, instructionCount, dataCount) && isSuccessful;

        if (*instructionCount + *dataCount > TOTAL_MEMORY_CELLS - STARTING_MEMORY_ADDRESS) {
            printError("Memory overflow. Too many words in the program.", fileName, lineNumber);
            isSuccessful = FALSE;
        }
    }

    return isSuccessful;
}

/**
 * Handles a line of the .am file.
 * Modifies the given arguments to contain the information read from the current line.
 * Returns whether or not no errors occurred.
 *
 * @param fileName The name of the file to read.
 * @param line The line that has been extracted from the .am file.
 * @param lineNumber The current line's line number.
 * @param macros The macro table.
 * @param code The code word list.
 * @param data The data word list.
 * @param entryLabels The list of entry labels.
 * @param externLabels The list of extern labels.
 * @param usedLabels The list of used labels.
 * @param foundLabels The list of found labels.
 * @param instructionCount The current instruction count to modify.
 * @param dataCount The current data count to modify.
 * @return TRUE if no errors occurred, FALSE otherwise.
 */
Boolean handleLine(char fileName[], char line[], LineNumber lineNumber, Macro *macros, Word **code, Word **data, Label **entryLabels, Label **externLabels, UsedLabel **usedLabels, FoundLabel **foundLabels, WordCount *instructionCount, WordCount *dataCount) {
    Boolean isSuccessful;
    char *token;
    char *nextToken;

    if (*line == ';') {
        return TRUE;
    }

    line = skipWhitespace(line);

    if (*line == '\0') {
        return TRUE;
    }

    isSuccessful = handleLabel(fileName, line, lineNumber, macros, foundLabels, *instructionCount, *dataCount);
    token = getNextToken(line);
    nextToken = getNextToken(skipWhitespace(skipCharacters(line)));

    if (checkIfLabel(token)) {
        free(token);
        token = nextToken;

        line = skipCharacters(line);
        line = skipWhitespace(line);
        nextToken = getNextToken(skipWhitespace(skipCharacters(line)));
    }

    if (strcmp(token, ".entry") == EQUAL_STRINGS) {
        if (getMacroContent(macros, token) != NULL) {
            free(token);
            free(nextToken);
            printError("Label's name already taken by a macro.", fileName, lineNumber);
            return FALSE;
        }

        if (containsLabel(*entryLabels, nextToken)) {
            free(token);
            free(nextToken);
            printWarning("Label already declared as entry.", fileName, lineNumber);
            return isSuccessful;
        }

        addLabel(entryLabels, nextToken, lineNumber);
    } else if (strcmp(token, ".extern") == EQUAL_STRINGS) {
        if (getMacroContent(macros, token) != NULL) {
            free(token);
            free(nextToken);
            printError("Label's name already taken by a macro.", fileName, lineNumber);
            return FALSE;
        }

        if (containsLabel(*externLabels, nextToken)) {
            free(token);
            free(nextToken);
            printWarning("Label already declared as extern.", fileName, lineNumber);
            return isSuccessful;
        }

        addLabel(externLabels, nextToken, lineNumber);
    } else if (strcmp(token, ".data") == EQUAL_STRINGS) {
        free(nextToken);
        encodeNumberList(data, skipWhitespace(skipCharacters(line)), dataCount);
    } else if (strcmp(token, ".string") == EQUAL_STRINGS) {
        free(nextToken);
        encodeString(data, skipWhitespace(skipCharacters(line)), dataCount);
    } else {
        free(nextToken);
        handleOperation(line, lineNumber, code, usedLabels, instructionCount);
    }

    free(token);
    return isSuccessful;
}

/**
 * Handles a potential label in the current line of the .am file.
 * Modifies the given arguments to contain the new information.
 *
 * @param fileName The name of the file to read.
 * @param line The current line.
 * @param lineNumber The current line's line number.
 * @param macros The macro table.
 * @param foundLabels The list of found labels.
 * @param instructionCount The current instruction count.
 * @param dataCount The current data count.
 * @return TRUE if no errors occurred, FALSE otherwise.
 */
Boolean handleLabel(char fileName[], char line[], LineNumber lineNumber, Macro *macros, FoundLabel **foundLabels, WordCount instructionCount, WordCount dataCount) {
    char *token;
    char *nextToken;

    token = getNextToken(line);

    if (checkIfLabel(token)) {
        removeEnding(token, ':');
        line = skipCharacters(line);
        line = skipWhitespace(line);
        nextToken = getNextToken(line);

        if (getFoundLabel(*foundLabels, token) != NULL) {
            printError("Label already defined.", fileName, lineNumber);
            free(token);
            free(nextToken);
            return FALSE;
        }

        if (getMacroContent(macros, token) != NULL) {
            printError("Label's name already taken by a macro.", fileName, lineNumber);
            free(token);
            free(nextToken);
            return FALSE;
        }

        if (strcmp(nextToken, ".entry") == EQUAL_STRINGS || strcmp(nextToken, ".extern") == EQUAL_STRINGS) {
            free(token);
            free(nextToken);
            return TRUE;
        }

        addFoundLabel(foundLabels, token);

        if (strcmp(nextToken, ".data") == EQUAL_STRINGS || strcmp(nextToken, ".string") == EQUAL_STRINGS) {
            markAsData(*foundLabels);
            setAddress(*foundLabels, dataCount);
        } else {
            setAddress(*foundLabels, instructionCount);
        }

        free(nextToken);
        return TRUE;
    }

    free(token);
    return TRUE;
}

/**
 * Handles an operation in the current line of the .am file.
 * Modifies the given arguments to contain the new information.
 *
 * @param line The current line.
 * @param lineNumber The current line's line number.
 * @param code The code word list.
 * @param usedLabels The list of used labels.
 * @param instructionCount The current instruction count.
 */
void handleOperation(char line[], LineNumber lineNumber, Word **code, UsedLabel **usedLabels, WordCount *instructionCount) {
    char *token;
    OperandCount operands;
    char *firstOperand;
    char *secondOperand;
    OperandType firstOperandType;
    OperandType secondOperandType;

    token = getNextToken(line);

    (*instructionCount)++;
    *code = addWord(*code);
    encodeAddressingMode(*code, 'A');
    encodeOperation(*code, token);

    operands = getOperandCount(token);
    free(token);

    if (operands == NO_OPERANDS) {
        return;
    }

    line = skipCharacters(line);
    line = skipWhitespace(line);
    firstOperand = getNextToken(line);

    encodeOperand(*code, firstOperand, operands != ONE_OPERAND);
    firstOperandType = getOperandType(firstOperand);

    if (operands == ONE_OPERAND) {
        *code = addWord(*code);
        encodeExtraWord(*code, firstOperand, FALSE);

        if (firstOperandType == DIRECT) {
            addUsedLabel(usedLabels, firstOperand, *instructionCount + STARTING_MEMORY_ADDRESS, lineNumber, *code);
        } else {
            free(firstOperand);
        }

        (*instructionCount)++;
        return;
    }

    line = skipCharacters(line);
    line = skipWhitespace(line);
    secondOperand = getNextToken(line);

    secondOperandType = getOperandType(secondOperand);
    encodeOperand(*code, secondOperand, FALSE);

    if ((firstOperandType == DIRECT_REGISTER || firstOperandType == INDIRECT_REGISTER) && (secondOperandType == DIRECT_REGISTER || secondOperandType == INDIRECT_REGISTER)) {
        (*instructionCount)++;
        *code = addWord(*code);
        encodeExtraWord(*code, firstOperand, TRUE);
        encodeExtraWord(*code, secondOperand, FALSE);
    } else {
        *code = addWord(*code);
        encodeExtraWord(*code, firstOperand, TRUE);

        if (firstOperandType == DIRECT) {
            addUsedLabel(usedLabels, firstOperand, *instructionCount + STARTING_MEMORY_ADDRESS, lineNumber, *code);
        } else {
            free(firstOperand);
        }
        (*instructionCount)++;

        *code = addWord(*code);
        encodeExtraWord(*code, secondOperand, FALSE);

        if (secondOperandType == DIRECT) {
            addUsedLabel(usedLabels, secondOperand, *instructionCount + STARTING_MEMORY_ADDRESS, lineNumber, *code);
        } else {
            free(secondOperand);
        }
        (*instructionCount)++;
    }
}
