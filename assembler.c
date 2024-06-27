#include "fileHandling.h"
#include "globals.h"

int main(int argc, char *argv[]) {
    checkNoFiles(argc - 1);
    compileFiles(&argv[1], argc - 1);

    return 0;
}
