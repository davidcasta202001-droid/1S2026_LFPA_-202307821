#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include <QFileDialog>
#include <QDesktopServices>
#include <QUrl>
#include <vector>
#include <fstream>
#include <sstream>
#include "Reporte.h"
#include "AnalizadorLexico.h"
#include "Parser.h"
#include "ErrorManager.h"
#include "Token.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_chargerButton_clicked()
{
    QString fileName = QFileDialog::getOpenFileName(this, "Abrir archivo", "", "*.med");

    std::ifstream file(fileName.toStdString());
    std::stringstream buffer;
    buffer << file.rdbuf();

    ui->textEdit->setText(QString::fromStdString(buffer.str()));
}



void MainWindow::on_analizarButton_clicked()
{
    std::string input = ui->textEdit->toPlainText().toStdString();

    LexicalAnalyzer lexer(input);

    std::vector<Token> tokens;
    Token t = lexer.nextToken();

    while (t.type != TokenType::END) {
        tokens.push_back(t);
        t = lexer.nextToken();
    }

    llenarTablaTokens(tokens);
    llenarTablaErrores(lexer.getErrores());

    // 🔥 PARSER
    Parser parser(tokens);
    parser.parse();

    // 🔥 REPORTES
    ReportGenerator rep;
    rep.generarTodo(
        parser.pacientes,
        parser.medicos,
        parser.citas,
        parser.diagnosticos
        );
}

void MainWindow::llenarTablaErrores(const std::vector<Error>& errors) {

    ui->tableErrores->setRowCount(errors.size());

    for (int i = 0; i < errors.size(); i++) {

        ui->tableErrores->setItem(i, 0,
                                new QTableWidgetItem(QString::fromStdString(errors[i].lexeme)));

        ui->tableErrores->setItem(i, 1,
                                new QTableWidgetItem(QString::fromStdString(errors[i].description)));

        ui->tableErrores->setItem(i, 2,
                                new QTableWidgetItem(QString::number(errors[i].line)));

        ui->tableErrores->setItem(i, 3,
                                new QTableWidgetItem(QString::number(errors[i].column)));
    }
}
void MainWindow::llenarTablaTokens(std::vector<Token>& tokens) {

    ui->tableWidget->setRowCount(tokens.size());

    for (int i = 0; i < tokens.size(); i++) {

        ui->tableWidget->setItem(i, 0,
                               new QTableWidgetItem(QString::fromStdString(tokens[i].lexeme)));

        ui->tableWidget->setItem(i, 1,
                               new QTableWidgetItem(QString::fromStdString(tokenTypeToString(tokens[i].type))));

        ui->tableWidget->setItem(i, 2,
                               new QTableWidgetItem(QString::number(tokens[i].line)));

        ui->tableWidget->setItem(i, 3,
                               new QTableWidgetItem(QString::number(tokens[i].column)));
    }
}



void MainWindow::on_reportesButton_clicked()
{
    QDesktopServices::openUrl(
        QUrl::fromLocalFile("../reports/pacientes.html")
        );
}




void MainWindow::on_tableWidget_cellActivated(int row, int column)
{

}


void MainWindow::on_tableErrores_cellActivated(int row, int column)
{

}

