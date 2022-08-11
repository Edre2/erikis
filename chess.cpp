#include "chess.h"


const int INF = 10000;
const int INFINF = 100000;
// const int NB_MOVES_MEMORY = 1024;

const std::string START_FEN = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1";

const int EP_FLAG = 1;
const int CASTLE_FLAG = 2;
const int PROMOTION_KNIGHT_FLAG = 4;
const int PROMOTION_BISHOP_FLAG = 8;
const int PROMOTION_ROOK_FLAG = 16;
const int PROMOTION_QUEEN_FLAG = 32;
const int PROMOTION_FLAG = 64;

const int CASTLE_WK = 1;
const int CASTLE_WQ = 2;
const int CASTLE_BK = 4;
const int CASTLE_BQ = 8;

const S8 PAWN_DIRECTION[2] = {16, -16};
const U8 PAWN_START_ROW[2] = {1, 6};
const U8 PAWN_PROMOTION_ROW[2] = {7, 0};

const S8 NORTH  = 16;
const S8 NN     = ( NORTH + NORTH );
const S8 SOUTH  = -16;
const S8 SS     = ( SOUTH + SOUTH );
const S8 EAST   = 1;
const S8 WEST   = -1;
const S8 NE     = 17;
const S8 SW     = -17;
const S8 NW    =  15;
const S8 SE    =  -15;

const U8 NB_MOVES[6] = {0, 8, 4, 4, 8, 8};
const bool slide[6] = {0, 0, 1, 1, 1, 0};

const S8 MOVES[6][8] = {
	{}, // panw
	{ -33, -31, -18, -14, 14, 18, 31, 33 }, // knight
	{ SW, SE, NW, NE }, // bishop
	{ SOUTH, WEST, EAST, NORTH }, // rook
	{ SW, SOUTH, SE, WEST, EAST, NW, NORTH, NE }, // queen
	{ SW, SOUTH, SE, WEST, EAST, NW, NORTH, NE }, // king
};


