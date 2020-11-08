import QtQuick 2.12
import QtQuick.Controls 2.5
import "Pages"

ApplicationWindow {
   id: mainWindow

   property var tabColors: ["green", "orange"]  // TODO: Move to settings


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
      Page3 {}
   }

   footer: TabBar {
      id: tabBar

      TabButton {
         id: photoPage
         height: 5
         background: Rectangle {
            color: getTabColor(0)
            radius: 5
         }
      }
      TabButton {
         id: keanuPage
         height: 5
         background: Rectangle {
            color: getTabColor(1)
            radius: 5
         }
      }
      TabButton {
         id: settingsPage
         height: 5
         background: Rectangle {
            color: getTabColor(2)
            radius: 5
         }
      }
   }

   function getTabColor(index) {
      return swipeView.currentIndex === index ? mainWindow.tabColors[0] : mainWindow.tabColors[1]
   }
}
