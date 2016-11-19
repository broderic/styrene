
#include <cstdio>
#include <cstdint>

#include "console.h"

int main(int argc, char **argv) {
    Console con(argc, argv);
    con.MainLoop();
    return 0;
}
