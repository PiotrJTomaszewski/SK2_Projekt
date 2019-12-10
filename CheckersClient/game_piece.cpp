#include "game_piece.h"

GamePiece::GamePiece(const QPixmap &man_pixmap, const QPixmap &king_pixmap, const GLOBAL::COLOR piece_color, QGraphicsItem *parent) : QGraphicsPixmapItem(man_pixmap, parent) {
    this->setZValue(10);
    this->piece_type = GLOBAL::MAN;
    this->man_pixmap = &man_pixmap;
    this->king_pixmap = &king_pixmap;
    this->piece_color = piece_color;
    this->setVisible(false);
}

QPointF GamePiece::getPos() {
    return this->pos();
}

void GamePiece::setPieceType(GLOBAL::PIECE_TYPE piece_type) {
    this->piece_type = piece_type;
    switch(piece_type) {
    case GLOBAL::MAN: this->setPixmap(*man_pixmap); break;
    case GLOBAL::KING: this->setPixmap(*king_pixmap); break;
    }
}

void GamePiece::promoteToKing() {
    this->piece_type = GLOBAL::KING;
    this->setPixmap(*king_pixmap);
}

GLOBAL::PIECE_TYPE GamePiece::getPieceType() {
    return this->piece_type;
}

GLOBAL::COLOR GamePiece::getPieceColor() {
    return this->piece_color;
}

void GamePiece::setMoveable(bool should_be_moveable) {
    if (should_be_moveable)
        this->setFlag(QGraphicsItem::ItemIsMovable);
    else
        this->setFlags(this->flags() & (~QGraphicsItem::ItemIsMovable));
}

void GamePiece::setField(int field) {
    this->field = field;
}


int GamePiece::getField() {
    return this->field;
}