const int PIECE_VALUE[7][3] = {
    {100, -100, 0},
    {325, -325, 0},
    {335, -335, 0},
    {500, -500, 0},
    {975, -975, 0},
    {0, 0, 0},
    {0, 0, 0}
};
// by : piece, color, square
// for middle game
const S8 PIECE_VALUE_SQUARE[6][2][128] = {
    // pawn
    {
        // white
        {0,   0,   0,   0,   0,   0,   0,   0,  0,0,0,0,0,0,0,0,
        -6,  -4,   1, -24, -24,   1,  -4,  -6,  0,0,0,0,0,0,0,0,
        -4,  -4,   1,   5,   5,   1,  -4,  -4,  0,0,0,0,0,0,0,0,
        -6,  -4,   5,  10,  10,   5,  -4,  -6,  0,0,0,0,0,0,0,0,
        -6,  -4,   2,   8,   8,   2,  -4,  -6,  0,0,0,0,0,0,0,0,
        -6,  -4,   1,   2,   1,   1,  -4,  -6,  0,0,0,0,0,0,0,0,
        -6,  -4,   1,   1,   1,   1,  -4,  -6,  0,0,0,0,0,0,0,0,
         0,   0,   0,   0,   0,   0,   0,   0,  0,0,0,0,0,0,0,0},
        // black
        {0,   0,   0,   0,   0,   0,   0,   0,  0,0,0,0,0,0,0,0,
         6,   4,  -1,  -1,  -1,  -1,   4,   6,  0,0,0,0,0,0,0,0,
         6,   4,  -1,  -2,  -2,  -1,   4,   6,  0,0,0,0,0,0,0,0,
         6,   4,  -2,  -8,  -8,  -2,   4,   6,  0,0,0,0,0,0,0,0,
         6,   4,  -5, -10, -10,  -5,   4,   6,  0,0,0,0,0,0,0,0,
         4,   4,  -1,  -5,  -5,  -1,   4,   4,  0,0,0,0,0,0,0,0,
         6,   4,  -1,  24,  24,  -1,   4,   6,  0,0,0,0,0,0,0,0,
         0,   0,   0,   0,   0,   0,   0,   0,  0,0,0,0,0,0,0,0}
    },
    // knight
    {
        // white
        {-8, -12,  -8,  -8,  -8,  -8, -12,  -8, 0,0,0,0,0,0,0,0,
         -8,   0,   0,   0,   0,   0,   0,  -8, 0,0,0,0,0,0,0,0,
         -8,   0,   4,   4,   4,   4,   0,  -8, 0,0,0,0,0,0,0,0,
         -8,   0,   4,   8,   8,   4,   0,  -8, 0,0,0,0,0,0,0,0,
         -8,   0,   4,   8,   8,   4,   0,  -8, 0,0,0,0,0,0,0,0,
         -8,   0,   4,   4,   4,   4,   0,  -8, 0,0,0,0,0,0,0,0,
         -8,   0,   1,   2,   2,   1,   0,  -8, 0,0,0,0,0,0,0,0,
         -8,  -8,  -8,  -8,  -8,  -8,  -8,  -8, 0,0,0,0,0,0,0,0},
        // black
        { 8,   8,   8,   8,   8,   8,   8,   8, 0,0,0,0,0,0,0,0,
          8,   0,  -1,  -2,  -2,  -1,   0,   8, 0,0,0,0,0,0,0,0,
          8,   0,  -4,  -4,  -4,  -4,   0,   8, 0,0,0,0,0,0,0,0,
          8,   0,  -4,  -8,  -8,  -4,   0,   8, 0,0,0,0,0,0,0,0,
          8,   0,  -4,  -8,  -8,  -4,   0,   8, 0,0,0,0,0,0,0,0,
          8,   0,  -4,  -4,  -4,  -4,   0,   8, 0,0,0,0,0,0,0,0,
          8,   0,   0,   0,   0,   0,   0,   8, 0,0,0,0,0,0,0,0,
          8,  12,   8,   8,   8,   8,   12,  8, 0,0,0,0,0,0,0,0}
    },
    // bishop
    {
        // white
        {-4,  -4, -12,  -4,  -4, -12,  -4,  -4, 0,0,0,0,0,0,0,0,
         -4,   2,   1,   1,   1,   1,   2,  -4, 0,0,0,0,0,0,0,0,
         -4,   1,   2,   4,   4,   2,   1,  -4, 0,0,0,0,0,0,0,0,
         -4,   0,   5,   6,   6,   5,   0,  -4, 0,0,0,0,0,0,0,0,
         -4,   0,   4,   6,   6,   4,   0,  -4, 0,0,0,0,0,0,0,0,
         -4,   0,   2,   4,   4,   2,   0,  -4, 0,0,0,0,0,0,0,0,
         -4,   0,   0,   0,   0,   0,   2,  -4, 0,0,0,0,0,0,0,0,
         -4,  -4,  -4,  -4,  -4,  -4,  -4,  -4, 0,0,0,0,0,0,0,0},
         // black
        {4,   4,   4,   4,   4,   4,   4,  -4,  0,0,0,0,0,0,0,0,
         4,   0,   0,   0,   0,   0,   0,  -4,  0,0,0,0,0,0,0,0,
         4,   0,  -2,  -4,  -4,  -2,   0,  -4,  0,0,0,0,0,0,0,0,
         4,   0,  -4,  -6,  -6,  -4,   0,  -4,  0,0,0,0,0,0,0,0,
         4,   0,  -5,  -6,  -6,  -5,   0,  -4,  0,0,0,0,0,0,0,0,
         4,  -1,  -2,  -4,  -4,  -2,  -1,  -4,  0,0,0,0,0,0,0,0,
         4,  -2,  -1,  -1,  -1,  -1,  -2,  -4,  0,0,0,0,0,0,0,0,
         4,   4,  12,   4,   4,  12,   4,  -4,  0,0,0,0,0,0,0,0}
    },
    // rook
    {
        // white
        { 0,   0,   0,   2,   2,   0,   0,   0, 0,0,0,0,0,0,0,0,
         -5,   0,   0,   0,   0,   0,   0,  -5, 0,0,0,0,0,0,0,0,
         -5,   0,   0,   0,   0,   0,   0,  -5, 0,0,0,0,0,0,0,0,
         -5,   0,   0,   0,   0,   0,   0,  -5, 0,0,0,0,0,0,0,0,
         -5,   0,   0,   0,   0,   0,   0,  -5, 0,0,0,0,0,0,0,0,
         -5,   0,   0,   0,   0,   0,   0,  -5, 0,0,0,0,0,0,0,0,
         20,  20,  20,  20,  20,  20,  20,  20, 0,0,0,0,0,0,0,0,
          5,   5,   5,   5,   5,   5,   5,   5, 0,0,0,0,0,0,0,0},
        // black
        {-5,  -5,  -5,  -5,  -5,  -5,  -5,  -5,  0,0,0,0,0,0,0,0,
        -20, -20, -20, -20, -20, -20, -20, -20,  0,0,0,0,0,0,0,0,
          5,   0,   0,   0,   0,   0,   0,   5,  0,0,0,0,0,0,0,0,
          5,   0,   0,   0,   0,   0,   0,   5,  0,0,0,0,0,0,0,0,
          5,   0,   0,   0,   0,   0,   0,   5,  0,0,0,0,0,0,0,0,
          5,   0,   0,   0,   0,   0,   0,   5,  0,0,0,0,0,0,0,0,
          5,   0,   0,   0,   0,   0,   0,   5,  0,0,0,0,0,0,0,0,
          0,   0,   0,  -2,  -2,   0,   0,   0,  0,0,0,0,0,0,0,0,}
    },
    // queen
    {
        // white
        {-5,-5,-5,-5,-5,-5,-5,-5, 0,0,0,0,0,0,0,0,
          0, 0, 1, 1, 1, 1, 0, 0, 0,0,0,0,0,0,0,0,
          0, 0, 1, 2, 2, 1, 0, 0, 0,0,0,0,0,0,0,0,
          0, 0, 2, 3, 3, 2, 0, 0, 0,0,0,0,0,0,0,0,
          0, 0, 2, 3, 3, 2, 0, 0, 0,0,0,0,0,0,0,0,
          0, 0, 1, 2, 2, 1, 0, 0, 0,0,0,0,0,0,0,0,
          0, 0, 1, 1, 1, 1, 0, 0, 0,0,0,0,0,0,0,0,
          0, 0, 0, 0, 0, 0, 0, 0, 0,0,0,0,0,0,0,0},
        // black
        {0, 0, 0, 0, 0, 0, 0, 0, 0,0,0,0,0,0,0,0,
         0, 0,-1,-1,-1,-1, 0, 0, 0,0,0,0,0,0,0,0,
         0, 0,-1,-2,-2,-1, 0, 0, 0,0,0,0,0,0,0,0,
         0, 0,-2,-3,-3,-2, 0, 0, 0,0,0,0,0,0,0,0,
         0, 0,-2,-3,-3,-2, 0, 0, 0,0,0,0,0,0,0,0,
         0, 0,-1,-2,-2,-1, 0, 0, 0,0,0,0,0,0,0,0,
         0, 0,-1,-1,-1,-1, 0, 0, 0,0,0,0,0,0,0,0,
         5, 5, 5, 5, 5, 5, 5, 5, 0,0,0,0,0,0,0,0}
    },
    // king
    {
        // white
        {40,  50,  30,  10,  10,  30,  50,  40,  0,0,0,0,0,0,0,0,
          30,  40,  20,   0,   0,  20,  40,  30,  0,0,0,0,0,0,0,0,
          10,  20,   0, -20, -20,   0,  20,  10,  0,0,0,0,0,0,0,0,
           0,  10, -10, -30, -30, -10,  10,   0,  0,0,0,0,0,0,0,0,
         -10,   0, -20, -40, -40, -20,   0, -10,  0,0,0,0,0,0,0,0,
         -20, -10, -30, -50, -50, -30, -10, -20,  0,0,0,0,0,0,0,0,
         -30, -20, -40, -60, -60, -40, -20, -30,  0,0,0,0,0,0,0,0,
         -40, -30, -50, -70, -70, -50, -30, -40,  0,0,0,0,0,0,0,0},
        // black
        {40,  30,  50,  70,  70,  50,  30,  40, 0,0,0,0,0,0,0,0,
         30,  20,  40,  60,  60,  40,  20,  30, 0,0,0,0,0,0,0,0,
         20,  10,  30,  50,  50,  30,  10,  20, 0,0,0,0,0,0,0,0,
         10,   0,  20,  40,  40,  20,   0,  10, 0,0,0,0,0,0,0,0,
          0, -10,  10,  30,  30,  10, -10,   0, 0,0,0,0,0,0,0,0,
        -10, -20,   0,  20,  20,   0, -20, -10, 0,0,0,0,0,0,0,0,
        -30, -40, -20,   0,   0, -20, -40, -30, 0,0,0,0,0,0,0,0,
        -40, -50, -30, -10, -10, -30, -50, -40, 0,0,0,0,0,0,0,0}
    }
};

