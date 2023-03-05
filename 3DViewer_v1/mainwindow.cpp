#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "winopengl.h"
#include <QMessageBox>
#include <QString>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    float frames_per_second = 60;  // 62.5
    float one_frame = 1000 / frames_per_second;
    connect( &timerUi, SIGNAL(timeout()), this, SLOT(timeUpdate()));
    timerUi.start(one_frame);
}

MainWindow::~MainWindow()
{
    delete ui;
}


// ---------- MenuBar start ----------


void MainWindow::on_action_openFile_triggered()  // Открытие файла
{
    on_pB_openFile_clicked();
}

void MainWindow::on_action_saveFile_triggered()  // Сохранение гифки
{
        QString buffer="";
        QString buf_parametrs = buffer.setNum(ui->openGLWidget->allColor.colorBGRed) +"bgr " + buffer.setNum(ui->openGLWidget->allColor.colorBGGreen) +"bgg " + buffer.setNum(ui->openGLWidget->allColor.colorBGBlue) +"bgb "
                + buffer.setNum(ui->openGLWidget->allColor.colorLineRed) +"lr " + buffer.setNum(ui->openGLWidget->allColor.colorLineGreen) +"lg " + buffer.setNum(ui->openGLWidget->allColor.colorLineBlue) +"lb "
                + buffer.setNum(ui->openGLWidget->allColor.colorVertexRed) +"vr " + buffer.setNum(ui->openGLWidget->allColor.colorVertexGreen) +"vg " + buffer.setNum(ui->openGLWidget->allColor.colorVertexBlue) +"vb "
                + buffer.setNum(ui->openGLWidget->sizeLine) +"sl " + buffer.setNum(ui->openGLWidget->typeLine) +"tl "
                + buffer.setNum(ui->openGLWidget->sizeVertex) +"sv " + buffer.setNum(ui->openGLWidget->typeVertex) +"tv "
                + buffer.setNum(ui->openGLWidget->typeProjection);

     //"0.15bgr 0.15bgg 0.15bgb 0.735lr 0.83lg 0.98lb 0.971832vr 0.171923vg 0.27657vb 1sl 1tl 2sv 0tv 1";

        QString path = address("/config.ini");

        const char * paramerts;
        const char *cLine;
        QByteArray inArr1 = path.toUtf8();
        cLine = inArr1.constData();
        QByteArray inArr2 = buf_parametrs.toUtf8();
        paramerts = inArr2.constData();
        config_save(cLine, paramerts);
}

void MainWindow::on_action_exit_triggered()  // Выход из программы
{
    QMessageBox msg;  //www.itmathrepetitor.ru
    msg.setWindowTitle("Закрытие программы");
    msg.setText("\nВыйти из программы?");
//    msg.setInformativeText("ОК - Выйти из программы\nCancel - Не выходить из программы");
    msg.setStandardButtons(QMessageBox::Ok | QMessageBox::Cancel);
    msg.setIcon(QMessageBox::Question);
    msg.setDefaultButton(QMessageBox::Ok);
    int res = msg.exec();
    if (res == QMessageBox::Ok) {//нажата кнопка Ok
        QApplication::quit();
        //    close();
    }
}

void MainWindow::on_action_screenShot_triggered()  // Создание скриншота
{
    on_pB_screenShot_clicked();
}


void MainWindow::on_action_GIF_triggered()  // Создание GIF-анимации
{
    on_pB_GIF_clicked();
}

void MainWindow::on_action_info_triggered()  // Информация: описание возможностей программы и как ей управлять
{
//    QString link = "https://www.google.com";
//    QDesktopServices::openUrl(QUrl(link));
    QString path = address("/index.html");
    QDesktopServices::openUrl(QUrl("file://" + path));
}

