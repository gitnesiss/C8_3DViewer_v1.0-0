#include "winopengl.h"
#include "mainwindow.h"
#include <object3d.h>
#include <QDebug>
#include <iostream>
#include <QMouseEvent>
#include <QWheelEvent>
#include <QOpenGLContext>

#define TEXTURE ":/cube0.png"
//#define TEXTURE ":/cube1.png"
//#define TEXTURE ":/cube2.png"
//#define TEXTURE ":/cube3.png"


// https://www.youtube.com/watch?v=rZcTaRU7AAw&list=PLQOaTSbfxUtCrKs0nicOg2npJQYSPGO9r&index=85 - уроки по C++

// https://doc.qt.io/qt-6/qtopengl-module.html // виджет qtOpenGLWidget
// https://doc.qt.io/qt-6/examples-widgets-opengl.html - OpenGL Examples from the Qt OpenGL module
// https://doc.qt.io/qt-6/qopenglwidget.html - QOpenGLWidget Class
// https://doc.qt.io/qt-6/qopenglwidget.html
// https://habr.com/ru/post/310790/ - learnopengl. Урок 1.1 — OpenGL

// https://www.youtube.com/watch?v=yJE34EuZNzs - хорошая лекция по openGL
// https://www.youtube.com/watch?v=9al64H0BPmM - как делать шейдеры

// https://startandroid.ru/ru/uroki/vse-uroki-spiskom/399-urok-170-opengl-graficheskie-primitivy.html


WinOpenGL::WinOpenGL(QWidget *parent) : QOpenGLWidget(parent)
{
    this->scale = 1;

    this->mov_x = 0;
    this->mov_y = 0;
    this->mov_z = 0;

    this->rot_x = 0;
    this->rot_y = 0;
    this->rot_z = 0;

    this->rotate_x = 0;
    this->rotate_y = 0;
    this->rotate_z = 0;

    this->sizeLine_px = 1;
    this->sizeVertex_px = 1;

    m_z = -5.0f;


    // 360 град = 1 об
    // 1000 мс / 16 мс = 62,5 кадра
    // 360 град / 62,5 кадра = 5,76 град / кадр
    this->frames_per_second = 62.5;  // 62.5
    this->one_frame = 1000 / frames_per_second;
    this->angle_per_frame = 360 / frames_per_second;

    connect( &timer, SIGNAL(timeout()), this, SLOT(update()));
    timer.start(this->one_frame);
}

// вызывается один раз при создании окна openGL
void WinOpenGL::initializeGL()
{
    initializeOpenGLFunctions();// подключение специальных функций openGL'я, в частности позволит работать
                                // glEnable(). Определяет подмножество спецификации OpenGL. Однако
                                // использование функций из этого подмножества может быть затруднено,
                                // поскольку в настольных системах их необходимо разрешать вручную.
                                // QOpenGLFunctions предоставляет гарантированный API, доступный во всех
                                // системах OpenGL, и заботится о разрешении функций в тех системах,
                                // где это необходимо.
    glClearColor(0.0, 0.0, 0.0, 0.0); // цвет фона - заполняем окно цветом по схеме RGBA

    // Функции glEnable и glDisable позволяют включать и отключать различные графические возможности OpenGL. https://learn.microsoft.com/ru-ru/windows/win32/opengl/glenable
    glEnable(GL_DEPTH_TEST);  // выполнить тест глубины до выполнения фрагментного шейдера. https://habr.com/ru/post/342610/
                              // Если становится известно, что фрагмент никак не может быть виден (перекрыт
                              // другими объектами), то его можно отбросить до этапа шейдинга. Включение
                              // теста глубины также требует очистки буфера от старых значений в каждом
                              // кадре. В знакомую функцию glClear добавляется новый флаг GL_DEPTH_BUFFER_BIT
                              // Область памяти, используемая механизмом удаления скрытых поверхностей. Хранит значения
                              // глубины (z-значения) всех фрагментов.
    glEnable(GL_CULL_FACE);   // Отсечение задних граней, чтобы они не были видны за видимыми полигонами (и не просчитывались).

    initShaders();

    initCube(1.0f);  // инициализация куба. Для инициализации куба нужно добавить две библиотеки
                 // вершинный и пиксельный буферы
//    m_object[0]->translate(QVector3D(-1.2, 0.0, 0.0));
    m_object[0]->translate(QVector3D(0.0, 0.0, 0.0));

//    initCube(2.0f);
//    m_object[1]->translate(QVector3D( 1.2, 0.0, 0.0));
}

