#include "console.h"

#include <cstdio>
#include <cstring>

Console::Console(int argc, char **argv)
    : _board()
{
}

Board::Player Console::ParseSide(char *str) const {
    if (!strcmp(str, "black") || !strcmp(str, "b"))
	return Board::BLACK;
    if (!strcmp(str, "white") || !strcmp(str, "w"))
	return Board::WHITE;
    return Board::INVALID_PLAYER;
}

Board::Move Console::ParseMove(char *str) const {
    if (strlen(str) != 4) {
	return Board::INVALID_MOVE;
    }
    Board::Square to = Board::FindSquareStr(str+2);
    if (to == Board::INVALID_SQUARE) {
	return Board::INVALID_MOVE;
    }
    str[2] = 0;
    Board::Square from = Board::FindSquareStr(str);
    if (from == Board::INVALID_SQUARE) {
	return Board::INVALID_MOVE;
    }
    return Board::Move(from, to);
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
	    Board::Player c = ParseSide(args[1]);
	    if (c == Board::INVALID_PLAYER) {
		printf("Could not parse player\n\n");
		continue;
	    }
	    Board::Move m = ParseMove(args[2]);
	    if (m == Board::INVALID_MOVE) {
		printf("Couldn't not parse move\n\n");
		continue;
	    }
	    _board.Play(c, m);
	} else if (!strcmp(cmd, "movelist")) {
	    std::vector<Board::Move> moves = _board.GetMoveList();
	    printf("[");
	    for (size_t i=0;i < moves.size(); i++) {
		printf(" %s", moves[i].String());
	    }
	    printf("]\n\n");
	}
	else {
	    printf("Unknown '%s'\n\n", cmd);
	}
    }
}
