#include "database.h"

DataBase::DataBase(QObject *parent) : QObject(parent)
{

}

DataBase::~DataBase()
{

}

void DataBase::connectToDataBase()
{
    if(!QFile(QString::fromStdString(DB_NAME)).exists()){
        this->restoreDataBase();
    } else {
        this->openDataBase();
    }
}


bool DataBase::restoreDataBase()
{
    if(this->openDataBase()){
        if(!this->createTable()){
            return false;
        } else {
            return true;
        }
    }
    qDebug() << "Unable to restore DataBase" + DB_NAME;
    return false;
}

bool DataBase::openDataBase()
{
    db = QSqlDatabase::addDatabase("QSQLITE");
    db.setHostName(QString::fromStdString(DB_HOST_NAME));
    db.setDatabaseName(QString::fromStdString(DB_NAME));
    if(db.open()){
        return true;
    }
    return false;
}

void DataBase::closeDataBase()
{
    db.close();
}

bool DataBase::createTable()
{

    QSqlQuery query;
    if(!query.exec(QString::fromStdString("CREATE TABLE IF NOT EXISTS " + TABLE + "( "
                    "ID INTEGER PRIMARY KEY AUTOINCREMENT, "
                    "LOGIN  TEXT(50) NOT NULL, "
                    "PASSWORD  TEXT(50) NOT NULL, "
                    "LINERULES  TEXT NOT NULL DEFAULT '', "
                    "COLUMNRULES  TEXT NOT NULL DEFAULT '', "
                    "GAME TEXT NOT NULL DEFAULT '');"
                                           ))){
        qDebug() << "DataBase: error of create " << DB_NAME;
        qDebug() << query.lastError().text();
        return false;
    }
    return true;
}


bool DataBase::inserIntoTable(QString user_login, QString user_password)
{
    QSqlQuery query;
    if (!checkLoginUnique(user_login)) {
        return false;
    }

    if(!query.exec(QString::fromStdString("INSERT INTO " + TABLE +
                                           "(LOGIN, PASSWORD)"
                                           " VALUES(\'" + user_login.toStdString() + "\',\'" + user_password.toStdString() + "\');"
                                           ))){
        qDebug() << "error insert into " << TABLE;
        qDebug() << query.lastError().text();
        return false;
    }
    qDebug() << "Inserted" + user_login;
    return true;
}

bool DataBase::checkUserInfo(QString user_login, QString user_password) {
    QSqlQuery query;
    query.exec(QString::fromStdString("SELECT PASSWORD from " + TABLE + " WHERE LOGIN ='" + user_login.toStdString() + "'"));
    return (query.next() && query.value(0).toString() == user_password);
}

bool DataBase::checkLoginUnique(QString user_login) {
    QSqlQuery query;
        query.exec(QString::fromStdString("SELECT ID from " + TABLE + " WHERE LOGIN ='" + user_login.toStdString() + "'"));
    return (!query.next());
}

int DataBase::getUserId(QString user_login) {
    QSqlQuery query;

    query.exec(QString::fromStdString("SELECT ID from " + TABLE + " WHERE LOGIN ='" + user_login.toStdString() + "'"));
    query.next();
    int ans = query.value(0).toInt();
    return ans;
}

std::string DataBase::getUserLineRules(int ID) {
    QSqlQuery query;

    query.exec(QString::fromStdString("SELECT LINERULES from " + TABLE + " WHERE ID =" + std::to_string(ID)));
    query.next();
    return query.value(0).toString().toStdString();
}

std::string DataBase::getUserColumnRules(int ID) {
    QSqlQuery query;

    query.exec(QString::fromStdString("SELECT COLUMNRULES from " + TABLE + " WHERE ID =" + std::to_string(ID)));
    query.next();
    return query.value(0).toString().toStdString();
}

std::string getUserGame(int ID) {
    QSqlQuery query;
    query.exec(QString::fromStdString("SELECT GAME from " + TABLE + " WHERE ID =" + std::to_string(ID)));
    query.next();
    return query.value(0).toString().toStdString();
}

bool DataBase::updateUserGame(int ID, std::string curr_game) {
    QSqlQuery query;
    qDebug() << ID << curr_game;
    qDebug() << "UPDATE " + TABLE + " SET GAME = '" + curr_game + "' WHERE ID = '" + std::to_string(ID) + "'";
    if (!query.exec(QString::fromStdString("UPDATE " + TABLE + " SET GAME = '" + curr_game + "' WHERE ID = '" + std::to_string(ID) + "'"))) {
        qDebug() << query.lastError().text();
        return false;
    }
    return true;
}


void DataBase::insertXAxis(int ID, std::string xAxis) {
    QSqlQuery query;
    query.exec(QString::fromStdString("UPDATE " + TABLE + " SET LINERULES ='" + xAxis + "' WHERE ID ='" + std::to_string(ID) + "'"));
}

void DataBase::insertYAxis(int ID, std::string yAxis) {
    QSqlQuery query;
    query.exec(QString::fromStdString("UPDATE " + TABLE + " SET COLUMNRULES ='" + yAxis + "' WHERE ID ='" + std::to_string(ID) + "'"));
}