void MainWindow::on_action_infoProgram_triggered()  // Информация о программе
{
    QMessageBox msg;
    msg.setWindowTitle("О программе 3DViewer");
    msg.setIcon(QMessageBox::Information);
    msg.setText("  Программа 3DViewer_v1.0 предназначена для просмотра 3D-моделей.\n\n"
                "  Авторы:\n"
                "    Белоусов Иван     - tristimu;\n"
                "    Кожухов Александр - jorgedom;\n"
                "    Трофимов Роман    - genesiss.\n\n"
                "  Программа сделана как учебный проект в School21.\n"
                "  Возможности программы:\n"
                "- распознавание и отображение файлов с разрешением *.obj;\n"
                "- двигает, вращает и масштабирует 3D-модель;\n"
                "- настраивает отображение модели (цвет и толщину линий, цвет и размер вершин, цвет фона);\n"
                "- создание GIF-анимации.");
    msg.exec();
}

// ---------- MenuBar end ----------



void MainWindow::on_comboBox_projection_activated(int index)  // изменение вида проекции
{
    if (index == 0) {
        ui->openGLWidget->typeProjection = 0;  // центральная проекция
    } else {
        ui->openGLWidget->typeProjection = 1;  // параллельная проекция
    }
}


void MainWindow::on_SpinBox_Scale_valueChanged(double arg1)  // масштабирование модели
{
    ui->openGLWidget->scale = arg1;
}


void MainWindow::on_Slider_X_mov_valueChanged(int value)
{
    ui->openGLWidget->mov_x = value;
    ui->SpinBox_X_mov->setValue(value);
}

void MainWindow::on_SpinBox_X_mov_valueChanged(int arg1)  // перемещение модели вдоль оси X
{
    ui->openGLWidget->mov_x = arg1;
    ui->Slider_X_mov->setValue(arg1);
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
}

void MainWindow::on_dial_X_rot_valueChanged(int value)
{
    ui->openGLWidget->rot_x = value;
    QObject::connect(ui->dial_X_rot, SIGNAL(valueChanged(int)), ui->SpinBox_X_rot, SLOT(setValue(int)));
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
}

void MainWindow::on_dial_Y_rot_valueChanged(int value)
{
    ui->openGLWidget->rot_y = value;
    QObject::connect(ui->dial_Y_rot, SIGNAL(valueChanged(int)), ui->SpinBox_Y_rot, SLOT(setValue(int)));
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
}

void MainWindow::on_dial_Z_rot_valueChanged(int value)
{
    ui->openGLWidget->rot_z = value;
    QObject::connect(ui->dial_Z_rot, SIGNAL(valueChanged(int)), ui->SpinBox_Z_rot, SLOT(setValue(int)));
}


void MainWindow::on_spinBox_X_speedRotate_valueChanged(double arg1)  // постоянное вращение модели вокруг оси X
{
    ui->openGLWidget->rotate_x = arg1;
}

void MainWindow::on_spinBox_Y_speedRotate_valueChanged(double arg1)  // постоянное вращение модели вокруг оси Y
{
    ui->openGLWidget->rotate_y = arg1;
}

void MainWindow::on_spinBox_Z_speedRotate_valueChanged(double arg1)  // постоянное вращение модели вокруг оси Z
{
    ui->openGLWidget->rotate_z = arg1;
}


// ---------- start Настройка отображения линий ----------

void MainWindow::on_spinBox_sizeLine_valueChanged(int arg1)  // размер линии в пикселях, спинбокс связан со слайдером
{
    ui->openGLWidget->sizeLine = arg1;
    QObject::connect(ui->spinBox_sizeLine, SIGNAL(valueChanged(int)), ui->slider_sizeLine, SLOT(setValue(int)));
}

void MainWindow::on_slider_sizeLine_valueChanged(int value)  // размер линии в пикселях, слайдер связан со спинбоксом
{
    ui->openGLWidget->sizeLine = value;
    QObject::connect(ui->slider_sizeLine, SIGNAL(valueChanged(int)), ui->spinBox_sizeLine, SLOT(setValue(int)));
}

