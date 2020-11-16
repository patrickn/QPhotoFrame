import QtQuick 2.0
import QtQuick.Controls 2.12
import QtGraphicalEffects 1.0

Popup {
   id: popup

   property var data: []

   x: 20
   y: 20

   implicitHeight: 100
   implicitWidth: 140

   height: content.implicitHeight
   width: content.implicitWidth

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
      anchors {
         topMargin: 10
         leftMargin: 25
      }

      implicitHeight: childrenRect.height + 25
      implicitWidth: childrenRect.width + 35

      Column {
         id: imageDataColumn

         Repeater {
            model: ["Name:", "Date:", "Time:", "Lat:", "Lon:", "Alt:"]

            Row {
               id: imageDataRow

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
