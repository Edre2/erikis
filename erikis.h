#ifndef ERIKIS_H
#define ERIKIS_H

#include <QMainWindow>
#include <QGridLayout>
#include <QPushButton>
#include <QMessageBox>
#include "chess.h"

class Erikis : public QMainWindow
{
    Q_OBJECT

public:
    Erikis(QWidget *parent = nullptr);
    ~Erikis();

    void genIcons(int theme);
    void createBoard();

    void resetBoardColor();

    void displayPieces();

    void getPossibleMoves();
    void showPossibleMoves();

    int getPromotion();

    void checkWinnerDraw();
    void displayDraw(QString why);
    void displayWinner(QString winner);

public slots:
    void squareClicked(int sq);

private:
    Chess board;

    QWidget *m_widget_principal;
    QGridLayout *m_echequier;
    QPushButton *m_cases[128];

    QIcon icons[7][3];

    QString players[2];

    U8 prev = -1;
    std::vector<smove> possibleMoves;

    bool ai;

};
#endif // ERIKIS_H
