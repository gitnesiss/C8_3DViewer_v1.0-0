#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QFileDialog>
#include <QtOpenGL/QtOpenGL>
#include <math.h>
#include <QDebug>
#include "winopengl.h"

extern "C" {
    #include "pars.h"
    #include "affine_transformations.h"
}

//Тип линий и вершин, цвет линий и вершин, цвет фона
typedef struct state_data_ui {
    int projection_view;// вид проекции 0 - центральная, 1 - параллельная

    double scale;       // масштаб

    int mov_x;          // перемещение вдоль оси ox
    int mov_y;          // перемещение вдоль оси oy
    int mov_z;          // перемещение вдоль оси oz

    int rot_x;          // поворот вокруг оси ox
    int rot_y;          // поворот вокруг оси oy
    int rot_z;          // поворот вокруг оси oz

    int speed_rot_x;    // скорость вращения вокруг оси ox
    int speed_rot_y;    // скорость вращения вокруг оси oy
    int speed_rot_z;    // скорость вращения вокруг оси oz

    int vertex_type;    // тип вершин
    int vertex_size;    // размер вершин

    int line_type;      // тип линии (ребра)
    int line_size;      // размер линии

    QColor vertex_color;        // цвет вершин
    QColor line_color;          // цвет линий
    QColor background_color;    // цвет фона

    QString filePath;   // путь до файла
    QString info_text;  // текст поля информации о файле и его содержимом
} state_ui_t;


QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);

    ~MainWindow();

    state_ui_t uiState;





    int pub;

    int GetMovX() {
        return priv;
    }

//    int getUiState_

private slots:

    void on_SpinBox_Scale_valueChanged(double arg1);

    void on_Slider_X_mov_valueChanged(int value);

    void on_Slider_Y_mov_valueChanged(int value);

    void on_Slider_Z_mov_valueChanged(int value);

    void on_SpinBox_X_mov_valueChanged(int arg1);

    void on_SpinBox_Y_mov_valueChanged(int arg1);

    void on_SpinBox_Z_mov_valueChanged(int arg1);

    void on_SpinBox_X_rot_valueChanged(int arg1);

    void on_SpinBox_Y_rot_valueChanged(int arg1);

    void on_SpinBox_Z_rot_valueChanged(int arg1);

    void on_dial_X_rot_valueChanged(int value);

    void on_dial_Y_rot_valueChanged(int value);

    void on_dial_Z_rot_valueChanged(int value);

    void on_SpinBox_X_speed_rotate_valueChanged(double arg1);

    void on_SpinBox_Y_speed_rotate_valueChanged(double arg1);

    void on_SpinBox_Z_speed_rotate_valueChanged(double arg1);

    void on_spinBox_sizeLine_valueChanged(int arg1);

    void on_spinBox_sizeVertex_valueChanged(int arg1);

    void on_Slider_sizeLine_valueChanged(int value);

    void on_Slider_sizeVertex_valueChanged(int value);

//    void on_pB_openFile_clicked();

    void on_pB_resetParam_clicked();

//    void on_pushButton_clicked();

//    void on_pB_colorVertex_clicked();

    void on_comboBox_projection_activated(int index);

    void on_comboBox_typeLine_activated(int index);

    void on_comboBox_typeVertex_activated(int index);

private:
    Ui::MainWindow *ui;
    int priv = 0;
};
#endif // MAINWINDOW_H
