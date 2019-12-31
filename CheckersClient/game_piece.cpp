#include "game_piece.h"

GamePiece::GamePiece(const QString man_pixmap, const QString king_pixmap, const GLOBAL::COLOR piece_color, QGraphicsItem *parent) : QGraphicsPixmapItem(man_pixmap, parent) {
    this->setZValue(10);
    this->piece_type = GLOBAL::MAN;
    this->piece_color = piece_color;
    this->setVisible(false);
    this->man_pixmap = QPixmap(man_pixmap);
    this->king_pixmap = QPixmap(king_pixmap);
}

void GamePiece::setPosition(QPointF position , int field) {
    this->setPos(position);
    this->field = field;
}

QPointF GamePiece::getPos() {
    return this->pos();
}

void GamePiece::setPieceType(GLOBAL::PIECE_TYPE piece_type) {
    this->piece_type = piece_type;
    switch(piece_type) {
    case GLOBAL::MAN: this->setPixmap(man_pixmap); break;
    case GLOBAL::KING: this->setPixmap(king_pixmap); break;
    case GLOBAL::NO_PIECE: break;
    }
}

void GamePiece::capture() {
    this->piece_type = GLOBAL::NO_PIECE;
    this->setVisible(false);
}

void GamePiece::promoteToKing() {
    this->piece_type = GLOBAL::KING;
    this->setPixmap(king_pixmap);
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

int GamePiece::getField() {
    return this->field;
}
