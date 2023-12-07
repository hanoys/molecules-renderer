#include "window.h"

#include <QApplication>
#include <cmath>

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);
    MainWindow window;
    window.showMaximized();
    return app.exec();
}
