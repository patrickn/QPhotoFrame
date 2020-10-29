import QtQuick 2.12
import QtQuick.Controls 2.12
import QtGraphicalEffects 1.12
import QtQml 2.12

Item {

   Rectangle {
      id: screenBorder
      radius: 5
      anchors.rightMargin: 3
      anchors.leftMargin: 3
      anchors.bottomMargin: 3
      anchors.topMargin: 3
      anchors.fill: parent
      border.width: 2
      border.color: "#fb9797"
      color: "transparent"

      Row {
         id: leftSide
         width: parent.width
         anchors.bottom: parent.bottom
         anchors.bottomMargin: 3
         anchors.top: parent.top
         anchors.topMargin: 3
         anchors.left: parent.left
         anchors.leftMargin: 3

         Image {
            property var sourceImage
            id: image
            fillMode: Image.PreserveAspectCrop
            sourceImage: getSourceImage()
            source: sourceImage
            layer.enabled: true
            layer.effect: OpacityMask {
               maskSource: mask
            }

            function getSourceImage() {
               return "https://placekeanu.com/" + parent.width + "/" + parent.height
            }
         }

         Rectangle {
            id: mask
            width: parent.width
            height: parent.height
            radius: 5
            visible: false
         }

         Timer {
            interval: 5000
            repeat: true
            running: true
            onTriggered: { image.sourceImage = image.getSourceImage() }
         }
      }
   }
}
