TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += \
    examples/SeqSort/sort.cpp \
    tests/test_async.cpp \
    tests/test_main.cpp

HEADERS += \
    examples/SeqSort/sort.hpp \
    include/async/async_fwd.hpp \
    include/async/async.hpp \
    include/async/type_traits.hpp \
    include/async/preprocessor/iterate_define.hpp

INCLUDEPATH += include

LIBS += -lboost_unit_test_framework
LIBS += -lboost_system

QMAKE_CXXFLAGS += -std=c++11 -g
