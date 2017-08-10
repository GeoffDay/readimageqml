import QtQuick 2.0

Item {
    id: togglebutton

    property string source1: ""
    property string source2: ""

    state: "on"
    width: icon.width;
    height: icon.height

    signal flipped(bool tsState)

    function toggle() {
        if (togglebutton.state == "on") {
            togglebutton.state = "off"
            togglebutton.flipped(false)
            icon.source = source1
        } else {
            togglebutton.state = "on";
            togglebutton.flipped(true)
            icon.source = source2
        }
    }


    Image {
        id: icon
        source: source1

        MouseArea {
            anchors.fill: parent
            onClicked: toggle()
        }
    }
}