void WinOpenGL::paintGL()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);  // очистка буфера цвета и буфера глубины

    // Загрузить матрицу проекций в шейдер и модельно-видовую матрицу
    // Модельно-видовую матрицу лучше делать в клоссе объекта
    QMatrix4x4 viewMatrix;
    viewMatrix.setToIdentity();  // переводим модельно-видовую матрицу в еденичную
    viewMatrix.translate(0.0, 0.0, m_z);  // отодвигаем камеру немного назад, работа с камерой
    viewMatrix.rotate(m_rotation);

    // Заполнение юниформ общих для всех объектов
    m_program.bind(); // биндим программу чтобы иметь к ней доступ
    m_program.setUniformValue("u_projectionMatrix", m_projectionMatrix);  // присваиваем юниформ значение: присваиваем проекционную матрицу
    m_program.setUniformValue("u_viewMatrix", viewMatrix);  // присваиваем юниформ значение: присваиваем видовую матрицу
    m_program.setUniformValue("u_lightPosition", QVector4D(0.0, 0.0, 0.0, 1.0));  // добавляем позицию источника освещения
    m_program.setUniformValue("u_lightPower", 2.5f);  // добавляем мощность светового источника

    for (int i = 0; i < m_object.size(); i++) {
        m_object[i]->draw(&m_program, context()->functions());
    }
}


void WinOpenGL::resizeGL(int w, int h)
{
    float aspect = w / (float)h;
    m_projectionMatrix.setToIdentity();  // устанавливаем как еденичную матрцу
    m_projectionMatrix.perspective(45, aspect, 0.01f, 100.0f); // посл. 2 параметра - настройка плоскостей отсечения
}


void WinOpenGL::initShaders()
{
    //
    if (!m_program.addShaderFromSourceFile(QOpenGLShader::Vertex, ":/vShader.vert"))
    {
        close();
    }
    if (!m_program.addShaderFromSourceFile(QOpenGLShader::Fragment, ":/fShader.frag"))
    {
        close();
    }
    // Линкуем программу. Линковка объеденяет все шейдеры в один
    if (!m_program.link())  // Если программа не с линкуется то выходим
    {
        close();
    }
    // биндить нужно перед отрисовкой
}

