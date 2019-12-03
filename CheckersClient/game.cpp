#include "game.h"

Game::Game(QWidget *parent) : QGraphicsView(parent) {
    scene = new QGraphicsScene(this);
    this->setScene(scene);
    const QPixmap board_pixmap(":/textures/board.png");
    const QPixmap dark_man_pixmap(":/textures/dark_man.png");
    const QPixmap light_man_pixmap(":/textures/light_man.png");
    const QPixmap dark_king_pixmap(":/textures/dark_king.png");
    const QPixmap light_king_pixmap(":/textures/light_king.png");
    // TODO: Add pixmaps for kings and pass them as array
    this->board = new QGraphicsPixmapItem(board_pixmap);
    this->scene->addItem(board);
    for (int i=0; i<NUMBER_OF_PIECES; ++i) {
        this->light_pieces[i] = new GamePiece(light_man_pixmap);
        this->scene->addItem(light_pieces[i]);
        this->dark_pieces[i]  = new GamePiece(dark_man_pixmap);
        this->scene->addItem(dark_pieces[i]);
    }
    light_pieces[11]->setPixmap(light_king_pixmap);
    dark_pieces[11]->setPixmap(dark_king_pixmap);
    // Set scene bounding rectangle so it won't be resized automatically
    int width = board_pixmap.width();
    int height = board_pixmap.height();
    this->scene->setSceneRect(0, 0, width, height);
}

Game::~Game() {
    delete this->board;
    for (int i=0; i<NUMBER_OF_PIECES; ++i) {
        delete this->light_pieces[i];
        delete this->dark_pieces[i];
    }
    delete this->scene;
}

void Game::mousePressEvent(QMouseEvent *event)
{
//    qInfo() << event->type();
    pickUpPiece(event->x(), event->y());
}

void Game::mouseMoveEvent(QMouseEvent *event)
{
//    qInfo() << event->type();
    movePickedUpPiece(event->x(), event->y());
}


void Game::mouseReleaseEvent(QMouseEvent *event)
{
//    qInfo() << event->type();
    dropPickedUpPiece();
}

void Game::pickUpPiece(int mouse_x, int mouse_y)
{
    QGraphicsItem *item = this->itemAt(mouse_x, mouse_y);
    if (item != nullptr)
        if ((item->flags() & QGraphicsItem::ItemIsMovable))
        {
            this->picked_up_piece = dynamic_cast<GamePiece*>(item);
            this->picked_up_piece->setZValue(100); // The piece will be drawn above other elements
        }
}

void Game::movePickedUpPiece(int mouse_x, int mouse_y)
{
    if (this->picked_up_piece != nullptr)
    {
        // TODO: Calculate offset
        qreal x_offset = 50;
        qreal y_offset = 30;
        this->picked_up_piece->setPos(mouse_x-x_offset, mouse_y-y_offset);
    }
}

void Game::dropPickedUpPiece() {
    if (picked_up_piece != nullptr) {
        this->picked_up_piece->setZValue(10);
        this->picked_up_piece = nullptr;
    }
}

void Game::placePiecesAtStart() {

}
