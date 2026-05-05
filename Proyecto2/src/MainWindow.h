#pragma once
#include <QMainWindow>
#include <QTabWidget>
#include <QTableWidget>
#include <QTextEdit>
#include <QPushButton>
#include <QLabel>
#include <QStatusBar>
#include <QSplitter>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QFileDialog>
#include <QMessageBox>
#include <QDesktopServices>
#include <QUrl>
#include <QString>
#include <QDir>
#include <string>
#include <vector>

#include "LexicalAnalyzer.h"
#include "SyntaxAnalyzer.h"
#include "ErrorManager.h"
#include "ReportGenerator.h"

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    explicit MainWindow(QWidget* parent = nullptr);
    ~MainWindow() = default;

private slots:
    void abrirArchivo();
    void analizarArchivo();
    void abrirReporteKanban();
    void abrirReporteResponsables();
    void abrirReporteErrores();
    void limpiarTodo();

private:
    // ── Construcción de la UI ─────────────────
    void construirUI();
    void construirToolbar();
    void construirPanelEditor();
    void construirPanelTokens();
    void construirPanelArbol();
    void construirPanelErrores();
    void construirPanelReportes();
    void aplicarEstilos();


    void poblarTablaTokens(const std::vector<Token>& tokens);
    void poblarTablaErrores(const ErrorManager& errMgr);
    void poblarArbol(const NodoPtr& raiz);
    void arbolToTexto(const NodoPtr& nodo, QString& out,
                      const QString& pref, bool esUltimo);
    void actualizarEstado(int tokens, int errores);

    QWidget*      centralWidget;
    QSplitter*    splitterPrincipal; 


    QWidget*      panelEditor;
    QTextEdit*    editorTexto;
    QLabel*       lblNombreArchivo;


    QTabWidget*   tabsResultado;
    QTableWidget* tablaTokens;
    QTextEdit*    vistaArbol;
    QTableWidget* tablaErrores;
    QWidget*      panelReportes;

    QPushButton*  btnAbrir;
    QPushButton*  btnAnalizar;
    QPushButton*  btnLimpiar;
    QPushButton*  btnKanban;
    QPushButton*  btnResponsables;
    QPushButton*  btnErroresHTML;

    QString       rutaArchivoActual;
    QString       carpetaReportes;
    bool          reportesGenerados = false;
};
