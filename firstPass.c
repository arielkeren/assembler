#include "firstPass.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "externLabelList.h"
#include "foundLabelList.h"
#include "globals.h"
#include "labelList.h"
#include "usedLabelList.h"
#include "utils.h"
#include "wordList.h"

void firstPass(char fileName[], word **code, word **data, label **entryLabels, externLabel **externLabels, usedLabel **usedLabels, foundLabel **foundLabels, unsigned *instructionCount, unsigned *dataCount) {
    FILE *file;

    file = openFile(fileName, "am", "r");
    firstPassFile(file, code, data, entryLabels, externLabels, usedLabels, foundLabels, instructionCount, dataCount);

    fclose(file);
}

void firstPassFile(FILE *file, word **code, word **data, label **entryLabels, externLabel **externLabels, usedLabel **usedLabels, foundLabel **foundLabels, unsigned *instructionCount, unsigned *dataCount) {
    char line[82];

    while (fgets(line, sizeof(line), file) != NULL) {
        if (line[strlen(line) - 1] != '\n') {
            printf("Line is too long. Maximum length is 80 characters (including whitespace).\n");
            continue;
        }

        handleLine(line, code, data, entryLabels, externLabels, usedLabels, foundLabels, instructionCount, dataCount);
    }
}

void handleLine(char line[], word **code, word **data, label **entryLabels, externLabel **externLabels, usedLabel **usedLabels, foundLabel **foundLabels, unsigned *instructionCount, unsigned *dataCount) {
    char *token;
    char *nextToken;

    line = skipWhitespace(line);

    if (line[0] == ';' || line[0] == '\0') {
        return;
    }

    token = getNextToken(line);

    if (checkIfLabel(token)) {
        line = skipCharacters(line);
        line = skipWhitespace(line);
        nextToken = getNextToken(line);

        if (nextToken == NULL) {
            printf("Label with nothing after it.\n");
            return;
        }

        if (strcmp(token, ".entry") == 0) {
            printf("WARNING: Label before .entry.\n");
        } else if (strcmp(token, ".extern") == 0) {
            printf("WARNING: Label before .extern.\n");
        } else {
            removeEnding(token, ':');
            addFoundLabel(foundLabels, token);
        }

        if (strcmp(token, ".data") == 0 || strcmp(token, ".string") == 0) {
            markAsData(*foundLabels);
            setAddress(*foundLabels, *dataCount);
        } else {
            setAddress(*foundLabels, *instructionCount);
        }

        token = nextToken;
    }

    if (strcmp(token, ".entry") == 0) {
        handleEntryLabel(skipWhitespace(skipCharacters(line)), entryLabels);
    } else if (strcmp(token, ".extern") == 0) {
        handleExternLabel(skipWhitespace(skipCharacters(line)), externLabels);
    } else if (strcmp(token, ".data") == 0) {
        handleData(skipWhitespace(skipCharacters(line)), data, dataCount);
    } else if (strcmp(token, ".string") == 0) {
        handleString(skipWhitespace(skipCharacters(line)), data, dataCount);
    } else {
        handleOperation(line, code, usedLabels, instructionCount);
    }

    free(token);
}

void handleEntryLabel(char line[], label **labels) {
    char *token;

    token = getNextToken(line);

    if (token == NULL) {
        return;
    }

    addLabel(labels, token);
}

void handleExternLabel(char line[], externLabel **externLabels) {
    char *token;

    token = getNextToken(line);

    if (token == NULL) {
        return;
    }

    addExternLabel(externLabels, token);
}

void handleString(char line[], word **data, unsigned *dataCount) {
    char *token;

    token = getNextToken(line);

    if (token == NULL) {
        return;
    }

    if (!validateString(token)) {
        free(token);
        return;
    }

    encodeString(data, token, dataCount);
    free(token);
}

void handleData(char line[], word **data, unsigned *dataCount) {
    /*if (!validateData(line)) {
        return;
    }*/

    encodeNumberList(data, line, dataCount);
}

void handleOperation(char line[], word **code, usedLabel **usedLabels, unsigned *instructionCount) {
    char *token;
    int operandCount;
    char *firstOperand;
    char *secondOperand;
    operandType firstOperandType;
    operandType secondOperandType;

    token = getNextToken(line);

    if (token == NULL) {
        return;
    }

    if (!validateOperation(token)) {
        free(token);
        return;
    }

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

    if (firstOperand == NULL) {
        return;
    }

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
    encodeOperand(*code, token, FALSE);

    if (firstOperandType == secondOperandType && (firstOperandType == DIRECT_REGISTER || firstOperandType == INDIRECT_REGISTER)) {
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
