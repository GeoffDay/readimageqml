import QtQuick 2.0

Item {
    id: toggleswitch
    property bool on: true

    width: loop.width;
    height: loop.height

    signal flipped(bool tsState)

    function toggle() {
        if (toggleswitch.state == "on") {
            toggleswitch.state = "off"
            toggleswitch.flipped(false)
            on = false
            loop.source = "playback_once.png"
        } else {
            toggleswitch.state = "on";
            toggleswitch.flipped(true)
            on = true
            loop.source = "playback_loop.png"
        }
    }


    Image {
        id: loop
        source: "playback_loop.png"

        MouseArea {
            anchors.fill: parent
            onClicked: toggle()
        }
    }

//    states: [
//        State {
//            name: "on"
//            PropertyChanges { target: loop; source: "playback_loop.png"}
////            PropertyChanges { target: toggleswitch; on: true }
//        },
//        State {
//            name: "off"
//            PropertyChanges { target: loop; source: "playback_once.png" }
////            PropertyChanges { target: toggleswitch; on: false }
//        }
//    ]
}
