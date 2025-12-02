#include "gamewindow.h"
#include <QMessageBox>
#include <QRandomGenerator>
#include <QVBoxLayout>
#include <QApplication>
#include <QPainter>
#include <QPaintEvent>

GameWindow::GameWindow(QWidget *parent)
    : QMainWindow(parent),
    timeElapsed(60),
    score(0),
    fallStep(0)
{
    //setMinimumSize(1200, 1200); // bigger window for bigger grid 1200
    setFixedSize(900, 600);

    // Create stacked pages
    pages = new QStackedWidget(this);
    setCentralWidget(pages);

    setupStartMenu();
    setupGamePage();
    setupEndMenu();

    pages->setCurrentWidget(startPage);

    // Initialize grid
    grid.resize(10);
    gridColors.resize(100);
    for (int i = 0; i < 10; ++i) {
        grid[i].resize(10);
        for (int j = 0; j < 10; ++j) {
            grid[i][j] = 0;
            gridColors[i * 10 + j] = Qt::white;
        }
    }

    // Game timers
    timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, &GameWindow::updateTimer);

    fallTimer = new QTimer(this);
    fallTimer->setInterval(50);
    connect(fallTimer, &QTimer::timeout, this, &GameWindow::performFallingStep);
}

GameWindow::~GameWindow() {}

// ------------------------------------------------------------
// START MENU
// ------------------------------------------------------------
void GameWindow::setupStartMenu()
{
    startPage = new QWidget(this);

    startPage->setStyleSheet(
        "QWidget {"
        "background: qlineargradient(spread:pad, x1:0, y1:0, x2:1, y2:1,"
        "stop:0 #0e0f2c, stop:0.5 #1b164a, stop:1 #2a1d69);}"
        );

    QVBoxLayout *layout = new QVBoxLayout(startPage);

    QLabel *title = new QLabel("Block Puzzle Game");
    title->setAlignment(Qt::AlignCenter);
    title->setStyleSheet("font-size: 34px; font-weight: 800; color: white; letter-spacing: 2px;");

    QLabel *info = new QLabel(
        " How to Play \n\n"
        "• Place blocks on the grid.\n"
        "• Complete rows or columns to clear them.\n"
        "• Earn +5 seconds for every block placed.\n"
        );
    info->setAlignment(Qt::AlignCenter);
    info->setStyleSheet("color: #e6e6ff; font-size: 16px;");

    startButton = new QPushButton("Start Game");
    startButton->setFixedHeight(48);
    startButton->setStyleSheet(
        "QPushButton {"
        "background: rgba(255,255,255,0.10);"
        "border: 2px solid rgba(255,255,255,0.25);"
        "border-radius: 18px;"
        "color: white; font-size: 20px; font-weight: bold; padding: 10px 20px;}"
        "QPushButton:hover { background: rgba(255,255,255,0.18); border-color: rgba(255,255,255,0.45);}"
        "QPushButton:pressed { background: rgba(255,255,255,0.25); }"
        );

    layout->addStretch();
    layout->addWidget(title);
    layout->addWidget(info);
    layout->addWidget(startButton);
    layout->addStretch();

    pages->addWidget(startPage);

    connect(startButton, &QPushButton::clicked, this, [=](){
        resetGame();
        pages->setCurrentWidget(gamePage);
    });
}

// ------------------------------------------------------------
// GAME PAGE
// ------------------------------------------------------------
void GameWindow::setupGamePage()
{
    gamePage = new QWidget(this);

    gamePage->setAttribute(Qt::WA_NoSystemBackground);
    gamePage->setAttribute(Qt::WA_TranslucentBackground);
    gamePage->setStyleSheet("background: transparent;");

    layout_game = new QVBoxLayout(gamePage);
    layout_game->setContentsMargins(0, 0, 0, 0);

    pages->addWidget(gamePage);
}


