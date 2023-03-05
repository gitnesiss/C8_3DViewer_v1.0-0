#include "mainwindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    QApplication::setStyle("Fusion");
    MainWindow w;
    w.setWindowTitle("3D Viewer 1.0");
//    w.setGeometry(500, 200, 1200, 1000);

    w.showMaximized();  // развернуть окно во весь экран

    w.show();
    return app.exec();
}
