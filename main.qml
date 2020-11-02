import QtQuick 2.12
import QtQuick.Controls 2.5
import "Pages"

ApplicationWindow {
   visible: true
   title: qsTr("Photo Frame")

   width: 800
   height: 480

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
         height: 5
         background: Rectangle {
            color: tabBar.currentIndex == 0 ? "green" : "orange"
            radius: 5
         }
      }
      TabButton {
         height: 5
         background: Rectangle {
            color: tabBar.currentIndex == 1 ? "green" : "orange"
            radius: 5
         }
      }
   }
}
