#ifndef GAME_H
#define GAME_H

#include <QWidget>
#include <QGraphicsView>
#include <QGraphicsTextItem>
#include <QtDebug>
#include <QtEvents>
#include <QGraphicsPixmapItem>
#include "game_piece.h"

#define NUMBER_OF_PIECES 12

class Game : public QGraphicsView {
    Q_OBJECT
public:
    explicit Game(QWidget *parent = nullptr);
    ~Game() override;
private:
    QGraphicsScene *scene;
    QGraphicsPixmapItem *board;
    GamePiece *light_pieces[NUMBER_OF_PIECES];
    GamePiece *dark_pieces[NUMBER_OF_PIECES];
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;
    void pickUpPiece(int mouse_x, int mouse_y);
    void movePickedUpPiece(int mouse_x, int mouse_y);
    void dropPickedUpPiece();
    GamePiece *picked_up_piece = nullptr;
signals:

public slots:
};

#endif // GAME_H
