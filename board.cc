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
    _pieces[WHITE][PAWN]   = 0xffUL << 8;
    _pieces[WHITE][KING]   = (1 << E1);
    _pieces[WHITE][QUEEN]  = (1 << D1);
    _pieces[WHITE][BISHOP] = (1 << C1) | (1 << F1);
    _pieces[WHITE][KNIGHT] = (1 << B1) | (1 << G1);
    _pieces[WHITE][ROOK]   = (1 << A1) | (1 << H1);
	
    _pieces[BLACK][PAWN]   = 0xffUL << 48;
    _pieces[BLACK][KING]   = 0x10UL << 56;
    _pieces[BLACK][QUEEN]  = 0x08UL << 56;
    _pieces[BLACK][BISHOP] = (0x04 | 0x20UL) << 56;
    _pieces[BLACK][KNIGHT] = (0x02 | 0x40UL) << 56;
    _pieces[BLACK][ROOK]   = (0x01 | 0x80UL) << 56;
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