char ascci_piece[7][3] = {
    {'P', 'p', '.'},
    {'N', 'n', '.'},
    {'B', 'b', '.'},
    {'R', 'r', '.'},
    {'Q', 'q', '.'},
    {'K', 'k', '.'},
    {'.', '.', '.'}
};


bool isPromotion(smove move) {
	return move.flags & PROMOTION_FLAG;
}

S8 square(S8 row, S8 col) {
    return row * 16 + col;
}

S8 square(std::string sq) {
    S8 col = toupper(sq[0]) - 'A';
    S8 row = sq[1] - '0';
    return square(row, col);
}

S8 rowOfSq(S8 sq) {
	return sq >> 4;
}

S8 colOfSq(S8 sq) {
	return sq & 7;
}

U64 rand64() {
 static U64 next = 1;

  next = next * 1103515245 + 12345;
  return next;
}

Chess::Chess() {

    loadFEN(START_FEN);
	 depth_max = 2;
	 m_materialCount = 0;
}

void Chess::clearSq(S8 sq) {
	if(piece[sq] == EMPTY)
		return;
	U8 piece = this->piece[sq];
	U8 color = this->color[sq];
	// update matetial count
	this->m_materialCount -= PIECE_VALUE[piece][color] + PIECE_VALUE_SQUARE[piece][color][sq];
	// empty the square
	this->piece[sq] = EMPTY;
	this->color[sq] = COLOR_EMPTY;
	// update the hash
	this->hash ^= zobrist.piecesquare[piece][color][sq];
}

