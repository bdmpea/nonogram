#ifndef MAIN_WINDOW_H
#define MAIN_WINDOW_H

#include "gamewindow.h"
#include <QDialog>
#include "database.h"

namespace Ui {
class main_window;
}

class main_window : public QDialog
{
    Q_OBJECT

public:
    explicit main_window(int id, DataBase *d, QWidget *parent = nullptr);
    ~main_window();


private slots:
    void on_new_game_button_clicked();

    void on_continue_button_clicked();

signals:
    void show_login_window();
    void show_opts_window();

private:
    int user_ID;
    Ui::main_window *ui;
    gamewindow *game;
    DataBase *main_db;
};

#endif // MAIN_WINDOW_H
