#include "gamewindow.h"

gamewindow::gamewindow(int id, DataBase *d, QWidget *parent) :
    user_ID(id),
    game_db(d),
    QMainWindow(parent)
{
    setWindowTitle("Game");
    game_was_changed = false;
    window = new QWidget(this);
    game_field = new QWidget(this);
    menu = new QMenuBar(this);
    menu->setNativeMenuBar(false);

    quitAct = new QAction(tr("&Quit"), this);
    quitAct->setShortcut(tr("CTRL+Q"));
    quitAct->setStatusTip(tr("Close app"));
    connect(quitAct, &QAction::triggered, this, &QApplication::quit);

    hintAct = new QAction("&Hint", this);
    hintAct->setShortcut(tr("CTRL+H"));
    hintAct->setStatusTip(tr("Get a hint to puzzle"));
    connect(hintAct, &QAction::triggered, this, &gamewindow::hint);

    solveAct = new QAction("Solution", this);
    solveAct->setStatusTip(tr("Get the solution to puzzle"));
    connect(solveAct, &QAction::triggered, this, &gamewindow::check_solve);

    aboutAct = new QAction("About", this);
    aboutAct->setStatusTip(tr("Get info about rules"));
    connect(solveAct, &QAction::triggered, this, &gamewindow::about);


    fileMenu = menu->addMenu("&File");
    fileMenu->addAction(quitAct);

    solveMenu = menu->addMenu("&Solve");
    solveMenu->addAction(hintAct);
    solveMenu->addAction(solveAct);

    aboutMenu = menu->addMenu("About");
    aboutMenu->addAction(aboutAct);

    layout = new QVBoxLayout(window);
    top = new QHBoxLayout(window);
    top->setAlignment(Qt::AlignLeft);
    grid = new QGridLayout();
    grid->setSpacing(0);
    grid->setAlignment(Qt::AlignTop | Qt::AlignLeft);
    widthBox = new QSpinBox(window);
    heightBox = new QSpinBox(window);
    widthBox->setRange(5, 200);
    heightBox->setRange(5, 200);
    widthBox->setValue(10);
    heightBox->setValue(10);
    widthLabel = new QLabel(tr("Columns:"), window);
    heightLabel = new QLabel(tr("Rows:"), window);
    generate = new QPushButton(tr("Generate puzzle"), this);
    connect(generate, &QPushButton::clicked, this, &gamewindow::generatePuzzle);
    surrender = new QPushButton(tr("Give up"), this);
    connect(surrender, &QPushButton::clicked, this, &gamewindow::check_solve);
    surrender->setEnabled(false);
    lockAction = false;


    window->setLayout(layout);
    setCentralWidget(window);
    top->addWidget(heightLabel);
    top->addWidget(heightBox);
    top->addWidget(widthLabel);
    top->addWidget(widthBox);
    top->addWidget(generate);
    top->addWidget(surrender);
    layout->addSpacing(20);
    layout->addLayout(top);
    layout->addLayout(grid);


}

gamewindow::~gamewindow() {
}

void gamewindow::hint() {
    // do nothing
}


void gamewindow::saveAllGame() {
    if (!game_was_changed) {
        return;
    }
    game_db->updateUserGame(user_ID, string_game);
    game_was_changed = false;
}

void gamewindow::saveChangedGame(int pos, CELL_TYPE new_type) {
    if (!game_was_changed) {
        return;
    }
    int index_of_change = std::to_string(height).size() + std::to_string(width).size() + 2 + pos;
    string_game[index_of_change] = new_type == UNKNOWN ? '0' : (new_type == FILLED ? '1' : '2');
    game_db->updateUserGame(user_ID, string_game);
    game_was_changed = false;
}


