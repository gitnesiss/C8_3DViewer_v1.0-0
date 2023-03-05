macx {  #win32, macx, unix
    QT       += core gui openglwidgets
    LIBS     += -framework GLUT -framework OpenGL -framework Cocoa
} else {
    QT       += core gui opengl
    LIBS     += -lOpenGL -lGLU -lglut
}

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    affine_transformations.c \
    config.c \
    main.cpp \
    mainwindow.cpp \
    pars.c \
    winopengl.cpp \
    lib/3rdParty/giflib/dgif_lib.c \
    lib/3rdParty/giflib/egif_lib.c \
    lib/3rdParty/giflib/gif_err.c \
    lib/3rdParty/giflib/gif_font.c \
    lib/3rdParty/giflib/gif_hash.c \
    lib/3rdParty/giflib/gifalloc.c \
    lib/3rdParty/giflib/quantize.c \
    lib/gifimage/qgifimage.cpp

HEADERS += \
    affine_transformations.h \
    config.h \
    mainwindow.h \
    pars.h \
    winopengl.h \
    lib/3rdParty/giflib/gif_hash.h \
    lib/3rdParty/giflib/gif_lib.h \
    lib/3rdParty/giflib/gif_lib_private.h \
    lib/gifimage/qgifglobal.h \
    lib/gifimage/qgifimage.h \
    lib/gifimage/qgifimage_p.h

FORMS += \
    mainwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

SUBDIRS += \
    lib/gifimage/gifimage.pro

DISTFILES += \
    lib/3rdParty/giflib/AUTHORS \
    lib/3rdParty/giflib/COPYING \
    lib/3rdParty/giflib/README \
