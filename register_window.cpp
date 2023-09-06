#include "register_window.h"
#include "ui_register_window.h"
#include <QMessageBox>
#include <QPixmap>


register_window::register_window(DataBase *r, QWidget *parent) :
    QDialog(parent),
    reg_db(r),
    ui(new Ui::register_window)
{
    ui->setupUi(this);
    QPixmap left_boarder_pic(":/resources/img/left_boarder.png");
    int left_boarder_width = ui->left_boarder->width();
    int left_boarder_height = ui->left_boarder->height();
    ui->left_boarder->setPixmap(left_boarder_pic.scaled(left_boarder_width, left_boarder_height, Qt::KeepAspectRatio));

}

register_window::~register_window()
{
    delete ui;

}


void register_window::on_pushButton_sing_up_clicked()
{
    QString login = ui->reg_login->text();
    QString password_first = ui->reg_password1->text();
    QString password_second = ui->reg_password2->text();
    if (reg_db->checkLoginUnique(login) && password_first == password_second && password_first.size() >= 6) {
        reg_db->inserIntoTable(login, password_first);
        this->close();
        emit successful_reg();
    } else if (!reg_db->checkLoginUnique(login)) {
        QMessageBox::warning(this, "failed", "Login is already in use");
    } else if (password_first != password_second) {
        QMessageBox::warning(this, "failed", "Passwords are not the same");
    }  else if (password_first.size() < 6) {
        QMessageBox::warning(this, "failed", "Pasword is too short");
    }
}



void register_window::on_pushButton_back_clicked()
{
    this->close();
    emit go_back();
}

