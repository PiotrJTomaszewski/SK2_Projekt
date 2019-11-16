#include "game_piece.h"

GamePiece::GamePiece(const QPixmap &pixmap, QGraphicsItem *parent) : QGraphicsPixmapItem(pixmap, parent)
{
    this->setFlag(QGraphicsItem::ItemIsMovable);
    this->setZValue(10);
}

//void GamePiece::setPos(qreal x, qreal y)
//{
//    // TODO: Calculate that move
//    QGraphicsPixmapItem::setPos(x, y);
//}
