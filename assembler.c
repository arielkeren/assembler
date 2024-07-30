/*
 * assembler.c
 *
 * Contains the main function to start the program.
 *
 * Name: Ariel Keren
 * Course: C Lab, 20465
 * Semester: 2024B
 * Date: 30/07/2024
 */

#include "fileHandling.h"
#include "globals.h"

int main(int argc, char *argv[]) {
    checkNoFiles(argc - ARGS_DIFF);
    compileFiles(argv + ARGS_DIFF, argc - ARGS_DIFF);

    return SUCCESS;
}
