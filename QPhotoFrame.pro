CONFIG += c++11
QT += core network positioning qml quick
#requires(qtConfig(bearermanagement))


DEFINES += QT_DEPRECATED_WARNINGS

SOURCES += \
        WeatherModel/WeatherData.cpp \
        WeatherModel/WeatherModel.cpp \
        main.cpp

RESOURCES += qml.qrc

# Additional import path used to resolve QML modules in Qt Creator's code model
QML_IMPORT_PATH =

# Additional import path used to resolve QML modules just for Qt Quick Designer
QML_DESIGNER_IMPORT_PATH =

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

HEADERS += \
    WeatherModel/WeatherData.h \
    WeatherModel/WeatherModel.h

DISTFILES += \
   readme.md
