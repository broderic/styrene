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

Board::Square Board::ParseSquare(const char* str) {
    for (int i = 0; i < 64; i++) {
	if (!strcmp(Board::s_squareStr[i], str))
	    return Board::Square(i);
    }
    return INVALID_SQUARE;
}

Board::Player Board::ParseSide(const char *str) {
    if (!strcmp(str, "black") || !strcmp(str, "b"))
	return BLACK;
    if (!strcmp(str, "white") || !strcmp(str, "w"))
	return WHITE;
    return INVALID_PLAYER;
}

Board::Piece Board::ParsePiece(const char *str) {
    if (!strcmp(str, "pawn") || !strcmp(str, "p"))
	return PAWN;
    if (!strcmp(str, "knight") || !strcmp(str, "n"))
	return KNIGHT;
    if (!strcmp(str, "bishop") || !strcmp(str, "b"))
	return BISHOP;
    if (!strcmp(str, "rook") || !strcmp(str, "r"))
	return ROOK;
    if (!strcmp(str, "queen") || !strcmp(str, "q"))
	return QUEEN;
    if (!strcmp(str, "king") || !strcmp(str, "k"))
	return KING;
    return INVALID_PIECE;
}

Board::Move Board::ParseMove(char *str) {
    if (strlen(str) != 4) {
	return INVALID_MOVE;
    }
    Square to = ParseSquare(str+2);
    if (to == INVALID_SQUARE) {
	return INVALID_MOVE;
    }
    str[2] = 0;
    Square from = ParseSquare(str);
    if (from == INVALID_SQUARE) {
	return INVALID_MOVE;
    }
    return Move(from, to);
}

void Board::PieceTables::ComputePawnAttacks() {
    memset(s_pawn_attacks, 0, sizeof(s_pawn_attacks));
    for (int i = 0; i < 48; i++) {
	Square sq = Square(i);
	File f = GetFile(sq);
	uint64_t& out = s_pawn_attacks[WHITE][sq];
	if (f > 0) out |= Bitmask(Nbr(sq, NW));
	if (f < 7) out |= Bitmask(Nbr(sq, NE));
    }
    for (int i = 8; i < 64; i++) {
	Square sq = Square(i);
	File f = GetFile(sq);
	uint64_t& out = s_pawn_attacks[BLACK][sq];
	if (f > 0) out |= Bitmask(Nbr(sq, SW));
	if (f < 7) out |= Bitmask(Nbr(sq, SE));
    }
}

void Board::PieceTables::ComputeKnightAttacks() {
    for (int s = 0; s < 64; s++) {
	Square sq = Square(s);
	Rank r = GetRank(sq);
	File f = GetFile(sq);
	uint64_t& out = s_knight_attacks[sq];
	out = 0L;
        if (f < 6 && r < 7) { out |= Bitmask(Nbr(sq, EEN)); }
	if (f < 7 && r < 6) { out |= Bitmask(Nbr(sq, ENN)); }
	if (f > 0 && r < 6) { out |= Bitmask(Nbr(sq, WNN)); }
	if (f > 1 && r < 7) { out |= Bitmask(Nbr(sq, WWN)); }
	if (f > 1 && r > 0) { out |= Bitmask(Nbr(sq, WWS)); }
	if (f > 0 && r > 1) { out |= Bitmask(Nbr(sq, WSS)); }
	if (f < 7 && r > 1) { out |= Bitmask(Nbr(sq, ESS)); }
	if (f < 6 && r > 0) { out |= Bitmask(Nbr(sq, EES)); }
    }
}

