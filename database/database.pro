######################################################################
# Automatically generated by qmake (3.0) Sat Jan 3 11:28:00 2015
######################################################################

TEMPLATE = app
TARGET = database
INCLUDEPATH += .
QMAKE_CXXFLAGS += -std=c++0x
LIBS += -lboost_regex -lboost_date_time
OBJECTS_DIR = obj

# Input
SOURCES += src/main.cpp \
    src/database.cpp

HEADERS += \
    src/database.h

OTHER_FILES += data/employees.txt \
               database.dox

DISTFILES += $${OTHER_FILES}
