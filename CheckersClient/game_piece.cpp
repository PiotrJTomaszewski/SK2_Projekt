#include "game_piece.h"

GamePiece::GamePiece(const QPixmap &man_pixmap, const QPixmap &king_pixmap, QGraphicsItem *parent) : QGraphicsPixmapItem(man_pixmap, parent) {
    this->setFlag(QGraphicsItem::ItemIsMovable);
    this->setZValue(10);
    this->piece_type = MAN;
    this->man_pixmap = &man_pixmap;
    this->king_pixmap = &king_pixmap;

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
