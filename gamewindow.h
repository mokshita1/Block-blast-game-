#ifndef GAMEWINDOW_H
#define GAMEWINDOW_H

#include <QMainWindow>
#include <QVector>
#include <QTimer>
#include "block.h"
#include <QStackedWidget>
#include <QPushButton>
#include <qmessagebox.h>
#include <QLabel>
#include <QVBoxLayout>

class GameWindow : public QMainWindow
{
    Q_OBJECT
public:
    GameWindow(QWidget *parent = nullptr);
    ~GameWindow();

protected:
    void paintEvent(QPaintEvent *event) override;


private:
    QVector<QVector<int>> grid;  // 10x10 grid
    QVector<QColor> gridColors;  // color per cell
    QVector<Block*> blocks;
    QTimer *timer;
    QTimer *fallTimer;           // for falling animation
    QStackedWidget *stacked_widget;
    QPushButton *start;
    QVBoxLayout *layout_game;
    int timeElapsed;
    int score;
    QStackedWidget *pages;

    // Start Menu
    QWidget *startPage;
    QPushButton *startButton;

    // Game Page
    QWidget *gamePage;

    // End Menu
    QWidget *endPage;
    QLabel *finalScoreLabel;
    QPushButton *playAgainButton;
    QPushButton *exitButton;

    QVector<int> rowsToClear;
    QVector<int> colsToClear;

    // For falling animation
    int fallStep;
    QVector<QVector<int>> tempGrid;
    QVector<QColor> tempColors;

    void generateBlocks();
    bool canPlaceBlock(Block* block, int row, int col);
    void placeBlock(Block* block, int row, int col);
    void snapBlockToGrid(Block* block);
    void checkFullLines();
    void startFallingAnimation();
    void performFallingStep();
    void checkGameOver();
    void setupStartMenu();
    void setupGamePage();
    void setupEndMenu();
    void showEndMenu();
    void resetGame();


private slots:
    void updateTimer();
    void blockReleased(Block* block);
};

#endif // GAMEWINDOW_H