void MainWindow::on_comboBox_typeLine_activated(int index)  // изменение типа линии
{
    switch (index) {
    case 0:  // убрать отображение линии
        ui->openGLWidget->typeLine = 0;
        break;
    case 1:  // отображение сплошных линий
        ui->openGLWidget->typeLine = 1;
        break;
    case 2:  // отображение пунктирных линий
        ui->openGLWidget->typeLine = 2;
        break;
    }
}

// ---------- finish Настройка отображения линий ----------


// ---------- start Настройка отображения вершин ----------

void MainWindow::on_spinBox_sizeVertex_valueChanged(int arg1)  // размер вершины в пикселях, спинбокс связан со слайдером
{
    ui->openGLWidget->sizeVertex = arg1;
    QObject::connect(ui->spinBox_sizeVertex, SIGNAL(valueChanged(int)), ui->slider_sizeVertex, SLOT(setValue(int)));
    //update();
}

void MainWindow::on_slider_sizeVertex_valueChanged(int value)  // размер вершины в пикселях, слайдер связан со спинбоксом
{
    ui->openGLWidget->sizeVertex = value;
    QObject::connect(ui->slider_sizeVertex, SIGNAL(valueChanged(int)), ui->spinBox_sizeVertex, SLOT(setValue(int)));
}

void MainWindow::on_comboBox_typeVertex_currentIndexChanged(int index)
{
    switch (index) {
    case 0:  // убрать отображение вершин
        ui->openGLWidget->typeVertex = 0;
        break;
    case 1:  // отображение вершин в виде кружков
        ui->openGLWidget->typeVertex = 1;
        break;
    case 2:  // отображение вершин в виде квадратов
        ui->openGLWidget->typeVertex = 2;
        break;
    }
}

// ---------- finish Настройка отображения вершин ----------


int MainWindow::count_e(int *value, int cnt)  // размер вершины в пикселях, слайдер связан со спинбоксом
{
    int sum = 0;
    for (int i = 0; i<cnt;i++) {
        sum +=value[i];
    }
    return sum;
}


