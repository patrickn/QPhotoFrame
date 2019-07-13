## Raspberry Pi Photo Frame

* Qt 5.12.4
* Raspberry Pi 3+
* Official 7" Touchscreen

If the touchscreen is not working make sure that _/dev/input/event*_ is readable by the user running the app.

Hide the mouse cursor:

```
export QT_QPA_EGLFS_HIDECURSOR=1
```


### Auto start

There is a _systemd_ startup script in the _startup_ directory. To use, copy to _/lib/systemd/system/_ on the Pi then execute the following commands:

```
   $ sudo systemctl daemon-reload

   $ sudo systemctl enable qphotoframe.service
```

