#include "secondPass.h"

#include <stdio.h>

#include "foundLabelList.h"
#include "globals.h"
#include "utils.h"

void secondPass(char fileName[], word *code, label *entryLabels, label *externLabels, usedLabel *usedLabels, unsigned instructionCount) {
    FILE *file;

    file = openFile(fileName, "am", "r");
    secondPassFile(file, code, entryLabels, externLabels, usedLabels, instructionCount);

    fclose(file);
}

void secondPassFile(FILE *file, word *code, label *entryLabels, label *externLabels, usedLabel *usedLabels, unsigned instructionCount) {
    foundLabel *foundLabels;
    unsigned currentInstruction;
    unsigned currentData;
    char line[82];

    currentInstruction = 0;
    currentData = 0;
    foundLabels = NULL;

    while (fgets(line, sizeof(line), file) != NULL) {
        findLabels(line, code, &foundLabels, entryLabels, externLabels, usedLabels, instructionCount, &currentInstruction, &currentData);
    }

    insertLabelAddresses(usedLabels, foundLabels, externLabels);
    freeFoundLabelList(foundLabels);
}

void findLabels(char line[], word *code, foundLabel **foundLabels, label *entryLabels, label *externLabels, usedLabel *usedLabels, unsigned instructionCount, unsigned *currentInstruction, unsigned *currentData) {
    char *token;

    line = skipWhitespace(line);
    token = getNextToken(line);

    if (!checkIfLabel(token)) {
        free(token);
        line = skipCharacters(line);
        line = skipWhitespace(line);
        token = getNextToken(line);

        if (strcmp(token, ".data") == 0) {
            (*currentData)++;
        } else if (strcmp(token, ".string") == 0) {
            (*currentData)++;
        } else if (strcmp(token, ".entry") != 0 && strcmp(token, ".extern") != 0) {
            (*currentInstruction)++;
        }

        free(token);
        return;
    }

    removeEnding(token, ':');
    addFoundLabel(foundLabels, token);

    line = skipCharacters(line);
    line = skipWhitespace(line);
    token = getNextToken(line);

    if (strcmp(token, ".data") == 0) {
        putAddress(entryLabels, (*foundLabels)->name, *currentData + instructionCount + 100);
        (*currentData)++;
    } else if (strcmp(token, ".string") == 0) {
        putAddress(entryLabels, (*foundLabels)->name, *currentData + instructionCount + 100);
        (*currentData)++;
    } else if (strcmp(token, ".entry") != 0 && strcmp(token, ".extern") != 0) {
        putAddress(entryLabels, (*foundLabels)->name, *currentInstruction + 100);
        (*currentInstruction)++;
    }

    free(token);
}

void insertLabelAddresses(usedLabel *usedLabels, foundLabel *foundLabels, label *externLabels) {
    foundLabel *currentFoundLabel;

    while (usedLabels != NULL) {
        encodeMetadata(usedLabels->wordPointer, 'E');
        currentFoundLabel = foundLabels;

        while (currentFoundLabel != NULL) {
            if (strcmp(usedLabels->name, currentFoundLabel->name) == 0) {
                encodeLabel(usedLabels->wordPointer, currentFoundLabel->address);
                break;
            }

            currentFoundLabel = currentFoundLabel->next;
        }
    }
}
