import QtQuick 2.0
import QtQuick.Controls 2.12
import QtGraphicalEffects 1.0

Popup {
   id: popup

   property var data: []

   x: 20
   y: 20
   width: imageData.width
   height: 150
   modal: true
   focus: true
   closePolicy: Popup.CloseOnEscape | Popup.CloseOnPressOutside

   leftPadding: 0;
   rightPadding: 0;
   topPadding: 0;
   bottomPadding: 0;
   visible: false
   opacity: 0.75

   background: Rectangle {
      id: background
      anchors.fill: parent
      color: "gray"
      radius: 7
      antialiasing: true

      layer.enabled: true
      layer.effect: DropShadow {
         id: rectShadow
         anchors.fill: source
         cached: true
         horizontalOffset: 5
         verticalOffset: 5
         transparentBorder: true
         radius: 8.0
         samples: 16
         color: "#80000000"
         smooth: true
         source: background
      }
   }

   Rectangle {
      id: content
      anchors.fill: parent
      color: "transparent"
      anchors.margins: 10

      Column {
         id: imageData

         Repeater {
            model: ["Name:", "Date:", "Time:", "Lat:", "Lon:", "Alt:"]

            Row {
               spacing: 10
               Text {
                  text: modelData
                  width: 40
                  horizontalAlignment: Text.AlignRight
                  color: "white"
                  font.italic: true
               }

               Text {
                  text: popup.data[index]
                  color: "white"
               }
            }
         }
      }
   }
}
