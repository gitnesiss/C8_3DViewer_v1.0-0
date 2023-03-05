#define GL_SILENCE_DEPRECATION
#include "winopengl.h"
#include <QDebug>
#include <iostream>

#ifdef Q_OS_MAC
    #include <glu.h>
#elif linux
    #include <GL/glu.h>
//#else
//    #error "We don't support that version yet..."
#endif

#include "mainwindow.h"

structobject obj;
int flag_open_file = 0;

WinOpenGL::WinOpenGL(QWidget *parent) : QOpenGLWidget(parent)
{
    sizeWinWidgetW = 0;
    sizeWinWidgetH = 0;

    sizeX = 0;
    sizeY = 0;
    sizeZ = 0;
    fieldView = 0;

    typeProjection = 0;
    oldTypeProjection = 0;

    scale = 1;

    this->mov_x = 0;
    this->mov_y = 0;
    this->mov_z = 0;

    this->rot_x = 0;
    this->rot_y = 0;
    this->rot_z = 0;

    this->rotate_x = 0;
    this->rotate_y = 0;
    this->rotate_z = 0;

    this->sizeLine = 1;
    this->sizeVertex = 1;

    this->typeLine = 1;
    this->typeVertex = 1;

    this->global_move_x = 0;
    this->global_move_y = 0;
    this->global_move_z = 0;

    this->angle_x = 0;
    this->angle_y = 0;
    this->angle_z = 0;

    allColor.colorVertexRed = 0.971832;
    allColor.colorVertexGreen = 0.171923;
    allColor.colorVertexBlue = 0.27657;
    allColor.colorVertexAlpha = 1.0;
    allColor.colorVertex = QColor(allColor.colorVertexRed, allColor.colorVertexGreen, allColor.colorVertexBlue, allColor.colorVertexAlpha);

    allColor.colorLineRed = 0.735;
    allColor.colorLineGreen = 0.83;
    allColor.colorLineBlue = 0.98;
    allColor.colorLineAlpha = 1.0;
    allColor.colorLine = QColor(allColor.colorLineRed, allColor.colorLineGreen, allColor.colorLineBlue, allColor.colorLineAlpha);

    allColor.colorBGRed = 0.15;
    allColor.colorBGGreen = 0.15;
    allColor.colorBGBlue = 0.15;
    allColor.colorBGAlpha = 1.0;
    allColor.colorBG = QColor(allColor.colorBGRed, allColor.colorBGGreen, allColor.colorBGBlue, allColor.colorBGAlpha);

    // 360 град = 1 об
    // 1000 мс / 16 мс = 62,5 кадра
    // 360 град / 62,5 кадра = 5,76 град / кадр
    frames_per_second = 60;  // 62.5
    one_frame = 1000 / frames_per_second;
    angle_per_frame = 360 / frames_per_second;

    connect( &timer, SIGNAL(timeout()), this, SLOT(update()));
    timer.start(this->one_frame);
}

WinOpenGL::~WinOpenGL()
{
    free_struct(&obj);
}


void WinOpenGL::initializeGL()
{
    initializeOpenGLFunctions();// подключение специальных функций openGL'я, в частности позволит работать
                                // glEnable(). Определяет подмножество спецификации OpenGL. Однако
                                // использование функций из этого подмножества может быть затруднено,
                                // поскольку в настольных системах их необходимо разрешать вручную.
                                // QOpenGLFunctions предоставляет гарантированный API, доступный во всех
                                // системах OpenGL, и заботится о разрешении функций в тех системах,
                                // где это необходимо.

    // Функции glEnable и glDisable позволяют включать и отключать различные графические возможности OpenGL. https://learn.microsoft.com/ru-ru/windows/win32/opengl/glenable
    glEnable(GL_DEPTH_TEST);  // выполнить тест глубины до выполнения фрагментного шейдера. https://habr.com/ru/post/342610/
                              // Если становится известно, что фрагмент никак не может быть виден (перекрыт
                              // другими объектами), то его можно отбросить до этапа шейдинга. Включение
                              // теста глубины также требует очистки буфера от старых значений в каждом
                              // кадре. В знакомую функцию glClear добавляется новый флаг GL_DEPTH_BUFFER_BIT
                              // Область памяти, используемая механизмом удаления скрытых поверхностей. Хранит значения
                              // глубины (z-значения) всех фрагментов.
    glEnable(GL_CULL_FACE);   // отбраковывать полигоны находящиеся за видимыми полигонами.


    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);  // поверхности будут закрашены с обеих сторон.
}

