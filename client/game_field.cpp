#include "game_field.h"
#include "ui_game_field.h"

game_field::game_field(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::game_field)
{
    ui->setupUi(this);
}

game_field::~game_field()
{
    delete ui;
}
