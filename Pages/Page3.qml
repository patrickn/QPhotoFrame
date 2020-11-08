import QtQuick 2.12
import QtQuick.Layouts 1.12
import QtGraphicalEffects 1.12
import QtQml 2.12

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
   }
}
