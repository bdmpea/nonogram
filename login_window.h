#ifndef LOGIN_WINDOW_H
#define LOGIN_WINDOW_H

#include <QMainWindow>
#include <QTimer>
#include <QTime>
#include "register_window.h"
#include "mainwindow.h"
#include <QSql>
#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlTableModel>
#include "common_vars.hpp"
#include "database.h"

QT_BEGIN_NAMESPACE
namespace Ui { class login_window; }
QT_END_NAMESPACE


class login_window : public QMainWindow
{
    Q_OBJECT

public:
    login_window(DataBase *d, QWidget *parent = nullptr);
    ~login_window();

private slots:
    void on_pushButton_enter_clicked();
    void registration_status();
    void reg_message_timer_alarm();
    void on_pushButton_reg_clicked();

signals:
    void reg_signal();
    void show_main_window();


private:
    Ui::login_window *ui;
    register_window *reg_window;
    main_window *menu;
    QTimer *timer;
    DataBase *login_db;
};
#endif // LOGIN_WINDOW_H
