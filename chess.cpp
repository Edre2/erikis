#include "chess.h"


const int INF = 10000;
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
const int CASTLE_BK = 3;
const int CASTLE_BQ = 4;

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
    {1, -1, 0},
    {3, -3, 0},
    {3, -3, 0},
    {5, -5, 0},
    {9, -9, 0},
    {0, 0, 0},
    {0, 0, 0}
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
	std::cout << "a " << int(move.flags & PROMOTION_FLAG) << "\n";
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

Chess::Chess() {
    loadFEN(START_FEN);
}

void Chess::clearSq(S8 sq) {
    this->materialCount -= PIECE_VALUE[piece[sq]][color[sq]];
    this->piece[sq] = EMPTY;
    this->color[sq] = COLOR_EMPTY;
}

void Chess::fillSq(U8 color, U8 piece, S8 sq) {
    this->piece[sq] = piece;
    this->color[sq] = color;
};

void Chess::clearBoard() {
    for (S8 row = 0; row < 8; row++)
        for (S8 col = 0; col < 8; col++)
            clearSq(16 * row + col);
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
    else
        sideToMove = BLACK;

    i += 2;

    castlingRights = 0;
    for(; FEN[i] != ' '; i++) {
        switch(FEN[i]) {
            case 'K': castlingRights &= CASTLE_WK; break;
            case 'k': castlingRights &= CASTLE_BK; break;
            case 'Q': castlingRights &= CASTLE_WQ; break;
            case 'q': castlingRights &= CASTLE_BQ; break;
        };
    }

    i++;
    // get en passant square
    if(FEN[i] != '-') {

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

    // promotion
    if (move.flags & PROMOTION_FLAG) {
        if (move.flags & PROMOTION_KNIGHT_FLAG)
            fillSq(!this->sideToMove, KNIGHT, move.to);
        if (move.flags & PROMOTION_BISHOP_FLAG)
            fillSq(!this->sideToMove, BISHOP, move.to);
        if (move.flags & PROMOTION_QUEEN_FLAG)
            fillSq(!this->sideToMove, QUEEN, move.to);
    }

    moveCount++;
    // moves[moveCount] = move;
}

void Chess::unmakeMove(smove move) {
    // changing side to move
    this->sideToMove = !this->sideToMove;

    // update this count
    this->withoutCaptureCount = move.moveTillDrawCount;

    // restore en passant sque
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
    this->castlingRights = move.castling_flags;

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
    )
    return 1;

  // diagonal sliders 
  if (diagAttack(side, sq, NE) ||
    diagAttack(side, sq, SE) ||
    diagAttack(side, sq, NW) ||
    diagAttack(side, sq, SW)
    )
    return 1;

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
			if(color[to] == side && (piece[to] == QUEEN || piece[to] == BISHOP))
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
			if(color[to] == side && (piece[to] == QUEEN || piece[to] == BISHOP))
				return 1;
			return 0;
		}
		to += dir;
	}
	return 0;
}

void Chess::addMove(std::vector<smove>& v, const U8 &from, const U8 &to, S8 flags, const U8& capture) const {
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
						if(!(newTo & 0x88) && piece[newTo] != EMPTY)
							this->addMove(pseudoLegalMoves, sq, newTo, 0, piece[newTo]);
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

