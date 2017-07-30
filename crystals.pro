QT += gui core waylandcompositor waylandcompositor


HEADERS += \
    window.h \
    compositor.h

SOURCES += main.cpp \
    window.cpp \
    compositor.cpp

DISTFILES += \
    fshader.glsl \
    vshader.glsl

RESOURCES += \
    res.qrc