void MainWindow::on_pB_openFile_clicked()  // отработка нажатия клавиши открытие файла
{
    // Выбираем файл, и записываем его адрес в переменную
    QString filePath = QFileDialog::getOpenFileName(this, tr("Open file"), ".", tr("Object files (*.obj)"));
    QString fin_text = ui->textEdit_fileInfo->toPlainText();
    ui->openGLWidget->filePath = filePath;
    ui->openGLWidget->flg = 1;
    if (filePath.isEmpty()) {
        ui->textEdit_fileInfo->setTextColor(QColor(255, 0, 0, 255));  // устанавливаем цвет текста: красный
        ui->textEdit_fileInfo->setPlainText("Error = Файл не выбран/Ошибка открытия файла");
    } else {
        int error = 0;
        structobject object;
        const char *cLine;
        QByteArray inArr = filePath.toUtf8();
        cLine = inArr.constData();
        error = pars(cLine, &object);  // 0 - good, 1 - error

        if (error == 1) {
            ui->textEdit_fileInfo->setTextColor(QColor(255, 0, 0, 255));  // устанавливаем цвет текста: красный
            ui->textEdit_fileInfo->setPlainText("Error = Файл не выбран/Ошибка открытия файла");
        } else {
            // Вычленение имени файла из пути файла
            QString fileName = QString::fromStdString(findName(filePath));

            // Запись текста в поле textEdit
            ui->textEdit_fileInfo->setTextColor(QColor(255, 255, 255, 255));  // устанавливаем цвет текста: стандартный белый
            ui->textEdit_fileInfo->setPlainText("Имя файла: " + fileName +
                                       "\nПуть к файлу: " + filePath +
                                       "\nКол-во вершин = " + fin_text.setNum(object.v_count) +
                                       "\nКол-во поверхностей = " + fin_text.setNum(object.f_count) +
                                       "\nКол-во рёбер = " + fin_text.setNum(count_e(object.arr_vf,object.f_count)) +
                                       "\n\n  Координаты вершин модели:" +
                                       "\nXmin = " + fin_text.setNum(object.min_x) +
                                       "   Xmax = " + fin_text.setNum(object.max_x) +
                                       "\nYmin = " + fin_text.setNum(object.min_y) +
                                       "   Ymax = " + fin_text.setNum(object.max_y) +
                                       "\nZmin = " + fin_text.setNum(object.min_z) +
                                       "   Zmax = " + fin_text.setNum(object.max_z));

            // Определение максимальных и минимальных размеров модели по одной из осей для цетровки модели при загрузке
            ui->openGLWidget->global_move_x = alignModelOnScreen(object.max_x, object.min_x);
            ui->openGLWidget->global_move_y = alignModelOnScreen(object.max_y, object.min_y);
            ui->openGLWidget->global_move_z = alignModelOnScreen(object.max_z, object.min_z);


            QString path = address("/config.ini");
            structconfig config;
            int er = 0;
            qDebug()<<path;
            const char *cLine;
            QByteArray inArr = path.toUtf8();
            cLine = inArr.constData();
            qDebug()<<er;
            er = config_load(cLine, &config);
            // применение настроек конфиг файла
            if (er == 0) {

                ui->openGLWidget->allColor.colorBGRed = config.color_background_r;
                ui->openGLWidget->allColor.colorBGGreen = config.color_background_g;
                ui->openGLWidget->allColor.colorBGBlue = config.color_background_b;

                ui->openGLWidget->allColor.colorVertexRed = config.color_vertex_r;
                ui->openGLWidget->allColor.colorVertexGreen = config.color_vertex_g;
                ui->openGLWidget->allColor.colorVertexBlue = config.color_vertex_b;

                ui->openGLWidget->allColor.colorLineRed = config.color_line_r;
                ui->openGLWidget->allColor.colorLineGreen = config.color_line_g;
                ui->openGLWidget->allColor.colorLineBlue = config.color_line_b;

                ui->openGLWidget->sizeLine = config.size_line;
                ui->openGLWidget->typeLine = config.type_line;

                ui->openGLWidget->sizeVertex = config.size_vertex;
                ui->openGLWidget->typeVertex = config.type_vertex;

                ui->openGLWidget->typeProjection = config.projection;

            }
            // сделать полную чистку памяти!!!!!!!!!!
            free(object.arr_v);
            free(object.arr_v_copy);
            free(object.arr_f);
            free(object.arr_vf);
            for (int i = 0; i < object.v_count; i++) {
                free(object.matr_v[i]);
            }
            free(object.matr_v);
            for (int i = 0; i < object.f_count; i++) {
                free(object.matr_f[i]);
            }
            free(object.matr_f);
        }
    }
}


QString MainWindow::address(QString str)
{
    QString path = QCoreApplication::applicationDirPath();
//    qDebug()<<path;
        QString c1 = ("b");
        QString c2 = ("u");
        QString c3 = ("i");
        QString c4 = ("l");
        QString c5 = ("d");

        for (int i = path.length()-1; i > 3; i--) {
//            qDebug()<<"===i==="<<i;
//            qDebug()<<"======="<<path[i];
//            qDebug()<<"==---=="<<path[i-1];
            if (path[i-4] == c1 && path[i-3] == c2 && path[i-2] == c3 && path[i-1] == c4 && path[i] == c5) {
                path.resize(i+1);
                break;
            }
    }
        path = path + str;
        return path;
}


// Определение максимальных и минимальных размеров модели по одной из осей для цетровки модели при загрузке
double MainWindow::alignModelOnScreen(double max, double min)
{
    double value;
    if ((max + min) > 0) {
        value = -(max + min) / 2;
    } else {
        value = (max + min) / 2;
    }
    return value;
}

