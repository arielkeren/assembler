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

#include "fileHandling.h" /* Handling the files in the command line arguments. */
#include "globals.h"      /* Constants. */

/**
 * The main function which executes the program.
 * Expects file names to be given as command line arguments.
 * Uses these names to read the .as source files.
 * Compiles them and generates the following files (if successful):
 * - .ob: Contains the words that need to be loaded into memory in octal
 * representation.
 * - .ent: Contains the entry labels, along with their addresses.
 * - .ext: Contains the extern labels, along with the addresses they have been
 * used at.
 *
 * @param argc The number of command line arguments (including the program's
 * name).
 * @param argv The command line arguments (including the program's name).
 * @return 0 when runs successfully.
 */
int main(int argc, char *argv[]) {
    /* Check the possibility of no files being provided. */
    checkNoFiles(argc - ARGS_DIFF);
    /* Start the compilation process of every file provided. */
    compileAllFiles(argv + ARGS_DIFF, argc - ARGS_DIFF);

    return SUCCESS;
}
