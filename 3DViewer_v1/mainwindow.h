#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <math.h>

#include <QColor>
#include <QColorDialog>
#include <QDebug>
#include <QDesktopServices>
#include <QFileDialog>
#include <QMainWindow>
#include <QTimer>
#include <QUrl>
#include <QtOpenGL/QtOpenGL>
#include <iostream>

#include "lib/3rdParty/giflib/gif_lib.h"
#include "lib/gifimage/qgifimage.h"

extern "C" {
#include "affine_transformations.h"
#include "config.h"
#include "pars.h"
}

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow {
  Q_OBJECT

 public:
  explicit MainWindow(QWidget* parent = nullptr);
  ~MainWindow();

 private slots:

  int count_e(int* value, int cnt);

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

  void on_spinBox_X_speedRotate_valueChanged(double arg1);

  void on_spinBox_Y_speedRotate_valueChanged(double arg1);

  void on_spinBox_Z_speedRotate_valueChanged(double arg1);

  void on_spinBox_sizeLine_valueChanged(int arg1);

  void on_spinBox_sizeVertex_valueChanged(int arg1);

  void on_slider_sizeLine_valueChanged(int value);

  void on_slider_sizeVertex_valueChanged(int value);

  void on_pB_openFile_clicked();

  void on_pB_resetParam_clicked();

  void on_comboBox_projection_activated(int index);

  void on_comboBox_typeLine_activated(int index);

  void on_comboBox_typeVertex_currentIndexChanged(int index);

  void on_action_exit_triggered();

  void on_action_openFile_triggered();

  QString address(QString str);

  void on_action_saveFile_triggered();

  void on_action_info_triggered();

  void on_action_infoProgram_triggered();

  void on_pB_colorVertex_clicked();

  void on_pB_colorLine_clicked();

  void on_pB_colorBG_clicked();

  void set_colorVertex_default();

  void set_colorLine_default();

  void set_colorBG_default();

  std::string findName(QString filePath);

  double alignModelOnScreen(double max, double min);

  void on_pB_screenShot_clicked();

  void on_pB_GIF_clicked();

  void on_action_screenShot_triggered();

  void on_action_GIF_triggered();

  void start_timer_gif();

  void record_gif();

  void timeUpdate();

 private:
  Ui::MainWindow* ui;

  QTimer timerUi;

  QString file_name;
  QTimer* timerGIF;  // идентификатор таймера
  int frame;
  QGifImage* gif;
  QFile* ptr_save_file;
};

#endif  // MAINWINDOW_H
