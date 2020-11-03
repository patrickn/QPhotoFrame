import QtQuick 2.0
import QtLocation 5.12
import QtPositioning 5.12

Item {
   id: mapDisplay
   property real lat: 51.03
   property real lon: 4.48

   Plugin {
       id: mapPlugin
       name: "osm" // "mapboxgl", "esri", ...
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
       zoomLevel: 14
   }
}
