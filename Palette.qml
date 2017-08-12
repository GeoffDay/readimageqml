import QtQuick 2.0

// this is the bigarf version of Palette.qml. it is vertical.

Item {
    id: palette
    width: 70
    height: 1600


    property int range: 4095                //max range of the file - could be 8 12 or 16 bits
    property int min: 0                     //min value
    property int max: 4095                  //max value
    property bool cTableType: true          //LCII colour palette true, greyscale false
    property bool aGC: true                 //AGC
    property double pixelScale: 1.0         // need to scale the values if pixel scale changes

    property string aGCText: "Left Click for AGC"
    property string cTableText: "Left Click to change colour"

    property double pScale: (palette.height - (2 * minPointer.height)) / range
    property int pHeight: palette.height - (2 * minPointer.height)  //not used now

    onCTableTypeChanged: setTableType()     //initially this came from the widget.cpp now its internal

    function setTableType(){
        cTableText = ""                     //get rid of the text
        if (cTableType) {
            grey.opacity = 0
            lcii.opacity = 100
        } else {
            grey.opacity = 100
            lcii.opacity = 0
        }
        aBinImageFile.setCTType(cTableType)
    }

    onAGCChanged: setAGC()     //initially this came from the widget.cpp now its internal


    function setAGC(){
        aGCText = ""                        //get rid of the text

        if (aGC) {
            aBinImageFile.setAGCOn()
            background.color = "lightyellow"
        } else {
            aBinImageFile.setAGCOff()
            background.color = "whitesmoke"
        }

    }

    Rectangle{
        id: background
        width: parent.width
        height: parent.height
        color: "lightyellow"


        MouseArea {
            id: agc
            anchors.leftMargin: 26
            anchors.left: parent.left
            anchors.right: parent.right
            anchors.top: parent.top
            anchors.bottom: parent.bottom
            acceptedButtons: Qt.LeftButton
            hoverEnabled: true
            onClicked: {aGC = !aGC}

            Text {
                id: agcText
                anchors.centerIn: parent
                font.pixelSize: 14;
                color: "black"
                rotation: 90
            }

            onEntered: agcText.text = aGCText
            onExited: agcText.text = ""
        }
    }

    Item {
        id: minPointer
        x: 1
        y: Math.round((palette.height - minPointer.height) - (min * pScale))
        width: 26;                  height: 12

        Rectangle {
            anchors.centerIn: parent
            width: parent.width;    height: parent.height
            color: "yellow";        border.color: "red"
        }

        Text {
            id: minText
            x: 30;          y: parent.top
            height: parent.height
            font.pixelSize: 13;     smooth: true
            color: "black"
            text: ("%i",  Math.round((palette.height - (minPointer.y + minPointer.height)) / pScale)) * pixelScale
        }


        MouseArea {
            id: minArea
            x: 0;   y: 0
            width: parent.width;            height: parent.height
            drag.target: minPointer;        drag.axis: Drag.YAxis
            drag.minimumY: (maxPointer.y + 1.2 * minPointer.height) + 1
            drag.maximumY: palette.height - minPointer.height
            onEntered: aGC = false
            onMouseYChanged: aBinImageFile.setCTMin(minText.text / pixelScale)
        }
    }


    Item {
        id: maxPointer
        x: 1
        y:  Math.round((palette.height - 2 * minPointer.height) - (max * pScale))
        width: 26;                  height: 12


        Rectangle {
            anchors.centerIn: parent
            width: parent.width;    height: parent.height
            color: "yellow";        border.color: "red"
        }

        Text {
            id: maxText
            x: 30;          y: parent.top
            height: parent.height
            font.pixelSize: 13;     smooth: true
            color: "black"
            text: ("%i",  Math.round((palette.height - (maxPointer.y + 2 * maxPointer.height)) / pScale)) * pixelScale
        }


        MouseArea {
            id: maxArea
            x: 0;   y: 0
            width: parent.width;            height: parent.height
            drag.target: maxPointer;        drag.axis: Drag.YAxis
            drag.minimumY: 0
            drag.maximumY: (minPointer.y - 1.2 * maxPointer.height) - 1
            onEntered: aGC = false
            onMouseYChanged:  aBinImageFile.setCTMax(maxText.text / pixelScale)
        }
    }


    Rectangle {
        id: lcii
        x: 2;           y: maxPointer.y + maxPointer.height + 1
        width: 24;      height: minPointer.y - maxPointer.y - maxPointer.height - 1
        opacity: 20

        gradient: Gradient {
             GradientStop { position: 0.0; color: "white"}
             GradientStop { position: 0.143; color: "yellow"}
             GradientStop { position: 0.286; color: "red"}
             GradientStop { position: 0.429; color: "lightgreen"}
             GradientStop { position: 0.572; color: "cyan"}
             GradientStop { position: 0.715; color: "blue"}
             GradientStop { position: 0.858; color: "magenta"}
             GradientStop { position: 1.0; color: "black"}
        }

        MouseArea {
            id: greyswitch
            anchors.fill: parent
            acceptedButtons: Qt.LeftButton
            hoverEnabled: true

            onClicked: {cTableType = !cTableType}

            Text {
                id: colourText
                anchors.centerIn: parent
                font.pixelSize: 14;
                color: "white"
                rotation: 90
            }

            onEntered: colourText.text = cTableText
            onExited: colourText.text = ""
        }

    }

    Rectangle {
        id: grey
        x: 2;           y: maxPointer.y + maxPointer.height + 1
        width: 24;      height: minPointer.y - maxPointer.y - minPointer.height - 1
        opacity: 0

        gradient: Gradient {
             GradientStop { position: 0.0; color: "white"}
             GradientStop { position: 1.0; color: "black"}
        }

        MouseArea {
            id: lciiswitch
            anchors.fill: parent
            acceptedButtons: Qt.LeftButton
            hoverEnabled: true
            onClicked: {cTableType = !cTableType}

            Text {
                id: greyText
                anchors.centerIn: parent
                font.pixelSize: 14;
                color: "white"
                rotation: 90
            }

            onEntered: greyText.text = cTableText
            onExited: greyText.text = ""
        }
    }
}
