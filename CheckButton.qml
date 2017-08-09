import QtQuick 2.0

Item {
    id: toggleswitch

    state: "on"
    width: loop.width;
    height: loop.height
 
    signal flipped(bool tsState)

    function toggle() {
        if (toggleswitch.state == "on") {
            toggleswitch.state = "off"
            toggleswitch.flipped(false)
            loop.source = "playback_once.png"
        } else {
            toggleswitch.state = "on";
            toggleswitch.flipped(true)
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
}
