#include <stdio.h>

#include "macroExpansion.h"

int main(int argc, char* argv[]) {
    expandMacros(&argv[1], argc - 1);

    return 0;
}
