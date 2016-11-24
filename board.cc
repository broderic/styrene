#include "board.h"

#include <sstream>
#include <string>

Board::Board() {
    NewGame();
}

//
//   56 57 58 59 60 61 62 63
//   48 49 50 51 52 53 54 55
//   40 41 42 43 44 45 46 47
//   32 33 34 35 36 37 38 39
//   24 25 26 27 28 29 30 31
//   16 17 18 19 20 21 22 23
//    8  9 10 11 12 13 14 15
//    0  1  2  3  4  5  6  7 
//
//    3  2  1  0
//    8  4  2  1

void Board::NewGame() {
    _pieces[WHITE][PAWN]   = Bitmask(RANK2);
    _pieces[WHITE][KING]   = Bitmask(E1);
    _pieces[WHITE][QUEEN]  = Bitmask(D1);
    _pieces[WHITE][BISHOP] = Bitmask(C1) | Bitmask(F1);
    _pieces[WHITE][KNIGHT] = Bitmask(B1) | Bitmask(G1);
    _pieces[WHITE][ROOK]   = Bitmask(A1) | Bitmask(H1);
	
    _pieces[BLACK][PAWN]   = Bitmask(RANK7);
    _pieces[BLACK][KING]   = Bitmask(E8);
    _pieces[BLACK][QUEEN]  = Bitmask(D8);
    _pieces[BLACK][BISHOP] = Bitmask(C8) | Bitmask(F8);
    _pieces[BLACK][KNIGHT] = Bitmask(B8) | Bitmask(G8);
    _pieces[BLACK][ROOK]   = Bitmask(A8) | Bitmask(H8);
}

char Board::s_pieceChar[2][6] =
    {
	{'P','N','B','R','Q','K'},
	{'p','n','b','r','q','k'},
    };

std::string Board::String() const {
    std::ostringstream ret;
    for (int rank = 7; rank >= 0; rank--) {
	for (int sq = rank*8; sq < rank*8+8; ) {
	    char x = '.';
	    for (int c = 0; c < 2; c++) {
		for (int p = 0; p < 6; p++) {
		    if (_pieces[c][p] & (1UL << sq)) {
			x = s_pieceChar[c][p];
			goto print;			
		    }
		}
	    }
	print:
	    ret.put(' ');
	    ret.put(x);
	    sq++;
	    if (sq % 8 == 0) {
		ret.put('\n');
	    }
	}
    }
    return ret.str();
}
