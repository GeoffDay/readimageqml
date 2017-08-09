import QtQuick 2.7
import QtQuick.Controls 2.0
import QtQuick.Layouts 1.0
import QtQuick.Dialogs 1.0
import QtQuick.Window 2.2
import Readstuff 1.0

ApplicationWindow {

    id: mainWindow
    visible: true

    width: Screen.width - 600
    height: Screen.height - 200

    onWindowStateChanged: {
        console.log( "onWindowStateChanged (Window), state: " +  windowState );
    }

    

    title: qsTr("Player")

        Row {
            spacing: 5

            ReadImage {
                id: aBinImageFile
    //            anchors.centerIn: parent
                x: 2
                y: 2
                width: 1000; height: 1000

                MouseArea {
                    id: mouseArea
                    anchors.fill: parent
                    acceptedButtons: Qt.LeftButton | Qt.RightButton

                    onClicked: {
                        if (mouse.button == Qt.RightButton)
                           aBinImageFile.play()
                        else
                           aBinImageFile.pause()
                    }

                    onWheel: {
                        if (wheel.angleDelta.y > 0)
                            aBinImageFile.forward()
                        else if (wheel.angleDelta.y < 0)
                            aBinImageFile.back()
                    }
                }


                onStartFrameChanged: {
//                    console.log("updates S " + startFrame)
                    iPos.startFrame = startFrame
                }
                onEndFrameChanged: {
//                    console.log("updates E " + endFrame)
                    iPos.endFrame = endFrame
                }
                onCurrentFrameChanged: {
//                    console.log("updates C " + currentFrame)
                    iPos.currentFrame = currentFrame
                }
                onNFramesChanged: {
//                    console.log("updates N " + nFrames)
                    iPos.nFrames = nFrames
                }
            }

            Palette {
                id: palette
                width: 50
                height: mainWindow.height - 100
                y: 0
                x: mainWindow.width - palette.width
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
//                    console.log("begin button pressed")
                    aBinImageFile.begin()
                }
            }

            Button {
                width: 32
                height: 32
                Image {source: "playback_rew.png"}

                onClicked: {
//                    console.log("rewind 1 button pressed")
                    aBinImageFile.back()
                }
            }

            Button {
                width: 32
                height: 32
                Image {source: "playback_play.png"}

                onClicked: {
//                    console.log("play button pressed")
                    aBinImageFile.play()
                    timer.start()
                }
            }

            Button {
                width: 32
                height: 32
                Image {source: "playback_pause.png"}

                onClicked: {
//                    console.log("pause button pressed")
                    timer.stop()
                    aBinImageFile.pause()
                }
            }

            Button {
                width: 32
                height: 32
                Image {source: "playback_ff.png"}

                onClicked: {
//                    console.log("forward button pressed")
                    aBinImageFile.forward()
                }
            }

            Button {
                width: 32
                height: 32
                Image {source: "playback_end.png"}

                onClicked: {
//                    console.log("end button pressed")
                    aBinImageFile.end()
                }
            }

            CheckButton {
                id: loopy

                onFlipped: {aBinImageFile.loop(tsState)}
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
//                    timer.start()
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
