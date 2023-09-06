#ifndef DATABASE_H
#define DATABASE_H

#include <QObject>
#include <QSql>
#include <QSqlQuery>
#include <QSqlError>
#include <QSqlDatabase>
#include <QFile>
#include <QDate>
#include <QDebug>
#include "nonogram.hpp"

class DataBase : public QObject
{
    Q_OBJECT

public:
    explicit DataBase(QObject *parent = 0);
    ~DataBase();
    void connectToDataBase();
    bool createTable();
    bool inserIntoTable(QString user_login, QString user_password);
    bool checkUserInfo(QString user_login, QString user_password);
    bool checkLoginUnique(QString user_login);
    int getUserId(QString login);
    bool updateUserGame(int ID, std::string curr_game);
    std::string getUserGame(int ID);
    bool openDataBase();
    bool restoreDataBase();
    void closeDataBase();
    void insertYAxis(int ID, std::string yAxis);
    void insertXAxis(int ID, std::string xAxis);
    std::string getUserLineRules(int ID);
    std::string getUserColumnRules(int ID);

private:
    QSqlDatabase db;

};

#endif // DATABASE_H