// ------------------------------------------------------------
// END MENU
// ------------------------------------------------------------
void GameWindow::setupEndMenu()
{
    endPage = new QWidget(this);

    endPage->setStyleSheet(
        "QWidget {"
        "background: qlineargradient(spread:pad, x1:0, y1:0, x2:1, y2:1,"
        "stop:0 #0e0f2c, stop:0.5 #1b164a, stop:1 #2a1d69);}"
        );

    QVBoxLayout *layout = new QVBoxLayout(endPage);

    QLabel *title = new QLabel("Game Over!");
    title->setAlignment(Qt::AlignCenter);
    title->setStyleSheet("font-size: 34px; font-weight: 800; color: white;");

    finalScoreLabel = new QLabel("");
    finalScoreLabel->setAlignment(Qt::AlignCenter);
    finalScoreLabel->setStyleSheet("font-size: 22px; color: #e6e6ff;");

    playAgainButton = new QPushButton("Play Again");
    exitButton = new QPushButton("Exit");
    QString buttonStyle =
        "QPushButton {"
        "background: rgba(255,255,255,0.10); border: 2px solid rgba(255,255,255,0.25);"
        "border-radius: 18px; color: white; font-size: 20px; font-weight: bold; padding: 10px 20px;}"
        "QPushButton:hover { background: rgba(255,255,255,0.18); border-color: rgba(255,255,255,0.45);}"
        "QPushButton:pressed { background: rgba(255,255,255,0.25); }";

    playAgainButton->setStyleSheet(buttonStyle);
    exitButton->setStyleSheet(buttonStyle);
    playAgainButton->setFixedHeight(48);
    exitButton->setFixedHeight(48);

    layout->addStretch();
    layout->addWidget(title);
    layout->addWidget(finalScoreLabel);
    layout->addWidget(playAgainButton);
    layout->addWidget(exitButton);
    layout->addStretch();

    pages->addWidget(endPage);

    connect(playAgainButton, &QPushButton::clicked, this, [=](){
        resetGame();
        pages->setCurrentWidget(gamePage);
    });

    connect(exitButton, &QPushButton::clicked, this, [=](){
        QApplication::quit();
    });
}

// ------------------------------------------------------------
// RESET GAME
// ------------------------------------------------------------
void GameWindow::resetGame()
{
    timeElapsed = 60;
    score = 0;

    for (int i = 0; i < 10; ++i)
        for (int j = 0; j < 10; ++j) {
            grid[i][j] = 0;
            gridColors[i * 10 + j] = Qt::white;
        }

    generateBlocks();

    timer->start(1000);
}

// ------------------------------------------------------------
// SHOW END MENU
// ------------------------------------------------------------
void GameWindow::showEndMenu()
{
    timer->stop();
    fallTimer->stop();

    finalScoreLabel->setText(QString("Final Score: %1").arg(score));
    pages->setCurrentWidget(endPage);
}

// ------------------------------------------------------------
// PAINT EVENT (Updated for Bigger Grid)
// ------------------------------------------------------------