void Chess::fillSq(U8 color, U8 piece, S8 sq) {
	if(piece == EMPTY)
		return;
	// update material count
	this->m_materialCount += PIECE_VALUE[piece][color] + PIECE_VALUE_SQUARE[piece][color][sq];
	// actually fill the square
	this->piece[sq] = piece;
	this->color[sq] = color;
	// update the hash key
	this->hash ^= zobrist.piecesquare[piece][color][sq];
};

void Chess::clearBoard() {
    for (int sq = 0;sq < 128; sq++) {
		 piece[sq] = EMPTY;
		 color[sq] = COLOR_EMPTY;
	}
	 m_materialCount = 0;
	 hash = 0;
	 castlingRights = 0;
	 enPassantSquare = -1;
}

void Chess::loadFEN(std::string FEN) {

    clearBoard();

    int i;
    S8 row = 7;
    S8 col = 0;

    for(i = 0; FEN[i] != ' '; i++) {
        switch(FEN[i]) {
             case 'K': fillSq(WHITE, KING, square(row, col));  king[WHITE] = square(row,col);  col++; break;
             case 'Q': fillSq(WHITE, QUEEN, square(row, col));  col++; break;
             case 'R': fillSq(WHITE, ROOK, square(row, col));   col++; break;
             case 'B': fillSq(WHITE, BISHOP, square(row, col)); col++; break;
             case 'N': fillSq(WHITE, KNIGHT, square(row, col)); col++; break;
             case 'P': fillSq(WHITE, PAWN, square(row, col));   col++; break;
             case 'k': fillSq(BLACK, KING, square(row, col)); king[BLACK] = square(row, col);  col++; break;
             case 'q': fillSq(BLACK, QUEEN, square(row, col));  col++; break;
             case 'r': fillSq(BLACK, ROOK, square(row, col));   col++; break;
             case 'b': fillSq(BLACK, BISHOP, square(row, col)); col++; break;
             case 'n': fillSq(BLACK, KNIGHT, square(row, col)); col++; break;
             case 'p': fillSq(BLACK, PAWN, square(row, col));   col++; break;
             case '/': row--; col = 0; break;
             case '1': col += 1; break;
             case '2': col += 2; break;
             case '3': col += 3; break;
             case '4': col += 4; break;
             case '5': col += 5; break;
             case '6': col += 6; break;
             case '7': col += 7; break;
             case '8': col += 8; break;
        };
    }

    i++;

	if (FEN[i] == 'w')
		sideToMove = WHITE;
	else {
		sideToMove = BLACK;
		this->hash ^= zobrist.color;
	}

    i += 2;

    castlingRights = 0;
    for(; FEN[i] != ' '; i++) {
        switch(FEN[i]) {
            case 'K': castlingRights |= CASTLE_WK; break;
            case 'k': castlingRights |= CASTLE_BK; break;
            case 'Q': castlingRights |= CASTLE_WQ; break;
            case 'q': castlingRights |= CASTLE_BQ; break;
        };
    }

	 this->hash ^= zobrist.castling[this->castlingRights];

    i++;
    // get en passant square
    if(FEN[i] != '-') {

		 // update the hash
    }
    // get move count until draw

    // get move count
};

