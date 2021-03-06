import QtQuick 2.12
import QtQuick.Controls 2.12
import QtQuick.Layouts 1.12

import QPhotoFrame 1.0

Item {
   anchors.fill: parent

   ListView {
      id: displayListView

      anchors.fill: parent
      spacing: 2
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
               width: index === 0 ? 90 : (index === 1 ? 230 : 70)
               background: Rectangle { color: "silver" }
            }
         }
      }

      model: imageService.imageList
      ScrollIndicator.vertical: ScrollIndicator {}

      delegate: Grid {
         id: grid
         columns: 3
         rows: 1
//         width: 400
         flow: GridLayout.LeftToRight
         spacing: 5

         Image {
            width: 90; height: 60
            fillMode: Image.PreserveAspectCrop
            source: modelData.url
         }

         Text {
            Layout.alignment: Qt.AlignVCenter
            text: modelData.name
            width: 230
            elide: Text.ElideRight
         }

         Text {
            Layout.alignment: Qt.AlignRight
            text: modelData.useCount
            font.pixelSize: 16
            width: 70
         }
      }
   }
}
