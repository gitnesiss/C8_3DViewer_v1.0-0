#include "object3d.h"
#include <QOpenGLTexture>
#include <QOpenGLFunctions>
#include <QOpenGLShaderProgram>

Object3D::Object3D() :
    m_indexBuffer(QOpenGLBuffer::IndexBuffer), m_texture(0)
{

}


Object3D::Object3D(const QVector<VertexData> &vertData, const QVector<GLuint> &indexes, const QImage &texture) :
    m_indexBuffer(QOpenGLBuffer::IndexBuffer), m_texture(0)
{
    init(vertData, indexes, texture);
}

Object3D::~Object3D()  // очистка данных из видеопамяти - освобождение ресурсов
{
    if (m_vertexBuffer.isCreated()) {
        m_vertexBuffer.destroy();
    }
    if (m_indexBuffer.isCreated()) {
        m_indexBuffer.destroy();
    }
    if (m_texture != 0) {
        if (m_texture->isCreated()) {
            m_texture->destroy();
        }
    }
}

void Object3D::init(const QVector<VertexData> &vertData, const QVector<GLuint> &indexes, const QImage &texture)
{
    if (m_vertexBuffer.isCreated()) {
        m_vertexBuffer.destroy();
    }
    if (m_indexBuffer.isCreated()) {
        m_indexBuffer.destroy();
    }
    if (m_texture != 0) {
        if (m_texture->isCreated()) {
            delete m_texture;
            m_texture = 0;
        }

    }

    m_vertexBuffer.create();
    m_vertexBuffer.bind();
    m_vertexBuffer.allocate(vertData.constData(), vertData.size() * sizeof(VertexData));
    m_vertexBuffer.release();

    m_indexBuffer.create();
    m_indexBuffer.bind();
    m_indexBuffer.allocate(indexes.constData(), indexes.size() * sizeof(GLuint));
    m_indexBuffer.release();

    // инициализируем текстуру
    m_texture = new QOpenGLTexture(texture.mirrored());

    // Фильтрация текстур
    m_texture->setMinificationFilter(QOpenGLTexture::Nearest);
    m_texture->setMinificationFilter(QOpenGLTexture::Linear);
    m_texture->setWrapMode(QOpenGLTexture::Repeat);

    m_modelMatrix.setToIdentity();  // делаем модельную матрицу еденичной
}

void Object3D::draw(QOpenGLShaderProgram *program, QOpenGLFunctions *functions)
{
    if (!m_vertexBuffer.isCreated() || !m_indexBuffer.isCreated()) return;

    // Чтобы была возможность воспользоваться текстурой её надо забиндить. Биндим её нулевым номером
    m_texture->bind(0);
    // передаём ещё одно значение в шейдер: переменная текстуры, ставим 0 - номер текстуры который будет отрисовываться
    program->setUniformValue("u_texture", 0);
    // присваиваем юниформ значение: присваиваем модельную матрицу
    program->setUniformValue("u_modelMatrix", m_modelMatrix);

    m_vertexBuffer.bind();

    int offset = 0;

    // Осталось шейдеру задать аттрибуты
    int  vertLoc = program->attributeLocation("a_position");  // получаем расположение аттрибутов
    program->enableAttributeArray(vertLoc);  // включаем использование этого аттрибута
    program->setAttributeBuffer(vertLoc, GL_FLOAT, offset, 3, sizeof(VertexData));

    offset += sizeof(QVector3D);

    // Осталось шейдеру задать аттрибуты
    int  texLoc = program->attributeLocation("a_texcoord");  // получаем расположение аттрибутов
    program->enableAttributeArray(texLoc);  // включаем использование этого аттрибута
    program->setAttributeBuffer(texLoc, GL_FLOAT, offset, 2, sizeof(VertexData));

    offset += sizeof(QVector2D);

    int  normLoc = program->attributeLocation("a_normal");  // получаем расположение аттрибутов
    program->enableAttributeArray(normLoc);  // включаем использование этого аттрибута
    program->setAttributeBuffer(normLoc, GL_FLOAT, offset, 3, sizeof(VertexData));

    // биндим сам индексный буффер, его можно биндить прям перед отображением
    m_indexBuffer.bind();

    functions->glDrawElements(GL_TRIANGLES, m_indexBuffer.size(), GL_UNSIGNED_INT, 0);  // 0 стоит потому что используется openGLBuffer

    m_vertexBuffer.release();
    m_indexBuffer.release();
    m_texture->release();
}

void Object3D::translate(const QVector3D &translateVector)
{
    m_modelMatrix.translate(translateVector);
}