void Chess::makeMove(smove move) {

    // if it's a capture or a pawn move, reset the count
    this->withoutCaptureCount++;
    if(piece[move.from] == PAWN || piece[move.to] != EMPTY)
        this->withoutCaptureCount = 0;

    // clear arrival square
    if(piece[move.to] != EMPTY)
        clearSq(move.to);

    // move the piece
    fillSq(this->sideToMove, piece[move.from], move.to);

    // clear the depart square
    clearSq(move.from);

    // change side to move
    this->sideToMove = !this->sideToMove;
	 hash ^= zobrist.color;

    // removing castling rights
    switch (move.from) {
      case H1: this->castlingRights &= ~CASTLE_WK; break;
      case E1: this->castlingRights &= ~(CASTLE_WK | CASTLE_WQ); break;
      case A1: this->castlingRights &= ~CASTLE_WQ; break;
      case H8: this->castlingRights &= ~CASTLE_BK; break;
      case E8: this->castlingRights &= ~(CASTLE_BK | CASTLE_BQ); break;
      case A8: this->castlingRights &= ~CASTLE_BQ; break;
   }
   switch (move.to) {
      case H1: this->castlingRights &= ~CASTLE_WK; break;
      case E1: this->castlingRights &= ~(CASTLE_WK | CASTLE_WQ); break;
      case A1: this->castlingRights &= ~CASTLE_WQ; break;
      case H8: this->castlingRights &= ~CASTLE_BK; break;
      case E8: this->castlingRights &= ~(CASTLE_BK | CASTLE_BQ); break;
      case A8: this->castlingRights &= ~CASTLE_BQ; break;
    }
	hash ^= zobrist.castling[move.castling_flags];
	hash ^= zobrist.castling[this->castlingRights];

    // moving the rook if the move is a castling one
    if (piece[move.to] == KING && abs(move.from - move.to) == 2) {
        if (move.to == G1) {
            clearSq(H1);
            fillSq(WHITE, ROOK, F1);
        }
        else if (move.to == C1) {
            clearSq(A1);
            fillSq(WHITE, ROOK, D1);
        }
        else if (move.to == G8) {
            clearSq(H8);
            fillSq(BLACK, ROOK, F8);
        }
        else if (move.to == C8) {
            clearSq(A8);
            fillSq(BLACK, ROOK, D8);
        }
  }
    // en passant
    if (move.flags & EP_FLAG) {
        if (!this->sideToMove == WHITE) {
            clearSq(move.to - 16);
        } else {
            clearSq(move.to + 16);
        }
    }
	if (int(enPassantSquare) != -1) {
		hash ^= zobrist.ep[this->enPassantSquare];
		enPassantSquare = -1;
	}
	if (piece[move.to] == PAWN && (abs(move.from - move.to) == 32)) {
		enPassantSquare = (move.from + move.to) / 2;
		hash ^= zobrist.ep[this->enPassantSquare];
	}

    // promotion
	if (move.flags & PROMOTION_FLAG) {
		if (move.flags & PROMOTION_KNIGHT_FLAG)
			fillSq(!this->sideToMove, KNIGHT, move.to);
		if (move.flags & PROMOTION_BISHOP_FLAG)
			fillSq(!this->sideToMove, BISHOP, move.to);
		if (move.flags & PROMOTION_ROOK_FLAG)
			fillSq(!this->sideToMove, ROOK, move.to);
		if (move.flags & PROMOTION_QUEEN_FLAG)
			fillSq(!this->sideToMove, QUEEN, move.to);
    }

	// keeping track of whree the king is
	if (piece[move.to] == KING)
		king[!sideToMove] = move.to;

    moveCount++;
}

void Chess::unmakeMove(smove move) {
    // changing side to move
    this->sideToMove = !this->sideToMove;
	 hash ^= zobrist.color;

    // update this count
    this->withoutCaptureCount = move.moveTillDrawCount;

    // restore en passant sque
	 if(int(this->enPassantSquare) != -1)
		 hash ^= zobrist.ep[this->enPassantSquare];
	 if(int(move.epSquare) != -1)
		 hash ^= zobrist.ep[move.epSquare];
    this->enPassantSquare = move.epSquare;

    // remove moved piece
    fillSq(this->sideToMove, piece[move.to], move.from);
    clearSq(move.to);
    // restore captured piece
    if(move.capture != EMPTY)
        fillSq(!this->sideToMove, move.capture, move.to);

    // if it was a promotion
    if (move.flags & PROMOTION_FLAG) {
        clearSq(move.from);
        fillSq(this->sideToMove, PAWN, move.from);
    }

    // un castle
    if (move.flags & CASTLE_FLAG) {
        if (move.to == G1) {
            clearSq(F1);
            fillSq(WHITE, ROOK, H1);
        } else if (move.to == C1) {
            clearSq(D1);
            fillSq(WHITE, ROOK, A1);
        } else if (move.to == G8) {
            clearSq(F8);
            fillSq(BLACK, ROOK, H8);
        } else if (move.to == C8) {
            clearSq(D8);
            fillSq(BLACK, ROOK, A8);
        }
    }

    // change castling rights
	 hash ^= zobrist.castling[this->castlingRights];
	 hash ^= zobrist.castling[move.castling_flags];
    this->castlingRights = move.castling_flags;

	 // reset the king pos
	 if(piece[move.from] == KING)
		 king[this->sideToMove] = move.from;

    // undo en passant
	if (move.flags & EP_FLAG) {
        if (this->sideToMove == WHITE) {
            fillSq(BLACK, PAWN, move.to - 16);
        } else {
            fillSq(WHITE, PAWN, move.to + 16);
        }
	}
};

