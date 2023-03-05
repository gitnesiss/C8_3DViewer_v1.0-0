#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "winopengl.h"

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


void MainWindow::on_SpinBox_Scale_valueChanged(double arg1)  // масштабирование модели
{
    uiState.scale = arg1;
    ui->openGLWidget->scale = uiState.scale;
}


void MainWindow::on_Slider_X_mov_valueChanged(int value)
{
    ui->openGLWidget->mov_x = value;
    QObject::connect(ui->Slider_X_mov, SIGNAL(valueChanged(int)), ui->SpinBox_X_mov, SLOT(setValue(int)));
}

void MainWindow::on_SpinBox_X_mov_valueChanged(int arg1)  // перемещение модели вдоль оси X
{
    ui->openGLWidget->mov_x = arg1;
    QObject::connect(ui->SpinBox_X_mov, SIGNAL(valueChanged(int)), ui->Slider_X_mov, SLOT(setValue(int)));
}


void MainWindow::on_Slider_Y_mov_valueChanged(int value)
{
    ui->openGLWidget->mov_y = value;
    QObject::connect(ui->Slider_Y_mov, SIGNAL(valueChanged(int)), ui->SpinBox_Y_mov, SLOT(setValue(int)));
}

void MainWindow::on_SpinBox_Y_mov_valueChanged(int arg1)  // перемещение модели вдоль оси Y
{
    ui->openGLWidget->mov_y = arg1;
    QObject::connect(ui->SpinBox_Y_mov, SIGNAL(valueChanged(int)), ui->Slider_Y_mov, SLOT(setValue(int)));
}


void MainWindow::on_Slider_Z_mov_valueChanged(int value)
{
    ui->openGLWidget->mov_z = value;
    QObject::connect(ui->Slider_Z_mov, SIGNAL(valueChanged(int)), ui->SpinBox_Z_mov, SLOT(setValue(int)));
}

void MainWindow::on_SpinBox_Z_mov_valueChanged(int arg1)  // перемещение модели вдоль оси Z
{
    ui->openGLWidget->mov_z = arg1;
    QObject::connect(ui->SpinBox_Z_mov, SIGNAL(valueChanged(int)), ui->Slider_Z_mov, SLOT(setValue(int)));
}


void MainWindow::on_SpinBox_X_rot_valueChanged(int arg1)  // вращение модели вокруг оси X
{
    if (arg1 >= 360) {
        ui->SpinBox_X_rot->setValue(0);  // в spinBox установить значение 0
    } else if (arg1 <= -1) {
        ui->SpinBox_X_rot->setValue(359);
    } else {
        ui->openGLWidget->rot_x = arg1;
    }
    QObject::connect(ui->SpinBox_X_rot, SIGNAL(valueChanged(int)), ui->dial_X_rot, SLOT(setValue(int)));
    update();
}

void MainWindow::on_dial_X_rot_valueChanged(int value)
{
    ui->openGLWidget->rot_x = value;
    QObject::connect(ui->dial_X_rot, SIGNAL(valueChanged(int)), ui->SpinBox_X_rot, SLOT(setValue(int)));
    update();
}


void MainWindow::on_SpinBox_Y_rot_valueChanged(int arg1)  // вращение модели вокруг оси Y
{
    if (arg1 >= 360) {
        ui->SpinBox_Y_rot->setValue(0);
    } else if (arg1 <= -1) {
        ui->SpinBox_Y_rot->setValue(359);
    } else {
        ui->openGLWidget->rot_y = arg1;
    }
    QObject::connect(ui->SpinBox_Y_rot, SIGNAL(valueChanged(int)), ui->dial_Y_rot, SLOT(setValue(int)));
    update();
}

void MainWindow::on_dial_Y_rot_valueChanged(int value)
{
    ui->openGLWidget->rot_y = value;
    QObject::connect(ui->dial_Y_rot, SIGNAL(valueChanged(int)), ui->SpinBox_Y_rot, SLOT(setValue(int)));
    update();
}