void WinOpenGL::initCube(float width)
{
    float width_div_2 = width / 2.0f;

    QVector<VertexData> vertexes;
    vertexes.append(VertexData(QVector3D(-width_div_2, width_div_2, width_div_2),
                               QVector2D(0.0f, 1.0f), QVector3D(0.0f, 0.0f, 1.0f)));
    vertexes.append(VertexData(QVector3D(-width_div_2, -width_div_2, width_div_2),
                               QVector2D(0.0f, 0.0f), QVector3D(0.0f, 0.0f, 1.0f)));
    vertexes.append(VertexData(QVector3D(width_div_2, width_div_2, width_div_2),
                               QVector2D(1.0f, 1.0f), QVector3D(0.0f, 0.0f, 1.0f)));
    vertexes.append(VertexData(QVector3D(width_div_2, -width_div_2, width_div_2),
                               QVector2D(1.0f, 0.0f), QVector3D(0.0f, 0.0f, 1.0f)));

    vertexes.append(VertexData(QVector3D(width_div_2, width_div_2, width_div_2),
                               QVector2D(0.0f, 1.0f), QVector3D(1.0f, 0.0f, 0.0f)));
    vertexes.append(VertexData(QVector3D(width_div_2, -width_div_2, width_div_2),
                               QVector2D(0.0f, 0.0f), QVector3D(1.0f, 0.0f, 0.0f)));
    vertexes.append(VertexData(QVector3D(width_div_2, width_div_2, -width_div_2),
                               QVector2D(1.0f, 1.0f), QVector3D(1.0f, 0.0f, 0.0f)));
    vertexes.append(VertexData(QVector3D(width_div_2, -width_div_2, -width_div_2),
                               QVector2D(1.0f, 0.0f), QVector3D(1.0f, 0.0f, 0.0f)));

    vertexes.append(VertexData(QVector3D(width_div_2, width_div_2, width_div_2),
                               QVector2D(0.0f, 1.0f), QVector3D(0.0f, 1.0f, 0.0f)));
    vertexes.append(VertexData(QVector3D(width_div_2, width_div_2, -width_div_2),
                               QVector2D(0.0f, 0.0f), QVector3D(0.0f, 1.0f, 0.0f)));
    vertexes.append(VertexData(QVector3D(-width_div_2, width_div_2, width_div_2),
                               QVector2D(1.0f, 1.0f), QVector3D(0.0f, 1.0f, 0.0f)));
    vertexes.append(VertexData(QVector3D(-width_div_2, width_div_2, -width_div_2),
                               QVector2D(1.0f, 0.0f), QVector3D(0.0f, 1.0f, 0.0f)));

    vertexes.append(VertexData(QVector3D(width_div_2, width_div_2, -width_div_2),
                               QVector2D(0.0f, 1.0f), QVector3D(0.0f, 0.0f, -1.0f)));
    vertexes.append(VertexData(QVector3D(width_div_2, -width_div_2, -width_div_2),
                               QVector2D(0.0f, 0.0f), QVector3D(0.0f, 0.0f, -1.0f)));
    vertexes.append(VertexData(QVector3D(-width_div_2, width_div_2, -width_div_2),
                               QVector2D(1.0f, 1.0f), QVector3D(0.0f, 0.0f, -1.0f)));
    vertexes.append(VertexData(QVector3D(-width_div_2, -width_div_2, -width_div_2),
                               QVector2D(1.0f, 0.0f), QVector3D(0.0f, 0.0f, -1.0f)));

    vertexes.append(VertexData(QVector3D(-width_div_2, width_div_2, width_div_2),
                               QVector2D(0.0f, 1.0f), QVector3D(-1.0f, 0.0f, 0.0f)));
    vertexes.append(VertexData(QVector3D(-width_div_2, width_div_2, -width_div_2),
                               QVector2D(0.0f, 0.0f), QVector3D(-1.0f, 0.0f, 0.0f)));
    vertexes.append(VertexData(QVector3D(-width_div_2, -width_div_2, width_div_2),
                               QVector2D(1.0f, 1.0f), QVector3D(-1.0f, 0.0f, 0.0f)));
    vertexes.append(VertexData(QVector3D(-width_div_2, -width_div_2, -width_div_2),
                               QVector2D(1.0f, 0.0f), QVector3D(-1.0f, 0.0f, 0.0f)));

    vertexes.append(VertexData(QVector3D(-width_div_2, -width_div_2, width_div_2),
                               QVector2D(0.0f, 1.0f), QVector3D(0.0f, -1.0f, 0.0f)));
    vertexes.append(VertexData(QVector3D(-width_div_2, -width_div_2, -width_div_2),
                               QVector2D(0.0f, 0.0f), QVector3D(0.0f, -1.0f, 0.0f)));
    vertexes.append(VertexData(QVector3D(width_div_2, -width_div_2, width_div_2),
                               QVector2D(1.0f, 1.0f), QVector3D(0.0f, -1.0f, 0.0f)));
    vertexes.append(VertexData(QVector3D(width_div_2, -width_div_2, -width_div_2),
                               QVector2D(1.0f, 0.0f), QVector3D(0.0f, -1.0f, 0.0f)));

//    QVector<VertexData> vertexes;
//    vertexes.append(VertexData(QVector3D(-width_div_2, width_div_2, width_div_2)));
//    vertexes.append(VertexData(QVector3D(-width_div_2, -width_div_2, width_div_2)));
//    vertexes.append(VertexData(QVector3D(width_div_2, width_div_2, width_div_2)));
//    vertexes.append(VertexData(QVector3D(width_div_2, -width_div_2, width_div_2)));

//    vertexes.append(VertexData(QVector3D(width_div_2, width_div_2, width_div_2)));
//    vertexes.append(VertexData(QVector3D(width_div_2, -width_div_2, width_div_2)));
//    vertexes.append(VertexData(QVector3D(width_div_2, width_div_2, -width_div_2)));
//    vertexes.append(VertexData(QVector3D(width_div_2, -width_div_2, -width_div_2)));

//    vertexes.append(VertexData(QVector3D(width_div_2, width_div_2, width_div_2)));
//    vertexes.append(VertexData(QVector3D(width_div_2, width_div_2, -width_div_2)));
//    vertexes.append(VertexData(QVector3D(-width_div_2, width_div_2, width_div_2)));
//    vertexes.append(VertexData(QVector3D(-width_div_2, width_div_2, -width_div_2)));

//    vertexes.append(VertexData(QVector3D(width_div_2, width_div_2, -width_div_2)));
//    vertexes.append(VertexData(QVector3D(width_div_2, -width_div_2, -width_div_2)));
//    vertexes.append(VertexData(QVector3D(-width_div_2, width_div_2, -width_div_2)));
//    vertexes.append(VertexData(QVector3D(-width_div_2, -width_div_2, -width_div_2)));

//    vertexes.append(VertexData(QVector3D(-width_div_2, width_div_2, width_div_2)));
//    vertexes.append(VertexData(QVector3D(-width_div_2, width_div_2, -width_div_2)));
//    vertexes.append(VertexData(QVector3D(-width_div_2, -width_div_2, width_div_2)));
//    vertexes.append(VertexData(QVector3D(-width_div_2, -width_div_2, -width_div_2)));

//    vertexes.append(VertexData(QVector3D(-width_div_2, -width_div_2, width_div_2)));
//    vertexes.append(VertexData(QVector3D(-width_div_2, -width_div_2, -width_div_2)));
//    vertexes.append(VertexData(QVector3D(width_div_2, -width_div_2, width_div_2)));
//    vertexes.append(VertexData(QVector3D(width_div_2, -width_div_2, -width_div_2)));

    QVector<GLuint> indexes;
    for (int i = 0; i < 24; i += 4) {
        indexes.append(i + 0);
        indexes.append(i + 1);
        indexes.append(i + 2);
        indexes.append(i + 2);
        indexes.append(i + 1);
        indexes.append(i + 3);
    }

    m_object.append(new Object3D(vertexes, indexes, QImage(TEXTURE)));
}

