#ifndef CHESS_H
#define CHESS_H

#include <stdio.h>
#include <iostream>
#include <vector>

typedef unsigned long long U64;
typedef unsigned int U32;
typedef unsigned short U16;
typedef unsigned char U8;
typedef signed   long long S64;
typedef signed   int S32 ;
typedef signed   short S16 ;
typedef signed   char S8 ;

enum piece {
	// white piece
	PAWN,
	KNIGHT,
	BISHOP,
	ROOK,
	QUEEN,
	KING,
 // empty square
	EMPTY
};

enum color {
	WHITE,
	BLACK,
	COLOR_EMPTY
};

enum squares {
  A1 = 0, B1, C1, D1, E1, F1, G1, H1,
  A2 = 16, B2, C2, D2, E2, F2, G2, H2,
  A3 = 32, B3, C3, D3, E3, F3, G3, H3,
  A4 = 48, B4, C4, D4, E4, F4, G4, H4,
  A5 = 64, B5, C5, D5, E5, F5, G5, H5,
  A6 = 80, B6, C6, D6, E6, F6, G6, H6,
  A7 = 96, B7, C7, D7, E7, F7, G7, H7,
  A8 = 112, B8, C8, D8, E8, F8, G8, H8
};

struct smove {
	U8 from;
	U8 to;
	S8 flags;
	S8 castling_flags;
	U8 capture;
	U8 epSquare;
	S8 moveTillDrawCount;
};

bool isPromotion(smove move);

S8 square(S8 row, S8 col);
S8 square(std::string sq);
S8 rowOfSq(S8 sq);
S8 colOfSq(S8 sq);

class Chess {
	public:
		Chess();

	private:
		void clearSq(S8 sq);
		void fillSq(U8 color, U8 piece, S8 sq);

	public:
		void clearBoard();
		void loadFEN(std::string FEN);

		void makeMove(smove move);
		void unmakeMove(smove move);

		bool isAttacked(bool side, S8 sq) const;
		bool leaperAttack(bool side, U8 piece, S8 sq) const;
		bool straightAttack(bool side, S8 sq, U8 dir) const;
		bool diagAttack(bool side, S8 sq, U8 dir) const;

		void addMove(std::vector<smove>& v, const U8 &from, const U8 &to, S8 flags = 0, const U8& capture = 0) const;
		std::vector<smove> genPseudoLegalMoves() const;

		void show() const;
		U8 pieceAt(S8 sq) const;
		U8 colorAt(S8 sq) const;

	private:;
		U8 piece[128];
		U8 color[128];

		U8 king[2];

		U8 enPassantSquare;
		S8 withoutCaptureCount;

		bool sideToMove;

		bool castlingRights;

		int materialCount;
		int moveCount;

		//smove moves[NB_MOVES_MEMORY];
};

// checking if a square if on the board : !(square &  0x88)

#endif
