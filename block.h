#ifndef BLOCK_H
#define BLOCK_H

#include <QWidget>
#include <QVector>
#include <QColor>

class Block : public QWidget
{
    Q_OBJECT
public:
    explicit Block(QWidget *parent = nullptr);

    void setShape(const QVector<QVector<int>> &shape);
    QVector<QVector<int>> getShape() const;
    void resetPosition();
    void setOriginalPos(const QPoint &pos);
    void setColor(const QColor &color);
    QColor getColor() const;

signals:
    void released(Block* block);

protected:
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;
    void paintEvent(QPaintEvent *event) override;

private:
    QVector<QVector<int>> shape;
    QPoint dragStartPos;
    QPoint originalPos;
    bool dragging = false;
    QColor color;
};

#endif // BLOCK_H
