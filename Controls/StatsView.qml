import QtQuick 2.12
import QtQuick.Layouts 1.12
import QtQuick.Controls 2.12

import QPhotoFrame 1.0

ListView {
   anchors.fill: parent
   spacing: 0
   clip: true

   contentWidth: headerItem.width
   flickableDirection: Flickable.VerticalFlick

   header: Row {
      spacing: 2
      function itemAt(index) { return repeater.itemAt(index) }
      Repeater {
         id: repeater
         model: ["Image", "Name", "Use Count"]
         Label {
            text: modelData
            font.bold: true
            padding: 2
            width: 120
            background: Rectangle { color: "silver" }
         }
      }
   }

   model: imageService

   delegate: Grid {
      id: grid
      columns: 3
      rows: 1
      width: 300
      flow: GridLayout.LeftToRight
      spacing: 5

      Image {
         width: 100; height: 70
         fillMode: Image.PreserveAspectFit
         source: imageRole
      }

      Text {
         Layout.alignment: Qt.AlignVCenter
         text: nameRole
         width: 100
      }

      Text {
         Layout.alignment: Qt.AlignRight
         text: useCountRole
         font.pixelSize: 16
         width: 100
      }
   }
   ScrollIndicator.vertical: ScrollIndicator {}
}
