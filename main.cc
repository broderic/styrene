#include <cstdio>
#include <cstdint>

#include "console.h"
#include "board.h"

int main(int argc, char **argv) {
    Console con(argc, argv);
    con.MainLoop();
#if 0
    Board b;
    printf("%s\n", b.String().c_str());
    b.Play(Board::WHITE, Board::Move(Board::E2,Board::E4));
    printf("%s\n", b.String().c_str());
    b.Play(Board::BLACK, Board::Move(Board::D7,Board::D5));
    printf("%s\n", b.String().c_str());
    b.Play(Board::WHITE, Board::Move(Board::E4,Board::D5));
    printf("%s\n", b.String().c_str());
    b.Play(Board::WHITE, Board::Move(Board::D5,Board::D6));
    printf("%s\n", b.String().c_str());
    b.Undo();
    printf("%s\n", b.String().c_str());
    b.Undo();
    printf("%s\n", b.String().c_str());
    b.Undo();
    printf("%s\n", b.String().c_str());    
    return 0;
#endif
}