bool Chess::isAttacked(bool side, S8 sq) const {
	// pawn
	if (side == WHITE) {
		S8 to1 = sq + SE;
		S8 to2 = sq + SW;
		if(!(to1 & 0x88) && piece[to1] == PAWN && color[to1] == side)
			return 1;
		if(!(to2 & 0x88) && piece[to2] == PAWN && color[to2] == side)
			return 1;
	} else {
		S8 to1 = sq + NE;
		S8 to2 = sq + NW;
		if(!(to1 & 0x88) && piece[to1] == PAWN && color[to1] == side)
			return 1;
		if(!(to2 & 0x88) && piece[to2] == PAWN && color[to2] == side)
			return 1;

	}
	
	// knight
	if(leaperAttack(side, KNIGHT, sq))
		return 1;
	
	// king
	if(leaperAttack(side, KING, sq))
		return 1;

	// straight line sliders 
   if (straightAttack(side, sq, NORTH) ||
    straightAttack(side, sq, SOUTH) ||
    straightAttack(side, sq, EAST) ||
    straightAttack(side, sq, WEST)
    )	{ 
    return 1;
	}

  // diagonal sliders 
  if (diagAttack(side, sq, NE) ||
    diagAttack(side, sq, SE) ||
    diagAttack(side, sq, NW) ||
    diagAttack(side, sq, SW)
    ) {
    return 1;
	}

	return 0;
}

bool Chess::leaperAttack(bool side, U8 piece, S8 sq) const {
	for(int i = 0; i < NB_MOVES[piece]; i++) {
		U8 to = sq + MOVES[piece][i];
		if(!(to & 0x88) && this->piece[to] == piece &&  color[to] == side)
			return 1;
	}
	return 0;
}

bool Chess::straightAttack(bool side, S8 sq, U8 dir) const {
	U8 to = sq + dir;
	while(!(to & 0x88)) {
		if(color[to] != COLOR_EMPTY) {
			if(color[to] == side && (piece[to] == QUEEN || piece[to] == ROOK))
				return 1;
			return 0;
		}
		to += dir;
	}
	return 0;
}

bool Chess::diagAttack(bool side, S8 sq, U8 dir) const {
	U8 to = sq + dir;
	while(!(to & 0x88)) {
		if(color[to] != COLOR_EMPTY) {
			if(color[to] == side && (piece[to] == QUEEN || piece[to] == BISHOP)) {
				return 1;
			}
			return 0;
		}
		to += dir;
	}
	return 0;
}

void Chess::addMove(std::vector<smove>& v, const U8 &from, const U8 &to, const S8 &flags, const U8& capture) const {
	v.push_back({from, to, flags, this->castlingRights, capture, this->enPassantSquare, this->withoutCaptureCount});
}

