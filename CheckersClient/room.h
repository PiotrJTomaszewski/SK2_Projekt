#ifndef ROOM_H
#define ROOM_H
#include <QListWidgetItem>

class Room : public QListWidgetItem {
public:
    Room();
    Room(int id, QString name);
    Room(const Room &other_room);
    ~Room();
    Room &operator=(const Room &other_room);
    QString text();
    int getId();
private:
    int id;
    QString name;
};

#endif // ROOM_H
