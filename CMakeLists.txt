cmake_minimum_required(VERSION 3.5)

project(pixpix VERSION 0.1 LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package (SQLite3)
find_package(Qt6Core REQUIRED)
find_package(Qt6Widgets REQUIRED)
find_package(Qt6Quick REQUIRED)
find_package(Qt6Sql REQUIRED)

include_directories(${SQLITE3_INCLUDE_DIRS})


set(PROJECT_SOURCES
    main.cpp
    login_window.cpp
    login_window.h
    login_window.ui
    register_window.cpp
    register_window.h
    register_window.ui
    mainwindow.cpp
    mainwindow.h
    main_window.ui
    gamewindow.h
    gamewindow.cpp
    cellbutton.cpp
    cellbutton.h
    nonogram.hpp
    nonogram.cpp
    include.qrc
    database.h
    database.cpp
    common_vars.hpp
)

add_executable(pixpix ${PROJECT_SOURCES}
    main.cpp
)
target_link_libraries(pixpix PRIVATE Qt6::Core Qt6::Widgets Qt6::Quick Qt6::Sql)
target_link_libraries(pixpix PRIVATE SQLite3)

