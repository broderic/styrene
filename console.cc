#include "console.h"

#include <cstdio>
#include <cstring>

Console::Console(int argc, char **argv) {

}

void Console::MainLoop() {
    while (true) {
	char cmd[1024];
	char *c = fgets(cmd, 1000, stdin);
	
	if (!strcmp(cmd, "quit")) {
	    break;
	}
	else {
	    printf("Unknown '%s'", cmd);
	}
    }
}
