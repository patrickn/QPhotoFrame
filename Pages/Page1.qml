import QtQuick 2.12
import QtQuick.Controls 2.12
import QtGraphicalEffects 1.12

import WeatherInfo 1.0

import ".."

Item {

   readonly property int clockFontPoinSize: 45
   readonly property int tempFontPointSize: 15
   readonly property int smallLabelFontPointSize: 8


   DigitalClock {
      id: digitalClock
   }

   WeatherModel {
       id: weatherModel
   }


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
         width: parent.width - rightSide.width
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
                  font.family: "Courier"
                  font.letterSpacing: -10
               }
               Text {
                  text: digitalClock.seconds % 2 ? ":" : "."
                  font.pointSize: clockFontPoinSize
                  font.family: "Courier"
                  font.letterSpacing: -15
               }
               Text {
                  text: digitalClock.minutes < 10 ? "0" + digitalClock.minutes : digitalClock.minutes
                  font.pointSize: clockFontPoinSize
                  font.family: "Courier"
                  font.letterSpacing: -10
               }
            }
         }

         Text {
             text: weatherModel.hasValidCity ? weatherModel.city : ""
             anchors.horizontalCenter: parent.horizontalCenter
         }

         Column {
            anchors.left: parent.left
            anchors.right: parent.right
            anchors.leftMargin: 6
            anchors.rightMargin: 6
            width: parent.width
            spacing: 5

            Text {
               text: qsTr("Internal Temperature")
               font.pointSize: smallLabelFontPointSize
            }
            Rectangle {
               border.color: "grey"
               color: "transparent"
               width: parent.width
               height: internalTemp.height
               Text {
                  id: internalTemp
                  font.family: "Helvetica"
                  font.pointSize: tempFontPointSize
                  text: "19.0 °C"
                  anchors.verticalCenter: parent.verticalCenter
                  anchors.horizontalCenter: parent.horizontalCenter
               }
            }

            Text {
               text: qsTr("External Temperature")
               font.pointSize: smallLabelFontPointSize
            }
            Rectangle {
               id: rectangle
               border.color: "grey"
               color: "transparent"
               width: parent.width
               height: externalTemp.height

               Text {
                  id: externalTemp
                  font.family: "Helvetica"
                  font.pointSize: tempFontPointSize
                  text: weatherModel.hasValidWeather ? weatherModel.weather.temperature + "C" : "??"
                  anchors.verticalCenter: parent.verticalCenter
                  anchors.horizontalCenter: parent.horizontalCenter
               }
            }
         }
      }
   }
}