void GameWindow::paintEvent(QPaintEvent *event)
{
    if (pages->currentWidget() != gamePage)
        return;

    Q_UNUSED(event);
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);

    // Background gradient
    QLinearGradient grad(0, 0, width(), height());
    grad.setColorAt(0, QColor("#0e0f2c"));
    grad.setColorAt(0.5, QColor("#1b164a"));
    grad.setColorAt(1, QColor("#2a1d69"));
    painter.fillRect(rect(), grad);

    // Radial glow
    QRadialGradient bgGlow(width()/2, height()/2, 300);
    bgGlow.setColorAt(0, QColor(255,255,255,40));
    bgGlow.setColorAt(1, Qt::transparent);
    painter.fillRect(rect(), bgGlow);

    int cell = 30;
    int gridSize = 10;
    int topMargin = 50; // for timer/score
    int ox = (width() - gridSize*cell)/2;
    int oy = topMargin + 50;

    // Grid panel
    QRect gridRect(ox - 10, oy - 10, gridSize*cell + 20, gridSize*cell + 20);
    painter.setBrush(QColor(0,0,0,0));
    painter.setPen(Qt::NoPen);
    painter.drawRoundedRect(gridRect, 20, 20);

    // Grid lines
    painter.setPen(QPen(QColor(255,255,255,220), 2));
    for (int i = 0; i <= gridSize; ++i) {
        painter.drawLine(ox, oy + i*cell, ox + gridSize*cell, oy + i*cell);
        painter.drawLine(ox + i*cell, oy, ox + i*cell, oy + gridSize*cell);
    }

    // Draw placed blocks
    for (int r = 0; r < gridSize; ++r) {
        for (int c = 0; c < gridSize; ++c) {
            if (grid[r][c]) {
                QRect rect(ox + c*cell + 1, oy + r*cell + 1, cell - 2, cell - 2);
                painter.setBrush(gridColors[r*gridSize + c]);
                painter.setPen(QPen(QColor(40,40,40),1));
                painter.drawRect(rect);
            }
        }
    }

    // Timer & score centered horizontally
    QFont f = painter.font();
    f.setBold(true);
    f.setPointSize(19);
    painter.setFont(f);

    painter.setPen(QColor("#4fe3c1"));
    painter.drawText(ox, 30, gridSize*cell, 30, Qt::AlignCenter, QString("🕒 Time: %1 s").arg(timeElapsed));

    painter.setPen(QColor("#5fa8ff"));
    painter.drawText(ox, 60, gridSize*cell, 30, Qt::AlignCenter, QString("🏆 Score: %1").arg(score));
}

// ------------------------------------------------------------
// GAME LOGIC
// ------------------------------------------------------------
void GameWindow::updateTimer()
{
    timeElapsed--;
    if (timeElapsed <= 0) {
        showEndMenu();
        return;
    }
    update();
}

void GameWindow::generateBlocks()
{
    // Clear existing blocks
    for (Block *b : blocks) {
        b->hide();
        b->deleteLater();
    }
    blocks.clear();

    QVector<QVector<QVector<int>>> shapes = {
        {{1,1},{1,0}}, {{0,1},{1,1}}, {{0,1},{0,1},{1,1}}, {{1,1},{0,1},{0,1}},
        {{1,1},{1,0},{1,0}}, {{1,0},{1,0},{1,1}}, {{1,1,1}}, {{1,1},{1,1}},
        {{0,1,0},{1,1,1}}, {{1,1,1},{1,1,1},{1,1,1}}, {{1,1,1},{1,1,1}}, {{1,0},{1,1}},
        {{1,1,1},{0,0,1}}, {{1,1,1},{1,0,0}}, {{1,1}}, {{0,1},{1,0}}, {{1,0},{0,1}}, {{1},{1},{1}}, {{1}}
    };

    QVector<QColor> colors = { Qt::blue, Qt::green, Qt::yellow, Qt::magenta, Qt::cyan, Qt::red };

    int spacing = 100; // <-- increase spacing between blocks
    int startX = (width() - 3*spacing)/2; // center all 3 blocks horizontally
    int yPos = 450; // below the grid 750

    for (int i = 0; i < 3; ++i) {
        Block *block = new Block(this);
        block->setShape(shapes[QRandomGenerator::global()->bounded(shapes.size())]);
        block->setColor(colors[QRandomGenerator::global()->bounded(colors.size())]);
        block->move(startX + i*spacing, yPos); // position with spacing
        block->setOriginalPos(block->pos());
        block->show();
        connect(block, &Block::released, this, &GameWindow::blockReleased);
        blocks.push_back(block);
    }
}

bool GameWindow::canPlaceBlock(Block* block, int row, int col)
{
    auto shape = block->getShape();
    if (row + shape.size() > 10 || col + shape[0].size() > 10)
        return false;

    for (int i = 0; i < shape.size(); ++i)
        for (int j = 0; j < shape[i].size(); ++j)
            if (shape[i][j] && grid[row + i][col + j])
                return false;

    return true;
}

