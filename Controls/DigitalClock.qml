import QtQuick 2.12
import QtQml 2.12

Item {
   id: clock

   //property alias city: cityLabel.text
   property int hours
   property int minutes
   property int seconds
   property string dateString
   property real shift
   property bool night: false
   property bool internationalTime: false

   function timeChanged() {
       var date = new Date
       hours = internationalTime ? date.getUTCHours() + Math.floor(clock.shift) : date.getHours()
       night = ( hours < 7 || hours > 19 )
       minutes = internationalTime ? date.getUTCMinutes() + ((clock.shift % 1) * 60) : date.getMinutes()
       seconds = date.getUTCSeconds()
       dateString = date.toDateString()
   }

   Timer {
       interval: 1000; running: true; repeat: true
       onTriggered: clock.timeChanged()
   }

}
