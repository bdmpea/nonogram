/*#include <iostream>
#include "sqlite3.h"

int main(int argc, char** argv)
{
    sqlite3* DB;
    std::string sql = "CREATE TABLE IF NOT EXISTS USERS_LOGIN_INFO("
                      "ID INT PRIMARY KEY     NOT NULL, "
                      "LOGIN TEXT KEY     NOT NULL, "
                      "PASSWORD TEXT KEY   NOT NULL );";
    int exit = 0;
    exit = sqlite3_open("users_login_info.db", &DB);
    char* messaggeError;
    exit = sqlite3_exec(DB, sql.c_str(), NULL, 0, &messaggeError);

    if (exit != SQLITE_OK) {
        std::cerr << "Error Create Table" << std::endl;
        sqlite3_free(messaggeError);
    }
    else
        std::cout << "Table created Successfully" << std::endl;
    sqlite3_close(DB);
    return (0);

}
*/
