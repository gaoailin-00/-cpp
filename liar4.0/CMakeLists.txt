cmake_minimum_required(VERSION 3.16)

project(liar4 LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

find_package(Qt6 COMPONENTS Core Gui Widgets REQUIRED)

set(PROJECT_SOURCES
    main.cpp
    mainwindow.cpp
    mainwindow.h
    card.h
    card.cpp
    player_data.h
    player_data.cpp
    npc_data.h
    npc_data.cpp
)

add_executable(liar4 ${PROJECT_SOURCES})

target_link_libraries(liar4 PRIVATE Qt6::Core Qt6::Gui Qt6::Widgets)