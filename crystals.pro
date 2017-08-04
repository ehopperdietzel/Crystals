QT += gui network core waylandcompositor


HEADERS += \
    window.h \
    compositor.h \
    background.h \
    headers.h \
    view.h \
    server.h \
    socket.h

SOURCES += main.cpp \
    window.cpp \
    compositor.cpp \
    background.cpp \
    view.cpp \
    server.cpp \
    socket.cpp

DISTFILES += \
    fshader.glsl \
    vshader.glsl

RESOURCES += \
    res.qrc



