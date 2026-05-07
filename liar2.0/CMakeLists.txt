cmake_minimum_required(VERSION 3.16)

project(liarliar LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

find_package(Qt6 COMPONENTS Core Gui Widgets REQUIRED)

set(PROJECT_SOURCES
    main.cpp
    mainwindow.cpp
    mainwindow.h
)

add_executable(liarliar ${PROJECT_SOURCES})

target_link_libraries(liarliar PRIVATE Qt6::Core Qt6::Gui Qt6::Widgets)