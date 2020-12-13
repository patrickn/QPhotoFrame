import QtQuick 2.12
import QtLocation 5.12
import QtPositioning 5.12

Item {
   id: mapDisplay
   property real lat: 51.03
   property real lon: 4.48

   Plugin {
      id: mapPlugin
      name: "esri" // "osm", "mapboxgl", "esri", ...
      // specify plugin parameters if necessary
      // PluginParameter {
      //     name:
      //     value:
      // }
   }

   Map {
      anchors.fill: parent
      plugin: mapPlugin
      center: QtPositioning.coordinate(mapDisplay.lat, mapDisplay.lon)
      zoomLevel: 3

      MapQuickItem {
         id: marker
         sourceItem: Rectangle { width: 14; height: 14; color: "#e41e25"; border.width: 2; border.color: "white"; smooth: true; radius: 7 }
         coordinate {
            latitude: mapDisplay.lat
            longitude: mapDisplay.lon
         }
         opacity:1.0
         anchorPoint: Qt.point(sourceItem.width/2, sourceItem.height/2)
      }
   }
}