void MainWindow::on_SpinBox_Z_rot_valueChanged(int arg1)  // вращение модели вокруг оси Z
{
    if (arg1 >= 360) {
        ui->SpinBox_Z_rot->setValue(0);
    } else if (arg1 <= -1) {
        ui->SpinBox_Z_rot->setValue(359);
    } else {
        ui->openGLWidget->rot_z = arg1;
    }
    QObject::connect(ui->SpinBox_Z_rot, SIGNAL(valueChanged(int)), ui->dial_Z_rot, SLOT(setValue(int)));
    update();
}

void MainWindow::on_dial_Z_rot_valueChanged(int value)
{
    ui->openGLWidget->rot_z = value;
    QObject::connect(ui->dial_Z_rot, SIGNAL(valueChanged(int)), ui->SpinBox_Z_rot, SLOT(setValue(int)));
    update();
}


void MainWindow::on_SpinBox_X_speed_rotate_valueChanged(double arg1)  // постоянное вращение модели вокруг оси X
{
    ui->openGLWidget->rotate_x = arg1;
}


void MainWindow::on_SpinBox_Y_speed_rotate_valueChanged(double arg1)  // постоянное вращение модели вокруг оси Y
{
    ui->openGLWidget->rotate_y = arg1;
}


void MainWindow::on_SpinBox_Z_speed_rotate_valueChanged(double arg1)  // постоянное вращение модели вокруг оси Z
{
    ui->openGLWidget->rotate_z = arg1;
}


void MainWindow::on_spinBox_sizeLine_valueChanged(int arg1)  // размер линии в пикселях, спинбокс связан со слайдером
{
//    ui->openGLWidget->sizeLine_px = arg1;
    uiState.line_size = arg1;
    if (uiState.line_size == 0) {
        glDisable(GL_LINE);
    } else {
        glEnable(GL_LINE);
        glLineWidth( uiState.line_size);
    }
    QObject::connect(ui->spinBox_sizeLine, SIGNAL(valueChanged(int)), ui->Slider_sizeLine, SLOT(setValue(int)));
}

void MainWindow::on_Slider_sizeLine_valueChanged(int value)  // размер линии в пикселях, слайдер связан со спинбоксом
{
    ui->openGLWidget->sizeLine_px = value;
    QObject::connect(ui->Slider_sizeLine, SIGNAL(valueChanged(int)), ui->spinBox_sizeLine, SLOT(setValue(int)));
}


void MainWindow::on_spinBox_sizeVertex_valueChanged(int arg1)  // размер вершины в пикселях, спинбокс связан со слайдером
{
    ui->openGLWidget->sizeVertex_px = arg1;
    QObject::connect(ui->spinBox_sizeVertex, SIGNAL(valueChanged(int)), ui->Slider_sizeVertex, SLOT(setValue(int)));
}

void MainWindow::on_Slider_sizeVertex_valueChanged(int value)  // размер вершины в пикселях, слайдер связан со спинбоксом
{
    ui->openGLWidget->sizeVertex_px = value;
    QObject::connect(ui->Slider_sizeVertex, SIGNAL(valueChanged(int)), ui->spinBox_sizeVertex, SLOT(setValue(int)));
}



