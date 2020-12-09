import QtGraphicalEffects 1.12
import QtQml 2.12
import QtQuick 2.12
import QtQuick.Layouts 1.12

import QPhotoFrame 1.0
import  "../Controls"

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

   Connections {
      target: imageService
      onImageListUpdated: imageService.updateImage()
   }

   TemperatureModule {
      id: temperatureModule
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
            source: imageService.image.sourcePath()
            fillMode: Image.PreserveAspectCrop
            layer.enabled: true
            layer.effect: OpacityMask { maskSource: mask }

            Rectangle {
               id: mask
               width: parent.width
               height: parent.height
               radius: 5
               visible: false
            }

            ImageProgress {
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

            MouseArea {
               id: mouseArea
               anchors.fill: parent
               hoverEnabled: true
               onDoubleClicked: dataPopup.open()
            }

            ImageDataWindow {
               id: dataPopup

               data: [imageService.image.name(),
                      imageService.image.date(),
                      imageService.image.time(),
                      imageService.image.latitudeStr(),
                      imageService.image.longitudeStr(),
                      imageService.image.altitude().toFixed(1) + "m"]
            }
         }

         Timer {
            id: timer
            interval: 1000 // Progress bar update interval (1s)
            repeat: true
            running: true
            onTriggered: {
               if (progressBar.value < progressBar.to) {
                  progressBar.value += 1.0
               } else {
                  progressBar.value = progressBar.from
                  imageService.updateImage()
               }
            }
         }
      }

      Column {
         id: rightSide
         anchors.top: parent.top
         anchors.right: parent.right
         anchors.bottom: parent.bottom
         width: clockDisplay.width + 12

         Row {
            id: clockDisplay

            Text {
               id: hours
               text: digitalClock.hours < 10 ? "0" + digitalClock.hours : digitalClock.hours
               font.pointSize: clockFontPoinSize
               font.family: "Hack"
               font.letterSpacing: -10
            }

            Image {
               id:tick
               source: digitalClock.seconds % 2 ? "/Assets/ticks0.png" : "/Assets/ticks1.png"
            }

            Text {
               id: minutes
               text: digitalClock.minutes < 10 ? "0" + digitalClock.minutes : digitalClock.minutes
               font.pointSize: clockFontPoinSize
               font.family: "Hack"
               font.letterSpacing: -10
            }
         }

         Text {
            id: cityText
            text: weatherModel.hasValidCity ? weatherModel.city : ""
            font.family: "Arial"
            width: parent.width
            horizontalAlignment: Text.AlignHCenter
            wrapMode: Text.Wrap
         }

         ColumnLayout {
            anchors.left: parent.left
            anchors.right: parent.right
            anchors.leftMargin: 2 * page1.margin
            anchors.rightMargin: page1.margin + 1
            width: parent.width
            height: parent.height - clockDisplay.height - cityText.height - page1.margin
            spacing: 2

            Rectangle {
               id: internalTemperature
               border.color: "grey"
               color: "transparent"
               Layout.fillWidth: true
               height: internalTemp.height
               radius: 5

               Row {
                  anchors.fill: parent
                  anchors.leftMargin: page1.margin
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
                     text: temperatureModule.temperature.toFixed(1) + "°C"
                     anchors.verticalCenter: parent.verticalCenter
                  }
               }
            }

            Rectangle {
               id: externalTemperature
               border.color: "grey"
               color: "transparent"
               Layout.fillWidth: true
               height: externalTemp.height
               radius: 5

               Row {
                  anchors.fill: parent
                  anchors.leftMargin: page1.margin
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
                     text: weatherModel.hasValidWeather ? weatherModel.weather.temperature + "°C" : "??"
                     anchors.verticalCenter: parent.verticalCenter
                  }
               }
            }

            Rectangle {
               id: imageData
               border.color: "grey"
               color: "transparent"
               Layout.fillWidth: true
               Layout.fillHeight: true
               radius: 5

               MapDisplay {
                  height: imageData.height - 2
                  width: imageData.width - 2
                  x: 1
                  y: 1
                  lat: imageService.image.latitude()
                  lon: imageService.image.longitude()

                  layer.enabled: true
                  layer.effect: OpacityMask { maskSource: mask }
               }
            }
         }
      }
   }
}
