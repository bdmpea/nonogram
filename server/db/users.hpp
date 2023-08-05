//#include <iostream>
/*#include <string>
#include <QMessageBox>
#include "sqlite3.h"


struct user {
    int id;
    QString login;
    QString password;
    sqlite3* DB;

    user(QString in_login, QString in_password, sqlite3* db) :
                            login(std::move(in_login)),
                            password(std::move(in_password)),
                            DB(db) {}


    void add_to_db() {
        char* error;
        std::string sql("INSERT INTO USERS_LOGIN_INFO VALUES(" + std::to_string(id) + "'" + login.toStdString() +
                        "', '" + password.toStdString() + "');");
        int exit = sqlite3_exec(DB, sql.c_str(), NULL, 0, &error);
        if (exit != SQLITE_OK) {
            std::cerr << "Error Insert" << std::endl;
            sqlite3_free(error);
        }
        sqlite3_close(DB);
    }
};
*/
