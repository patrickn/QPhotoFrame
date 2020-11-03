import QtQuick 2.12
import QtGraphicalEffects 1.12
import QtQml 2.12

import "../Controls"

Item {
   id: mainPage
   readonly property int margin: 3
   readonly property int timerSeconds: 60 * 60 * 24 // 24 hours

   Rectangle {
      id: screenBorder
      radius: 5
      anchors.rightMargin: mainPage.margin
      anchors.leftMargin: mainPage.margin
      anchors.bottomMargin: mainPage.margin
      anchors.topMargin: mainPage.margin
      anchors.fill: parent
      border.width: 2
      border.color: "#fb9797"
      color: "transparent"

      Row {
         id: leftSide
         width: parent.width - (2 * mainPage.margin)
         anchors.bottom: parent.bottom
         anchors.bottomMargin: mainPage.margin
         anchors.top: parent.top
         anchors.topMargin: mainPage.margin
         anchors.left: parent.left
         anchors.leftMargin: mainPage.margin

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
               anchors.leftMargin: 2 * mainPage.margin
               anchors.rightMargin: 2 * mainPage.margin
               anchors.bottomMargin: 3
               from: 0
               to: mainPage.timerSeconds
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
