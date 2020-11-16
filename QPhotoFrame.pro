CONFIG += c++17
QT += core network positioning qml quick
#requires(qtConfig(bearermanagement))


DEFINES += QT_DEPRECATED_WARNINGS

SOURCES += \
        ImageService/Image.cpp \
        ImageService/ImageService.cpp \
        ImageService/easyexif/exif.cpp \
        TemperatureModule/DS18B20.cpp \
        TemperatureModule/TemperatureModule.cpp \
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
    ImageService/Image.h \
    ImageService/ImageService.h \
    ImageService/easyexif/exif.h \
    TemperatureModule/DS18B20.h \
    TemperatureModule/TemperatureModule.h \
    WeatherModel/WeatherData.h \
    WeatherModel/WeatherModel.h

DISTFILES += \
   readme.md
