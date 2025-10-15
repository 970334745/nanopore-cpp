#include <QtWidgets/QApplication>
#include "ui/AppWindow.h"

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);
    AppWindow w;
    w.resize(1200, 800);
    w.show();
    return app.exec();
}