#include <iostream>
#include "sqlite3.h"
#include <string>
#include <cstring>
#include <exception>


void createDB(const char* s) {
    sqlite3* db;
    int exit = 0;
    exit = sqlite3_open(s, &db);
    sqlite3_close(db);
}

void createTable(const char * s) {
    sqlite3*db;
    std::string create_command = "CREATE TABLE IF NOT EXISTS USERS( "
                                 "ID INTEGER PRIMARY KEY AUTOINCREMENT, "
                                 "LOGIN  TEXT NOT NULL, "
                                 "PASSWORD TEXT NOT NULL);";


    try {
        int exit = sqlite3_open(s, &db);
        char* msgError;
        exit = sqlite3_exec(db, create_command.c_str(), NULL, 0, &msgError);
        if (exit != SQLITE_OK) {
            std::cerr << "Error while creating a table USERS";
            sqlite3_free(msgError);
        } else {
            std::cout << "Table USERS created successfully" << std::endl;
        }
        sqlite3_close(db);
    } catch (const std::exception &e) {
        std::cerr << e.what();
    }

}

int main() {
    sqlite3* DB;
    const char* PATH_TO_DB = "/Users/bdmpea/Downloads/nonogram-main 2/client/users.db";
    createDB(PATH_TO_DB);
    createTable(PATH_TO_DB);
}
