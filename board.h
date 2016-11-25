#ifndef BOARD_
#define BOARD_

#include <cstdint>
#include <string>

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
    typedef enum {WHITE=0, BLACK} Player;
    typedef enum {PAWN=0, KNIGHT, BISHOP, ROOK, QUEEN, KING, INVALID_PIECE} Piece;
    typedef enum {RANK1=0, RANK2, RANK3, RANK4, RANK5, RANK6, RANK7, RANK8} Rank;
    typedef enum {A1=0, B1, C1, D1, E1, F1, G1, H1,
		  A2, B2, C2, D2, E2, F2, G2, H2,
		  A3, B3, C3, D3, E3, F3, G3, H3,
		  A4, B4, C4, D4, E4, F4, G4, H4,
		  A5, B5, C5, D5, E5, F5, G5, H5,
		  A6, B6, C6, D6, E6, F6, G6, H6,
		  A7, B7, C7, D7, E7, F7, G7, H7,
		  A8, B8, C8, D8, E8, F8, G8, H8} Square;


    class Move {
    public:
	
        Move(Square f, Square t) : _from(f), _to(t) {}
	
	Square From() const { return Square(_from); }
	Square To() const { return Square(_to); }
	
    private:
	uint8_t _from;
	uint8_t _to;
    };

    
    Board();

    void NewGame();

    void Play(const Player p, const Move m);

    inline Piece PieceAt(Player p, Square sq) const { return _state[p].PieceAt(sq); }
    
    std::string String() const;

    inline static uint64_t Bitmask(const Rank r) { return (0xffUL << (r*8)); }
    inline static uint64_t Bitmask(const Square sq) { return (1UL << sq); }
    
 private:

    struct State {
	uint64_t _pieces[6];
	
	Piece PieceAt(Square sq) const;
    };
    
    State _state[2];

    static char s_pieceChar[2][6];
    static char *s_squareStr[64];

};

#endif // BOARD_
