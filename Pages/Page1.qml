import QtQuick 2.12
import QtQuick.Controls 2.12
import QtGraphicalEffects 1.12

import QPhotoFrame 1.0
import "../Controls" as CustomControls
import ".."

Item {
   id: page1

   readonly property int clockFontPoinSize: 45
   readonly property int tempFontPointSize: 15
   readonly property int smallLabelFontPointSize: 8
   readonly property int tinyLabelFontPointSize: 6
   readonly property int timerSeconds: 60 * 10
   readonly property int margin: 3

   DigitalClock {
      id: digitalClock
   }

   WeatherModel {
       id: weatherModel
   }

   ImageService {
      id: imageService
   }

   Rectangle {
      id: screenBorder
      radius: 5
      anchors.rightMargin: page1.margin
      anchors.leftMargin: page1.margin
      anchors.bottomMargin: page1.margin
      anchors.topMargin: page1.margin
      anchors.fill: parent
      border.width: 2
      border.color: "#fb9797"
      color: "transparent"

      Row {
         id: leftSide
         width: parent.width - rightSide.width
         anchors.bottom: parent.bottom
         anchors.bottomMargin: page1.margin
         anchors.top: parent.top
         anchors.topMargin: page1.margin
         anchors.left: parent.left
         anchors.leftMargin: page1.margin

         Image {
            id: image
            height: parent.height
            width: parent.width
            source: imageService.randomImage
            fillMode: Image.PreserveAspectCrop
            layer.enabled: true
            layer.effect: OpacityMask {
               maskSource: mask
            }

            CustomControls.ProgressBar {
               id: progressBar
               height: 3
               anchors.right: parent.right
               anchors.left: parent.left
               anchors.bottom: parent.bottom
               anchors.leftMargin: 2 * page1.margin
               anchors.rightMargin: 2 * page1.margin
               anchors.bottomMargin: page1.margin
               from: 0
               to: page1.timerSeconds
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
            interval: 1000 // 1 second
            repeat: true
            running: true
            onTriggered: {
               if (progressBar.value < progressBar.to) {
                  progressBar.value += 1.0
               } else {
                  progressBar.value = progressBar.from
                  image.source = imageService.randomImage
               }
            }
         }
      }

      Column {
         id: rightSide
         anchors.top: parent.top
         anchors.right: parent.right
         anchors.bottom: parent.bottom
         width: clockDisplay.width + 20

         Column {
            spacing: 40
            anchors.horizontalCenter: parent.horizontalCenter

            Row {
               id: clockDisplay

               Text {
                  text: digitalClock.hours < 10 ? "0" + digitalClock.hours : digitalClock.hours
                  font.pointSize: clockFontPoinSize
                  font.family: "Hack"
                  font.letterSpacing: -10
               }
               Text {
                  text: digitalClock.seconds % 2 ? ":" : "."
                  font.pointSize: clockFontPoinSize
                  font.family: "Hack"
                  font.letterSpacing: -15
               }
               Text {
                  text: digitalClock.minutes < 10 ? "0" + digitalClock.minutes : digitalClock.minutes
                  font.pointSize: clockFontPoinSize
                  font.family: "Hack"
                  font.letterSpacing: -10
               }
            }
         }

         Text {
             text: weatherModel.hasValidCity ? weatherModel.city : ""
             font.family: "Arial"
             width: parent.width
             horizontalAlignment: Text.AlignHCenter
             wrapMode: Text.Wrap
         }

         Column {
            anchors.left: parent.left
            anchors.right: parent.right
            anchors.leftMargin: 6
            anchors.rightMargin: 6
            width: parent.width
            spacing: 5

            Rectangle {
               border.color: "grey"
               color: "transparent"
               width: parent.width
               height: internalTemp.height

               Row {
                  anchors.fill: parent
                  anchors.leftMargin: 3
                  spacing: 10

                  Text {
                     text: qsTr("Internal\nTemperature")
                     font.pointSize: tinyLabelFontPointSize
                     anchors.verticalCenter: parent.verticalCenter
                  }

                  Text {
                     id: internalTemp
                     font.family: "Arial"
                     font.pointSize: tempFontPointSize
                     text: "19.0°C"
                     anchors.verticalCenter: parent.verticalCenter
                  }
               }
            }

            Rectangle {
               id: rectangle
               border.color: "grey"
               color: "transparent"
               width: parent.width
               height: externalTemp.height

               Row {
                  anchors.fill: parent
                  anchors.leftMargin: 3
                  spacing: 10

                  Text {
                     text: qsTr("External\nTemperature")
                     font.pointSize: tinyLabelFontPointSize
                     anchors.verticalCenter: parent.verticalCenter
                  }

                  Text {
                     id: externalTemp
                     font.family: "Arial Narrow"
                     font.pointSize: tempFontPointSize
                     text: weatherModel.hasValidWeather ? weatherModel.weather.temperature + "C" : "??"
                     anchors.verticalCenter: parent.verticalCenter
                  }
               }
            }
         }
      }
   }
}