// tous les coups possibles si les échecs n'existent pas
std::vector<smove> Chess::genPseudoLegalMoves() const {
	std::vector<smove> pseudoLegalMoves;

	S8 pawn_direction = PAWN_DIRECTION[sideToMove]; 

	for (int row = 0; row < 8; row++) {
		for (int col = 0; col < 8; col++) {
			// on a pas de tableau des pièces
			U8 sq = square(row, col);
			
			if(color[sq] == sideToMove) {

				// les pions
				if(piece[sq] == PAWN) {
					S8 to = sq + pawn_direction;
					// normal move + 2 squares move + promotion
					// le pion ne peut jamais sortir du plateau en avancant
					if(piece[to] == EMPTY) {
						// on peut avancer d'une case
						// promotion
						if(rowOfSq(to) == PAWN_PROMOTION_ROW[sideToMove]) {
							this->addMove(pseudoLegalMoves, sq, to, PROMOTION_FLAG | PROMOTION_KNIGHT_FLAG);
							this->addMove(pseudoLegalMoves, sq, to, PROMOTION_FLAG | PROMOTION_BISHOP_FLAG);
							this->addMove(pseudoLegalMoves, sq, to, PROMOTION_FLAG | PROMOTION_ROOK_FLAG);
							this->addMove(pseudoLegalMoves, sq, to, PROMOTION_FLAG | PROMOTION_QUEEN_FLAG);
						} else {
							// psa promotion
							this->addMove(pseudoLegalMoves, sq, to);
							// si il est sur la case de départ
							if (rowOfSq(sq) == PAWN_START_ROW[sideToMove] && piece[to + pawn_direction] == EMPTY) {
								this->addMove(pseudoLegalMoves, sq, to + pawn_direction);
							}
						}
					}
					// captures
					for (const S8 &d : {1, -1}) {
						S8 newTo = to+d;
						if(!(newTo & 0x88) && piece[newTo] != EMPTY && color[newTo] == !sideToMove) {
							if(rowOfSq(newTo) == PAWN_PROMOTION_ROW[sideToMove]) {
								this->addMove(pseudoLegalMoves, sq, newTo, PROMOTION_FLAG | PROMOTION_KNIGHT_FLAG, piece[newTo]);
								this->addMove(pseudoLegalMoves, sq, newTo, PROMOTION_FLAG | PROMOTION_BISHOP_FLAG, piece[newTo]);
								this->addMove(pseudoLegalMoves, sq, newTo, PROMOTION_FLAG | PROMOTION_ROOK_FLAG, piece[newTo]);
								this->addMove(pseudoLegalMoves, sq, newTo, PROMOTION_FLAG | PROMOTION_QUEEN_FLAG, piece[newTo]);
							} else {
								this->addMove(pseudoLegalMoves, sq, newTo, 0, piece[newTo]);
							}
						}
						else if (enPassantSquare == newTo)
							this->addMove(pseudoLegalMoves, sq, newTo, EP_FLAG);
					}
				} 
				// si c'est pas un pion
				else {
					U8 curPiece = piece[sq];
					// si c'est un roi / cavalier, on ajoute juste tous les coups
					if (!slide[curPiece]) {
						for(int i = 0; i < NB_MOVES[curPiece]; i++) {
							S8 to = sq + MOVES[curPiece][i];
							if(!(to & 0x88) && color[to] != sideToMove)
								this->addMove(pseudoLegalMoves, sq, to, 0, piece[to]);
						}
					} else {
						// on ajoute tous les coups
						for(int i = 0; i < NB_MOVES[curPiece]; i++) {
							// tant que le coup est dans l'échécquier, que l'on a pas rencontré de pièce de notre couleur
							S8 dir = MOVES[curPiece][i];
							for (S8 to = sq + dir; !(to & 0x88) && color[to] != sideToMove; to += dir) {
								this->addMove(pseudoLegalMoves, sq, to, 0, piece[to]);
								// si on trouve une piece de l'autre couleur, on arrete
								if(color[to] != COLOR_EMPTY)
									break;
							}
						}
					}
				}
			}
		}
	}

		// adding castle if possible
	if (sideToMove == WHITE) {
		if (castlingRights & CASTLE_WK) {
			if ((piece[F1] == EMPTY) &&
				(piece[G1] == EMPTY) &&
				(!isAttacked(!sideToMove, E1)) &&
				(!isAttacked(!sideToMove, F1)) &&
				(!isAttacked(!sideToMove, G1)))
			addMove(pseudoLegalMoves, E1, G1, CASTLE_FLAG);
		}
		if (castlingRights & CASTLE_WQ) {
			if ((piece[B1] == EMPTY) &&
				 (piece[C1] == EMPTY) &&
				 (piece[D1] == EMPTY) &&
				 (!isAttacked(!sideToMove, E1)) &&
				 (!isAttacked(!sideToMove, D1)) &&
				 (!isAttacked(!sideToMove, C1)))
			addMove(pseudoLegalMoves, E1, C1, CASTLE_FLAG);
		}
   }
	else {
		if (castlingRights & CASTLE_BK) {
			if ((piece[F8] == EMPTY) &&
				 (piece[G8] == EMPTY) &&
				 (!isAttacked(!sideToMove, E8)) &&
				 (!isAttacked(!sideToMove, F8)) &&
				 (!isAttacked(!sideToMove, G8)))
			addMove(pseudoLegalMoves, E8, G8, CASTLE_FLAG);
		}
		if (castlingRights & CASTLE_BQ) {
			if ((piece[B8] == EMPTY) &&
				 (piece[C8] == EMPTY) &&
				 (piece[D8] == EMPTY) &&
				 (!isAttacked(!sideToMove, E8)) &&
				 (!isAttacked(!sideToMove, D8)) &&
				 (!isAttacked(!sideToMove, C8)))
			addMove(pseudoLegalMoves, E8, C8, CASTLE_FLAG);
		}
	}


	return pseudoLegalMoves;
}

