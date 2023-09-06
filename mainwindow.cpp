#include "mainwindow.h"
#include "ui_main_window.h"
#include <QPixmap>

main_window::main_window(int id, DataBase *d, QWidget *parent) :
    user_ID(id),
    QDialog(parent),
    main_db(d),
    ui(new Ui::main_window)
{
    ui->setupUi(this);
    setWindowTitle("Game options");
    QPixmap left_boarder_pic(":/resources/img/left_boarder.png");
    int left_boarder_width = ui->left_boarder->width();
    int left_boarder_height = ui->left_boarder->height();
    ui->left_boarder->setPixmap(left_boarder_pic.scaled(left_boarder_width, left_boarder_height, Qt::KeepAspectRatio));
    game = new gamewindow(user_ID, main_db, this);
}

main_window::~main_window()
{
    delete ui;
}


void main_window::on_new_game_button_clicked()
{
    game->show();
    this->hide();
}


void main_window::on_continue_button_clicked()
{
    game->generateSaved();
    this->hide();
}

