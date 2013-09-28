TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += \
    tests/unfused.cpp \
    tests/main.cpp

HEADERS += \
    examples/SeqSort/sort.hpp \
    include/async/async_fwd.hpp \
    include/async/async.hpp \
    include/async/type_traits.hpp

INCLUDEPATH += include
