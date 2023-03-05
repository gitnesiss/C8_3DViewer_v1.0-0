#ifndef OBJECT3D_H
#define OBJECT3D_H

#include <QOpenGLBuffer>
#include <QMatrix4x4>  // нужна чтобы хранить в объекте модельную матрицу
#include <QVector2D>

class QOpenGLTexture;
class QOpenGLFunctions;
class QOpenGLShaderProgram;

// объявляем структуру с информацией о вершинах
struct VertexData
{
    VertexData()
    {
    }
    VertexData(QVector3D p) :
        position(p)
    {
    }
    VertexData(QVector3D p, QVector2D t, QVector3D n) :
        position(p), texCoord(t), normal(n)
    {
    }
    QVector3D position;
    QVector2D texCoord;
    QVector3D normal;
};

class Object3D
{
public:
    Object3D();
    Object3D(const QVector<VertexData> &vertData, const QVector<GLuint> &indexes, const QImage &texture);
    ~Object3D();
    void init(const QVector<VertexData> &vertData, const QVector<GLuint> &indexes, const QImage &texture);
    void draw(QOpenGLShaderProgram *program, QOpenGLFunctions *functions);
    void translate(const QVector3D &translateVector);

private:
    QOpenGLTexture *m_texture;
    QOpenGLBuffer m_vertexBuffer;   // вершинный буфер
    QOpenGLBuffer m_indexBuffer;    // индексный буфер
    QMatrix4x4 m_modelMatrix;       // объявляем модельную матрицу

};

#endif // OBJECT3D_H
