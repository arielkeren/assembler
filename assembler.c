#include "fileHandling.h"
#include "globals.h"

int main(int argc, char *argv[]) {
    checkNoFiles(argc - ARGS_DIFF);
    compileFiles(argv + ARGS_DIFF, argc - ARGS_DIFF);

    return SUCCESS;
}
