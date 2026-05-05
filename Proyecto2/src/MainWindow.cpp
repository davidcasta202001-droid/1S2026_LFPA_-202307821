#include "MainWindow.h"
#include <QApplication>
#include <QFont>
#include <QHeaderView>
#include <QFileInfo>
#include <QTextStream>
#include <QFile>
#include <QGroupBox>
#include <QFrame>
#include <QToolBar>
#include <QAction>
#include <QIcon>
#include <QSizePolicy>
#include <QScrollArea>
#include <sstream>

// ═════════════════════════════════════════════
//  Constructor
// ═════════════════════════════════════════════
MainWindow::MainWindow(QWidget* parent)
    : QMainWindow(parent)
{
    setWindowTitle("TaskScript Analyzer — LFP USAC 2026");
    setMinimumSize(1200, 750);
    resize(1400, 820);

    // Carpeta de reportes al lado del ejecutable
    carpetaReportes = QDir::currentPath() + "/reportes";
    QDir().mkpath(carpetaReportes);

    construirUI();
    aplicarEstilos();
}

// ═════════════════════════════════════════════
//  CONSTRUCCIÓN DE LA UI
// ═════════════════════════════════════════════
void MainWindow::construirUI() {
    construirToolbar();

    // Widget central con splitter horizontal
    centralWidget = new QWidget(this);
    setCentralWidget(centralWidget);

    auto* layoutPrincipal = new QHBoxLayout(centralWidget);
    layoutPrincipal->setContentsMargins(10, 10, 10, 10);
    layoutPrincipal->setSpacing(10);

    splitterPrincipal = new QSplitter(Qt::Horizontal, centralWidget);
    splitterPrincipal->setHandleWidth(6);
    layoutPrincipal->addWidget(splitterPrincipal);

    // ── Panel izquierdo: editor ───────────────
    construirPanelEditor();
    splitterPrincipal->addWidget(panelEditor);

    // ── Panel derecho: resultados ─────────────
    tabsResultado = new QTabWidget;
    tabsResultado->setObjectName("tabsResultado");
    tabsResultado->setTabPosition(QTabWidget::North);
    construirPanelTokens();
    construirPanelArbol();
    construirPanelErrores();
    construirPanelReportes();
    splitterPrincipal->addWidget(tabsResultado);

    // Proporción 40/60
    splitterPrincipal->setSizes({ 480, 720 });

    // Status bar
    statusBar()->showMessage("Listo — Abre un archivo .task para comenzar");
}

void MainWindow::construirToolbar() {
    auto* toolbar = addToolBar("Principal");
    toolbar->setObjectName("mainToolbar");
    toolbar->setMovable(false);
    toolbar->setIconSize(QSize(18, 18));
    toolbar->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);

    // Botón Abrir
    btnAbrir = new QPushButton("📂  Abrir archivo .task");
    btnAbrir->setObjectName("btnAbrir");
    btnAbrir->setFixedHeight(36);
    connect(btnAbrir, &QPushButton::clicked, this, &MainWindow::abrirArchivo);
    toolbar->addWidget(btnAbrir);

    toolbar->addSeparator();

    // Botón Analizar
    btnAnalizar = new QPushButton("▶  Analizar");
    btnAnalizar->setObjectName("btnAnalizar");
    btnAnalizar->setFixedHeight(36);
    btnAnalizar->setEnabled(false);
    connect(btnAnalizar, &QPushButton::clicked, this, &MainWindow::analizarArchivo);
    toolbar->addWidget(btnAnalizar);

    toolbar->addSeparator();

    // Botón Limpiar
    btnLimpiar = new QPushButton("🗑  Limpiar");
    btnLimpiar->setObjectName("btnLimpiar");
    btnLimpiar->setFixedHeight(36);
    connect(btnLimpiar, &QPushButton::clicked, this, &MainWindow::limpiarTodo);
    toolbar->addWidget(btnLimpiar);

    // Spacer
    auto* spacer = new QWidget;
    spacer->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);
    toolbar->addWidget(spacer);

    // Botones de reportes en la toolbar
    btnKanban = new QPushButton("📋  Kanban");
    btnKanban->setObjectName("btnReporte");
    btnKanban->setFixedHeight(36);
    btnKanban->setEnabled(false);
    connect(btnKanban, &QPushButton::clicked, this, &MainWindow::abrirReporteKanban);
    toolbar->addWidget(btnKanban);

    btnResponsables = new QPushButton("👥  Responsables");
    btnResponsables->setObjectName("btnReporte");
    btnResponsables->setFixedHeight(36);
    btnResponsables->setEnabled(false);
    connect(btnResponsables, &QPushButton::clicked, this, &MainWindow::abrirReporteResponsables);
    toolbar->addWidget(btnResponsables);

    btnErroresHTML = new QPushButton("⚠️  Errores HTML");
    btnErroresHTML->setObjectName("btnReporte");
    btnErroresHTML->setFixedHeight(36);
    btnErroresHTML->setEnabled(false);
    connect(btnErroresHTML, &QPushButton::clicked, this, &MainWindow::abrirReporteErrores);
    toolbar->addWidget(btnErroresHTML);
}

