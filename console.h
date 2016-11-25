#ifndef CONSOLE_H_
#define CONSOLE_H_

#include "board.h"

class Console {
 public:
    Console(int argc, char **argv);

    void MainLoop();

 private:

    Board _board;
};

#endif // CONSOLE_H_