// отработка вращение модели с помощью мыши в окошке
void WinOpenGL::mousePressEvent(QMouseEvent* mouseEvent)
{
    // проверка на нажатие именно левой кнопки мыши
    if (mouseEvent->buttons() == Qt::LeftButton)
    {
        m_mousePosition = QVector2D(mouseEvent->pos());
    }
    mouseEvent->accept();  // подтверждение нажатия
}

void WinOpenGL::mouseMoveEvent(QMouseEvent* mouseEvent)
{
    // проверка на нажатие именно левой кнопки мыши
    if (mouseEvent->buttons() != Qt::LeftButton)
    {
        return;
    }
    // diff - вектор направления, также будет применяться ниже для определения вектора вокруг которого нужно вращать модель
    QVector2D diff = QVector2D(mouseEvent->pos()) - m_mousePosition;  // сохраняем позицию между прошлым и текущим положение (дельта)
    m_mousePosition = QVector2D(mouseEvent->pos());  // сохраняем текущую позицию

    float rotationFactor = 2.0;
    float angle = diff.length() / rotationFactor;  // сохраняем длину перемещения мыши которая переводится в угол

    QVector3D axis = QVector3D(diff.y(), diff.x(), 0.0);  // вектор вокруг которого вращаем модель

    m_rotation = QQuaternion::fromAxisAndAngle(axis, angle) * m_rotation;  // куантарион оси вращения и угла умножается на совершённые повороты,
                // если этого не делать то вращение каждый раз будет начинаться с начала
    update();
    // остаётся m_rotation применить, оно пока ещё только посчитатно
    // Применяется оно в *::paintGL() modelViewMatrix.rotate
}

void WinOpenGL::wheelEvent(QWheelEvent *event)
{
//    if (event->delta() > 0) {
//        m_z += 0.25f;
//    } else if (event->delta() < 0) {
//        m_z -= 0.25f;
//    }


//    update();

//    QPoint numPixels = event->pixelDelta();
//    QPoint numDegrees = event->angleDelta() / 8;

//    if (!numPixels.isNull()) {
//        scrollWithPixels(numPixels);
//    } else if (!numDegrees.isNull()) {
//        QPoint numSteps = numDegrees / 15;
//        scrollWithDegrees(numSteps);
//    }

//    event->accept();
}