void MainWindow::construirPanelEditor() {
    panelEditor = new QWidget;
    auto* layout = new QVBoxLayout(panelEditor);
    layout->setContentsMargins(0, 0, 0, 0);
    layout->setSpacing(6);

    // Header del editor
    auto* headerEditor = new QWidget;
    headerEditor->setObjectName("headerEditor");
    auto* hLayout = new QHBoxLayout(headerEditor);
    hLayout->setContentsMargins(12, 8, 12, 8);

    auto* lblEditorTitulo = new QLabel("✏️  Editor de código fuente");
    lblEditorTitulo->setObjectName("lblSeccion");
    hLayout->addWidget(lblEditorTitulo);
    hLayout->addStretch();

    lblNombreArchivo = new QLabel("Sin archivo");
    lblNombreArchivo->setObjectName("lblArchivo");
    hLayout->addWidget(lblNombreArchivo);

    layout->addWidget(headerEditor);

    // Editor de texto
    editorTexto = new QTextEdit;
    editorTexto->setObjectName("editorTexto");
    editorTexto->setPlaceholderText(
        "Abre un archivo .task o escribe aquí directamente...\n\n"
        "Ejemplo:\n"
        "TABLERO \"Mi Proyecto\" {\n"
        "    COLUMNA \"Por Hacer\" {\n"
        "        tarea: \"Mi tarea\" [prioridad: ALTA,\n"
        "            responsable: \"Ana\", fecha_limite: 2026-05-01]\n"
        "    };\n"
        "};"
    );
    QFont fuenteMono("Consolas", 11);
    fuenteMono.setStyleHint(QFont::Monospace);
    editorTexto->setFont(fuenteMono);
    editorTexto->setLineWrapMode(QTextEdit::NoWrap);
    layout->addWidget(editorTexto);

    // Habilitar análisis cuando hay texto
    connect(editorTexto, &QTextEdit::textChanged, [this]() {
        btnAnalizar->setEnabled(!editorTexto->toPlainText().trimmed().isEmpty());
    });
}

void MainWindow::construirPanelTokens() {
    tablaTokens = new QTableWidget(0, 5);
    tablaTokens->setObjectName("tablaTokens");
    tablaTokens->setHorizontalHeaderLabels({ "No.", "Tipo", "Lexema", "Línea", "Columna" });
    tablaTokens->horizontalHeader()->setSectionResizeMode(2, QHeaderView::Stretch);
    tablaTokens->horizontalHeader()->setSectionResizeMode(1, QHeaderView::ResizeToContents);
    tablaTokens->setSelectionBehavior(QAbstractItemView::SelectRows);
    tablaTokens->setEditTriggers(QAbstractItemView::NoEditTriggers);
    tablaTokens->setAlternatingRowColors(true);
    tablaTokens->verticalHeader()->setVisible(false);
    tablaTokens->setShowGrid(false);
    tabsResultado->addTab(tablaTokens, "🔤  Tokens");
}

void MainWindow::construirPanelArbol() {
    vistaArbol = new QTextEdit;
    vistaArbol->setObjectName("vistaArbol");
    vistaArbol->setReadOnly(true);
    QFont fArbol("Consolas", 10);
    fArbol.setStyleHint(QFont::Monospace);
    vistaArbol->setFont(fArbol);
    tabsResultado->addTab(vistaArbol, "🌳  Árbol de Derivación");
}

