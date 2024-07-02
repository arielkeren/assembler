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

void readFile(char fileName[], macro *macros, word **code, word **data, label **entryLabels, label **externLabels, usedLabel **usedLabels, foundLabel **foundLabels, unsigned *instructionCount, unsigned *dataCount) {
    FILE *file;

    file = openFile(fileName, "am", "r");

    if (file == NULL) {
        return;
    }

    readLines(file, macros, code, data, entryLabels, externLabels, usedLabels, foundLabels, instructionCount, dataCount);

    fclose(file);
}

void readLines(FILE *file, macro *macros, word **code, word **data, label **entryLabels, label **externLabels, usedLabel **usedLabels, foundLabel **foundLabels, unsigned *instructionCount, unsigned *dataCount) {
    char line[82];
    unsigned lineNumber;

    lineNumber = 0;

    while (fgets(line, sizeof(line), file) != NULL) {
        lineNumber++;

        if (line[strlen(line) - 1] != '\n' && !feof(file)) {
            printError("Line is too long. Maximum length is 80 characters (including whitespace).", lineNumber);
            continue;
        }

        if (*instructionCount + *dataCount > 3997) {
            printError("Too many words in the program - memory overflow.", lineNumber);
        }

        if (!validateLine(line, lineNumber)) {
            continue;
        }

        handleLine(line, lineNumber, macros, code, data, entryLabels, externLabels, usedLabels, foundLabels, instructionCount, dataCount);
    }
}

void handleLine(char line[], unsigned lineNumber, macro *macros, word **code, word **data, label **entryLabels, label **externLabels, usedLabel **usedLabels, foundLabel **foundLabels, unsigned *instructionCount, unsigned *dataCount) {
    char *token;
    char *nextToken;

    line = skipWhitespace(line);

    if (line[0] == ';' || line[0] == '\0') {
        return;
    }

    token = getNextToken(line);

    if (checkIfLabel(token)) {
        removeEnding(token, ':');
        line = skipCharacters(line);
        line = skipWhitespace(line);
        nextToken = getNextToken(line);

        if (getFoundLabel(*foundLabels, token) != NULL) {
            printError("Label already defined.", lineNumber);
        }

        if (getMacroContent(macros, token) != NULL) {
            printError("Label's name already taken by a macro.", lineNumber);
        }

        if (strcmp(nextToken, ".entry") != 0 && strcmp(nextToken, ".extern") != 0) {
            addFoundLabel(foundLabels, token);

            if (strcmp(nextToken, ".data") == 0 || strcmp(nextToken, ".string") == 0) {
                markAsData(*foundLabels);
                setAddress(*foundLabels, *dataCount);
            } else {
                setAddress(*foundLabels, *instructionCount);
            }
        } else {
            free(token);
        }

        token = nextToken;
    }

    nextToken = getNextToken(skipWhitespace(skipCharacters(line)));

    if (strcmp(token, ".entry") == 0) {
        if (containsLabel(*entryLabels, nextToken)) {
            printError("Label already marked as entry.", lineNumber);
        }

        if (getMacroContent(macros, token) != NULL) {
            printError("Label's name already taken by a macro.", lineNumber);
        }

        addLabel(entryLabels, nextToken, lineNumber);
    } else if (strcmp(token, ".extern") == 0) {
        if (containsLabel(*externLabels, nextToken)) {
            printError("Label already marked as extern.", lineNumber);
        }

        if (getMacroContent(macros, token) != NULL) {
            printError("Label's name already taken by a macro.", lineNumber);
        }

        addLabel(externLabels, nextToken, lineNumber);
    } else if (strcmp(token, ".data") == 0) {
        encodeNumberList(data, skipWhitespace(skipCharacters(line)), dataCount);
        free(nextToken);
    } else if (strcmp(token, ".string") == 0) {
        encodeString(data, nextToken, dataCount);
        free(nextToken);
    } else {
        handleOperation(line, code, usedLabels, instructionCount);
        free(nextToken);
    }

    free(token);
}

void handleOperation(char line[], word **code, usedLabel **usedLabels, unsigned *instructionCount) {
    char *token;
    int operandCount;
    char *firstOperand;
    char *secondOperand;
    operandType firstOperandType;
    operandType secondOperandType;

    token = getNextToken(line);

    (*instructionCount)++;
    addWord(code);
    encodeMetadata(*code, 'A');
    encodeOperation(*code, token);

    operandCount = getOperandCount(token);
    free(token);

    if (operandCount == 0) {
        return;
    }

    line = skipCharacters(line);
    line = skipWhitespace(line);
    firstOperand = getNextToken(line);

    encodeOperand(*code, firstOperand, operandCount != 1);
    firstOperandType = getOperandType(firstOperand);

    if (operandCount == 1) {
        addWord(code);
        encodeExtraWord(*code, firstOperand, FALSE);

        if (firstOperandType == DIRECT) {
            addUsedLabel(usedLabels, firstOperand, *instructionCount + 100, *code);
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
        addWord(code);
        encodeExtraWord(*code, firstOperand, TRUE);
        encodeExtraWord(*code, secondOperand, FALSE);
    } else {
        addWord(code);
        encodeExtraWord(*code, firstOperand, TRUE);

        if (firstOperandType == DIRECT) {
            addUsedLabel(usedLabels, firstOperand, *instructionCount + 100, *code);
        } else {
            free(firstOperand);
        }
        (*instructionCount)++;

        addWord(code);
        encodeExtraWord(*code, secondOperand, FALSE);

        if (secondOperandType == DIRECT) {
            addUsedLabel(usedLabels, secondOperand, *instructionCount + 100, *code);
        } else {
            free(secondOperand);
        }
        (*instructionCount)++;
    }
}
