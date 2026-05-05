cmake_minimum_required(VERSION 3.16)
project(TaskScriptAnalyzer VERSION 1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_AUTOMOC ON)   # Necesario para las macros Q_OBJECT

# ── Buscar Qt (compatible con Qt5 y Qt6) ───────
find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Core Gui Widgets)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Core Gui Widgets)

add_executable(TaskScriptAnalyzer
    main_qt.cpp
    MainWindow.cpp
    MainWindow.h
    LexicalAnalyzer.cpp
    LexicalAnalyzer.h
    SyntaxAnalyzer.cpp
    SyntaxAnalyzer.h
    ReportGenerator.cpp
    ReportGenerator.h
    Token.h
    ErrorManager.h
    NodoArbol.h
)

target_link_libraries(TaskScriptAnalyzer PRIVATE
    Qt${QT_VERSION_MAJOR}::Core
    Qt${QT_VERSION_MAJOR}::Gui
    Qt${QT_VERSION_MAJOR}::Widgets
)