void gamewindow::generatePuzzle() {
    int pos, spacer_x, spacer_y;
    widthBox->setEnabled(false);
    heightBox->setEnabled(false);
    generate->setEnabled(false);

    width = widthBox->value();
    height = heightBox->value();
    height = 8;
    width = 15;
    std::vector <std::vector <int>> rows(height);
    std::vector <std::vector <int>> columns(width);
    rows = { {13},
            {2, 1, 4},
            {1, 1, 3},
            {1, 1, 3},
            {2, 1, 3},
            {5, 3},
            {4},
            {3} };
    columns = { {4},
        {2, 2},
        {1, 1},
        {2, 2},
        {1, 2, 1},
        {1, 1},
        {1, 1},
        {1, 1},
        {1, 1},
        {1, 1},
        {1, 1},
        {2, 1, 1},
        {6, 1},
        {6},
        {4}
    };

    nonogram ex(rows, columns);
    ngram = ex;
    string_game = ngram.convert_field_into_str();
    game_db->insertXAxis(user_ID, ngram.convert_xAxis_into_str());
    game_db->insertYAxis(user_ID, ngram.convert_yAxis_into_str());
    game_was_changed = true;
    saveAllGame();
    xAxisClue = ngram.get_xAxisClue();
    yAxisClue = ngram.get_yAxisClue();

    nonogram ex2(rows, columns);
    solved_ngram = ex2;
    solved_ngram.propagate();
    spacer_x = 0;
    spacer_y = 0;
    for (int i = 0; i < width; ++i) {
        QString str(QLatin1String(""));
        QString n(QLatin1String(""));
        for (int j = 0; j < xAxisClue[i].size(); ++j) {
            n.setNum(xAxisClue[i][j], 10);
            str.append(n);
            if (j < xAxisClue[i].size() - 1) {
                str.append("\n");
            }
        }
        xAxis.push_back(new QLabel(str));
        xAxis.at(i)->setAlignment(Qt::AlignCenter | Qt::AlignBottom);
        if (i > 0 && i % 5 == 0) {
            ++spacer_x;
            grid->setColumnMinimumWidth(i + spacer_x, 2);
        }
        grid->addWidget(xAxis.at(i), 0, i + spacer_x + 1);
    }
    for (int i = 0; i < height; ++i) {
        QString str(QLatin1String(""));
        QString n = "";
        for (int j = 0; j < yAxisClue[i].size(); ++j) {
            n.setNum(yAxisClue[i][j], 10);
            str.append(n);
            str.append("  ");
        }
        yAxis.push_back(new QLabel(str));
        yAxis.at(i)->setAlignment(Qt::AlignRight | Qt::AlignVCenter);
        if (i > 0 && i % 5 == 0) {
            ++spacer_y;
            grid->setRowMinimumHeight(i + spacer_y, 2);
        }
        grid->addWidget(yAxis.at(i), i + spacer_y + 1, 0);
    }

    spacer_y = 0;
    for (int i = 0; i < height; ++i) {
        if (i > 0 && i % 5 == 0) {
            ++spacer_y;
        }
        spacer_x = 0;
        for (int j = 0; j < width; ++j) {
            if (j > 0 && j % 5 == 0) {
                ++spacer_x;
            }
            pos = i * width + j;
            status.push_back(UNKNOWN);
            puzzle.push_back(new cellbutton(&mouseButton, &firstClick, pos));
            connect(puzzle.at(pos), SIGNAL(filled(int)), this, SLOT(filled_clicked(int)));
            connect(puzzle.at(pos), SIGNAL(scratched(int)), this, SLOT(scratched_clicked(int)));
            connect(puzzle.at(pos), SIGNAL(released()), this, SLOT(checkSolution()));
            grid->addWidget(puzzle.at(pos), i + spacer_y + 1, j + spacer_x + 1);
        }
    }
    surrender->setEnabled(true);
}

void gamewindow::generateSaved() {
    std::string line_rules = game_db->getUserLineRules(user_ID);
    std::string column_rules = game_db->getUserColumnRules(user_ID);
    std::string game = game_db->getUserGame(user_ID);
    if (game.empty()) {
        QMessageBox::warning(this, "failed", "There is no saved game for you");
        return;
    }
    std::vector <std::vector <int>> rows;
    std::vector <std::vector <int>> columns;
    qDebug() << line_rules;
    qDebug() << column_rules;
}


void gamewindow::cleanUp() {
    xAxis.clear();
    yAxis.clear();
    generatePuzzle();
    puzzle.clear();
    status.clear();
}

void gamewindow::check_solve() {
    surrender->setEnabled(false);
    int pos;
    for (int i = 0; i < height; ++i) {
        for (int j = 0; j < width; ++j) {
            pos = i * width + j;
            if (status.at(pos) == FILLED && solved_ngram.rows_grid[i].s[j + 2] == SCRATCHED) {
                game_was_changed = true;
                puzzle[pos]->setStyleSheet(QString::fromUtf8("background-color: rgb(255, 0, 0)"));
            }
            else if (status.at(pos) == SCRATCHED && solved_ngram.rows_grid[i].s[j + 2] == FILLED) {
                game_was_changed = true;
                puzzle[pos]->setStyleSheet(QString::fromUtf8("background-color: rgb(150, 150, 150)"));

            } else if (solved_ngram.rows_grid[i].s[j + 2] == SCRATCHED) {
                game_was_changed = true;
                puzzle.at(pos)->setText(QString::fromUtf8("X"));
                puzzle[pos]->setStyleSheet(QString::fromUtf8("background-color: rgb(215, 215, 215)"));
            } else if (solved_ngram.rows_grid[i].s[j + 2] == FILLED) {
                game_was_changed = true;
                puzzle[pos]->setStyleSheet(QString::fromUtf8("background-color: rgb(50, 50, 50)"));
            }
        }
    }
    widthBox->setEnabled(true);
    heightBox->setEnabled(true);
    generate->setEnabled(true);
}

