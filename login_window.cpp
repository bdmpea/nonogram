#include "login_window.h"
#include "./ui_login_window.h"
#include <QMessageBox>
#include <QPixmap>
#include <string>
#include <iostream>
#include <QSql>
#include <QFileInfo>
#include <QSqlQuery>
#include "database.h"


login_window::login_window(DataBase *d, QWidget *parent)
    : login_db(d),
    QMainWindow(parent),
    ui(new Ui::login_window)
{
    setWindowTitle("Registration");
    ui->setupUi(this);
    QPixmap left_boarder_pic(":/resources/img/left_boarder.png");
    int left_boarder_width = ui->left_boarder->width();
    int left_boarder_height = ui->left_boarder->height();
    ui->left_boarder->setPixmap(left_boarder_pic.scaled(left_boarder_width, left_boarder_height, Qt::KeepAspectRatio));

    reg_window = new register_window(login_db);
    connect(reg_window, &::register_window::go_back, this, &login_window::show);
    connect(reg_window, &::register_window::successful_reg, this, &login_window::registration_status);

    timer = new QTimer();
    connect(timer, &::QTimer::timeout, this, &login_window::reg_message_timer_alarm);

}

login_window::~login_window()
{
    delete ui;
}


void login_window::on_pushButton_enter_clicked()
{
    QString login = ui->login->text();
    QString password = ui->password->text();

    bool correct_info = login_db->checkUserInfo(login, password);
    int user_ID = login_db->getUserId(login);

    if (correct_info) {
        this->hide();
        menu = new main_window(user_ID, login_db, this);
        menu->show();
    } else {
        qDebug() << login << " " << password;
        QMessageBox::warning(this, "failed", "Login or password is not right");
    }
}

void login_window::registration_status() {
    ui->registration_status->setText("Registration was successful");
    timer->start(1000);
}

void login_window::login_window::reg_message_timer_alarm() {
    ui->registration_status->setText("");
}


void login_window::on_pushButton_reg_clicked()
{
    reg_window->show();
}