// Определение имени из пути файла
std::string MainWindow::findName(QString filePath)
{
    std::string utf8_text = filePath.toUtf8().constData();
    std::string file = utf8_text.substr(utf8_text.rfind('/') + 1);
//    std::cout << "File name: " << file << std::endl;
    return file;
}

void MainWindow::on_pB_resetParam_clicked()  // кнопка сброса настроек
{
    ui->SpinBox_Scale->setValue(1);
    ui->SpinBox_X_mov->setValue(0.0);
    ui->SpinBox_Y_mov->setValue(0.0);
    ui->SpinBox_Z_mov->setValue(0.0);
    ui->SpinBox_X_rot->setValue(1);
    ui->SpinBox_Y_rot->setValue(1);
    ui->SpinBox_Z_rot->setValue(1);
    ui->SpinBox_X_rot->setValue(0);
    ui->SpinBox_Y_rot->setValue(0);
    ui->SpinBox_Z_rot->setValue(0);
    ui->spinBox_X_speedRotate->setValue(1.0);
    ui->spinBox_Y_speedRotate->setValue(1.0);
    ui->spinBox_Z_speedRotate->setValue(1.0);
    ui->spinBox_X_speedRotate->setValue(0.0);
    ui->spinBox_Y_speedRotate->setValue(0.0);
    ui->spinBox_Z_speedRotate->setValue(0.0);
    ui->slider_sizeLine->setValue(2);
    ui->slider_sizeVertex->setValue(2);
    ui->slider_sizeLine->setValue(1);
    ui->slider_sizeVertex->setValue(1);
    ui->comboBox_typeVertex->activated(0);
    ui->comboBox_typeVertex->activated(1);
    ui->comboBox_typeLine->activated(0);
    ui->comboBox_typeLine->activated(1);
    set_colorVertex_default();
    set_colorLine_default();
    set_colorBG_default();
}

void MainWindow::on_pB_colorVertex_clicked()
{
    ui->openGLWidget->allColor.colorVertex = QColorDialog::getColor(QColor(248, 44, 71, 255));  // (130, 212, 250, 255) - 0.51, 0.83, 0.98 // (248, 44, 71, 255) - 0.971832, 0.171923, 0.27657
    qDebug() << "Цвет вершин при нажатии на кнопку: " << ui->openGLWidget->allColor.colorVertex;
    if (!ui->openGLWidget->allColor.colorVertex.isValid()) {
        qDebug() << "Выбор цвета вершин отменён!";
    } else {
        ui->openGLWidget->allColor.colorVertexRed = ui->openGLWidget->allColor.colorVertex.redF();
        ui->openGLWidget->allColor.colorVertexGreen = ui->openGLWidget->allColor.colorVertex.greenF();
        ui->openGLWidget->allColor.colorVertexBlue = ui->openGLWidget->allColor.colorVertex.blueF();
        ui->openGLWidget->allColor.colorVertexAlpha = ui->openGLWidget->allColor.colorVertex.alphaF();
    }
}

void MainWindow::set_colorVertex_default()
{
    ui->openGLWidget->allColor.colorVertex = QColor(248, 44, 71, 255);  // 0.971832, 0.171923, 0.27657
    ui->openGLWidget->allColor.colorVertexRed = 0.971832;
    ui->openGLWidget->allColor.colorVertexGreen = 0.171923;
    ui->openGLWidget->allColor.colorVertexBlue = 0.27657;
    ui->openGLWidget->allColor.colorVertexAlpha = 1.0;
}


