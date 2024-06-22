#include <stdio.h>

void checkNoFiles(int fileCount) {
    if (fileCount == 0) {
        printf("No files to compile.\n");
        exit(1);
    }
}

void compileFiles(char *fileNames[], int fileCount) {
    char *inputFileName;
    char *outputFileName;
    FILE *inputFile;
    FILE *outputFile;

    while (fileCount > 0) {
        expandMacros(*fileNames);
        firstPass(*fileNames);

        fileCount--;
        fileNames++;
    }
}