/*

void MainWindow::on_pB_openFile_clicked()
{

    // узнаём адрес файла который хотим открыть
    QString fileName = QFileDialog::getOpenFileName(this, tr("Open file"), ".", tr("Text files (*.*)"));
    ui->l_fileName->setText(fileName);  // записываем переменную в бирку
    int error = 0;
    QString fin_text = ui->textEdit_fileInfo->toPlainText();
    QString fin_matr = ui->textEdit_fileInfo->toPlainText();

    structobject object;
    const char *cLine;
    QByteArray inArr = fileName.toUtf8();
    cLine = inArr.constData();

    error = pars(cLine, &object);
    if (error == 0) {
        ui->textEdit_fileInfo->setPlainText("Error = " + fin_text.setNum(error) +
                                   "\nкол-во вершин = " + fin_text.setNum(object.v_count) +
                                   "\nкол-во поверхностей = " + fin_text.setNum(object.f_count) +
                                   "\nmin_x = " + fin_text.setNum(object.min_x) +
                                   "  max_x = " + fin_text.setNum(object.max_x) +
                                   "\nmin_y = " + fin_text.setNum(object.min_y) +
                                   "  max_y = " + fin_text.setNum(object.max_y) +
                                   "\nmin_z = " + fin_text.setNum(object.min_z) +
                                   "  max_z = " + fin_text.setNum(object.max_z));
    }

//    for (int i = 0; i<object.f_count*3;i++) {
//        QString fin_matr = ui->textEdit_2->toPlainText();
//        ui->textEdit_2->setPlainText(fin_matr + "\n" + fin_text.setNum(object.matr_f[i]));
//    }

    free(object.matr_v);
}

*/

// void MainWindow::on_pB_openFile_clicked()
// {
// //    // узнаём адрес файла который хотим открыть
// //    QString fileName = QFileDialog::getOpenFileName(this, tr("Open file"), ".", tr("Text files (*.txt)"));
// //    ui->l_fileName->setText(fileName);  // записываем переменную в бирку
// //    qDebug() << fileName;

//     QString QbasePath = QCoreApplication::applicationDirPath();
//     basePath = QbasePath.toStdString();
//     qDebug() << QbasePath;
// }

void MainWindow::on_pB_resetParam_clicked()
{
    ui->SpinBox_Scale->setValue(1);
    ui->SpinBox_X_mov->setValue(0.0);
    ui->SpinBox_Y_mov->setValue(0.0);
    ui->SpinBox_Z_mov->setValue(0.0);
    ui->SpinBox_X_rot->setValue(0);
    ui->SpinBox_Y_rot->setValue(0);
    ui->SpinBox_Z_rot->setValue(0);
    ui->SpinBox_X_speed_rotate->setValue(0);
    ui->SpinBox_Y_speed_rotate->setValue(0);
    ui->SpinBox_Z_speed_rotate->setValue(0);
    ui->Slider_sizeLine->setValue(1);
    ui->Slider_sizeVertex->setValue(1);
    ui->openGLWidget->update();
}

//void MainWindow::on_pB_colorVertex_clicked()
//{
//    QColor color = QColorDialog::getColor(QColor(255,100,200,255));
//    if (!color.isValid()) {
//    // Cancel
//    }
//}


void MainWindow::on_comboBox_projection_activated(int index)  // изменение вида проекции
{
    if (index == 0) {
        uiState.projection_view = 0;  // центральная проекция
    } else {
        uiState.projection_view = 1;  // параллельная проекция
    }
}


void MainWindow::on_comboBox_typeLine_activated(int index)  // изменение типа линии
{
    uiState.line_type = index;
    if (index == 0) {
        uiState.line_type = 0;  // линия отсутствует
        glDisable(GL_LINE_STIPPLE);
    } else if (index == 1){
        uiState.line_type = 1;  // сплошная линия
        glDisable(GL_LINE_STIPPLE);
    } else {
        uiState.line_type = 2;  // пунктирная линия
        glEnable(GL_LINE_STIPPLE);  // Включение режима пунктирной линий (stipple_line)
    //    glLineStipple(5, 0x0101);  // Параметр пунктирной линии: точечный(factor, pattern)
    //    glLineStipple(3, 0x00F0);	 // Параметр пунктирной линии: штриховой
        glLineStipple(3, 0x3333);	 // Параметр пунктирной линии: штриховой
    //    glLineStipple(5, 0x1C47);  // Параметр пунктирной линии: штрихпунктир
    //    glLineStipple(5, 0x1C47);  // Параметр пунктирной линии: тире, точка, тире
        glDisable(GL_LINE_STIPPLE);
    }
}


void MainWindow::on_comboBox_typeVertex_activated(int index)  // изменение типа вершины
{

}

