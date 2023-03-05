#include "mainwindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QCoreApplication::setAttribute(Qt::AA_UseDesktopOpenGL);
    QApplication app(argc, argv);
    QApplication::setStyle("Fusion");

    // Убераем зернистость на текстуре
    QSurfaceFormat format;
    format.setSamples(16);  // 16 семплов при отрисовке
    format.setDepthBufferSize(24);  // размер буфера глубины
    QSurfaceFormat::setDefaultFormat(format);

    MainWindow w;
    w.setWindowTitle("3D Viewer 1.0");
    w.show();
    return app.exec();
}