void MainWindow::construirPanelErrores() {
    tablaErrores = new QTableWidget(0, 6);
    tablaErrores->setObjectName("tablaErrores");
    tablaErrores->setHorizontalHeaderLabels({ "No.", "Lexema", "Tipo", "Descripción", "Línea", "Col" });
    tablaErrores->horizontalHeader()->setSectionResizeMode(3, QHeaderView::Stretch);
    tablaErrores->horizontalHeader()->setSectionResizeMode(1, QHeaderView::ResizeToContents);
    tablaErrores->setSelectionBehavior(QAbstractItemView::SelectRows);
    tablaErrores->setEditTriggers(QAbstractItemView::NoEditTriggers);
    tablaErrores->setAlternatingRowColors(true);
    tablaErrores->verticalHeader()->setVisible(false);
    tablaErrores->setShowGrid(false);
    tabsResultado->addTab(tablaErrores, "⚠️  Errores (0)");
}

void MainWindow::construirPanelReportes() {
    panelReportes = new QWidget;
    auto* layout = new QVBoxLayout(panelReportes);
    layout->setContentsMargins(30, 30, 30, 30);
    layout->setSpacing(16);

    auto* lblTitulo = new QLabel("📊  Reportes Generados");
    lblTitulo->setObjectName("lblTituloReportes");
    layout->addWidget(lblTitulo);

    auto* lblDesc = new QLabel(
        "Después de analizar un archivo sin errores críticos, los reportes\n"
        "se generan automáticamente y se pueden abrir en el navegador.");
    lblDesc->setObjectName("lblDescReportes");
    lblDesc->setWordWrap(true);
    layout->addWidget(lblDesc);

    layout->addSpacing(10);

    // Tarjetas de reporte
    struct InfoReporte {
        QString icono, titulo, desc, boton;
        QPushButton** btn;
    };

    std::vector<InfoReporte> reportes = {
        { "📋", "Tablero Kanban Visual",
          "Vista tipo Trello con columnas, tarjetas y prioridades coloreadas.",
          "Abrir Reporte 1", &btnKanban },
        { "👥", "Carga por Responsable",
          "Distribución de tareas por persona con barras de progreso.",
          "Abrir Reporte 2", &btnResponsables },
        { "⚠️", "Tabla de Errores",
          "Errores léxicos y sintácticos con posición exacta.",
          "Abrir Reporte 3", &btnErroresHTML },
    };

    for (auto& r : reportes) {
        auto* card = new QFrame;
        card->setObjectName("cardReporte");
        auto* cLayout = new QHBoxLayout(card);
        cLayout->setContentsMargins(20, 16, 20, 16);

        auto* lblIcono = new QLabel(r.icono);
        lblIcono->setObjectName("iconoReporte");
        lblIcono->setFixedWidth(40);
        cLayout->addWidget(lblIcono);

        auto* textos = new QVBoxLayout;
        auto* lblT = new QLabel(r.titulo);
        lblT->setObjectName("tituloCard");
        auto* lblD = new QLabel(r.desc);
        lblD->setObjectName("descCard");
        textos->addWidget(lblT);
        textos->addWidget(lblD);
        cLayout->addLayout(textos, 1);

        auto* btn = new QPushButton(r.boton);
        btn->setObjectName("btnAbrirReporte");
        btn->setFixedWidth(140);
        btn->setEnabled(false);
        *r.btn = btn;   // actualiza el puntero de MainWindow también
        cLayout->addWidget(btn);
        layout->addWidget(card);
    }

    // Reconnectar signals después de recrear los botones
    connect(btnKanban,       &QPushButton::clicked, this, &MainWindow::abrirReporteKanban);
    connect(btnResponsables, &QPushButton::clicked, this, &MainWindow::abrirReporteResponsables);
    connect(btnErroresHTML,  &QPushButton::clicked, this, &MainWindow::abrirReporteErrores);

    layout->addStretch();

    auto* lblRuta = new QLabel("📁  Carpeta de salida: " + carpetaReportes);
    lblRuta->setObjectName("lblRuta");
    layout->addWidget(lblRuta);

    tabsResultado->addTab(panelReportes, "📊  Reportes");
}

