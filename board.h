#ifndef BOARD_
#define BOARD_

#include <cstdint>
#include <cstring>
#include <string>
#include <vector>

class Board {

    //     a b c d e f g h
    //
    //  8  r n b q k b n r  8
    //  7  p p p p p p p p  7
    //  6  . . . . . + . +  6
    //  5  . . . . + . + .  5
    //  4  . . . . . + . +  4
    //  3  . . . . + . + .  3
    //  2  P P P P P P P P  2 
    //  1  R N B Q K B N R  1
    //
    //     a b c d e f g h 


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

 public:
    typedef enum {WHITE=0, BLACK, INVALID_PLAYER} Player;
    inline static Player OtherPlayer(Player p) { return Player(p^1); }
    
    typedef enum {PAWN=0, KNIGHT, BISHOP, ROOK, QUEEN, KING, INVALID_PIECE} Piece;
    typedef enum {RANK1=0, RANK2, RANK3, RANK4, RANK5, RANK6, RANK7, RANK8} Rank;
    typedef enum {FILEA=0, FILEB, FILEC, FILED, FILEE, FILEF, FILEG, FILEH} File;
    typedef enum {A1=0, B1, C1, D1, E1, F1, G1, H1,
		  A2, B2, C2, D2, E2, F2, G2, H2,
		  A3, B3, C3, D3, E3, F3, G3, H3,
		  A4, B4, C4, D4, E4, F4, G4, H4,
		  A5, B5, C5, D5, E5, F5, G5, H5,
		  A6, B6, C6, D6, E6, F6, G6, H6,
		  A7, B7, C7, D7, E7, F7, G7, H7,
		  A8, B8, C8, D8, E8, F8, G8, H8,
		  INVALID_SQUARE} Square;
    typedef enum { E=0, NE, N, NW, W, SW, S, SE,
		   EEN, ENN, WNN, WWN, WWS, WSS, ESS, EES } Direction;
    
    static const char s_pieceChar[2][6];
    static const char *const s_squareStr[64];

    static Player ParseSide(const char *str);
    static Piece ParsePiece(const char *str);
    static Square ParseSquare(const char* str);
    
    static Rank GetRank(Square sq) { return Rank(sq >> 3); }
    static File GetFile(Square sq) { return File(sq & 0x7); } 
    static Square Nbr(Square sq, Direction d) {
	static int offsets[16] =
	    {
		1, 9, 8, 7, -1, -9, -8, -7,
		10, 17, 15, 6, -10, -17, -15, -6,  
	    };
	return Square(sq + offsets[d]);
    }
    
    inline static uint64_t Bitmask(const Rank r) { return (0xffUL << (r*8)); }
    inline static uint64_t Bitmask(const Square sq) { return (1UL << sq); }

    inline static int BitScanLS1B(uint64_t x) {
	return __builtin_ffsll(x) - 1; 	// GCC builtin
    }
    
    class BitsetIterator {
    public:
	
        BitsetIterator(uint64_t x) : _x(x) {};

	operator bool() const { return _x; }
	
	void operator++() { _x = _x & (_x-1); }
	
	Square Index() const { return Square(BitScanLS1B(_x)); }
	
    private:
	uint64_t _x;
    };
    
    class Move {
    public:
        Move() : _from(0), _to(0) {}
        Move(Square f, Square t) : _from(f), _to(t) {}
	
	Square From() const { return Square(_from); }
	Square To() const { return Square(_to); }
	bool operator==(const Move& o) {
	    return _from == o._from && _to == o._to;
	}

	const char* String() { return GetStatic().s_names[From()][To()]; }
	
    private:
	uint8_t _from;
	uint8_t _to;
	
	struct StaticData {
	    char s_names[64][64][8];
	    StaticData() {
		for (int f = 0; f < 64; f++) {
		    for (int t = 0; t < 64; t++) {
			strcpy(s_names[f][t], s_squareStr[f]);
			strcat(s_names[f][t], s_squareStr[t]);
		    }
		}
	    }
	};

	StaticData& GetStatic() {
	    static StaticData data;
	    return data;	    
	}
    };

    static const Move INVALID_MOVE;
    static Move ParseMove(char *str);
    
    Board();

    void NewGame();

    void Play(const Player p, const Move m);
    void Undo() {
	_move_number--;
    }

    std::vector<Move> GetMoveList(Player p) {
	MoveQueue& q = _move_queue[_move_number];
	CurrentState().GenerateMoves(p, q);
	std::vector<Move> ret;
	while (!q.Empty()) {
	    ret.push_back(q.PopFront());
	}
	return ret;
    }
	
    std::string String() const { return CurrentState().String(); }
    
    inline Piece PieceAt(Player p, Square sq) const {
	return CurrentState().PieceAt(p, sq);
    }
    
 private:

    struct MoveQueue {
	static const int MAX_GENERATED_MOVES = 256;

	int _i, _n;	
	Move _moves[MAX_GENERATED_MOVES];

        MoveQueue() : _i(0), _n(0) {}
	void Clear() { _i = 0; _n = 0; }
	void PushBack(Move m) { _moves[_n++] = m; }
	Move PopFront() { return _moves[_i++]; }
	int NumMoves() const { return _n - _i; }
	bool Empty() const { return _n == _i; }
    };

    struct State {
	struct Side {
	    uint64_t _pieces[6];
	    uint64_t _occupied;
	    
	    void Clear(Square sq) {
		for (int p = 0; p < 6; p++) {
		    _pieces[p] &= ~Bitmask(sq);
		}
		_occupied &= ~Bitmask(sq);
	    };
	    
	    Piece PieceAt(Square sq) const {
		for (int p = 0; p < 6; p++) {
		    if (_pieces[p] & Bitmask(sq)) {
			return Piece(p);
		    }
		}
		return INVALID_PIECE;
	    };
	};

	std::string String() const {return String(0L); }
	std::string String(uint64_t mask) const;
	
	Piece PieceAt(Player p, Square sq) const { return _side[p].PieceAt(sq); }

	void GenerateMoves(Player c, MoveQueue& moves);
	void GeneratePawnMoves(Player c, MoveQueue& moves);
	void GenerateKnightMoves(Player c, MoveQueue& moves);
	
	Side _side[2];

    private:
	struct StaticData {
	    uint64_t s_knight_moves[64];

	    StaticData() {
		ComputeKnightMoves();
	    }

	    void ComputeKnightMoves();
	};

	StaticData& GetStaticData() {
	    static StaticData data;
	    return data;
	}	    
    };

    State& CurrentState() { return _state_history[_move_number]; }
    const State& CurrentState() const { return _state_history[_move_number]; }
    
    static const int MAX_GAME_MOVES = 128;
    int _move_number;
    Move _move_history[MAX_GAME_MOVES];
    State _state_history[MAX_GAME_MOVES];
    MoveQueue _move_queue[MAX_GAME_MOVES];
};

#endif // BOARD_
