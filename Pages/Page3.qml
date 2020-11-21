import QtQuick 2.12
import QtQuick.Layouts 1.12
import QtQuick.Controls 2.12
import QtGraphicalEffects 1.12
import QtQml 2.12
import QtQuick.Controls.Styles 1.4
import QtQuick.Dialogs 1.0
import QtQuick.Extras 1.4


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

      Rectangle {
         id: infoScreen
         anchors.left: parent.left
         anchors.right: parent.right
         anchors.margins: 8
         height: 100
         color: "transparent"

         Column {
            Text {
               text: "neavey.net - Adventure Miles"
               font.family: "Helvetica Neue"
               font.pointSize: 18
               font.bold: true
               color: "#57ad68"
            }

            Row {
               Text {
                  text: "All the photographs displayed are from the website <b>Adventure Miles</b> " +
                        "at <i>www.neavey.net</i>. For more information enter <i>www.neavey.net</i> " +
                        "into your browser, or scan the QR code on the right."
                  width: infoScreen.width - qrCode.width
                  wrapMode: TextEdit.WordWrap
               }

               Image {
                  id: qrCode
                  source: "/Assets/neavey_net.png"
                  width: 70; height: 70
               }
            }
         }

         VersionText {
            version: "0.1.0"
         }
      }

      Column {
         id: leftSide
         anchors.top: infoScreen.bottom
         anchors.left: parent.left
         anchors.bottom: parent.bottom
         width: screenBorder.width / 2

         GroupBox {
            anchors.fill: parent
            title: "Preferences"
            anchors.margins: 2 * page3.margin

            background: Rectangle {
               color: "transparent"
               border.color: "#21be2b"
               radius: 3
            }
         }

         ColumnLayout {
            id: trebleColumn
            width: parent.width / 2
            height: (dialColumn.height - dialColumn.spacing) / 2
            anchors.leftMargin: 0
            anchors.topMargin: 20
            spacing: height * 0.025

            Dial {
               id: dial2
               Layout.alignment: Qt.AlignCenter
               Layout.fillWidth: false
               Layout.fillHeight: false

               stepSize: 1
               maximumValue: 10

               style: DialStyle {
                  labelInset: outerRadius * 0
               }
            }

            ControlLabel {
               id: trebleText
               text: "Image Refresh"
               Layout.alignment: Qt.AlignCenter
            }
         }
      }

      Column {
         id: rightSide
         anchors.top: infoScreen.bottom
         anchors.right: parent.right
         anchors.bottom: parent.bottom
         width: screenBorder.width / 2

         GroupBox {
            anchors.fill: parent
            title: "Frequency Stats"
            anchors.margins: 2 * page3.margin

            background: Rectangle {
               color: "transparent"
               border.color: "#21be2b"
               radius: 3
            }


            ListView {
               id: listView
               anchors.fill: parent
               flickableDirection: Flickable.HorizontalAndVerticalFlick
               clip: true

               header: Row {
                  spacing: 1
                  Repeater {
                     id: repeater
                     model: ["Image", "Name", "Occurence"]
                     Label {
                        text: modelData
                        font.bold: true
                        font.pixelSize: 12
                        padding: 2
                        background: Rectangle { color: "silver" }
                        width: listView.width / 3
                     }
                  }
               }

               model: 222
               delegate: Column {
                  id: delegate

                  Row {
                     spacing: 2

                     Image {
                        width: listView.width / 3
                        source: "/Assets/error.jpg"
                        fillMode: Image.PreserveAspectFit
                     }

                     Text {
                        width: listView.width / 3
                        text: "name"
                     }

                     Text {
                        width: listView.width / 3
                        text: index
                     }
                  }
                  Rectangle {
                     color: "silver"
                     width: parent.width
                     height: 1
                  }
               }
               ScrollIndicator.vertical: ScrollIndicator {}
            }
         }
      }
   }
}