void MainWindow::on_pB_colorLine_clicked()
{
    ui->openGLWidget->allColor.colorLine = QColorDialog::getColor(QColor(187, 212, 250, 255));  // (187, 212, 250, 255) - 0.735, 0.830, 0.980
    qDebug() << "Цвет линий при нажатии на кнопку:  " << ui->openGLWidget->allColor.colorLine;
    if (!ui->openGLWidget->allColor.colorLine.isValid()) {
        qDebug() << "Выбор цвета линии отменён!";
    } else {
        ui->openGLWidget->allColor.colorLineRed = ui->openGLWidget->allColor.colorLine.redF();
        ui->openGLWidget->allColor.colorLineGreen = ui->openGLWidget->allColor.colorLine.greenF();
        ui->openGLWidget->allColor.colorLineBlue = ui->openGLWidget->allColor.colorLine.blueF();
        ui->openGLWidget->allColor.colorLineAlpha = ui->openGLWidget->allColor.colorLine.alphaF();
    }
    //    glColor3d(0.4, 1.0, 0.4);  // Параметр линии: Цвет линии - мягкий зелёный
    //    glColor3d(1.0, 1.0, 0.4);  // Параметр линии: Цвет линии - мягкий жёлтый
    //    glColor3d(0.4, 1.0, 1.0);  // Параметр линии: Цвет линии - мягкий голубой
    //    glColor3d(0.4, 0.4, 1.0);  // Параметр линии: Цвет линии - мягкий фиолетово-синий
    //    glColor3d(1.0, 0.4, 1.0);  // Параметр линии: Цвет линии - мягкий фиолетово-красный
    //    glColor3d(1.0, 0.4, 0.4);  // Параметр линии: Цвет линии - мягкий красный
    //    glColor3d(0.51, 0.83, 0.98);  // Параметр линии: Цвет линии - мягкий фиолетово-красный
}

void MainWindow::set_colorLine_default()
{
    ui->openGLWidget->allColor.colorLine = QColor(187, 212, 250, 255);  // 0.735, 0.830, 0.980
    ui->openGLWidget->allColor.colorLineRed = 0.735;
    ui->openGLWidget->allColor.colorLineGreen = 0.830;
    ui->openGLWidget->allColor.colorLineBlue = 0.980;
    ui->openGLWidget->allColor.colorLineAlpha = 1.0;
}


void MainWindow::on_pB_colorBG_clicked()
{
    ui->openGLWidget->allColor.colorBG = QColorDialog::getColor(QColor(38, 38, 38, 255));  // 0.15, 0.15, 0.15
    qDebug() << "Цвет фона при нажатии на кнопку:    " << ui->openGLWidget->allColor.colorBG;
    if (!ui->openGLWidget->allColor.colorBG.isValid()) {
        qDebug() << "Выбор цвета фона отменён!";
    } else {
        ui->openGLWidget->allColor.colorBGRed = ui->openGLWidget->allColor.colorBG.redF();
        ui->openGLWidget->allColor.colorBGGreen = ui->openGLWidget->allColor.colorBG.greenF();
        ui->openGLWidget->allColor.colorBGBlue = ui->openGLWidget->allColor.colorBG.blueF();
        ui->openGLWidget->allColor.colorBGAlpha = ui->openGLWidget->allColor.colorBG.alphaF();
    }
}

void MainWindow::set_colorBG_default()
{
    ui->openGLWidget->allColor.colorBG = QColor(38, 38, 38, 255);  // 0.971832, 0.171923, 0.27657
    ui->openGLWidget->allColor.colorBGRed = 0.15;
    ui->openGLWidget->allColor.colorBGGreen = 0.15;
    ui->openGLWidget->allColor.colorBGBlue = 0.15;
    ui->openGLWidget->allColor.colorBGAlpha = 1.0;
}

