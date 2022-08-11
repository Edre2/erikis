#include "chess.h"

#include <sys/time.h>

int main() {

	Chess test;

	test.loadFEN("r3k2r/Pppp1ppp/1b3nbN/nP6/BBP1P3/q4N2/Pp1P2PP/R2Q1RK1 w kq - 0 1");

	test.show();

	std::cout << test.minmax(2) << "\n";
	std::cout << test.alphabeta(2, -10000000, 1000000) << "\n";

// 	int start = clock();
//
// 	int nbMoves = test.numberMoves(5);
//
// 	int end = clock();
//
// 	float seconds = ((float)end - start)/CLOCKS_PER_SEC;
//
// std::cout << "it took " << end - start << "ticks, or " << seconds << "seconds.\n";;
// std::cout << "number of moves : " << nbMoves << "\n";
// std::cout << "speed : " << ((float)nbMoves)/seconds << "Nps\n";

	return 0;
}
