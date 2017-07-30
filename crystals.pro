QT += gui core waylandcompositor waylandcompositor


HEADERS += \
    window.h \
    compositor.h \
    background.h \
    headers.h

SOURCES += main.cpp \
    window.cpp \
    compositor.cpp \
    background.cpp

DISTFILES += \
    fshader.glsl \
    vshader.glsl

RESOURCES += \
    res.qrc



