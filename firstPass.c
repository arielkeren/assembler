#include "firstPass.h"

#include <stdio.h>
#include <string.h>

#include "globals.h"
#include "labelList.h"
#include "usedLabelList.h"
#include "utils.h"
#include "wordList.h"

void firstPass(char fileName[], word **code, word **data, label **entryLabels, label **externLabels, usedLabel **usedLabels, unsigned *instructionCount, unsigned *dataCount) {
    FILE *file;

    file = openFile(fileName, "am", "r");
    firstPassFile(file, code, data, entryLabels, externLabels, usedLabels, instructionCount, dataCount);

    fclose(file);
}

void firstPassFile(FILE *file, word **code, word **data, label **entryLabels, label **externLabels, usedLabel **usedLabels, unsigned *instructionCount, unsigned *dataCount) {
    char line[82];

    while (fgets(line, sizeof(line), file) != NULL) {
        if (line[strlen(line) - 1] != '\n') {
            printf("Line is too long. Maximum length is 80 characters (including whitespace).\n");
            continue;
        }

        handleLine(line, code, data, entryLabels, externLabels, usedLabels, instructionCount, dataCount);
    }
}

void handleLine(char line[], word **code, word **data, label **entryLabels, label **externLabels, usedLabel **usedLabels, unsigned *instructionCount, unsigned *dataCount) {
    char *token;

    line = skipWhitespace(line);

    if (line[0] == ';' || line[0] == '\0') {
        return;
    }

    token = getNextToken(line);

    if (checkIfLabel(token)) {
        line = skipCharacters(line);
        line = skipWhitespace(line);
        free(token);
        token = getNextToken(line);

        if (token == NULL) {
            printf("Label with nothing after it.\n");
            return;
        }
    }

    if (strcmp(token, ".entry") == 0) {
        handleLabel(skipWhitespace(skipCharacters(line)), entryLabels);
    } else if (strcmp(token, ".extern") == 0) {
        handleLabel(skipWhitespace(skipCharacters(line)), externLabels);
    } else if (strcmp(token, ".data") == 0) {
        (*dataCount)++;
        handleData(skipWhitespace(skipCharacters(line)), data);
    } else if (strcmp(token, ".string") == 0) {
        (*dataCount)++;
        handleString(skipWhitespace(skipCharacters(line)), data);
    } else {
        (*instructionCount)++;
        handleOperation(line, code, usedLabels);
    }

    free(token);
}

void handleLabel(char line[], label **labels) {
    char *token;

    token = getNextToken(line);

    if (token == NULL) {
        return;
    }

    addLabel(labels, token);
}

void handleString(char line[], word **data) {
    char *token;

    token = getNextToken(line);

    if (token == NULL) {
        return;
    }

    if (!validateString(token)) {
        free(token);
        return;
    }

    encodeString(data, token);
    free(token);
}

void handleData(char line[], word **data) {
    if (!validateData(line)) {
        return;
    }

    encodeNumberList(data, line);
}

void handleOperation(char line[], word **code, usedLabel **usedLabels) {
    char *token;
    int operandCount;
    int isRegister;
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

    addWord(*code);
    encodeMetadata(*code, 'A');
    encodeOperation(*code, token);

    operandCount = getOperandCount(*code, token);
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
            addUsedLabel(usedLabels, firstOperand, *code);
        }

        free(firstOperand);
        return;
    }

    line = skipCharacters(line);
    line = skipWhitespace(line);
    secondOperand = getNextToken(line);

    secondOperandType = getOperandType(secondOperand);
    encodeOperand(*code, token, FALSE);

    if (firstOperandType == secondOperandType && (firstOperandType == DIRECT_REGISTER || firstOperandType == INDIRECT_REGISTER)) {
        addWord(code);
        encodeExtraWord(*code, firstOperand, TRUE);
        encodeExtraWord(*code, secondOperand, FALSE);
    } else {
        addWord(code);
        encodeExtraWord(*code, firstOperand, TRUE);

        if (firstOperandType == DIRECT) {
            addUsedLabel(usedLabels, firstOperand, *code);
        }

        addWord(code);
        encodeExtraWord(*code, secondOperand, FALSE);

        if (secondOperandType == DIRECT) {
            addUsedLabel(usedLabels, secondOperand, *code);
        }
    }
}
