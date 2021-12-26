## Raspberry Pi Photo Frame

![QPhotoFrame Screen Shot](Assets/screenshot.png?raw=true "QPhotoFrame Screen Shot")

This started out as an exercise to learn a bit about Qt/QML, cross compiling for the Raspberry Pi and to do something with the Touchscreen I had lying around. It downloads and displays images from [my travel website](https://www.neavey.net/) and although the URL is currently hard coded it is relatively straightforward change it. You just need to create a JSON file containing URLs to the images (I [use a script to do this](https://github.com/patrickn/neaveynet-hugo/blob/main/image-search.py) whenever my static site is generated).

The top right corner displays the local time, location and external temperature from the web and the internal temperature read via a [DS18B20](https://datasheets.maximintegrated.com/en/ds/DS18B20.pdf) digital thermometer. The main image is randomly updated at regular intervals and the location read from the EXIF data and displayed on the map.

### Requirements

* Qt 5.13.2
* Raspberry Pi 3+
* Official 7" Touchscreen

For information about cross compiling for Raspberry Pi using Qt see the article at https://mechatronicsblog.com/cross-compile-and-deploy-qt-5-12-for-raspberry-pi/


### Dependencies

#### Qt/QML

The following additional Qt modules are needed:

* qtdeclarative
* qtgraphicaleffects
* qtlocation
* qtquickcontrols2
* qtsvg

From https://wiki.qt.io/RaspberryPi2EGLFS

```
git clone git://code.qt.io/qt/<qt-module>.git -b <qt-version>
cd <qt-module>

~/raspi/qt5/bin/qmake
make
make install
```

Then deploy new files by running:

```
rsync -avz qt5pi pi@raspberrypi.local:/usr/local
```

#### OS

Apart from those from the article referenced above, you will need to install the following packages using:

```
   $ sudo apt install <package-name>
```

* geoclue-2.0

#### Fonts

True type fonts (ttf) should be copied to _~/raspi/qt5pi/lib/fonts_ directory, then deployed via the rsync command shown above. 


### Auto startup

There is a _systemd_ startup script in the _startup_ directory. To use, copy to _/lib/systemd/system/_ on the Pi then execute the following commands:

```
   $ sudo systemctl daemon-reload

   $ sudo systemctl enable qphotoframe.service
```

### APP ID Key

The external temperature and the current location is uses data provided by [OpenWeather](openweathermap.org). To use the service you will need to register for free and get an APP ID. Then set the environment variable QPF_WEATHERDATA_APPID to the APP ID, ie:

```
 export QPF_WEATHERDATA_APPID="lkcbqn9872335hjdne98sdf9f98skjsr"
```

### Raspberry Pi Environment

```
export QT_QPA_EGLFS_PHYSICAL_WIDTH=155
export QT_QPA_EGLFS_PHYSICAL_HEIGHT=86
export QT_QPA_EGLFS_HIDECURSOR=1
```

### Controlling the TouchScreen

The official Raspberry Pi 7" touchscreen creates a virtual filesystem _/sys/class/backlight/rpi_backlight_, which can be used to control the touchscreen. For example, to set the screen brightness from 0 (dark) to 255 (full brightness):

```
sudo sh -c 'echo "128" > /sys/class/backlight/rpi_backlight/brightness'
```

To turn on/off the backlight:

```
sudo sh -c 'echo "0" > /sys/class/backlight/rpi_backlight/bl_power'
sudo sh -c 'echo "1" > /sys/class/backlight/rpi_backlight/bl_power'
```

### Troubleshooting

#### Logging

By default only critical errors and warnings are logged, the levels are:

* debug
* warning
* critical
* information
* fatal

And the categories that can be logged are:

* qpf.img         // image
* qpf.img.svc     // image service
* qpf.temp.mod    // temperature module
* qpf.wthr.data   // weather data
* qpf.wthr.mod    // weather module

Logging can be set through the environment variable _QT_LOGGING_RULES_, and switching a particular category and level off or on. For example:

```
QT_LOGGING_RULES="qpf.*.debug=true;qpf.img.info=false"
```

For more on Qt logging see: https://doc.qt.io/qt-5/qloggingcategory.html


#### Touchscreen

If the touchscreen is not working make sure that _/dev/input/event*_ is readable by the user running the app.

After 'apt update' or any other changes you may start getting QOpenGLShader errors:

```
QOpenGLShaderProgram: could not create shader program
QOpenGLShader: could not create shader
QOpenGLShader: could not create shader
shader compilation failed:
```

It seems that the lib _/usr/lib/arm-linux-gnueabihf/libGLESv2.so.2_ is wrong and sometimes gets replaced when updating. The correct lib is _/opt/vc/lib/libbrcmGLESv2.so_. So:

```
    $ sudo mv /usr/lib/arm-linux-gnueabihf/libGLESv2.so.2 /usr/lib/arm-linux-gnueabihf/libGLESv2.so.2.backup

    $ sudo ln -s /opt/vc/lib/libbrcmGLESv2.so /usr/lib/arm-linux-gnueabihf/libGLESv2.so.2
```

#### External temperature not being displayed

[geoclue](https://gitlab.freedesktop.org/geoclue/geoclue/-/wikis/home) is a service that is used to provide positioning information to an application and is used by the Qt Positioning library. An update to this service caused a failure in the external temperature display, meaning it always displayed ?? as it could not resolve the location information required to retrieve the temperature.

Additionall you will see something like this in the log file: 

```
[26/12/2021 16:40:48]{crit:qt.positioning} Unable to start the client: "org.freedesktop.DBus.Error.AccessDenied" "'QPhotoFrame' disallowed, no agent for UID 1000"
```

To fix this issue you need to start the _geoclue_ agent at start up with a systemd script. As _root_, create the file _/lib/systemd/system/geoclue-agent.service_, containing the following:

```
[Unit]
Description=QPhotoFrame needs to get a (geo)clue

[Service]
User=pi
Group=pi
ExecStart=/usr/lib/geoclue-2.0/demos/agent

[Install]
WantedBy=default.target
```

Then enable the service by running:

```
    $ sudo systemctl enable geoclue-agent.service
```

Reboot and the external temperature should be displayed correctly.
