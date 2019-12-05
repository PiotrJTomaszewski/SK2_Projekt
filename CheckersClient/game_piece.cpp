#include "game_piece.h"

GamePiece::GamePiece(const QPixmap &man_pixmap, const QPixmap &king_pixmap, const COLOR piece_color, QGraphicsItem *parent) : QGraphicsPixmapItem(man_pixmap, parent) {
    this->setZValue(10);
    this->piece_type = MAN;
    this->man_pixmap = &man_pixmap;
    this->king_pixmap = &king_pixmap;
    this->piece_color = piece_color;
}

QPointF GamePiece::getPos() {
    return this->pos();
}

void GamePiece::setPieceType(PIECE_TYPE piece_type) {
    this->piece_type = piece_type;
    switch(piece_type) {
    case MAN: this->setPixmap(*man_pixmap); break;
    case KING: this->setPixmap(*king_pixmap); break;
    }
}

void GamePiece::promoteToKing() {
    this->piece_type = KING;
    this->setPixmap(*king_pixmap);
}

PIECE_TYPE GamePiece::getPieceType() {
    return this->piece_type;
}

COLOR GamePiece::getPieceColor() {
    return this->piece_color;
}

void GamePiece::setMoveable(bool should_be_moveable) {
    if (should_be_moveable)
        this->setFlag(QGraphicsItem::ItemIsMovable);
    else
        this->setFlags(this->flags() & (~QGraphicsItem::ItemIsMovable));
}
