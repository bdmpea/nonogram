cmake_minimum_required(VERSION 3.5)

project(pic)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_PREFIX_PATH "/Users/bdmpea/Qt/6.5.1/macos/lib/cmake")

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
        main_window.cpp
        main_window.h
        main_window.ui
        new_game_options.cpp
        new_game_options.h
        new_game_options.ui
        game_field.cpp
        game_field.h
        game_field.ui
        include.qrc
        )
add_executable(pic ${PROJECT_SOURCES})
target_link_libraries(pic PRIVATE Qt6::Core Qt6::Widgets Qt6::Quick Qt6::Sql)
target_link_libraries(pic PRIVATE SQLite3)

