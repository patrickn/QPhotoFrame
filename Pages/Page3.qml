import QtQuick 2.12
import QtQuick.Layouts 1.12
import QtQuick.Controls 2.12

import QPhotoFrame 1.0
import  "../Controls"

Item {
   id: page3

   readonly property int margin: 3

   Rectangle {
      id: screenBorder
      radius: 5
      anchors.fill: parent
      anchors.margins: page3.margin
      border.width: 2
      border.color: "#fb9797"
      color: "transparent"

      ColumnLayout {
         anchors.fill: parent

         Rectangle {
            id: infoScreen
            Layout.preferredWidth: parent.width
            Layout.preferredHeight: 130
            anchors.margins: 8
            color: "transparent"

            ColumnLayout {
               spacing: 0

               Text {
                  Layout.leftMargin: 10
                  text: "neavey.net - Adventure Miles"
                  font.family: "Helvetica Neue"
                  font.pointSize: 18
                  font.bold: true
                  color: "#57ad68"
               }

               RowLayout  {
                  spacing: 0
                  Text {
                     Layout.leftMargin: 10
                     Layout.preferredWidth: infoScreen.width - 105
                     text: "All the photographs are from my website <b>Adventure Miles</b> " +
                           "at <i>www.neavey.net</i>. For more information enter <i>www.neavey.net</i> " +
                           "into your browser, or scan the QR code on the right."
                     wrapMode: TextEdit.WordWrap
                  }

                  Image {
                     id: qrCode
                     source: "/Assets/neavey_net.png"
                     Layout.preferredWidth: 90
                     Layout.preferredHeight: 90
                  }
               }
            }

            VersionText {
               version: "0.1.0"   // TODO: Generate version numbers automatically
            }
         }

         ColumnLayout {
            spacing: 0

            RowLayout {
               spacing: 0

               GroupBox {
                  id: leftSide
                  title: "Preferences"
                  Layout.fillHeight: true
                  Layout.preferredWidth: infoScreen.width / 2
                  Layout.leftMargin: 3
                  Layout.bottomMargin: 3

                  background: Rectangle {
                     color: "transparent"
                     border.color: "#21be2b"
                     radius: 3
                  }

                  SettingsView {}
               }

               GroupBox {
                  id: rightSide
                  title: "Frequency Stats"
                  Layout.fillHeight: true
                  Layout.fillWidth: true
                  Layout.rightMargin: 3
                  Layout.bottomMargin: 3

                  background: Rectangle {
                     color: "transparent"
                     border.color: "#21be2b"
                     radius: 3
                  }

                  StatsView {}
               }
            }
         }
      }
   }
}
