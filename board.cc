#include "board.h"

#include <sstream>
#include <string>

const char Board::s_pieceChar[2][6] =
    {
	{'P','N','B','R','Q','K'},
	{'p','n','b','r','q','k'},
    };

const char * const Board::s_squareStr[64] =
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

const Board::Move Board::INVALID_MOVE = Board::Move();

Board::Square Board::FindSquareStr(const char* str) {
    for (int i = 0; i < 64; i++) {
	if (!strcmp(Board::s_squareStr[i], str))
	    return Board::Square(i);
    }
    return Board::INVALID_SQUARE;
}

Board::Board()
{
    NewGame();
}

void Board::NewGame() {
    _move_number = 0;
    State& state = CurrentState();
    state._side[WHITE]._pieces[PAWN]   = Bitmask(RANK2);
    state._side[WHITE]._pieces[KING]   = Bitmask(E1);
    state._side[WHITE]._pieces[QUEEN]  = Bitmask(D1);
    state._side[WHITE]._pieces[BISHOP] = Bitmask(C1) | Bitmask(F1);
    state._side[WHITE]._pieces[KNIGHT] = Bitmask(B1) | Bitmask(G1);
    state._side[WHITE]._pieces[ROOK]   = Bitmask(A1) | Bitmask(H1);
	
    state._side[BLACK]._pieces[PAWN]   = Bitmask(RANK7);
    state._side[BLACK]._pieces[KING]   = Bitmask(E8);
    state._side[BLACK]._pieces[QUEEN]  = Bitmask(D8);
    state._side[BLACK]._pieces[BISHOP] = Bitmask(C8) | Bitmask(F8);
    state._side[BLACK]._pieces[KNIGHT] = Bitmask(B8) | Bitmask(G8);
    state._side[BLACK]._pieces[ROOK]   = Bitmask(A8) | Bitmask(H8);
}

void Board::Play(const Player c, const Move m) {
    _move_history[_move_number] = m;
    _state_history[_move_number+1] = _state_history[_move_number];
    _move_number++;

    State& state = CurrentState();
    const Piece p = state._side[c].PieceAt(m.From());
    state._side[c]._pieces[p] ^= Bitmask(m.From());
    state._side[c]._pieces[p] ^= Bitmask(m.To());
    state._side[OtherPlayer(c)].Clear(m.To());  // capture
}

std::string Board::State::String() const {
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

void Board::State::GenerateMoves(Board::Player c, Board::MoveQueue& moves) {
    GeneratePawnMoves(c, moves);
}

void Board::State::GeneratePawnMoves(Board::Player c, Board::MoveQueue& moves) {
    
    
}
