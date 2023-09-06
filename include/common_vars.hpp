#ifndef COMMON_VARS_HPP
#define COMMON_VARS_HPP
#include <iostream>


static std::string DB_HOST_NAME = "db";
static std::string DB_NAME = "/Users/bdmpea/pixpix/users3.db";
static std::string TABLE = "USERS";

enum CELL_TYPE {
    UNKNOWN, FILLED, SCRATCHED
};


enum NONOGRAM_STATE {
    INCOMPLETE, SOLVED, CONFLICT, PAINTED
};



#endif // COMMON_VARS_HPP
