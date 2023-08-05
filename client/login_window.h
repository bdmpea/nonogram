#ifndef LOGIN_WINDOW_H
#define LOGIN_WINDOW_H

#include <QMainWindow>
#include <QTimer>
#include <QTime>
#include "register_window.h"
#include "main_window.h"
#include <QSql>


QT_BEGIN_NAMESPACE
namespace Ui { class login_window; }
QT_END_NAMESPACE

class login_window : public QMainWindow
{
    Q_OBJECT

public:
    login_window(QWidget *parent = nullptr);
    ~login_window();

private slots:
    void on_pushButton_enter_clicked();
    void registration_status();
    void reg_message_timer_alarm();
    void on_pushButton_reg_clicked();

signals:
    void reg_signal();


private:
    Ui::login_window *ui;
    register_window *reg_window;
    main_window *menu;
    QTimer *timer;
};
#endif // LOGIN_WINDOW_H
