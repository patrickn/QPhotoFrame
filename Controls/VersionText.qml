import QtQuick 2.0

Text {
   property var version: ""

   anchors.top: parent.top
   anchors.right: parent.right
   anchors.rightMargin: 10
   anchors.topMargin: 3
   text: "QPhotoFrame Version: v" + version
   font.pointSize: 7
   font.family: "Helvetica"
   color: "gray"
}