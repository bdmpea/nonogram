#include "login_window.h"
#include "./ui_login_window.h"
#include <QMessageBox>
#include <QPixmap>
#include <string>
#include <iostream>
#include "users.hpp"


login_window::login_window(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::login_window)
{
    ui->setupUi(this);
    QPixmap left_boarder_pic(":/resources/img/left_boarder.png");
    int left_boarder_width = ui->left_boarder->width();
    int left_boarder_height = ui->left_boarder->height();
    ui->left_boarder->setPixmap(left_boarder_pic.scaled(left_boarder_width, left_boarder_height, Qt::KeepAspectRatio));
    reg_window = new register_window();
    menu = new main_window();
    connect(reg_window, &::register_window::firstWindow, this, &login_window::show);
    connect(reg_window, &::register_window::firstWindow, this, &login_window::registration_status);
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
    //user new_user(login, password, );

    if (login == "bdmpea" && password == "1234") {
        this->hide();
        menu->show();
        //new_user.add_to_db();
        //QMessageBox::information(this, "Ура", "success");
    } else {
        QMessageBox::warning(this, "эээ", "failed");
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
    this->hide();
    reg_window->show();

}
