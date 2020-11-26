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
         model: ["Image", "Name", "#"]
         Label {
            text: modelData
            font.bold: true
            padding: 1
            width: index === 0 ? 100 : (index === 1 ? 210 : 50)
            background: Rectangle { color: "silver" }
         }
      }
   }

   model: sortFilterProxy

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
         width: 210
         elide: Text.ElideRight
      }

      Text {
         Layout.alignment: Qt.AlignRight
         text: useCountRole
         font.pixelSize: 16
         width: 50
      }
   }
   ScrollIndicator.vertical: ScrollIndicator {}
}
