#include "fileReading.h"

#include <stdlib.h>
#include <string.h>

#include "encoder.h"
#include "foundLabelList.h"
#include "globals.h"
#include "instructionInformation.h"
#include "labelList.h"
#include "lineValidation.h"
#include "macroTable.h"
#include "usedLabelList.h"
#include "utils.h"
#include "wordList.h"

Boolean readFile(char fileName[], Macro *macros, Word *code, Word *data, Label **entryLabels, Label **externLabels, UsedLabel **usedLabels, FoundLabel **foundLabels, unsigned *instructionCount, unsigned *dataCount) {
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

Boolean readLines(char fileName[], FILE *file, Macro *macros, Word *code, Word *data, Label **entryLabels, Label **externLabels, UsedLabel **usedLabels, FoundLabel **foundLabels, unsigned *instructionCount, unsigned *dataCount) {
    Boolean isSuccessful;
    char line[MAX_LINE_LENGTH + NEWLINE_BYTE + NULL_BYTE];
    unsigned lineNumber;

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

Boolean handleLine(char fileName[], char line[], unsigned lineNumber, Macro *macros, Word **code, Word **data, Label **entryLabels, Label **externLabels, UsedLabel **usedLabels, FoundLabel **foundLabels, unsigned *instructionCount, unsigned *dataCount) {
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
        if (containsLabel(*entryLabels, nextToken)) {
            printError("Label already marked as entry.", fileName, lineNumber);
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

        addLabel(entryLabels, nextToken, lineNumber);
    } else if (strcmp(token, ".extern") == EQUAL_STRINGS) {
        if (containsLabel(*externLabels, nextToken)) {
            printError("Label already marked as extern.", fileName, lineNumber);
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

        addLabel(externLabels, nextToken, lineNumber);
    } else if (strcmp(token, ".data") == EQUAL_STRINGS) {
        free(nextToken);
        encodeNumberList(data, skipWhitespace(skipCharacters(line)), dataCount);
    } else if (strcmp(token, ".string") == EQUAL_STRINGS) {
        encodeString(data, nextToken, dataCount);
        free(nextToken);
    } else {
        free(nextToken);
        handleOperation(line, lineNumber, code, usedLabels, instructionCount);
    }

    free(token);
    return isSuccessful;
}

Boolean handleLabel(char fileName[], char line[], unsigned lineNumber, Macro *macros, FoundLabel **foundLabels, unsigned instructionCount, unsigned dataCount) {
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

void handleOperation(char line[], unsigned lineNumber, Word **code, UsedLabel **usedLabels, unsigned *instructionCount) {
    char *token;
    OperandCount operands;
    char *firstOperand;
    char *secondOperand;
    OperandType firstOperandType;
    OperandType secondOperandType;

    token = getNextToken(line);

    (*instructionCount)++;
    *code = addWord(*code);
    encodeMetadata(*code, 'A');
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