void WinOpenGL::paintGL()
{
    // очистка буфера цвета и буфера глубины
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glMatrixMode(GL_MODELVIEW);            // Функция для возможности использования матрицы. GL_MODELVIEW - видовая матрица - матрица текстур
    glLoadIdentity();                      // Загружаем в стек еденичную матрицу

    // Настройка камеры: положение камеры в пространстве, цель (точка) куда направлена камера, направление верха камеры
    gluLookAt(0.0, 0.0, fieldView * 2,  0.0, 0.0, 0.0,  0.0, 1.0, 0.0);

    // Выбор цвета фона - заполняем окно цветом по схеме RGBA
    glClearColor(this->allColor.colorBGRed, this->allColor.colorBGGreen, this->allColor.colorBGBlue, 1.0);

    // Настройка проекции (выбирается тип проеции):
    changeViewPerspective();

    // Настройка отображения линии:
    customizeLineDisplay();

    // Настройка отображения вершин:
    customizeVertexDisplay();

    // Смещение модели в центр экрана - пересчёт координат для смещения происходит в другом месте:
    glTranslated(global_move_x, global_move_y, global_move_y);


    // Вращение модели
    rotateDinamicModel();

    glEnableVertexAttribArray(0);   // для отрисовки массива вершин.
                                    // Включает универсальный массив атрибутов вершин, заданный индексом.
                                    // По умолчанию все общие массивы атрибутов вершин отключены. Если этот
                                    // параметр включен, значения в универсальном массиве атрибутов вершин
                                    // будут доступны и использованы для рендеринга при вызове команд
                                    // массива вершин, таких как glDrawArrays, glDrawArraysInstanced,
                                    // glDrawElements, glDrawElementsInstanced или glDrawRangeElements.

    paint_model();  // собственная функция с отрисовкой куба в 2D

    glDisableVertexAttribArray(0);  // выключить отрисовку вершин.
                                    // Отключает общий массив атрибутов вершин, заданный индексом.
}

// Изменение вида проекции
void WinOpenGL::changeViewPerspective()
{
    // Проверка: совпадает ли текущий номер комбобокса со старым номером
    if (oldTypeProjection != typeProjection) {  // если не совпадает,
        oldTypeProjection = typeProjection;     // то заменить номер комбобокса
        WinOpenGL::resizeGL(sizeWinWidgetW, sizeWinWidgetH);  // запустить перерисовку
    }
}

// Настройка отображения линии
void WinOpenGL::customizeLineDisplay()
{
    glLineWidth(sizeLine);  // Параметр линии: ширина линии, в скобках указывается величина в пикселях
    if (typeLine == 1) {
        glDisable(GL_LINE_STIPPLE);  // отключение пунктирноой линии, а можно просто сделать её прозрачной полностью
    } else if (typeLine == 2) {
//        glLineStipple(1, 0x00FF);	 // параметр пунктирной линии: штриховой
        glLineStipple(5, 0x00FF);	 // параметр пунктирной линии: штриховой
//        glLineStipple(5, 0x0101);   // параметр пунктирной линии: точечный(factor, pattern)
//        glLineStipple(3, 0x00F0);	 // параметр пунктирной линии: штриховой
//        glLineStipple(3, 0x3333);	 // параметр пунктирной линии: штриховой
//        glLineStipple(5, 0x1C47);  // параметр пунктирной линии: штрихпунктир
//        glLineStipple(5, 0x1C47);  // параметр пунктирной линии: тире, точка, тире
        glEnable(GL_LINE_STIPPLE);  // включение режима пунктирной линий (stipple_line)
    }
}

// Настройка отображения вершин
void WinOpenGL::customizeVertexDisplay()
{
    glPointSize(sizeVertex);  // диаметр точки, в скобках указывается величина в пикселях
    if (typeVertex == 1) {  // выбор формы отображения вершины
        glEnable(GL_POINT_SMOOTH);  // отображать вершины в форме круга
    } else if(typeVertex == 2) {
        glDisable(GL_POINT_SMOOTH);  //отображать вершины в форме квадрата
    }
}

// Настройка вращения модели
void WinOpenGL::rotateDinamicModel()
{
//    double angle_x_old = angle_x;
    angle_x += rotate_x * (2 * M_PI);
    angle_y += rotate_y * (2 * M_PI);
    angle_z += rotate_z * (2 * M_PI);
//    qDebug() << "angle_x = " << angle_x << "   angle_y = " << angle_y << "   angle_z = " << angle_z;

    glRotatef(angle_x, 1, 0, 0);
    glRotatef(angle_y, 0, 1, 0);
    glRotatef(angle_z, 0, 0, 1);

//    setValueMoveY();
}

