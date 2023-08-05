#include "main_window.h"
#include "ui_main_window.h"
#include <QPixmap>

main_window::main_window(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::main_window)
{
    ui->setupUi(this);
    QPixmap left_boarder_pic(":/resources/img/left_boarder.png");
    int left_boarder_width = ui->left_boarder->width();
    int left_boarder_height = ui->left_boarder->height();
    ui->left_boarder->setPixmap(left_boarder_pic.scaled(left_boarder_width, left_boarder_height, Qt::KeepAspectRatio));
    new_game = new new_game_options(this);
}

main_window::~main_window()
{
    delete ui;
}


void main_window::on_pushButton_2_clicked()
{
    this->hide();
    new_game->show();
    //this->close();
}

