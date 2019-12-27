#ifndef GAME_PIECE_H
#define GAME_PIECE_H

#include <QGraphicsPixmapItem>
#include <QDebug>
#include <QGraphicsSceneDragDropEvent>
#include "globals.h"

class GamePiece : public QGraphicsPixmapItem {
public:
    GamePiece(const QPixmap &man_pixmap, const QPixmap &king_pixmap, const GLOBAL::COLOR piece_color, QGraphicsItem *parent = nullptr);
    QPointF getPos();
    void setPieceType(GLOBAL::PIECE_TYPE piece_type);
    void promoteToKing();
    GLOBAL::PIECE_TYPE getPieceType();
    GLOBAL::COLOR getPieceColor();
    void setMoveable(bool should_be_moveable);
    void setField(int field);
    int getField();
//    void setPos(qreal x, qreal y);
private:
    int field;
    GLOBAL::PIECE_TYPE piece_type;
    GLOBAL::COLOR piece_color;
    const QPixmap *man_pixmap;
    const QPixmap *king_pixmap;
};

#endif // GAME_PIECE_H
