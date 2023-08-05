#include "register_window.h"
#include "ui_register_window.h"
#include <QMessageBox>
#include <QPixmap>

register_window::register_window(QWidget *parent) :
    QDialog(parent),
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
    if (login == "new_user" && password_first == "0000" && password_first == password_second) {
        //QMessageBox::information(this, "Ура", "success");
        this->close();
        emit firstWindow();
    } else {
        QMessageBox::warning(this, "эээ", "failed");
    }
}





void register_window::on_pushButton_back_clicked()
{
    this->close();
    emit firstWindow();
}

