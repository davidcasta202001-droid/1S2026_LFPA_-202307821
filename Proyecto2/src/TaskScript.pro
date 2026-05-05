QT += core gui widgets

CONFIG += c++17
TARGET  = TaskScriptAnalyzer
TEMPLATE = app

# Deshabilitar avisos de APIs obsoletas de Qt
DEFINES += QT_DEPRECATED_WARNINGS

SOURCES += \
    main_qt.cpp           \
    MainWindow.cpp        \
    LexicalAnalyzer.cpp   \
    SyntaxAnalyzer.cpp    \
    ReportGenerator.cpp

HEADERS += \
    MainWindow.h          \
    Token.h               \
    ErrorManager.h        \
    NodoArbol.h           \
    LexicalAnalyzer.h     \
    SyntaxAnalyzer.h      \
    ReportGenerator.h

# ── Configuración de release ───────────────────
CONFIG(release, debug|release) {
    QMAKE_CXXFLAGS += -O2
}

# ── Icono de la app (opcional) ─────────────────
# win32: RC_ICONS = resources/icon.ico
# mac:   ICON = resources/icon.icns
