#include <cstdio>
#include <cstdint>

#include "console.h"
#include "board.h"

int main(int argc, char **argv) {
    //Console con(argc, argv);
    //con.MainLoop();

    Board b;
    printf("%s\n", b.String().c_str());
	
    return 0;
}
