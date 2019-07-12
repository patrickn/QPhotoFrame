import QtQuick 2.12
import QtQuick.Controls 2.5

import ".."


Item {
    DigitalClock {
       id: digitalClock
    }

    Row {
       Rectangle {
          height: parent.height
          width: 400
          border.color: "blue"
          border.width: 3
       }

       Column {
          spacing: 40

          Row {
             id: clockDisplay

             Text {
                text: digitalClock.hours
                font.pointSize: 70
             }
             Text {
                text: digitalClock.seconds % 2 ? ":" : "."
                font.pointSize: 70
             }
             Text {
                text: digitalClock.minutes < 10 ? "0" + digitalClock.minutes : digitalClock.minutes
                font.pointSize: 70
             }
          }

          Rectangle {
             width: 50
             height: 200
             border.color: "blue"
             border.width: 3
             Text {
                text: "00 C"
                anchors.centerIn: parent
             }
          }
       }
    }
}
