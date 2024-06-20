#ifndef MACRO_EXPANSION_H
#define MACRO_EXPANSION_H

void expandMacros(char *fileNames[], int fileCount);
void expandFileMacros(char inputFileName[], char outputFileName[]);
char *addExtension(char fileName[], char extension[]);
int isEndOfMacro(char line[]);
char *getMacroName(char line[]);
int isValidMacroName(char line[]);
void removeEndingWhitespace(char line[]);
char *skipWhitespace(char line[]);
char *skipCharacters(char line[]);
int isSingleWord(char line[]);

#endif