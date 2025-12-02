#include "block.h"
#include <QPainter>
#include <QMouseEvent>

Block::Block(QWidget *parent) : QWidget(parent)
{
    setFixedSize(90, 90); // max size for 3x3
}

void Block::setShape(const QVector<QVector<int>> &s)
{
    shape = s;
}

QVector<QVector<int>> Block::getShape() const
{
    return shape;
}

void Block::setOriginalPos(const QPoint &pos)
{
    originalPos = pos;
}

void Block::resetPosition()
{
    move(originalPos);
}

void Block::setColor(const QColor &c)
{
    color = c;
}

QColor Block::getColor() const
{
    return color;
}

void Block::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton) {
        dragging = true;
        dragStartPos = event->pos();
        raise();
    }
}

void Block::mouseMoveEvent(QMouseEvent *event)
{
    if (dragging) {
        move(mapToParent(event->pos() - dragStartPos));
    }
}

void Block::mouseReleaseEvent(QMouseEvent *event)
{
    Q_UNUSED(event);
    if (dragging) {
        dragging = false;
        emit released(this);
    }
}

void Block::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);
    painter.setBrush(color);
    painter.setPen(Qt::black);

    int cellSize = 30; // match the GameWindow grid cell size!

    for (int i = 0; i < shape.size(); ++i) {
        for (int j = 0; j < shape[i].size(); ++j) {
            if (shape[i][j] == 1) {
                painter.drawRect(j * cellSize, i * cellSize, cellSize, cellSize);
            }
        }
    }

    // Resize the widget itself to fit the shape
    int w = shape[0].size() * cellSize;
    int h = shape.size() * cellSize;
    setFixedSize(w, h);
}