// ═════════════════════════════════════════════
//  ESTILOS (QSS — equivalente a CSS en Qt)
// ═════════════════════════════════════════════
void MainWindow::aplicarEstilos() {
    qApp->setStyle("Fusion");

    setStyleSheet(R"(
/* ── Ventana general ──────────────────────── */
QMainWindow {
    background: #f0f2f5;
}

/* ── Toolbar ──────────────────────────────── */
QToolBar {
    background: #1a1a2e;
    border: none;
    padding: 6px 10px;
    spacing: 6px;
}
QToolBar::separator {
    background: rgba(255,255,255,0.15);
    width: 1px;
    margin: 6px 4px;
}

/* ── Botón Abrir ──────────────────────────── */
QPushButton#btnAbrir {
    background: #1565c0;
    color: white;
    border: none;
    border-radius: 6px;
    padding: 0 16px;
    font-size: 13px;
    font-weight: 600;
}
QPushButton#btnAbrir:hover  { background: #1976d2; }
QPushButton#btnAbrir:pressed{ background: #0d47a1; }

/* ── Botón Analizar ───────────────────────── */
QPushButton#btnAnalizar {
    background: #2e7d32;
    color: white;
    border: none;
    border-radius: 6px;
    padding: 0 16px;
    font-size: 13px;
    font-weight: 600;
}
QPushButton#btnAnalizar:hover   { background: #388e3c; }
QPushButton#btnAnalizar:pressed { background: #1b5e20; }
QPushButton#btnAnalizar:disabled{ background: #555; color: #999; }

/* ── Botón Limpiar ────────────────────────── */
QPushButton#btnLimpiar {
    background: #546e7a;
    color: white;
    border: none;
    border-radius: 6px;
    padding: 0 16px;
    font-size: 13px;
}
QPushButton#btnLimpiar:hover { background: #607d8b; }

/* ── Botones de reporte en toolbar ─────────── */
QPushButton#btnReporte {
    background: #37474f;
    color: #eceff1;
    border: none;
    border-radius: 6px;
    padding: 0 14px;
    font-size: 12px;
}
QPushButton#btnReporte:hover    { background: #455a64; }
QPushButton#btnReporte:disabled { background: #2a3439; color: #607d8b; }

/* ── Splitter ─────────────────────────────── */
QSplitter::handle {
    background: #cfd8dc;
    border-radius: 3px;
}

/* ── Header del editor ────────────────────── */
QWidget#headerEditor {
    background: #1a237e;
    border-radius: 8px 8px 0 0;
}
QLabel#lblSeccion {
    color: #fff;
    font-size: 13px;
    font-weight: 700;
}
QLabel#lblArchivo {
    color: #90caf9;
    font-size: 11px;
    font-style: italic;
}

/* ── Editor de código ─────────────────────── */
QTextEdit#editorTexto {
    background: #0d1117;
    color: #c9d1d9;
    border: none;
    border-radius: 0 0 8px 8px;
    padding: 12px;
    font-size: 12px;
    selection-background-color: #264f78;
}

/* ── Árbol de derivación ──────────────────── */
QTextEdit#vistaArbol {
    background: #0d1117;
    color: #c9d1d9;
    border-radius: 8px;
    padding: 12px;
    font-size: 11px;
}

/* ── Tabs ─────────────────────────────────── */
QTabWidget#tabsResultado::pane {
    border: none;
    background: #f0f2f5;
}
QTabWidget#tabsResultado > QTabBar::tab {
    background: #cfd8dc;
    color: #37474f;
    padding: 8px 18px;
    margin-right: 2px;
    border-radius: 6px 6px 0 0;
    font-size: 12px;
    font-weight: 500;
}
QTabWidget#tabsResultado > QTabBar::tab:selected {
    background: #1a237e;
    color: #fff;
    font-weight: 700;
}
QTabWidget#tabsResultado > QTabBar::tab:hover:!selected {
    background: #b0bec5;
}

/* ── Tablas ───────────────────────────────── */
QTableWidget {
    background: #fff;
    alternate-background-color: #f5f7fa;
    border: none;
    border-radius: 8px;
    font-size: 12px;
    gridline-color: transparent;
}
QTableWidget::item { padding: 6px 10px; }
QTableWidget::item:selected {
    background: #e3f2fd;
    color: #1a237e;
}
QHeaderView::section {
    background: #1a237e;
    color: #fff;
    padding: 8px 10px;
    border: none;
    font-size: 12px;
    font-weight: 600;
}

/* ── Panel reportes ───────────────────────── */
QLabel#lblTituloReportes {
    font-size: 18px;
    font-weight: 800;
    color: #1a1a2e;
}
QLabel#lblDescReportes {
    font-size: 13px;
    color: #546e7a;
    line-height: 1.5;
}
QFrame#cardReporte {
    background: #fff;
    border-radius: 10px;
    border: 1px solid #e0e0e0;
}
QFrame#cardReporte:hover {
    border-color: #90caf9;
}
QLabel#iconoReporte  { font-size: 26px; }
QLabel#tituloCard    { font-size: 14px; font-weight: 700; color: #1a1a2e; }
QLabel#descCard      { font-size: 12px; color: #78909c; }
QPushButton#btnAbrirReporte {
    background: #1565c0;
    color: white;
    border: none;
    border-radius: 6px;
    padding: 8px;
    font-size: 12px;
    font-weight: 600;
}
QPushButton#btnAbrirReporte:hover    { background: #1976d2; }
QPushButton#btnAbrirReporte:disabled { background: #b0bec5; color: #eceff1; }
QLabel#lblRuta {
    font-size: 11px;
    color: #90a4ae;
    font-style: italic;
}

