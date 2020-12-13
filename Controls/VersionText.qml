
import QtQuick 2.12

Text {
   anchors.top: parent.top
   anchors.right: parent.right
   anchors.rightMargin: 10
   anchors.topMargin: 3
   text: "Version: " + "0.1.0+3" + " Built: " + Qt.formatDateTime(new Date(), "dd MMM yy hh:mm")
   font.pointSize: 7
   font.family: "Helvetica"
   color: "gray"
}