// Создание screenShot'а
void MainWindow::on_pB_screenShot_clicked()
{
    // https://doc.qt.io/qt-5/qtwidgets-desktop-screenshot-example.html создание screenShot'а
    qDebug() << "push screenShot";

    // Определение размеров окна виджета openGLWidget
    const QRect rect(0, 0, ui->openGLWidget->width(), ui->openGLWidget->height());
    QPixmap pixmap = ui->openGLWidget->grab(rect);
    qDebug() << "pixmap = " << pixmap;

    // Создание имени файла с которым будет загружаться меню сохранения файла screenShot'а
    const QString format = "png";
    QString initialPath = QStandardPaths::writableLocation(QStandardPaths::PicturesLocation);
    if (initialPath.isEmpty()) {
        initialPath = QDir::currentPath();
    }
    initialPath += tr("/untitled.") + format;

    // Вызов диалогового окна "сохранить как"
    QFileDialog fileDialog(this, tr("Save As"), initialPath);
    fileDialog.setAcceptMode(QFileDialog::AcceptSave);
    fileDialog.setFileMode(QFileDialog::AnyFile);
    fileDialog.setDirectory(initialPath);

    //
    QStringList mimeTypes;  // контейнер - список строк,  http://blog.kislenko.net/show.php?id=1717
    const QList<QByteArray> baMimeTypes = QImageWriter::supportedMimeTypes();
    for (const QByteArray &bf : baMimeTypes) {
        mimeTypes.append(QLatin1String(bf));
    }
    fileDialog.setMimeTypeFilters(mimeTypes);
    fileDialog.selectMimeTypeFilter("image/" + format);
    fileDialog.setDefaultSuffix(format);
    if (fileDialog.exec() != QDialog::Accepted) {
        return;
    }
    const QString fileName = fileDialog.selectedFiles().first();
    if (!pixmap.save(fileName)) {
        QMessageBox::warning(this, tr("Ошибка при сохранении"), tr("Снимок экрана не сохранён saved to \"%1\".")
                             .arg(QDir::toNativeSeparators(fileName)));
    }
}


void MainWindow::on_pB_GIF_clicked()
{
    // Руководство пользователя ImageMagick v. 7.1.0 [Иван Георгиевич Титаренко] - книга по работе с цветами
    // https://itnotesblog.ru/note/qmovie-gif-animaciya-v-qt - как сделать gif-animation
    // https://github.com/dbzhang800/QtGifImage/blob/master/README.md - библиотека для создания GIF'ок
    qDebug() << "push GIF";
    QString filter = "*.gif";
    file_name = "/0";
    file_name = QFileDialog::getSaveFileName(this, "Сохранить GIF", QDir::homePath(), filter);

    ptr_save_file = new QFile(file_name);
    if (!ptr_save_file->open(QIODevice::WriteOnly)) {
        QMessageBox::critical(this, "Warning", "Не удалось записать файл");
    } else {
        start_timer_gif();
    }
}

void MainWindow::start_timer_gif() {
//    const QRect rect(0, 0, ui->openGLWidget->width(), ui->openGLWidget->height());
    gif = new QGifImage(QSize(640, 480));
    timerGIF = new QTimer(this);
    connect(timerGIF, &QTimer::timeout, this, &MainWindow::record_gif);
    timerGIF->start(100);
    frame = 0;
}

void MainWindow::record_gif() {
    qDebug() << ui->openGLWidget->width();
    qDebug() << ui->openGLWidget->height();
    const QRect rect(0, 0, 640, 480);
    QPixmap pixmap = ui->openGLWidget->grab(rect);

    qDebug() << "pixmap = " << pixmap;

    QImage myImage = pixmap.toImage();
    frame++;
    gif->addFrame(myImage, 100);  // цифра это задержка
    if (frame == 50) {
        timerGIF->stop();
        gif->save(ptr_save_file);
        ptr_save_file->close();
        delete ptr_save_file;
        ptr_save_file = nullptr;
        delete gif;
        gif = nullptr;
        QMessageBox::about(this, "Статус", "GIF сохранена");
    }
}


void MainWindow::timeUpdate()
{
    QString tempSizeW;
    ui->label_wWidth->setText(tempSizeW.setNum(ui->openGLWidget->sizeWinWidgetW));
    ui->label_wHight->setText(tempSizeW.setNum(ui->openGLWidget->sizeWinWidgetH));
}
