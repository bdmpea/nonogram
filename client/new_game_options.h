#ifndef NEW_GAME_OPTIONS_H
#define NEW_GAME_OPTIONS_H

#include <QDialog>
#include "game_field.h"

namespace Ui {
class new_game_options;
}

class new_game_options : public QDialog
{
    Q_OBJECT

public:
    explicit new_game_options(QWidget *parent = nullptr);
    ~new_game_options();

private slots:
    void on_pushButton_enter_clicked();

private:
    Ui::new_game_options *ui;
    game_field *field;
};

#endif // NEW_GAME_OPTIONS_H
