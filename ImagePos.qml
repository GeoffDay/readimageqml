import QtQuick 1.1

//this is the arfPos.qml files from BigArfPlayer


Item {
    id: ImagePos
    width: 100
    height: 25
    y: 0

    property int nFrames: 5           //number of frames in the file
    property int startFrame: 1
    property int endFrame: 5
    property int currentFrame: 1     //currently displayed frame

    property double iScale: (ImagePos.width - (2 * currentPointer.width)) / (nFrames - 1)

    Rectangle {
        id: filePosBackGround
        y: 0
        width: parent.width
        height: parent.height
        color: "black"


        MouseArea {
            id: setStartArea
            x: 0
            width: (currentFrame - 1) * iScale + currentPointer.width / 2
            y: 0
            height: parent.height
            acceptedButtons: Qt.LeftButton
            hoverEnabled: true

            onClicked: startFrame = currentFrame
        }

        MouseArea {
            id: setEndArea
            x: currentPointer.x
            width: parent.width - currentPointer.x - 2 * currentPointer.width
            y: 0
            height: parent.height
            acceptedButtons: Qt.LeftButton
            hoverEnabled: true

            onClicked: {endFrame = currentFrame
//                console.log("Current value ", currentFrame)
//                console.log("end value ", endFrame)
            }
        }
    }

    Rectangle {
        id: centreBar
        width: filePosBackGround.width - filePosBackGround.height / 2.0
        height: ImagePos.height / 40
        color: "#faff00"
        radius: 2
        anchors.centerIn: filePosBackGround
    }


    Item {
        id: currentPointer
        x: (currentFrame - 1) * iScale + currentPointer.width / 2
        width: filePosBackGround.height / 2; height: filePosBackGround.height / 2
        anchors.bottom: filePosBackGround.verticalCenter

        Image {
             width: 30; height: 30
             smooth: true
             source: "current.svg"
         }

        Text {
            id: currentText
             anchors {centerIn: parent; verticalCenterOffset: -4; horizontalCenterOffset: 3}
            font.pixelSize: parent.height * 0.5
            style: Text.Sunken; color: "white"; styleColor: "black"; smooth: true
            text: 1 + Math.round((currentPointer.x - currentPointer.width / 2) / (ImagePos.width - (2 * currentPointer.width)) * (nFrames - 1))
        }

        MouseArea {
            id: pointerArea
            anchors.fill: currentPointer
            drag.target: currentPointer
            drag.axis: Drag.XAxis
            drag.minimumX: 0; drag.maximumX: ImagePos.width - currentPointer.width
        }

        onXChanged: ImagePosObject.getCurrentFrame(1 + Math.round((currentPointer.x - currentPointer.width / 2)/ (ImagePos.width - (2 * currentPointer.width)) * (nFrames - 1)))

    }


    Item {
        id: startPointer
        x: (startFrame - 1) * iScale
        width: filePosBackGround.height / 2; height: filePosBackGround.height / 2
        anchors.topMargin: 0 ;anchors.top: filePosBackGround.verticalCenter

        Image {
             width: 30; height: 30
             smooth: true
             source: "start.svg"
         }

        Text {
            id: startText
            anchors {centerIn: parent; verticalCenterOffset: 4; horizontalCenterOffset: 3}
            font.pixelSize: parent.height * 0.5
            style: Text.Sunken; color: "white"; styleColor: "black"; smooth: true
            text: 1 + Math.round(startPointer.x / iScale)
        }

        MouseArea {
            id: startArea
            anchors.fill: startPointer
            drag.target: startPointer
            drag.axis: Drag.XAxis
            drag.minimumX: 0; drag.maximumX: endPointer.x - endPointer.width
        }

        onXChanged: ImagePosObject.getStartFrame(1 + Math.round(startPointer.x / iScale))
    }


    Item {
        id: endPointer
        x: ((endFrame - 1) * iScale) + startPointer.width
        width: filePosBackGround.height / 2; height: filePosBackGround.height / 2
        anchors.topMargin: 0 ;anchors.top: filePosBackGround.verticalCenter

        Image {
             width: 30; height: 30
             smooth: true
             source: "end.svg"
         }


        Text {
            id: endText
            anchors {centerIn: parent; verticalCenterOffset: 4; horizontalCenterOffset: 3}
            font.pixelSize: parent.height * 0.5
            style: Text.Sunken; color: "white"; styleColor: "black"; smooth: true
            text: 1 + Math.round((endPointer.x - startPointer.width) / iScale)
        }

        MouseArea {
            id: endArea
            anchors.fill: endPointer
            drag.target: endPointer
            drag.axis: Drag.XAxis
            drag.minimumX: startPointer.x + startPointer.width; drag.maximumX: ImagePos.width - endPointer.width
        }

        onXChanged: {ImagePosObject.getEndFrame(1 + Math.round((endPointer.x - startPointer.width) / iScale))
            console.log("end value ", endFrame)
        }
    }

}

