import QtQuick 2.7
import QtQuick.Controls 2.0
import QtQuick.Layouts 1.0
import QtQuick.Dialogs 1.0
import QtQuick.Window 2.2

import Readstuff 1.0
//import myModel 1.0

ApplicationWindow {

    id: mainWindow
    visible: true

    width: Screen.width - 500
    height: Screen.height - 100

//    property alias imagesanstuff: imagesanstuff
    property int magnification: 20
    property string fNameInfo: ""

    onWindowStateChanged: {
        console.log( "onWindowStateChanged (Window), state: " +  windowState );
         aBinImageFile.update()
    }

    title: qsTr("Player")

//            Column {
//                id: imagesanstuff

    Row {
        id: toprow
        spacing: 5

                ReadImage {
                    id: aBinImageFile
                    width: 1000
                    height: 1000

                    MouseArea {
                        id: mouseArea
                        anchors.fill: parent
                        acceptedButtons: Qt.LeftButton | Qt.RightButton

                        onClicked: {
                            if (mouse.button == Qt.LeftButton)      // click to pause or play
                                    playPause.toggle()
                            }

                        onWheel: {
                            if (wheel.buttons == Qt.RightButton) {  // roll up to increase image size
                                if((wheel.angleDelta.y > 0) && (magnification < 32)) aBinImageFile.setMagnification(magnification++)
                                if((wheel.angleDelta.y < 0) && (magnification > 1)) aBinImageFile.setMagnification(magnification--)
                            } else {
                                playPause.reset()                   // play button is on left mouse button
                                if (wheel.angleDelta.y > 0)
                                    aBinImageFile.forward()
                                else if (wheel.angleDelta.y < 0)
                                    aBinImageFile.back()
                            }
                        }
                    }

//                    onFNameInfoChanged: fileNameInfo.text = fNameInfo
                    onCtMinChanged: palette.min = ctMin
                    onCtMaxChanged: palette.max = ctMax
                    onStartFrameChanged: iPos.startFrame = startFrame
                    onEndFrameChanged: iPos.endFrame = endFrame
                    onCurrentFrameChanged: iPos.currentFrame = currentFrame
                    onNFramesChanged: iPos.nFrames = nFrames
                    onMetaDataChanged: metaDataa.myModel = metaData
                }


            Item {
                id: metaDataa
                width: 100
                height: 100

                ListModel {
                    id: myModel
                    ListElement { name: "Dog"; colour: "green" }
                    ListElement { name: "Cat"; colour: "red" }
                }

                Component {
                    id: myDelegate
                     Text {text: name + ", " + colour}
                }

                ListView {
                    anchors.fill: parent
                    model: myModel
                    delegate: myDelegate
                }
            }


//            }

            Palette {
                id: palette
                width: 70
                height: mainWindow.height - 100
            }
        }



        ImagePos {
            id: iPos
            width: mainWindow.width - 4
            height: 60
            y: mainWindow.height - 96
            x: 2
        }

        Row {
            spacing: 2
            x: 2
            y: mainWindow.height - 34

            MyButton {
                annunc: "Prev File"
                frColor: "#000000"
                bkColor: "#80a060"
                bdColor: "#004040"
                onActivated: {
                    console.log("prev file button")
                    aBinImageFile.prevIFile()
                }
            }

            MyButton {
                annunc: "Open File"
                frColor: "#000000"
                bkColor: "#b09070"
                bdColor: "#004040"
                onActivated: {
                      console.log("open file button pressed")
                      fileDialog.open()
                }
            }

            MyButton {
                annunc: "Next File"
                frColor: "#000000"
                bkColor: "#c0b030"
                bdColor: "#004040"

                onActivated: {
                    console.log("next file button pressed")
                    aBinImageFile.nextIFile()
                }
            }

            Button {
                    width: 32
                    height: 32
                Image {source: "playback_begin.png"}

                onClicked: {
                    aBinImageFile.begin()
                    playPause.reset()
                }
            }

            Button {
                width: 32
                height: 32
                Image {source: "playback_rew.png"}

                onClicked: {
                    aBinImageFile.back()
                }
            }

            ToggleButton {
                id: playPause
                state: "on"
                source1: "playback_play.png"
                source2: "playback_pause.png"

                onStateChanged: {
                    if (playPause.state == "on") {
                        aBinImageFile.play()
                        timer.start()
                    } else {
                        aBinImageFile.pause()
                        timer.stop()
                    }
                }
            }

            Button {
                width: 32
                height: 32
                Image {source: "playback_ff.png"}

                onClicked: {
                    aBinImageFile.forward()
                }
            }

            Button {
                width: 32
                height: 32
                Image {source: "playback_end.png"}

                onClicked: {
                    aBinImageFile.end()
                    playPause.reset()
                }
            }

            CheckButton {
                id: loopy

                onFlipped: {
                    aBinImageFile.loop(tsState)
                }
            }

            ComboBox {
                id: fRate
                height: 32
                font.pointSize: 16
                textRole: "FrameRate"

                displayText: "Frame Rate " + currentText
                model: ListModel {
                    id: fRates
                    ListElement { FrameRate: "50 fps"; value: 20 }
                    ListElement { FrameRate: "25 fps"; value: 40 }
                    ListElement { FrameRate: "10 fps"; value: 100 }
                    ListElement { FrameRate: "5 fps"; value: 200 }
                    ListElement { FrameRate: "2 fps"; value: 500 }
                    ListElement { FrameRate: "1 fps"; value: 1000 }
                }

                background: Rectangle {
                    color: "#86b88a"
                    implicitWidth: 250
                    implicitHeight: 32
                    border.color: "#76782a"
                    border.width: 1
                }

                onActivated: {
                    timer.interval = fRates.get(currentIndex).value
                    console.log("setting frame rate " + fRates.get(currentIndex).value)
                }
            }

            MyButton {
                annunc: "Export"
                frColor: "#303030"
                bkColor: "#b6a0b0"
                bdColor: "#7678da"

                onActivated: {
                      console.log("export button pressed")
                }
            }

            ComboBox {
                id: xport
                height: 32
                font.pointSize: 16
                textRole: "Xport"

                background: Rectangle {
                              color: "#b6a0b0"
                              border.color: "#76782a"
                              implicitWidth: 120
                              border.width: 1
                }

                model: ListModel {
                    ListElement { Xport: "as BIN"; value: 20 }
                    ListElement { Xport: "as ARF"; value: 20 }
                    ListElement { Xport: "as BMP"; value: 40 }
                    ListElement { Xport: "as AVI"; value: 20 }
                }
            }


            Label {
                text: " Pixel Scale "
                width: Text.implicitWidth
                height: 32
                font.pixelSize: 32 * 5/8
                verticalAlignment: Text.AlignVCenter
                background: Rectangle {
                                  color: "#8688ca"
                                  border.color: "#76782a"
                                  border.width: 1
                            }
            }

            SpinBox {
                 id: spinbox
                 from: 0
                 value: 1000
                 to: 1000 * 1000
                 stepSize: 1
                 editable: true

                 width: 160
                 height: 32
                 font.pointSize: 16

                 background: Rectangle {
                                   color: "#8688ca"
                                   border.color: "#76782a"
                                   border.width: 1
                             }

                 property int decimals: 3
                 property real realValue: value / 1000

                 validator: DoubleValidator {
                     bottom: Math.min(spinbox.from, spinbox.to)
                     top:  Math.max(spinbox.from, spinbox.to)
                 }

                 textFromValue: function(value, locale) {
                     return Number(value / 1000).toLocaleString(locale, 'f', spinbox.decimals)
                 }

                 valueFromText: function(text, locale) {
                     return Number.fromLocaleString(locale, text) * 1000
                 }
             }
        }


    Timer {
        id: timer
        interval: 20
        running: false
        repeat: true
        triggeredOnStart: true
        onTriggered: {
            aBinImageFile.timerTimeout()
            aBinImageFile.update()
        }
    }

    FileDialog {
        id: fileDialog
        title: "Please choose a file"
        nameFilters: ["Bin files (*.bin)","Arf files (*.arf)"]
        folder: shortcuts.home
        onAccepted: {
            console.log("You chose: " + fileDialog.fileUrls)
            aBinImageFile.openIFileName(fileDialog.fileUrls)
            timer.start()
        }

        onRejected: {
            console.log("Cancelled")
            Qt.quit()
        }

        Component.onCompleted: visible = false
    }    
}
