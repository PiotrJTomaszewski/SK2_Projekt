#ifndef GAME_H
#define GAME_H

#include <QWidget>
#include <QGraphicsView>
#include <QGraphicsTextItem>
#include <QtDebug>
#include <QtEvents>
#include <QGraphicsPixmapItem>
#include "game_piece.h"
#include "globals.h"
#include "tcpClient.h"

#define ONE_COLOR_PIECES 12
#define FIELDS 32
#define FIELDS_IN_ROW 4
#define ROWS 8
#define X_OFFSET 23
#define Y_OFFSET 22
#define FIELD_SIZE 77

class Game : public QGraphicsView {
    Q_OBJECT
public:
    explicit Game(QWidget *parent = nullptr);
    ~Game() override;
    void startGame(GLOBAL::COLOR player_color);

private:
    TcpClient *server_connection;
    GLOBAL::COLOR player_color;
    QGraphicsScene *scene;
    QGraphicsPixmapItem *board;
    GamePiece *light_pieces[ONE_COLOR_PIECES];
    GamePiece *dark_pieces[ONE_COLOR_PIECES];
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;
    void pickUpPiece(int mouse_x, int mouse_y);
    void movePickedUpPiece(int mouse_x, int mouse_y);
    void dropPickedUpPiece(int mouse_x, int mouse_y);
    GamePiece *picked_up_piece = nullptr;
    void placePiecesAtStart();
    QPointF getFieldPosition(int field);
    int getFieldNumber(int x_position , int y_position);
    int getFieldNumber(QPointF position);
signals:

public slots:
};

#endif // GAME_H
