TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += \
    tests/unfused.cpp \
    tests/main.cpp \
    examples/SeqSort/sort.cpp

HEADERS += \
    examples/SeqSort/sort.hpp \
    include/async/async_fwd.hpp \
    include/async/async.hpp \
    include/async/type_traits.hpp \
    include/async/preprocessor/iterate_define.hpp

INCLUDEPATH += include

LIBS += -lboost_unit_test_framework

#QMAKE_CXXFLAGS += -E
