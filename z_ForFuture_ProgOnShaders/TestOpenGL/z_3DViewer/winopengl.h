#ifndef WINOPENGL_H
#define WINOPENGL_H

#include <QWidget>
#include <QMatrix4x4>    // для создания проекционной матрицы, что работала перспектива
#include <QOpenGLWidget>
#include <QOpenGLFunctions>
#include <QOpenGLExtraFunctions>
#include <QOpenGLShaderProgram>  // для работы с шейдерами
#include <QOpenGLTexture>  // для работы с шейдерными текстурами
#include <QOpenGLBuffer>  // для работы с шейдерными буферами
#include <QOpenGLVertexArrayObject>
#include <QTimer>
#ifdef Q_OS_MAC
    #include <glu.h>
#elif linux
    #include <GL/glu.h>
//#else
//    #error "We don't support that version yet..."
#endif
//#include <glu.h>     // для macOS
//#include <GL/glu.h>  // <glu.h> для ubuntu
#include <object3d.h>
#include <math.h>

class Object3D;

class WinOpenGL : public QOpenGLWidget, protected QOpenGLFunctions
{
    Q_OBJECT
public:
    WinOpenGL(QWidget *parent = 0);

    float frames_per_second;
    float one_frame;
    float angle_per_frame;

    void paint_model();

    double scale;

    int mov_x;
    int mov_y;
    int mov_z;

    int rot_x;
    int rot_y;
    int rot_z;

    double rotate_x;
    double rotate_y;
    double rotate_z;

    int sizeLine_px;
    int sizeVertex_px;

    float xRot, yRot, zRot;

protected:
    void initializeGL() override;  // используется для глобальных настроек построения изображения, которые нет необходимости указывать при построении кадра.
    void resizeGL(int w, int h) override;  // служит для перестроения размера окна. При изменении размера окна запускается этот метод.
    void paintGL() override;  // этот метод будет выстраивать каждый наш кадр для отображения.

    void mousePressEvent(QMouseEvent*) override;  // функция вызываемая при нажатии на клавишу мышки
    void mouseMoveEvent(QMouseEvent*) override;  // функция вызываемая при движении мышки
    void wheelEvent(QWheelEvent*) override;  // функция работы с колёсиком мыши

    void initShaders(); // объявляем функцию инициализации шейдеров
    void initCube(float width);    // объявляем функцию инициализации куба

    void scrollWithPixels(QPoint*);

private:
    QMatrix4x4 m_projectionMatrix;  // для создания проекционной матрицы, чтобы работала перспектива
    QOpenGLShaderProgram m_program;
    QVector2D m_mousePosition;  // переменная для хранения позиции мыши при расчёте поворота мышкой по виджету
    QQuaternion m_rotation;  // куантарион - объект который содержит в себе информацию о векторе вокруг которого
                // будет вращаться и угле на который следует поворачивать, нужен для вращения модели
    QVector<Object3D *> m_object;

    float m_z;

    QTimer timer;  // переменная для таймера
};

#endif // WINOPENGL_H
