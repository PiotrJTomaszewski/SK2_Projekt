#ifndef GAME_PIECE_H
#define GAME_PIECE_H

#include <QGraphicsPixmapItem>
#include <QDebug>
#include <QGraphicsSceneDragDropEvent>
#include "globals.h"

class GamePiece : public QGraphicsPixmapItem {
public:
    GamePiece(const QPixmap &man_pixmap, const QPixmap &king_pixmap, const COLOR piece_color, QGraphicsItem *parent = nullptr);
    QPointF getPos();
    void setPieceType(PIECE_TYPE piece_type);
    void promoteToKing();
    PIECE_TYPE getPieceType();
    COLOR getPieceColor();
    void setMoveable(bool should_be_moveable);
//    void setPos(qreal x, qreal y);
private:
    PIECE_TYPE piece_type;
    COLOR piece_color;
    const QPixmap *man_pixmap;
    const QPixmap *king_pixmap;
};

#endif // GAME_PIECE_H
