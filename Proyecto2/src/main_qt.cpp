#include <QApplication>
#include <QFont>
#include "MainWindow.h"

int main(int argc, char* argv[]) {
    QApplication app(argc, argv);

    // Fuente global limpia
    QFont fuenteApp("Segoe UI", 10);
    app.setFont(fuenteApp);

    MainWindow ventana;
    ventana.show();

    return app.exec();
}
