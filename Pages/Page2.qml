import QtQuick 2.12
import QtQuick.Controls 2.5

Item {
   width: 600
   height: 400

   Label {
      text: qsTr("Page 2")
      font.pixelSize: Qt.application.font.pixelSize * 2
      padding: 10
   }

   Label {
      text: qsTr("You are on Page 2.")
      anchors.centerIn: parent
   }
}
