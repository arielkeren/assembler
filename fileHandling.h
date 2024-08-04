/*
 * fileHandling.h
 *
 * Contains the function prototypes for the functions in fileHandling.c.
 *
 * Name: Ariel Keren
 * Course: C Lab, 20465
 * Semester: 2024B
 * Date: 30/07/2024
 */

#ifndef FILE_HANDLING_H
#define FILE_HANDLING_H

/**
 * Checks whether or not no files have been provided as command line arguments.
 * If no files have been provided, prints an error and exits the program.
 *
 * Assumes that the given file count is the number of file names provided as
 * command line arguments.
 *
 * @param fileCount The number of files provided as command line arguments.
 */
void checkNoFiles(int fileCount);

/**
 * Starts the compilation process of the provided files.
 * Compiles each file separately.
 *
 * Assumes that the given file names array is not NULL and contains only
 * non-NULL and null-terminated file names.
 * Assumes that the given file count is the number of file names to try to
 * compile from the given file names array.
 *
 * @param fileNames The names of the files to compile.
 * @param fileCount The number of files to compile.
 */
void compileFiles(char *fileNames[], int fileCount);

#endif
