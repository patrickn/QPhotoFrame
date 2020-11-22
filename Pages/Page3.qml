import QtQuick 2.12
import QtQuick.Layouts 1.12
import QtQuick.Controls 2.12
import QtGraphicalEffects 1.12
import QtQml 2.12

import Qt.labs.qmlmodels 1.0



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


      Column {
         id: rightSide
         //            anchors.top: infoScreen.bottom
         //            anchors.right: parent.right
         //            anchors.bottom: parent.bottom


         TableView {
            columnSpacing: 1
            rowSpacing: 1
            clip: true

            implicitWidth: 200
            implicitHeight: 100


            model: 10

            delegate: Rectangle {
                implicitWidth: 100
                implicitHeight: 10
                border.width: 1

                Text {
                    text: index
                    anchors.centerIn: parent
                }
            }
         }

         ListView {
            id: listView
            implicitWidth: 340
            implicitHeight: 200
            flickableDirection: Flickable.VerticalFlick
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
                     width: 90
                  }
               }
            }

            model: imageService
            delegate: Column {
               id: delegate

               width: listView.width

               Row {
                  spacing: 2

                  Image {
                     width: 50
                     source: imageRole
                     fillMode: Image.PreserveAspectFit
                  }

                  Text {
                     width: 90
                     text: nameRole
                  }

                  Text {
                     width: 90
                     text: useCountRole
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
