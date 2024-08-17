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

#include "errorHandling.h" /* Getting the error status. */
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
 * @return The exit status of the program (0 - successful, 1 - failed).
 */
int main(int argc, char *argv[]) {
    /* Check the possibility of no files being provided. */
    handleNoFiles(argc - ARGS_DIFF);
    /* Start the compilation process of every file provided. */
    compileAllFiles(argv + ARGS_DIFF, argc - ARGS_DIFF);

    /* Return 1 if any errors occurred, otherwise 0. */
    return getErrorStatus() ? ERROR : SUCCESS;
}
