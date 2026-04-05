cmake_minimum_required(VERSION 3.16)

project(MedLexerQt VERSION 0.1 LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt6 REQUIRED COMPONENTS Widgets)

# 👇 IMPORTANTE
include_directories(src)

set(PROJECT_SOURCES
    main.cpp
    mainwindow.cpp
    mainwindow.h
    mainwindow.ui

    src/AnalizadorLexico.cpp
    src/AnalizadorLexico.h
    src/DataModel.h
    src/ErrorManager.cpp
    src/ErrorManager.h
    src/Reporte.cpp
    src/Reporte.h
    src/Token.cpp
    src/Token.h
    src/Parser.cpp
    src/Parser.h
)

qt_add_executable(MedLexerQt
    ${PROJECT_SOURCES}
)

target_link_libraries(MedLexerQt PRIVATE Qt6::Widgets)

set_target_properties(MedLexerQt PROPERTIES
    WIN32_EXECUTABLE TRUE
)

qt_finalize_executable(MedLexerQt)