#include "room.h"

Room::Room() : QListWidgetItem() {

}

Room::Room(int id, QString name) : QListWidgetItem() {
    this->id = id;
    this->name = name;
    QString displayable_name = QString::number(id) + ": " + name;
    setData(Qt::DisplayRole, displayable_name);
}

Room::Room(const Room &other_room) : QListWidgetItem() {
    this->id = other_room.id;
    this->name = other_room.name;
    QString displayable_name = QString::number(id) + ": " + name;
    setData(Qt::DisplayRole, displayable_name);
}

Room::~Room() {

}

Room &Room::operator=(const Room &other_room) {
    this->id = other_room.id;
    this->name = other_room.name;
    QString displayable_name = QString::number(id) + ": " + name;
    setData(Qt::DisplayRole, displayable_name);
    return *this;
}

int Room::getId() {
    return this->id;
}
