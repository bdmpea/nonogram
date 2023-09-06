#include "login_window.h"

#include <QApplication>
#include <QLocale>
#include <QTranslator>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    DataBase *user_db;
    user_db = new DataBase();
    user_db->openDataBase();
    user_db->createTable();
    login_window w(user_db);
    w.setWindowTitle("Log in page");

    w.show();
    return a.exec();
}
