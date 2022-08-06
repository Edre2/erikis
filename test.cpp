#include "chess.h"


const int EP_FLAG = 1;
const int CASTLE_FLAG = 2;
const int PROMOTION_KNIGHT_FLAG = 4;
const int PROMOTION_BISHOP_FLAG = 8;
const int PROMOTION_ROOK_FLAG = 16;
const int PROMOTION_QUEEN_FLAG = 32;
const int PROMOTION_FLAG = 64;

bool testz() {
	return ((PROMOTION_FLAG | PROMOTION_KNIGHT_FLAG) & PROMOTION_FLAG );
}


int main() {

	Chess test;

	test.show();

	std::vector<smove> t = test.genPseudoLegalMoves();

	for (smove &move : t) {
		std::cout << int(move.from) << " " << int(move.to) << "\n";
	}

	test.makeMove(t[1]);

	test.show();

	if (testz())
		std::cout << "a";
	else
		std::cout << "b";

	std::cout << "\n\n" << (((PROMOTION_FLAG | PROMOTION_KNIGHT_FLAG) & PROMOTION_FLAG ) == true ) << "\n";

	return 0;
}
