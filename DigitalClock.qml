import QtQuick 2.0

Item {
   id: clock

   //property alias city: cityLabel.text
   property int hours
   property int minutes
   property int seconds
   property real shift
   property bool night: false
   property bool internationalTime: false

   function timeChanged() {
       var date = new Date;
       hours = internationalTime ? date.getUTCHours() + Math.floor(clock.shift) : date.getHours()
       night = ( hours < 7 || hours > 19 )
       minutes = internationalTime ? date.getUTCMinutes() + ((clock.shift % 1) * 60) : date.getMinutes()
       seconds = date.getUTCSeconds();
   }

   Timer {
       interval: 1000; running: true; repeat: true;
       onTriggered: clock.timeChanged()
   }

}
