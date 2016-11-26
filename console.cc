#include "console.h"

#include <cstdio>
#include <cstring>

Console::Console(int argc, char **argv)
    : _board()
{
}

void Console::MainLoop() {
    while (true) {
	char buffer[1024];
	char *cmdline = fgets(buffer, 1000, stdin);
	char **ap, *args[10];
	for (ap = args; (*ap = strsep(&cmdline, " \r\n\t")) != NULL;)
	    if (**ap != '\0')
		if (++ap >= &args[10])
		    break;
	char *cmd = args[0];
	
	if (!strcmp(cmd, "quit")) {
	    break;
	} else if (!strcmp(cmd, "showboard")) {
	    printf("%s\n\n", _board.String().c_str());	    
	} else if (!strcmp(cmd, "newgame")) {
	    _board.NewGame();
	} else if (!strcmp(cmd, "undo")) {
	    _board.Undo();
	} else if (!strcmp(cmd, "play")) {
	    Board::Player c = Board::ParseSide(args[1]);
	    if (c == Board::INVALID_PLAYER) {
		printf("Could not parse player\n\n");
		continue;
	    }
	    Board::Move m = Board::ParseMove(args[2]);
	    if (m == Board::INVALID_MOVE) {
		printf("Couldn't not parse move\n\n");
		continue;
	    }
	    _board.Play(c, m);
	} else if (!strcmp(cmd, "movelist")) {
	    Board::Player c = Board::ParseSide(args[1]);
	    if (c == Board::INVALID_PLAYER) {
		printf("Could not parse player\n\n");
		continue;
	    }
	    std::vector<Board::Move> moves = _board.GetMoveList(c);
	    printf("[");
	    for (size_t i=0;i < moves.size(); i++) {
		printf(" %s", moves[i].String());
	    }
	    printf("]\n\n");
	} else if (!strcmp(cmd, "attacktables")) {
	    Board::Piece p = Board::ParsePiece(args[1]);
	    if (p == Board::INVALID_PIECE) {
		printf("Could not parse piece\n\n");
		continue;
	    }
	    Board::Square sq = Board::ParseSquare(args[2]);
	    if (sq == Board::INVALID_SQUARE) {
		printf("Could not parse square\n\n");
		continue;
	    }
	    if (p == Board::KNIGHT) {
		
		printf("%s\n\n", _board.String(Board::GetPieceTables().KnightAttacks(sq)).c_str());
	    } else if (p == Board::PAWN) {

		Board::Player p = Board::ParseSide(args[3]);
		if (p == Board::INVALID_PLAYER) {
		    printf("Could not parse player\n\n");
		    continue;
		}
		printf("%s\n\n", _board.String(Board::GetPieceTables().PawnAttacks(p, sq)).c_str());	
	    }
	    
	}
	else {
	    printf("Unknown '%s'\n\n", cmd);
	}
    }
}
