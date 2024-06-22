#ifndef FIRST_PASS_H
#define FIRST_PASS_H

#include <stdio.h>

#include "globals.h"

void firstPass(char fileName[]);
void firstPassFile(FILE *inputFile);
void handleLine(char line[], word **code, word **data, label **entryLabels, label **externLabels, usedLabel **usedLabels);
void handleLabel(char line[], label **labels);
void handleString(char line[], word **data);
void handleData(char line[], word **data);
void handleOperation(char line[], word **code, usedLabel **usedLabels);


#endif
