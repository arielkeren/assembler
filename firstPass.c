#include <stdio.h>

#include "globals.h"
#include "utils.h"
#include "wordList.h"

void firstPass(char fileName[]) {
    char *fileNameWithExtension;
    FILE *file;

    fileNameWithExtension = addExtension(fileName, "am");
    file = openFile(fileNameWithExtension, "r");

    firstPassFile(file);

    free(fileNameWithExtension);
    fclose(file);
}

void firstPassFile(FILE *inputFile) {
    word *code;
    word *data;
    label *entryLabels;
    label *externLabels;
    usedLabel *usedLabels;
    char line[82];

    code = NULL;
    data = NULL;

    while (fgets(line, sizeof(line), inputFile) != NULL) {
        handleLine(line, &code, &data, &entryLabels, &externLabels, &usedLabels);
    }
}

void handleLine(char line[], word **code, word **data, label **entryLabels, label **externLabels, usedLabel **usedLabels) {
    line = skipWhitespace(line);

    if (line[0] == ';') {
        return;
    }

    if (strncmp(line, ".entry", 5) == 0) {
        handleLabel(skipWhitespace(&line[5]), entryLabels);
    } else if (strncmp(line, ".extern", 7) == 0) {
        handleLabel(skipWhitespaces(&line[7]), externLabels);
    } else if (strncmp(line, ".data", 5) == 0) {
        handleData(skipWhitespace(&line[5]), data);
    } else if (strncmp(line, ".string", 7) == 0) {
        handleString(skipWhitespace(&line[7]), data);
    } else {
        handleOperation(line, code, usedLabels);
    }
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