std::vector<smove> Chess::genLegalMoves() {
	std::vector<smove> pseudoLegalMoves = genPseudoLegalMoves();
	std::vector<smove> legalMoves;

	for (smove move : pseudoLegalMoves) {
		makeMove(move);
		if (!isAttacked(sideToMove, king[!sideToMove])) {
			legalMoves.push_back(move);
		}
		unmakeMove(move);
	}

	return legalMoves;
}

void Chess::show() const {
    for (S8 row = 7; row >= 0; row--) {
        for (S8 col = 0; col < 8; col++) {
            U8 sq = square(row, col);
            printf("%c ", ascci_piece[piece[sq]][color[sq]]);
        }
        printf("\n");
    }
}

U8 Chess::pieceAt(S8 sq) const {
    return this->piece[sq];
}
U8 Chess::colorAt(S8 sq) const {
    return this->color[sq];
}
bool Chess::toPlay() const {
	return this-> sideToMove;
}
int Chess::materialCount() const {
	return this->m_materialCount;
}

U8 Chess::whereIsKing(U8 color) const {
	return king[color];
}

bool Chess::isCurrentPlayerChecked() const {
	return isAttacked(!sideToMove, king[sideToMove]);
}



// test
int Chess::numberMoves(int depth) {
	if (depth == 0)
		return 1;

	std::vector<smove> pseudoLegalMoves = genPseudoLegalMoves();
	int tot = 0;

	for (smove move : pseudoLegalMoves) {
		makeMove(move);
		if (!isAttacked(sideToMove, king[!sideToMove])) {
			int n = numberMoves(depth - 1);
			// if (depth == 2) {
			// 	std::cout << int(move.from) << " " << int(move.to) << " : " << n << "\n";
			// 	show();
			// }

			tot += n;
		}
		unmakeMove(move);
	}

	return tot;
}

// bot
int Chess::eval() const {
	return this->m_materialCount;
}

smove Chess::bestMove() {
	std::cout << "eval : " << minmax(0) << "\n";
	// std::cout << "eval : " << alphabeta(0, -INFINF, +INFINF) << "\n";
	return m_bestMove;
}

int Chess::minmax(int depth) {
	if (depth > depth_max) {
		return eval();
	}

	int bestEval = -INFINF;
	if (sideToMove == BLACK)
		bestEval = INFINF;
	std::vector<smove> pseudoLegalMoves = genPseudoLegalMoves();

	for (smove move : pseudoLegalMoves) {
		makeMove(move);
		if (!isAttacked(sideToMove, king[!sideToMove])) {
			int curEval = minmax(depth+1);
			// we're looking for the best move for white
			if (sideToMove == BLACK) {
				if (bestEval < curEval) {
					bestEval = curEval;
					if (depth == 0)
						m_bestMove = move;
				}
			} else {
				if (bestEval > curEval) {
					bestEval = curEval;
					if (depth == 0)
						m_bestMove = move;
				}
			}
		}
		unmakeMove(move);
	}

	return bestEval;
}

int Chess::alphabeta(int depth, int alpha, int beta) {
	if(depth > depth_max)
		return eval();
	
	// white
	if(this->sideToMove) {
		int value = -INFINF;
		std::vector<smove> pseudoLegalMoves = genPseudoLegalMoves();

		for (smove move : pseudoLegalMoves) {
			makeMove(move);
			if(!isAttacked(sideToMove, king[!sideToMove])) {
				int curEval = alphabeta(depth+1, alpha, beta);
				if(curEval > value) {
					value = curEval;
					if(value >= beta)
						return -INF;
					if(value > alpha)
						alpha = value;
				}
			}
			unmakeMove(move);
		}

		return value;
	} else {
		int value = INFINF;
		std::vector<smove> pseudoLegalMoves = genPseudoLegalMoves();

		for (smove move : pseudoLegalMoves) {
			makeMove(move);
			if(!isAttacked(sideToMove, king[!sideToMove])) {
				int curEval = alphabeta(depth+1, alpha, beta);
				if(curEval < value) {
					value = curEval;
					if(value <= alpha)
						return -INF;
					if(value > beta)
						beta = value;
				}
			}
			unmakeMove(move);
		}

		return value;
	}
}

// hash tables
void Chess::initZobrist() {
	for (int pnr = 0; pnr <= 5; pnr++) {
		for (int cnr = 0; cnr <= 1; cnr++) {
			for (int snr = 0; snr <= 127; snr++) {
				zobrist.piecesquare[pnr][cnr][snr] = rand64();
			}
		}
	}

	zobrist.color = rand64();

	for (int castling = 0; castling <= 15; castling++) {
		zobrist.castling[castling] = rand64();
	}

	for (int ep = 0; ep <= 127; ep++) {
		zobrist.ep[ep] = rand64();
	}
}
