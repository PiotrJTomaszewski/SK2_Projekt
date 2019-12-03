#ifndef GAME_PIECE_H
#define GAME_PIECE_H

#include <QGraphicsPixmapItem>
#include <QDebug>
#include <QGraphicsSceneDragDropEvent>

class GamePiece : public QGraphicsPixmapItem {
public:
    GamePiece(const QPixmap &pixmap, QGraphicsItem *parent = nullptr);
//    void setPos(qreal x, qreal y);
private:
//    int width;
//    int height;
};

#endif // GAME_PIECE_H
