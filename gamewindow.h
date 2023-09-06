#ifndef GAMEWINDOW_H
#define GAMEWINDOW_H

#include <QMainWindow>
#include <QString>
#include <QUrl>
#include <vector>
#include <QMenu>
#include <QMenuBar>
#include <QAction>
#include <QWidget>
#include <QApplication>
#include <QLabel>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QSpinBox>
#include <QMessageBox>
#include <QPushButton>
#include <QString>
#include <string>

#include "cellbutton.h"
#include "nonogram.hpp"
#include "database.h"


class gamewindow : public QMainWindow
{
    Q_OBJECT

public:
    gamewindow(int id, DataBase *d, QWidget *parent = nullptr);
    ~gamewindow();
    void generateSaved();

signals:
    void show_main_window();

private slots:
    void hint();
    void check_solve();
    void about();
    void generatePuzzle();
    void checkSolution();
    void scratched_clicked(int pos);
    void filled_clicked(int pos);


private:
    int user_ID;
    bool game_was_changed;
    QWidget *window;
    QVBoxLayout *layout;
    QHBoxLayout *top;
    QLabel *widthLabel;
    QLabel *heightLabel;
    QPushButton *generate;
    QPushButton *surrender;
    QGridLayout *grid;
    QSpinBox *widthBox;
    QSpinBox *heightBox;
    QWidget *game_field;
    nonogram *current_game;
    QMenu *fileMenu;
    QMenu *solveMenu;
    QMenu *aboutMenu;
    nonogram ngram;
    nonogram solved_ngram;

    DataBase *game_db;
    std::string string_game;
    std::vector<QPushButton*> puzzle;
    std::vector<QLabel*> xAxis, yAxis;
    std::vector<std::vector <int>> xAxisClue;
    std::vector<std::vector <int>> yAxisClue;
    std::vector<int> status;
    int width, height, mouseButton;
    bool firstClick, lockAction;

    QMenuBar *menu;
    QAction *quitAct;
    QAction *hintAct;
    QAction *solveAct;
    QAction *aboutAct;
    void cleanUp();
    void saveAllGame();
    void saveChangedGame(int pos, CELL_TYPE new_type);
};
#endif // GAMEWINDOW_H