/* ── Status bar ───────────────────────────── */
QStatusBar {
    background: #1a1a2e;
    color: #90caf9;
    font-size: 12px;
    padding: 4px 12px;
}
    )");
}

// ═════════════════════════════════════════════
//  SLOTS — Acciones del usuario
// ═════════════════════════════════════════════

void MainWindow::abrirArchivo() {
    QString ruta = QFileDialog::getOpenFileName(
        this,
        "Abrir archivo TaskScript",
        QDir::homePath(),
        "Archivos TaskScript (*.task);;Todos los archivos (*)"
    );
    if (ruta.isEmpty()) return;

    QFile file(ruta);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QMessageBox::critical(this, "Error", "No se pudo abrir el archivo:\n" + ruta);
        return;
    }
    QTextStream in(&file);
    in.setEncoding(QStringConverter::Utf8);
    editorTexto->setPlainText(in.readAll());
    file.close();

    rutaArchivoActual = ruta;
    QFileInfo fi(ruta);
    lblNombreArchivo->setText(fi.fileName());
    setWindowTitle("TaskScript Analyzer — " + fi.fileName());
    statusBar()->showMessage("Archivo cargado: " + fi.absoluteFilePath());
    btnAnalizar->setEnabled(true);
}

void MainWindow::analizarArchivo() {
    QString codigoFuente = editorTexto->toPlainText();
    if (codigoFuente.trimmed().isEmpty()) return;

    std::string fuente = codigoFuente.toStdString();

    // ── Análisis léxico ──────────────────────
    ErrorManager errMgr;
    LexicalAnalyzer lexer(fuente, &errMgr);
    auto tokens = lexer.tokenizarTodo();

    // ── Análisis sintáctico ──────────────────
    SyntaxAnalyzer parser(tokens, &errMgr);
    NodoPtr raiz = parser.parsear();

    // ── Poblar tablas ────────────────────────
    poblarTablaTokens(tokens);
    poblarTablaErrores(errMgr);
    poblarArbol(raiz);

    // ── Generar reportes HTML + DOT ──────────
    ReportGenerator gen(raiz, tokens, errMgr);
    bool ok = gen.generarTodos(carpetaReportes.toStdString());

    // ── Habilitar botones de reporte ─────────
    reportesGenerados = ok;
    btnKanban->setEnabled(ok);
    btnResponsables->setEnabled(ok);
    btnErroresHTML->setEnabled(ok);

    // Botones duplicados en panel reportes
    // (se habilitan automáticamente vía los mismos punteros)

    // ── Ir a la pestaña más relevante ────────
    if (errMgr.hayErrores())
        tabsResultado->setCurrentIndex(2); // errores
    else
        tabsResultado->setCurrentIndex(0); // tokens

    // ── Status bar ───────────────────────────
    int nTok = (int)tokens.size() - 1; // -1 por EOF
    int nErr = (int)errMgr.obtenerErrores().size();
    actualizarEstado(nTok, nErr);
}

void MainWindow::poblarTablaTokens(const std::vector<Token>& tokens) {
    tablaTokens->setRowCount(0);
    int fila = 0;
    for (const auto& tok : tokens) {
        if (tok.tipo == TokenType::FIN_ARCHIVO) break;
        tablaTokens->insertRow(fila);
        tablaTokens->setItem(fila, 0, new QTableWidgetItem(QString::number(fila + 1)));
        tablaTokens->setItem(fila, 1, new QTableWidgetItem(
            QString::fromStdString(tokenTypeToString(tok.tipo))));
        tablaTokens->setItem(fila, 2, new QTableWidgetItem(
            QString::fromStdString(tok.lexema)));
        tablaTokens->setItem(fila, 3, new QTableWidgetItem(
            QString::number(tok.linea)));
        tablaTokens->setItem(fila, 4, new QTableWidgetItem(
            QString::number(tok.columna)));

        // Colorear tokens de error en rojo claro
        if (tok.tipo == TokenType::ERROR) {
            for (int c = 0; c < 5; c++) {
                if (auto* it = tablaTokens->item(fila, c))
                    it->setBackground(QColor("#ffebee"));
            }
        }
        fila++;
    }
}

