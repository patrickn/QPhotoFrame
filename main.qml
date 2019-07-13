import QtQuick 2.12
import QtQuick.Controls 2.5
import "Pages"

ApplicationWindow {
   visible: true
   title: qsTr("Photo Frame")

   //   height: 720
   //   width: 1280

   SwipeView {
      id: swipeView
      anchors.fill: parent
      currentIndex: tabBar.currentIndex

      Page1 {}

      Page2 {}
   }

   footer: TabBar {
      id: tabBar
      currentIndex: swipeView.currentIndex

      TabButton {
         text: qsTr("Page 1")
      }
      TabButton {
         text: qsTr("Page 2")
      }

//      MouseArea {
//         anchors.fill: parent
//         onClicked: { console.log ("xxxxxxxxxxxxxxxxxxxxxxxxxx");
//         }
//      }
   }
}