void Board::PieceTables::ComputeKingAttacks() {
   for (int s = 0; s < 64; s++) {
	Square sq = Square(s);
	Rank r = GetRank(sq);
	File f = GetFile(sq);
	uint64_t& out = s_king_attacks[sq];
	out = 0L;
        if (f < 7         ) { out |= Bitmask(Nbr(sq, E)); }
	if (f < 7 && r < 7) { out |= Bitmask(Nbr(sq, NE)); }
	if (         r < 7) { out |= Bitmask(Nbr(sq, N)); }
	if (f > 0 && r < 7) { out |= Bitmask(Nbr(sq, NW)); }
	if (f > 0         ) { out |= Bitmask(Nbr(sq, W)); }
	if (f > 0 && r > 0) { out |= Bitmask(Nbr(sq, SW)); }
	if (         r > 0) { out |= Bitmask(Nbr(sq, S)); }
	if (f < 7 && r > 0) { out |= Bitmask(Nbr(sq, SE)); }
    }
}

void Board::PieceTables::ComputeRookAttacks() {
   for (int s = 0; s < 64; s++) {
	Square sq = Square(s);
	s_rook_attacks[sq] = Bitmask(GetRank(sq)) | Bitmask(GetFile(sq));
    }    
}

void Board::PieceTables::ComputeBishopAttacks() {
    static const int xoff[] = { 1, 1, -1, -1};
    static const int yoff[] = { 1, -1, 1, -1 };
    memset(s_bishop_attacks, 0, sizeof(s_bishop_attacks));
    for (int i = 0; i < 64; i++) {
	Square sq = Square(i);
	uint64_t attacks = 0l;	
	for (int d = 0; d < 4; d++) {
	    int r = sq / 8;
	    int f = sq & 0x7;
	    while (1) {
		r += yoff[d];
		f += xoff[d];
		if (r >= 0 && r < 8 && f >= 0 && f < 8) 
		    attacks |= 1L << (8*r + f);
		else
		    break;
	    }
	}
	s_bishop_attacks[sq] = attacks;
    }
}

void Board::PieceTables::ComputeQueenAttacks() {
    for (int i = 0; i < 64; i++) {
	s_queen_attacks[i] = s_bishop_attacks[i] | s_rook_attacks[i];
    }
}

void Board::PieceTables::ComputeBehindBlockerTable() {
    static const int xoff[] = { 1, 1, 0, -1, -1, -1, 0, 1};
    static const int yoff[] = { 0, 1, 1, 1, 0, -1, -1, -1};
    memset(s_behind_blocker, 0, sizeof(s_behind_blocker));
    
    for (int i = 0; i < 64; i++) {
	Square sq = Square(i);
	for (int d = 0; d < 8; d++) {
	    uint64_t ray = 0l;

	    int r = sq / 8;
	    int f = sq & 0x7;
	    while (1) {
		r += yoff[d];
		f += xoff[d];
		if (r >= 0 && r < 8 && f >= 0 && f < 8) 
		    ray |= 1L << (8*r + f);
		else
		    break;
	    }

	    r = sq / 8;
	    f = sq & 0x7;
	    while (1) {
		r += yoff[d];
		f += xoff[d];
		if (r >= 0 && r < 8 && f >= 0 && f < 8) {
		    int blocker = r*8 + f;
		    ray &= ~(1L << blocker);
		    s_behind_blocker[sq][blocker] = ray;
		}
		else
		    break;
	    }
	}
    }
}

Board::Board()
{
    NewGame();
}

