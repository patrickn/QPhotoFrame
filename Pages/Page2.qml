import QtQuick 2.12
import QtGraphicalEffects 1.12
import QtQml 2.12

import "../Controls"

Item {
   id: page2
   readonly property int margin: 3
   readonly property int timerSeconds: 60 * 60 * 24 // 24 hours

   Rectangle {
      id: screenBorder
      radius: 5
      anchors.rightMargin: page2.margin
      anchors.leftMargin: page2.margin
      anchors.bottomMargin: page2.margin
      anchors.topMargin: page2.margin
      anchors.fill: parent
      border.width: 2
      border.color: "#fb9797"
      color: "transparent"

      Row {
         id: leftSide
         width: parent.width - (2 * page2.margin)
         anchors.bottom: parent.bottom
         anchors.bottomMargin: page2.margin
         anchors.top: parent.top
         anchors.topMargin: page2.margin
         anchors.left: parent.left
         anchors.leftMargin: page2.margin

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

            ImageProgress {
               id: progressBar
               height: 5
               anchors.right: parent.right
               anchors.left: parent.left
               anchors.bottom: parent.bottom
               anchors.leftMargin: 2 * page2.margin
               anchors.rightMargin: 2 * page2.margin
               anchors.bottomMargin: 3
               from: 0
               to: page2.timerSeconds
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
            id: timer
            interval: 1000
            repeat: true
            running: true
            onTriggered: {
               if (progressBar.value < progressBar.to) {
                  progressBar.value += 1.0
               } else {
                  progressBar.value = progressBar.from
                  image.sourceImage = image.getSourceImage()
               }
            }
         }
      }
   }
}