void MainWindow::poblarTablaErrores(const ErrorManager& errMgr) {
    tablaErrores->setRowCount(0);
    const auto& errores = errMgr.obtenerErrores();

    // Actualizar título de la pestaña con contador
    tabsResultado->setTabText(2,
        QString("⚠️  Errores (%1)").arg(errores.size()));

    int fila = 0;
    for (const auto& e : errores) {
        tablaErrores->insertRow(fila);
        tablaErrores->setItem(fila, 0, new QTableWidgetItem(QString::number(e.numero)));
        tablaErrores->setItem(fila, 1, new QTableWidgetItem(
            QString::fromStdString(e.lexema)));
        tablaErrores->setItem(fila, 2, new QTableWidgetItem(
            QString::fromStdString(e.tipoStr())));
        tablaErrores->setItem(fila, 3, new QTableWidgetItem(
            QString::fromStdString(e.descripcion)));
        tablaErrores->setItem(fila, 4, new QTableWidgetItem(
            QString::number(e.linea)));
        tablaErrores->setItem(fila, 5, new QTableWidgetItem(
            QString::number(e.columna)));

        // Color de fondo según tipo
        QColor bg = (e.tipo == TipoError::LEXICO)
                  ? QColor("#fff3e0") : QColor("#fce4ec");
        for (int c = 0; c < 6; c++) {
            if (auto* it = tablaErrores->item(fila, c))
                it->setBackground(bg);
        }
        fila++;
    }
}

void MainWindow::arbolToTexto(const NodoPtr& nodo, QString& out,
                               const QString& pref, bool esUltimo) {
    if (!nodo) return;
    out += pref + (esUltimo ? "└── " : "├── ");
    if (nodo->esTerminal)
        out += "[" + QString::fromStdString(nodo->etiqueta) + "]\n";
    else
        out += QString::fromStdString(nodo->etiqueta) + "\n";

    QString np = pref + (esUltimo ? "    " : "│   ");
    for (size_t i = 0; i < nodo->hijos.size(); i++)
        arbolToTexto(nodo->hijos[i], out, np, i == nodo->hijos.size() - 1);
}

void MainWindow::poblarArbol(const NodoPtr& raiz) {
    if (!raiz) {
        vistaArbol->setPlainText("(árbol vacío — el archivo tiene errores críticos)");
        return;
    }
    QString texto;
    arbolToTexto(raiz, texto, "", true);
    vistaArbol->setPlainText(texto);
}

void MainWindow::actualizarEstado(int tokens, int errores) {
    QString msg = QString("✅  Análisis completado — %1 token(s)").arg(tokens);
    if (errores > 0)
        msg += QString(" — ⚠️  %1 error(es) detectado(s)").arg(errores);
    else
        msg += " — Sin errores";
    if (reportesGenerados)
        msg += " — 📊 Reportes generados en: " + carpetaReportes;
    statusBar()->showMessage(msg);
}

void MainWindow::abrirReporteKanban() {
    QDesktopServices::openUrl(
        QUrl::fromLocalFile(carpetaReportes + "/reporte1_kanban.html"));
}

void MainWindow::abrirReporteResponsables() {
    QDesktopServices::openUrl(
        QUrl::fromLocalFile(carpetaReportes + "/reporte2_responsables.html"));
}

void MainWindow::abrirReporteErrores() {
    QDesktopServices::openUrl(
        QUrl::fromLocalFile(carpetaReportes + "/reporte3_errores.html"));
}

void MainWindow::limpiarTodo() {
    editorTexto->clear();
    tablaTokens->setRowCount(0);
    tablaErrores->setRowCount(0);
    vistaArbol->clear();
    tabsResultado->setTabText(2, "⚠️  Errores (0)");
    lblNombreArchivo->setText("Sin archivo");
    rutaArchivoActual.clear();
    reportesGenerados = false;
    btnAnalizar->setEnabled(false);
    btnKanban->setEnabled(false);
    btnResponsables->setEnabled(false);
    btnErroresHTML->setEnabled(false);
    setWindowTitle("TaskScript Analyzer — LFP USAC 2026");
    statusBar()->showMessage("Listo — Abre un archivo .task para comenzar");
}
