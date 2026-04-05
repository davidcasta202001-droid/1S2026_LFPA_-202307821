#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <vector>
#include "src/Token.h"
#include "src/ErrorManager.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow() override;

private slots:
    void on_chargerButton_clicked();

    void on_analizarButton_clicked();

    void on_tableWidget_cellActivated(int row, int column);

    void on_reportesButton_clicked();

    void on_tableErrores_cellActivated(int row, int column);

private:
    Ui::MainWindow *ui;
    void llenarTablaTokens(std::vector<Token>& tokens);
    void llenarTablaErrores(const std::vector<Error>& errors);
};
#endif // MAINWINDOW_H
