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
    _state[WHITE]._pieces[PAWN]   = Bitmask(RANK2);
    _state[WHITE]._pieces[KING]   = Bitmask(E1);
    _state[WHITE]._pieces[QUEEN]  = Bitmask(D1);
    _state[WHITE]._pieces[BISHOP] = Bitmask(C1) | Bitmask(F1);
    _state[WHITE]._pieces[KNIGHT] = Bitmask(B1) | Bitmask(G1);
    _state[WHITE]._pieces[ROOK]   = Bitmask(A1) | Bitmask(H1);
	
    _state[BLACK]._pieces[PAWN]   = Bitmask(RANK7);
    _state[BLACK]._pieces[KING]   = Bitmask(E8);
    _state[BLACK]._pieces[QUEEN]  = Bitmask(D8);
    _state[BLACK]._pieces[BISHOP] = Bitmask(C8) | Bitmask(F8);
    _state[BLACK]._pieces[KNIGHT] = Bitmask(B8) | Bitmask(G8);
    _state[BLACK]._pieces[ROOK]   = Bitmask(A8) | Bitmask(H8);
}

char Board::s_pieceChar[2][6] =
    {
	{'P','N','B','R','Q','K'},
	{'p','n','b','r','q','k'},
    };

char *Board::s_squareStr[64] =
    {
	"a1", "b1", "c1", "d1", "e1", "f1", "g1", "h1",
	"a2", "b2", "c2", "d2", "e2", "f2", "g2", "h2",
	"a3", "b3", "c3", "d3", "e3", "f3", "g3", "h3",
	"a4", "b4", "c4", "d4", "e4", "f4", "g4", "h4",
	"a5", "b5", "c5", "d5", "e5", "f5", "g5", "h5",
	"a6", "b6", "c6", "d6", "e6", "f6", "g6", "h6",
	"a7", "b7", "c7", "d7", "e7", "f7", "g7", "h7",
	"a8", "b8", "c8", "d8", "e8", "f8", "g8", "h8",
    };

void Board::Play(const Player p, const Move m) {
        
    
}

Board::Piece Board::State::PieceAt(Square sq) const {
    for (int p = 0; p < 6; p++) {
	if (_pieces[p] & Bitmask(sq)) {
	    return Piece(p);
	}
    }
    return INVALID_PIECE;
}

std::string Board::String() const {
    std::ostringstream ret;
    for (int rank = 7; rank >= 0; rank--) {
	for (int sq = rank*8; sq < rank*8+8; ) {
	    char x = '.';
	    Piece p = PieceAt(WHITE, Square(sq));
	    if (p != INVALID_PIECE) {
		x = s_pieceChar[WHITE][p];
	    } else {
		p = PieceAt(BLACK, Square(sq));
		if (p != INVALID_PIECE) {
		    x = s_pieceChar[BLACK][p];
		}
	    }
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
