#ifndef CONSOLE_H_
#define CONSOLE_H_

#include "board.h"

class Console {
 public:
    Console(int argc, char **argv);

    void MainLoop();

 private:

    Board::Move ParseMove(char *str) const;
    Board::Player ParseSide(char *str) const;
    
    Board _board;
};

#endif // CONSOLE_H_
