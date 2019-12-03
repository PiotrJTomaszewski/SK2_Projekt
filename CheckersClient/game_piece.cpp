#include "game_piece.h"

GamePiece::GamePiece(const QPixmap &pixmap, QGraphicsItem *parent) : QGraphicsPixmapItem(pixmap, parent) {
    this->setFlag(QGraphicsItem::ItemIsMovable);
    this->setZValue(10);
}

QPointF GamePiece::getPos() {
    return this->pos();
}

