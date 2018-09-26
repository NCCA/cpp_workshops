TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle
CONFIG -= qt
QMAKE_CXXFLAGS += -std=c++0x
OBJECTS_DIR = obj

SOURCES += src/main.cpp \
    src/linearallocator.cpp

HEADERS += \
    src/linearallocator.h

OTHER_FILES += poolalloc.dox
DISTFILES += $${OTHER_FILES}
