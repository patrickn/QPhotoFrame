## Raspberry Pi Photo Frame

* Qt 5.13.2
* Raspberry Pi 3+
* Official 7" Touchscreen

For information about cross compiling for Raspberry Pi using Qt see the article at https://mechatronicsblog.com/cross-compile-and-deploy-qt-5-12-for-raspberry-pi/


### Dependencies

#### Qt

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




### Auto startup

There is a _systemd_ startup script in the _startup_ directory. To use, copy to _/lib/systemd/system/_ on the Pi then execute the following commands:

```
   $ sudo systemctl daemon-reload

   $ sudo systemctl enable qphotoframe.service
```

### Raspberry Pi Environment

```
export QT_QPA_EGLFS_PHYSICAL_WIDTH=155
export QT_QPA_EGLFS_PHYSICAL_HEIGHT=86
export QT_QPA_EGLFS_HIDECURSOR=1
```

### Troubleshooting

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


