#ifndef MAIN_WINDOW_H
#define MAIN_WINDOW_H

#include <QDialog>
#include "new_game_options.h"

namespace Ui {
class main_window;
}

class main_window : public QDialog
{
    Q_OBJECT

public:
    explicit main_window(QWidget *parent = nullptr);
    ~main_window();


private slots:
    void on_pushButton_2_clicked();

private:
    Ui::main_window *ui;
    new_game_options *new_game;
};

#endif // MAIN_WINDOW_H
