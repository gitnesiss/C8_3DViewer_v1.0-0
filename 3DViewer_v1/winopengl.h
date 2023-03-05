#ifndef WINOPENGL_H
#define WINOPENGL_H

#include <math.h>

#include <QOpenGLBuffer>
#include <QOpenGLExtraFunctions>
#include <QOpenGLFunctions>
#include <QOpenGLShaderProgram>
#include <QOpenGLVertexArrayObject>
#include <QOpenGLWidget>
#include <QTimer>
#include <QWidget>

#include "mainwindow.h"

typedef struct colorStruct {
  QColor colorVertex;
  QColor colorLine;
  QColor colorBG;

  double colorVertexRed;
  double colorVertexGreen;
  double colorVertexBlue;
  double colorVertexAlpha;

  double colorLineRed;
  double colorLineGreen;
  double colorLineBlue;
  double colorLineAlpha;

  double colorBGRed;
  double colorBGGreen;
  double colorBGBlue;
  double colorBGAlpha;
} colorData;

class WinOpenGL : public QOpenGLWidget, protected QOpenGLFunctions {
  Q_OBJECT
 public:
  WinOpenGL(QWidget *parent = 0);

  ~WinOpenGL();

  colorData allColor;

  int sizeWinWidgetW;
  int sizeWinWidgetH;

  double sizeX;
  double sizeY;
  double sizeZ;
  double fieldView;

  int flg;
  int v_count;

  double global_move_x;
  double global_move_y;
  double global_move_z;

  float frames_per_second;
  float one_frame;
  float angle_per_frame;

  QString filePath;

  double scale;

  int typeProjection;
  int oldTypeProjection;

  int mov_x;  // Движение вдоль оси X
  int mov_y;  // Движение вдоль оси Y
  int mov_z;  // Движение вдоль оси Z

  int rot_x;  // ручной поворот вокруг оси X
  int rot_y;  // ручной поворот вокруг оси Y
  int rot_z;  // ручной поворот вокруг оси Y

  double rotate_x;  // постоянное вращение вокруг оси X
  double rotate_y;  // постоянное вращение вокруг оси Y
  double rotate_z;  // постоянное вращение вокруг оси Z

  double angle_x;
  double angle_y;
  double angle_z;

  int sizeLine;
  int typeLine;
  int sizeVertex;
  int typeVertex;

  void changeViewPerspective();
  void customizeLineDisplay();
  void customizeVertexDisplay();
  void rotateDinamicModel();

  void paint_model();
  double sizeToAxis(double min,
                    double max);  // Определение размера модели по заданной оси
  double maxSizeToAxis(
      double x, double y,
      double z);  // Выбор максимально размера модели по одной из осей

 private:
  QTimer timer;  // переменная для таймера
  QPoint mPos;  // переменная для записи пололожения мышки

  void mousePressEvent(QMouseEvent *)
      override;  // функция вызываемая при нажатии на клавишу мышки
  void mouseMoveEvent(
      QMouseEvent *) override;  // функция вызываемая при движении мышки

  void initializeGL()
      override;  // используется для глобальных настроек построения изображения,
                 // которые нет необходимости указывать при построении кадра.
  void resizeGL(
      int w, int h) override;  // служит для перестроения размера окна. При
                               // изменении размера окна запускается этот метод.
  void paintGL() override;  // этот метод будет выстраивать каждый наш кадр для
                            // отображения.

  double *Vertex;
};

#endif  // WINOPENGL_H