void gamewindow::filled_clicked(int position) {
    puzzle.at(position)->setText("");
    if (status.at(position) == FILLED) {
        if (firstClick) {
            game_was_changed = true;
            saveChangedGame(position, UNKNOWN);
            status.at(position) = UNKNOWN;
            puzzle[position]->setStyleSheet(QString::fromUtf8("background-color: rgb(215, 215, 215)"));
            lockAction = true;
        }
    }
    else {
        game_was_changed = true;
        saveChangedGame(position, FILLED);
        status.at(position) = FILLED;
        puzzle[position]->setStyleSheet(QString::fromUtf8("background-color: rgb(50, 50, 50)"));
    }
    firstClick = false;

}


void gamewindow::scratched_clicked(int position) {
    if (status.at(position) == SCRATCHED) {
        if (firstClick) {
            game_was_changed = true;
            saveChangedGame(position, UNKNOWN);
            status.at(position) = UNKNOWN;
            puzzle.at(position)->setText("");
            puzzle[position]->setStyleSheet(QString::fromUtf8("background-color: rgb(215, 215, 215)"));
            lockAction = true;
        }
    }
    else {
        game_was_changed = true;
        status.at(position) = SCRATCHED;
        saveChangedGame(position, SCRATCHED);
        puzzle.at(position)->setText(QString::fromUtf8("X"));
        puzzle[position]->setStyleSheet(QString::fromUtf8("background-color: rgb(215, 215, 215)"));
    }
    firstClick = false;
}

void gamewindow::checkSolution() {
    lockAction = false;
    std::vector <std::vector <CELL_TYPE>> answer_grid = solved_ngram.get_grid();
    for (int i = 0; i < height; ++i) {
        for (int j = 0; j < width; ++j) {
            if (status.at(i * width + j) != answer_grid[i][j]) {
                if (status.at(i * width + j) == UNKNOWN && answer_grid[i][j] == SCRATCHED) {
                    continue;
                } else {
                    return;
                }
            }
        }
    }
    for (int i = 0; i < width * height; ++i) {
        puzzle.at(i)->setEnabled(false);
    }
    QMessageBox mb;
    mb.setWindowTitle(tr("Well done"));
    mb.setText(tr("You have solved the puzzle!"));
    mb.exec();
    widthBox->setEnabled(true);
    heightBox->setEnabled(true);
    generate->setEnabled(true);
    surrender->setEnabled(false);
}


void gamewindow::about() {
    QWidget *helpWindow = new QWidget();
    helpWindow->setWindowTitle(tr("About"));
    QHBoxLayout *bl = new QHBoxLayout(helpWindow);
    QLabel *text = new QLabel(tr("<h2>What are nonograms?</h2>\
                                 <p>Nonograms are logic puzzles consisting of a rectangular grid divided into cells. These cells can be either<br>\
                                 colored (solids) or blank (dots). At the start of the game all cells are blank, and the purpose of the game is<br>\
                                 to figure out which ones should be colored.</p>\
                                 <p>Each row and column in the grid is fitted with a clue consisting of a series of numbers. These numbers reveal<br>\
                                 how many solids there are on the row/column as well as something about their grouping. If e.g. the clue is<br>\
                                 (3 1 2) we know that from left to right there is first a series of 3 consequtive solids, then X number of blanks,<br>\
                                 where X >= 1, then a single solid, then another unspecified number of blanks, and finally 2 consequtive solids.<br>\
                                 If the row was 10 cells long, one possible arrangement would be (-###-#--##), another one would be (###-#-##--).<br>\
                                 Since there are rules for both rows and columns, only one arrangement is actually correct, though. The puzzle is<br>\
                                 to find the arrangement that conforms to all the given clues.</p>\
                                 <h2>How to play</h2>\
                                 <p>In order to start a new game, please select the size of the desired playing field, and click the &quot;Generate puzzle&quot;<br>\
            button. Generating the puzzle can take a few seconds - especially if it's a big one, so please be patient.</p>\
            <p>When starting all cells are blank. You can mark a cell as a solid by clicking on it. You can also mark larger areas<br>\
            by dragging the mouse with the button pressed. If you've made a mistake, just click the cell again to revert it to a<br>\
            dot. You can also mark known dots by right clicking (and dragging). This will be shown by an 'X' appearing in that<br>\
            cell. Notice that you don't need to explicitly mark the dots in order to solve the puzzle. That functionality only<br>\
            exists for your own convenience.</p>\
            <p>Every time you make a move the computer will automatically check whether you've successfully solved the puzzle<br>\
            or not. Once the puzzle is solved, you will immediately be informed. If the puzzle turns out to be too hard, you can<br>\
            end it, and look at the solution by pressing the &quot;Give up&quot; button. Remaining solids will be displayed in<br>\
            grey, and possible mistakes (i.e. dots marked as solids) will be displayed in red.</p>"), helpWindow);
            helpWindow->setLayout(bl);
    bl->addWidget(text);
    helpWindow->show();
}



