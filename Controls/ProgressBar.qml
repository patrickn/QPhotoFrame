import QtQuick 2.12
import QtQuick.Controls 2.12

ProgressBar {
    id: control
    value: 0.5
    padding: 1

    background: Rectangle {
        implicitWidth: 200
        implicitHeight: 6
        color: "#e6e6e6"
        radius: 3
    }

    contentItem: Item {
        implicitWidth: 200
        implicitHeight: 4

        Rectangle {
            width: control.visualPosition * parent.width
            height: parent.height
            radius: 2
            color: "#17a81a"
        }
    }
}
