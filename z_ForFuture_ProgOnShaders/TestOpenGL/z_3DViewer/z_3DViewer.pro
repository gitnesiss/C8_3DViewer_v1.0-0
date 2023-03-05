macx {  #win32, macx, unix
    QT       += core gui openglwidgets
    LIBS     += -framework GLUT -framework OpenGL -framework Cocoa
} else {
    QT       += core gui opengl
    LIBS     += -lOpenGL -lGLU -lglut
}

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    affine_transformations.c \
    main.cpp \
    mainwindow.cpp \
    object3d.cpp \
    pars.c \
    winopengl.cpp

HEADERS += \
    affine_transformations.h \
    mainwindow.h \
    object3d.h \
    pars.h \
    winopengl.h

FORMS += \
    mainwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

DISTFILES +=

RESOURCES += \
    shaders.qrc \
    textures.qrc

win32-g* {
    LIBS += -lopengl32
}

win32-msvc* {
    LIBS += opengl32.lib
}