std::string Board::State::String(uint64_t mask) const {
    std::ostringstream ret;
    for (int rank = 7; rank >= 0; rank--) {
	for (int sq = rank*8; sq < rank*8+8; ) {
	    char x = '.';
	    if (mask & Bitmask(Square(sq))) {
		x = '*';
	    } else {
		Piece p = PieceAt(WHITE, Square(sq));
		if (p != INVALID_PIECE) {
		    x = s_pieceChar[WHITE][p];
		} else {
		    p = PieceAt(BLACK, Square(sq));
		    if (p != INVALID_PIECE) {
			x = s_pieceChar[BLACK][p];
		    }
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

    
    for (int c = 0; c < 2; c++) {
	state._side[c]._occupied = 0L;
	for (int p = 0; p < 6; p++) {
	    state._side[c]._occupied |= state._side[c]._pieces[p];
	}
    }
    state.ComputeAttackingSet(WHITE);
    state.ComputeAttackingSet(BLACK);
}

void Board::Play(const Player c, const Move m) {
    _move_history[_move_number] = m;
    _state_history[_move_number+1] = _state_history[_move_number];
    _move_number++;

    State& state = CurrentState();
    const Piece p = state._side[c].PieceAt(m.From());
    
    state._side[c]._pieces[p] ^= Bitmask(m.From());
    state._side[c]._occupied ^= Bitmask(m.From());
    
    state._side[c]._pieces[p] ^= Bitmask(m.To());
    state._side[c]._occupied ^= Bitmask(m.To());
    
    state._side[OtherPlayer(c)].Clear(m.To());  // capture

    state.ComputeAttackingSet(WHITE);
    state.ComputeAttackingSet(BLACK);
}

void Board::State::ComputeAttackingSet(Player c) {
    uint64_t& out = _side[c]._attacking;
    out = 0L;
    for (BitsetIterator it(_side[c]._pieces[PAWN]); it; ++it) {
	out |= GetPieceTables().PawnAttacks(c, it.Index());
    }
    for (BitsetIterator it(_side[c]._pieces[KNIGHT]); it; ++it) {
	out |= GetPieceTables().KnightAttacks(it.Index());
    }
    for (BitsetIterator it(_side[c]._pieces[KING]); it; ++it) {
	out |= GetPieceTables().KingAttacks(it.Index());
    }
    const uint64_t occupied = _side[WHITE]._occupied | _side[BLACK]._occupied;
    for (BitsetIterator it(_side[c]._pieces[ROOK]); it; ++it) {
	Square sq = it.Index();
	uint64_t attacking = GetPieceTables().RookAttacks(sq);
	for (BitsetIterator b(attacking & occupied); b; ++b) {
	    attacking &= ~GetPieceTables().BehindBlocker(sq, b.Index());
	}
	out |= attacking;
    }
    for (BitsetIterator it(_side[c]._pieces[BISHOP]); it; ++it) {
	Square sq = it.Index();
	uint64_t attacking = GetPieceTables().BishopAttacks(sq);
	for (BitsetIterator b(attacking & occupied); b; ++b) {
	    attacking &= ~GetPieceTables().BehindBlocker(sq, b.Index());
	}
	out |= attacking;
    }
    for (BitsetIterator it(_side[c]._pieces[QUEEN]); it; ++it) {
	Square sq = it.Index();
	uint64_t attacking = GetPieceTables().QueenAttacks(sq);
	for (BitsetIterator b(attacking & occupied); b; ++b) {
	    attacking &= ~GetPieceTables().BehindBlocker(sq, b.Index());
	}
	out |= attacking;
    }
}

void Board::State::GenerateMoves(Board::Player c, Board::MoveQueue& moves) {
    GeneratePawnMoves(c, moves);
    GenerateKnightMoves(c, moves);
    GenerateKingMoves(c, moves);
    GenerateRookMoves(c, moves);
    GenerateBishopMoves(c, moves);
    GenerateQueenMoves(c, moves);
}

void Board::State::GeneratePawnMoves(Board::Player c, Board::MoveQueue& moves) {
    uint64_t attackable = _side[OtherPlayer(c)]._occupied;
    uint64_t empty = ~(attackable | _side[c]._occupied);
    for (BitsetIterator it(_side[c]._pieces[PAWN]); it; ++it) {
	Square from = it.Index();
	if (c == WHITE) {
	    uint64_t attacks = GetPieceTables().PawnAttacks(WHITE, from);
	    for (BitsetIterator at(attacks & attackable); at; ++at) {
		Square to = at.Index();
		moves.PushBack(Move(from, to));
	    }
    	    Square up = Nbr(from, N);
	    if (empty & Bitmask(up)) {
		moves.PushBack(Move(from, up));
		if (GetRank(from) == RANK2 && (empty & Bitmask(Nbr(up, N)))) {
		    moves.PushBack(Move(from, Nbr(up, N)));
		}
	    }
	} else {
	    uint64_t attacks = GetPieceTables().PawnAttacks(BLACK, from);
	    for (BitsetIterator at(attacks & attackable); at; ++at) {
		Square to = at.Index();
		moves.PushBack(Move(from, to));
	    }
	    Square dn = Nbr(from, S);
	    if (empty & Bitmask(dn)) {
		moves.PushBack(Move(from, dn));
		if (GetRank(from) == RANK7 && (empty & Bitmask(Nbr(dn,S)))) {
		    moves.PushBack(Move(from, Nbr(dn, S)));
		}
	    }
	    
	}
    }
}

void Board::State::GenerateKnightMoves(Board::Player c, Board::MoveQueue& moves) {
    uint64_t attackable = _side[OtherPlayer(c)]._occupied;
    uint64_t empty = ~(attackable | _side[c]._occupied);
    uint64_t valid = attackable | empty;
    for (BitsetIterator it(_side[c]._pieces[KNIGHT]); it; ++it) {
	Square from = it.Index();
	for (BitsetIterator mv(GetPieceTables().KnightAttacks(from) & valid); mv; ++mv) {
	    moves.PushBack(Move(from, mv.Index()));
	}
    }    
}

void Board::State::GenerateKingMoves(Board::Player c, Board::MoveQueue& moves) {
    uint64_t attackable = _side[c^1]._occupied;
    uint64_t attacked = _side[c^1]._attacking;
    uint64_t empty = ~(attackable | _side[c]._occupied);
    uint64_t valid = (attackable | empty) & (~attacked);
    for (BitsetIterator it(_side[c]._pieces[KING]); it; ++it) {
	Square from = it.Index();
	for (BitsetIterator mv(GetPieceTables().KingAttacks(from) & valid); mv; ++mv) {
	    moves.PushBack(Move(from, mv.Index()));
	}
    }    
}

void Board::State::GenerateRookMoves(Board::Player c, Board::MoveQueue& moves) {
    const uint64_t occupied = _side[WHITE]._occupied | _side[BLACK]._occupied;
    const uint64_t own_pieces = _side[c]._occupied;
    for (BitsetIterator it(_side[c]._pieces[ROOK]); it; ++it) {
	Square from = it.Index();
	uint64_t attacking = GetPieceTables().RookAttacks(from);
	for (BitsetIterator b(attacking & occupied); b; ++b) {
	    attacking &= ~GetPieceTables().BehindBlocker(from, b.Index());
	}
	for (BitsetIterator to(attacking & ~own_pieces); to; ++to) {
	    moves.PushBack(Move(from, to.Index()));
	}
    }
}

void Board::State::GenerateBishopMoves(Board::Player c, Board::MoveQueue& moves) {
    const uint64_t occupied = _side[WHITE]._occupied | _side[BLACK]._occupied;
    const uint64_t own_pieces = _side[c]._occupied;
    for (BitsetIterator it(_side[c]._pieces[BISHOP]); it; ++it) {
	Square from = it.Index();
	uint64_t attacking = GetPieceTables().BishopAttacks(from);
	for (BitsetIterator b(attacking & occupied); b; ++b) {
	    attacking &= ~GetPieceTables().BehindBlocker(from, b.Index());
	}
	for (BitsetIterator to(attacking & ~own_pieces); to; ++to) {
	    moves.PushBack(Move(from, to.Index()));
	}
    }
}

void Board::State::GenerateQueenMoves(Board::Player c, Board::MoveQueue& moves) {
    const uint64_t occupied = _side[WHITE]._occupied | _side[BLACK]._occupied;
    const uint64_t own_pieces = _side[c]._occupied;
    for (BitsetIterator it(_side[c]._pieces[QUEEN]); it; ++it) {
	Square from = it.Index();
	uint64_t attacking = GetPieceTables().QueenAttacks(from);
	for (BitsetIterator b(attacking & occupied); b; ++b) {
	    attacking &= ~GetPieceTables().BehindBlocker(from, b.Index());
	}
	for (BitsetIterator to(attacking & ~own_pieces); to; ++to) {
	    moves.PushBack(Move(from, to.Index()));
	}
    }
}
