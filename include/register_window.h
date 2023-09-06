#ifndef REGISTER_WINDOW_H
#define REGISTER_WINDOW_H

#include <QDialog>
#include "common_vars.hpp"
#include "database.h"

namespace Ui {
class register_window;
}

class register_window : public QDialog
{
    Q_OBJECT

public:
    explicit register_window(DataBase *r, QWidget *parent = nullptr);
    ~register_window();

private slots:
    void on_pushButton_sing_up_clicked();
    void on_pushButton_back_clicked();

signals:
    void go_back();
    void successful_reg();

private:
    Ui::register_window *ui;
    DataBase *reg_db;
};

#endif // REGISTER_WINDOW_H
