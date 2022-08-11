#include "erikis.h"

const int NB_THEMES = 1;

const QString BG_COLOR[2] = {"#F0D9B5", "#B58863"};
const QString POSSIBLE_MOVE = "#AEB188";
const QString ATTACKED_KING = "#b30000";
const int SQUARE_SIZE = 100;

const QString THEMES[NB_THEMES] = {"default"};
const QString PIECE_NAME[7] = {"pawn", "knight", "bishop", "rook", "queen", "king", "empty"};
const QString COLOR_NAME[3] = {"white", "black", "empty"};

Erikis::Erikis(QWidget *parent)
    : QMainWindow(parent), board()
{
    createBoard();
    genIcons(0);
    getPossibleMoves();
    this->displayPieces();
    players[0] = "Blanc";
    players[1] = "Noir";
    ai = true;
}

Erikis::~Erikis()
{
}

void Erikis::createBoard() {
    m_echequier = new QGridLayout;

    for(int iCol = 0; iCol < 8; iCol++) {
        for (int iRow = 0; iRow < 8; iRow++) {
            int sq = square(iRow, iCol);
            m_cases[sq] = new QPushButton;

            m_cases[sq]->setFixedSize(SQUARE_SIZE, SQUARE_SIZE);
            m_cases[sq]->setIconSize(QSize(SQUARE_SIZE, SQUARE_SIZE));
            m_echequier->addWidget(m_cases[sq], 7-iRow, iCol);

            // connecting the clicks

            connect(m_cases[sq], &QPushButton::clicked, [this, sq]() {
                squareClicked(sq);
             });

        }
   }

    m_widget_principal = new QWidget;
    m_widget_principal->setFixedSize(8 * SQUARE_SIZE, 8 * SQUARE_SIZE);
    m_widget_principal->setLayout(m_echequier);

    setCentralWidget(m_widget_principal);

    resetBoardColor();
}

void Erikis::resetBoardColor() {
    for(int iCol = 0; iCol < 8; iCol++) {
        for (int iRow = 0; iRow < 8; iRow++) {
            int sq = square(iRow, iCol);
            if( (iRow + iCol) % 2 != 0)
                m_cases[sq]->setStyleSheet("background-color: " + BG_COLOR[0]);
            else
                m_cases[sq]->setStyleSheet("background-color: " + BG_COLOR[1]);
         }
    }
    if (board.isCurrentPlayerChecked())
         m_cases[board.whereIsKing(board.toPlay())]->setStyleSheet("background-color: " + ATTACKED_KING);
    // m_cases[board.whereIsKing(WHITE)]->setStyleSheet("background-color: " + ATTACKED_KING);
    // m_cases[board.whereIsKing(BLACK)]->setStyleSheet("background-color: " + ATTACKED_KING);
}

void Erikis::genIcons(int theme) {
    if (theme + 1 > NB_THEMES || theme < 0)
        return;
    for (int i = 0; i < 6; i++) {
        for (int j = 0; j < 2; j++)
            icons[i][j] = QIcon("./res/" + THEMES[theme] + "/" + COLOR_NAME[j] + "_" + PIECE_NAME[i] + ".png");
        icons[i][2] =  QIcon("./res/" + THEMES[theme] + "/empty.png");
    }
    for (int j = 0; j < 3; j++)
        icons[6][j] =  QIcon("./res/" + THEMES[theme] + "/empty.png");
    displayPieces();
}

void Erikis::displayPieces() {
    board.show();
    std::cout << "matériel : " << board.materialCount() << "\n";

    for (S8 row = 7; row >= 0; row--) {
        for (S8 col = 0; col < 8; col++) {
            U8 sq = square(row, col);
            this->m_cases[sq]->setIcon(icons[int(board.pieceAt(sq))][int(board.colorAt(sq))]);

            // std::cout << (int)(board.pieceAt(sq)) << " " << (int)(board.colorAt(sq)) << "\n";
            // printf("%c ", ascci_piece[piece[sq]][color[sq]]);
        }
    }
}

void Erikis::getPossibleMoves() {
//    this->possibleMoves = this->board.genPseudoLegalMoves();
    this->possibleMoves = this->board.genLegalMoves();
}

void Erikis::showPossibleMoves() {
    // finding the possible moves for this square :
    for (smove &move : this->possibleMoves) {
        if (move.from == prev) {
            this->m_cases[move.to]->setStyleSheet("background-color: " + POSSIBLE_MOVE);
        }
    }
}

int Erikis::getPromotion() {
    QMessageBox piecePicker;
    piecePicker.setText("Choisissez en quoi vous voulez promouvoir votre pion");

    QPushButton *queenButton = piecePicker.addButton("Reine", QMessageBox::ActionRole);
    QPushButton *rookButton = piecePicker.addButton("Tour", QMessageBox::ActionRole);
    QPushButton *bishopButton = piecePicker.addButton("Fou", QMessageBox::ActionRole);
    QPushButton *knightButton = piecePicker.addButton("Cavalier", QMessageBox::ActionRole);

    piecePicker.exec();

    // a queen
    int picked = 3;
    if (piecePicker.clickedButton() == queenButton) {
        picked = 3;
    } else if (piecePicker.clickedButton() == rookButton) {
        picked = 2;
    } else if (piecePicker.clickedButton() == bishopButton) {
        picked = 1;
    } else if (piecePicker.clickedButton() == knightButton) {
        picked = 0;
    }

    return picked;
}

void Erikis::checkWinnerDraw() {
    if (possibleMoves.empty()) {
        // checked : win
        if(board.isCurrentPlayerChecked())
            displayWinner(players[!board.toPlay()]);
        // not checked : stalemate
        else
            displayDraw("pat");
    }

}

void Erikis::displayWinner(QString winner) {
    QMessageBox winnerDisplay;
    winnerDisplay.setText(winner + " a gagné !");
    winnerDisplay.exec();
}

void Erikis::displayDraw(QString why) {
    QMessageBox drawDisplay;
    drawDisplay.setText("Nulle par " + why);
    drawDisplay.exec();
}

void Erikis::squareClicked(int sq) {
    // si la case séléctionnée n'était pas dans le tableau
    if(prev & 0x88) {
        prev = sq;
        showPossibleMoves();

    } else {
        // if we can play the move :
        bool possible = false;
        int i = 0;
        for (i = 0; i < int(this->possibleMoves.size()); i++) {
            if (this->possibleMoves[i].from == prev && this->possibleMoves[i].to == sq) {
                possible = true;
                break;
            }
        }

        if (possible) {

            if(isPromotion(possibleMoves[i])) {
                int choice = getPromotion();
                i += choice;
            }

            board.makeMove(possibleMoves[i]);
            resetBoardColor();

            if (ai) {
                this->getPossibleMoves();
                prev = -1;
                this->displayPieces();
                this->checkWinnerDraw();

                board.makeMove(board.bestMove());
            }

            this->getPossibleMoves();
            prev = -1;
            this->displayPieces();
            this->checkWinnerDraw();
        } else {
            prev = sq;
            resetBoardColor();
            showPossibleMoves();
        }
    }
}