void GameWindow::placeBlock(Block* block, int row, int col)
{
    auto shape = block->getShape();
    QColor color = block->getColor();

    for (int i = 0; i < shape.size(); ++i)
        for (int j = 0; j < shape[i].size(); ++j)
            if (shape[i][j]) {
                grid[row + i][col + j] = 1;
                gridColors[(row + i) * 10 + (col + j)] = color;
            }

    block->hide();
    score += 10;
    timeElapsed += 5;

    checkFullLines();
    update();

    bool allPlaced = true;
    for (Block *b : blocks)
        if (b->isVisible())
            allPlaced = false;

    if (allPlaced)
        generateBlocks();

    checkGameOver();
}

void GameWindow::snapBlockToGrid(Block* block)
{
    int cell = 30;
    int gridSize = 10;
    int topMargin = 50;
    int ox = (width() - gridSize*cell)/2;
    int oy = topMargin + 50;

    QPoint pos = block->pos();

    // Outside grid → reset
    if (pos.y() < oy - 20 || pos.x() < ox - 20 || pos.x() > ox + gridSize*cell || pos.y() > oy + gridSize*cell) {
        block->resetPosition();
        return;
    }

    int col = (pos.x() - ox + cell/2)/cell;
    int row = (pos.y() - oy + cell/2)/cell;

    if (row < 0 || col < 0 || row > 9 || col > 9)
    {
        block->resetPosition();
        return;
    }

    if (canPlaceBlock(block, row, col))
        placeBlock(block, row, col);
    else
        block->resetPosition();
}

void GameWindow::blockReleased(Block* block)
{
    snapBlockToGrid(block);
}

void GameWindow::checkFullLines()
{
    rowsToClear.clear();
    colsToClear.clear();

    for (int i = 0; i < 10; ++i) {
        bool full = true;
        for (int j = 0; j < 10; ++j)
            if (!grid[i][j]) full = false;
        if (full) rowsToClear.push_back(i);
    }

    for (int j = 0; j < 10; ++j) {
        bool full = true;
        for (int i = 0; i < 10; ++i)
            if (!grid[i][j]) full = false;
        if (full) colsToClear.push_back(j);
    }

    if (!rowsToClear.isEmpty() || !colsToClear.isEmpty())
        startFallingAnimation();
}

void GameWindow::startFallingAnimation()
{
    fallStep = 0;
    tempGrid = grid;
    tempColors = gridColors;

    fallTimer->start();
}

void GameWindow::performFallingStep()
{
    int maxStep = 6; // 5

    for (int r : rowsToClear)
        for (int j = 0; j < 10; ++j)
            tempColors[r * 10 + j].setAlpha(255 - fallStep*50);

    for (int c : colsToClear)
        for (int i = 0; i < 10; ++i)
            tempColors[i * 10 + c].setAlpha(255 - fallStep*50);

    fallStep++;
    update();

    if (fallStep >= maxStep) {
        for (int r : rowsToClear)
            for (int j = 0; j < 10; ++j) {
                grid[r][j] = 0;
                gridColors[r*10 + j] = Qt::white;
            }
        for (int c : colsToClear)
            for (int i = 0; i < 10; ++i) {
                grid[i][c] = 0;
                gridColors[i*10 + c] = Qt::white;
            }

        score += (rowsToClear.size() + colsToClear.size()) * 50;
        fallTimer->stop();
        update();
    }
}

void GameWindow::checkGameOver()
{
    bool anyBlockCanBePlaced = false;

    for (Block *block : blocks) {
        if (!block->isVisible()) continue;

        bool canPlaceThisBlock = false;
        for (int r = 0; r <= 10 - block->getShape().size(); ++r) {
            for (int c = 0; c <= 10 - block->getShape()[0].size(); ++c) {
                if (canPlaceBlock(block, r, c)) {
                    canPlaceThisBlock = true;
                    break;
                }
            }
            if (canPlaceThisBlock) break;
        }
        if (canPlaceThisBlock) {
            anyBlockCanBePlaced = true;
            break;
        }
    }

    if (!anyBlockCanBePlaced)
        showEndMenu();
}
