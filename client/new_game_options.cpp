#include "new_game_options.h"
#include "ui_new_game_options.h"
#include <QPixmap>

new_game_options::new_game_options(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::new_game_options)
{
    ui->setupUi(this);
    QPixmap left_boarder_pic(":/resources/img/left_boarder.png");
    int left_boarder_width = ui->left_boarder->width();
    int left_boarder_height = ui->left_boarder->height();
    ui->left_boarder->setPixmap(left_boarder_pic.scaled(left_boarder_width, left_boarder_height, Qt::KeepAspectRatio));
    field = new game_field(this);
}

new_game_options::~new_game_options()
{
    delete ui;
}

void new_game_options::on_pushButton_enter_clicked()
{
    this->hide();
    field->show();
}