void WinOpenGL::resizeGL(int w, int h)
{
    sizeWinWidgetW = w;
    sizeWinWidgetH = h;

    sizeX = sizeToAxis(obj.min_x, obj.max_x);
    sizeY = sizeToAxis(obj.min_y, obj.max_y);
    sizeZ = sizeToAxis(obj.min_z, obj.max_z);
    fieldView = maxSizeToAxis(sizeX, sizeY, sizeZ);

//    qDebug() << "Перемещение модели для центровки модели по экрану resizeGL fieldView = " << fieldView;

    glViewport(0, 0, w, h);                     // пересчёт размеров изображения под систему координат растрового изображения, с координатной системой от по x[-1, 1] и y[-1, 1]
    glMatrixMode(GL_PROJECTION);                // установка режима (GL_PROJECTION - матрица проекционная и GL_MODELVIEW - матрица представления модели (матрца перспективная)
    glLoadIdentity();                           // загрузка единичной матрицы
    qreal aspectratio = qreal(w) / qreal(h);    // определеление коэффициента отношения сторон ширине к высоте экрана

    // Выбор вила проекции: центральная, параллельная
    if (typeProjection == 0) {
        // Настройка центральной перспективы: FOV (field of view), соотношение сторон ширины к высоте, область от камеры попадающая в проссчёт геометрии
        gluPerspective(45.0, aspectratio, 0.0001, fieldView * 6);
    } else {
        // Задание плоскостей обрезки области видимости left, right, bottom, top, near, far  - naer и far должны быть положительными, это расстояние отглаз, а не расстояние на сисеме координат
        glOrtho(-fieldView * aspectratio, fieldView * aspectratio, -fieldView, fieldView, 0.0001, fieldView * 6);
    }

}

// Определение размера модели в доль рассчитываемой оси
double WinOpenGL::sizeToAxis(double min, double max)
{
    double size = 0;
    if (max < 0 && min < 0) {
        size = abs(min) - abs(max);
    } else if (max > 0 && min > 0) {
        size = max - min;
    } else {
        size = max + abs(min);
    }
    return size;
}

// Определение максимального размера модели по одной из осей
double WinOpenGL::maxSizeToAxis(double x, double y, double z)
{
    double result = 0;
    if (x > y) {
        if (x > z) {
            result = x;
        } else {
            result = y;
        }
    } else {
        if (y > z) {
            result = y;
        } else {
            result = z;
        }
    }
    return result;
}

// Выбор модели, аффинные преобразования, отрисовка линий и вершин
void WinOpenGL::paint_model() {
    int error = 0;
    if (!filePath.isEmpty()) {

        if (flg == 1) {                                             // flag_open_file 0,1 -флаг окрытого файла
            free_struct(&obj);
            const char *cLine;
            QByteArray inArr = filePath.toUtf8();
            cLine = inArr.constData();                              //
            error = pars(cLine, &obj);                              // запуск парсера файла *.obj
            WinOpenGL::resizeGL(sizeWinWidgetW, sizeWinWidgetH);    // вызов перерисовки окна
            flg = 2;
        }

        all_rotation(&obj, mov_x, mov_y, mov_z, rot_x, rot_y, rot_z, scale);

        // Подача массива для расчёта
        glVertexAttribPointer(0, 3, GL_DOUBLE, GL_FALSE, 0, obj.arr_v_copy);
                    // 0 - определяемый атрибут
                    // 3 - количество координат на вершину, от 1 до 4
                    // type - GL_DOUBLE - тип данных в массиве
                    // normalized - GL_FALSE - не нужно нормализовать, значения данных преобразуются непосредственно как значения с фиксированноый точкой при доступе к ним.
                    // 0 - Указывает смещение в байтах между последовательными общими атрибутами вершин. Если шаг равен 0, общие атрибуты вершин считаются плотно упакованными в массиве.
                    // Vertex_2 - указатель на массив вершин

        // Отрисовка вершин
        glColor3d(this->allColor.colorVertexRed, this->allColor.colorVertexGreen, this->allColor.colorVertexBlue);  // цвет вершин - RGB
        if (typeVertex != 0) {  // Выбор: отрисовывать вершины или нет
            glDrawArrays(GL_POINTS, 0, obj.v_count);  // Отрисовка вершин
        }

        // Отрисовка линий
        glColor3d(this->allColor.colorLineRed, this->allColor.colorLineGreen, this->allColor.colorLineBlue);  // цвет линии - RGB
        if (typeLine != 0) {  // Выбор: отрисовывать линии или нет
            glDrawElements(GL_LINES, obj.lines_count, GL_UNSIGNED_INT, obj.arr_f);
        }

        glDisable(GL_BLEND);
    }
}

// ---------- start Настройка мыши ----------

void WinOpenGL::mousePressEvent(QMouseEvent* mousePress)
{
    mPos = mousePress->pos();
}

void WinOpenGL::mouseMoveEvent(QMouseEvent* mouseEvent)
{
    float kx = 0.01;  // коэффициент для замедления вращения модели
    float ky = 0.01;  // коэффициент для замедления вращения модели
    rot_x += kx * 180 / (M_PI) * (mouseEvent->pos().y() - mPos.y());
    rot_y += ky * 180 / (M_PI) * (mouseEvent->pos().x() - mPos.x());
    mPos = mouseEvent->pos();
}

// ---------- finish Настройка мыши ----------
