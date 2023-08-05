#ifndef GAME_FIELD_H
#define GAME_FIELD_H

#include <QWidget>

namespace Ui {
class game_field;
}

class game_field : public QWidget
{
    Q_OBJECT

public:
    explicit game_field(QWidget *parent = nullptr);
    ~game_field();

private:
    Ui::game_field *ui;
};

#endif // GAME_FIELD_H
