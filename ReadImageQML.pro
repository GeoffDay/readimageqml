QT += qml quick

CONFIG += c++11


HEADERS += readimage.h \
    dataobject.h
SOURCES += main.cpp \
        readimage.cpp \
    dataobject.cpp

RESOURCES += qml.qrc

DISTFILES += MyButton.qml \
    MyMain.qml \
    playback_begin.png \
    playback_end.png \
    rewind.svg \
    playback_begin_2.png \
    playback_play.png \
    playback_loop.png \
    playback_once.png \
    CheckButton.qml \
    MetaData.qml
