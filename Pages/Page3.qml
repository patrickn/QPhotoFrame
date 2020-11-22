import QtQuick 2.12
import QtQuick.Layouts 1.12
import QtQuick.Controls 2.12
import QtGraphicalEffects 1.12
import QtQml 2.12

import QPhotoFrame 1.0
import  "../Controls"

Item {
   id: page3

   readonly property int margin: 3

   Rectangle {
      id: screenBorder
      radius: 5
      anchors.rightMargin: page3.margin
      anchors.leftMargin: page3.margin
      anchors.bottomMargin: page3.margin
      anchors.topMargin: page3.margin
      anchors.fill: parent
      border.width: 2
      border.color: "#fb9797"
      color: "transparent"

      ListView {
         anchors.fill: parent
         spacing: 1
         clip: true

         contentWidth: headerItem.width
         flickableDirection: Flickable.VerticalFlick

         header: Row {
            spacing: 5
            function itemAt(index) { return repeater.itemAt(index) }
            Repeater {
               id: repeater
               model: ["Image", "Name", "Use Count"]
               Label {
                  text: modelData
                  font.bold: true
                  padding: 10
                  width: 100
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
   }
}
