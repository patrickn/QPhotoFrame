## Raspberry Pi Photo Frame

* Qt 5.12.4
* Raspberry Pi 3+
* Official 7" Touchscreen

For information about cross compiling for Raspberry Pi using Qt see the article at https://mechatronicsblog.com/cross-compile-and-deploy-qt-5-12-for-raspberry-pi/


### Auto start

There is a _systemd_ startup script in the _startup_ directory. To use, copy to _/lib/systemd/system/_ on the Pi then execute the following commands:

```
   $ sudo systemctl daemon-reload

   $ sudo systemctl enable qphotoframe.service
```

### Troubleshooting

If the touchscreen is not working make sure that _/dev/input/event*_ is readable by the user running the app.

Hide the mouse cursor:

```
export QT_QPA_EGLFS_HIDECURSOR=1
```


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


