#include "game.h"
#include "mainwindow.h"
#include "serverconnectionobject.h"

Game::Game(QWidget *parent) : QGraphicsView(parent) {
    scene = new QGraphicsScene(this);
    this->setScene(scene);
    const QPixmap board_pixmap(":/textures/board.png");
    const QPixmap dark_man_pixmap(":/textures/dark_man.png");
    const QPixmap light_man_pixmap(":/textures/light_man.png");
    const QPixmap dark_king_pixmap(":/textures/dark_king.png");
    const QPixmap light_king_pixmap(":/textures/light_king.png");
    this->board = new QGraphicsPixmapItem(board_pixmap);
    this->scene->addItem(board);
    for (int i=0; i< ONE_COLOR_PIECES; ++i) {
        this->light_pieces[i] = new GamePiece(light_man_pixmap, light_king_pixmap, GLOBAL::LIGHT);
        this->scene->addItem(light_pieces[i]);
        this->dark_pieces[i]  = new GamePiece(dark_man_pixmap, dark_king_pixmap, GLOBAL::DARK);
        this->scene->addItem(dark_pieces[i]);
    }
    this->player_color = GLOBAL::LIGHT; // TODO: Get that from server
    // Set scene bounding rectangle so it won't be resized automatically
    int width = board_pixmap.width();
    int height = board_pixmap.height();
    this->scene->setSceneRect(0, 0, width, height);
    this->server_connection = ServerConnectionObject::getServerConnection();
    // Connect signals from server
    connect(this->server_connection, &TcpClient::startGameSignal, this, &Game::startGame);
}

Game::~Game() {
    delete this->board;
    for (int i=0; i< ONE_COLOR_PIECES; ++i) {
        delete this->light_pieces[i];
        delete this->dark_pieces[i];
    }
    delete this->scene;
}

void Game::startGame(GLOBAL::COLOR player_color) {
    qInfo("Starting game");
    this->player_color = player_color;
    placePiecesAtStart();
    for (int i=0; i < ONE_COLOR_PIECES; ++i) {
        // Show pieces
        this->light_pieces[i]->setVisible(true);
        this->dark_pieces[i]->setVisible(true);
        // Set pieces as moveable
        if (player_color == GLOBAL::LIGHT) {
            this->light_pieces[i]->setMoveable(true);
            this->dark_pieces[i]->setMoveable(false);
        }
        else {
            this->light_pieces[i]->setMoveable(false);
            this->dark_pieces[i]->setMoveable(true);
        }
    }
}

void Game::mousePressEvent(QMouseEvent *event) {
//    qInfo() << event->type();
    pickUpPiece(event->x(), event->y());
}

void Game::mouseMoveEvent(QMouseEvent *event) {
//    qInfo() << event->type();
    movePickedUpPiece(event->x(), event->y());
}


void Game::mouseReleaseEvent(QMouseEvent *event) {
//    qInfo() << event->type();
    dropPickedUpPiece(event->x(), event->y());
}

void Game::pickUpPiece(int mouse_x, int mouse_y) {
    QGraphicsItem *item = this->itemAt(mouse_x, mouse_y);
    if (item != nullptr)
        if ((item->flags() & QGraphicsItem::ItemIsMovable)) {
            this->picked_up_piece = dynamic_cast<GamePiece*>(item);
            this->picked_up_piece->setZValue(100); // The piece will be drawn above other elements
        }
}

void Game::movePickedUpPiece(int mouse_x, int mouse_y) {
    if (this->picked_up_piece != nullptr) {
         qreal x_mouse_offset = 53;
         qreal y_mouse_offset = 53;
        this->picked_up_piece->setPos(mouse_x-x_mouse_offset, mouse_y-y_mouse_offset);
    }
}

void Game::dropPickedUpPiece(int mouse_x, int mouse_y) {
    if (picked_up_piece != nullptr) {
        int from_field = picked_up_piece->getField();
        int to_field = getFieldNumber(mouse_x, mouse_y);
        if (to_field == -1) {
            picked_up_piece->setPos(getFieldPosition(from_field));
        }
        else {
            picked_up_piece->setPos(getFieldPosition(to_field));
            picked_up_piece->setField(to_field);
            this->server_connection->movePiece(from_field, to_field);
        }
        this->picked_up_piece->setZValue(10);
        this->picked_up_piece = nullptr;
    }
}

void Game::placePiecesAtStart() {
    int light_pieces_start = FIELDS - ONE_COLOR_PIECES;
    for (int i=0; i< ONE_COLOR_PIECES; ++i) {
        this->dark_pieces[i]->setPos(getFieldPosition(i));
        this->dark_pieces[i]->setField(i);
        this->light_pieces[i]->setPos(getFieldPosition(light_pieces_start+i));
        this->light_pieces[i]->setField(light_pieces_start+i);
    }
}

QPointF Game::getFieldPosition(int field) {
    if (this->player_color == GLOBAL::DARK)  // When the player is dark the board is turned upside down
        field = FIELDS - field - 1;
    int piece_row = field / FIELDS_IN_ROW;
    int piece_col = field % FIELDS_IN_ROW;
    int x_distance = 2*FIELD_SIZE; // There is unused white field between fields in rows
    int y_distance = FIELD_SIZE;
    int x_position = X_OFFSET + piece_col*x_distance;
    if (piece_row % 2 == 0) { // Even rows start with a white field
        x_position += FIELD_SIZE;
    }
    int y_position = Y_OFFSET + piece_row*y_distance;
    QPointF field_position(x_position, y_position);
    return field_position;
}

int Game::getFieldNumber(int x_position , int y_position) {
    // TODO: Change that offset
    y_position -= 2 * Y_OFFSET;
    x_position -= 2 * X_OFFSET;
    int row = y_position / FIELD_SIZE;
    int col = x_position / FIELD_SIZE;
    if (row % 2 == 0) {  // Even rows start with a white field
        col -= 1;
    }
    if (col % 2 == 0) {  // Collumns that here have an even index are black
        col = col / 2;
    }
    else {
        col = -1;
    }

    if (col < 0 || col > FIELDS_IN_ROW || row < 0 || row > ROWS) {  // Incorrect field
        return -1;
    }
    int field_number = row * FIELDS_IN_ROW + col;
    if (this->player_color == GLOBAL::DARK)  // When the player is dark the board is turned upside down
        field_number = FIELDS - field_number - 1;
    return field_number;
}

int Game::getFieldNumber(QPointF position) {
    return getFieldNumber(static_cast<int>(position.x()), static_cast<int>(position.y()));
}
