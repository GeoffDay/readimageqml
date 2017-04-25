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
    height: Screen.height - 100

    onWindowStateChanged: {
        console.log( "onWindowStateChanged (Window), state: " +  windowState );
    }

    title: qsTr("Player")

//    Column {
//        spacing: 2
//        anchors.centerIn: mainWindow
//        x: 0
//        y: 0

        ReadImage {
            id: aPieChart
            anchors.centerIn: parent
            width: 100; height: 100
            name: "bob"
            onChartCleared: console.log("The chart has been cleared")
        }

        ImagePos {
            width: Screen.width - 600
            height: 50
            currentFrame: 34
            nFrames: 109
            startFrame: 1
            endFrame: 109
        }

        Row {
            spacing: 2
            y: 300
            MyButton {
                annunc: "Prev File"
                frColor: "#000000"
                bkColor: "#80a060"
                bdColor: "#004040"
                onActivated: {console.log("prev file button")}
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

                onActivated: {console.log("next file button pressed")}
            }

            Button {
                    width: 32
                    height: 32
                Image {source: "playback_begin.png"}

                onClicked: {console.log("begin button pressed")}
            }
            Button {
                width: 32
                height: 32
                Image {source: "playback_rew.png"}

                onClicked: {console.log("begin button pressed")}
            }
            Button {
                width: 32
                height: 32
                Image {source: "playback_play.png"}

                onClicked: {console.log("back button pressed")}
            }
            Button {
                width: 32
                height: 32
                Image {source: "playback_pause.png"}

                onClicked: {console.log("pause button pressed")}
            }
            Button {
                width: 32
                height: 32
                Image {source: "playback_ff.png"}

                onClicked: {console.log("forward button pressed")}
            }
            Button {
                width: 32
                height: 32
                Image {source: "playback_end.png"}

                onClicked: {console.log("end button pressed")}
            }

            CheckButton {
                id: loopy
                on: true
                onFlipped: { console.log("loop " + tsState)}
            }


            ComboBox {
                id: fRate
                height: 32
                font.pointSize: 16
                textRole: "FrameRate"
                model: ListModel {
                    ListElement { FrameRate: "50 fps"; value: 20 }
                    ListElement { FrameRate: "25 fps"; value: 40 }
                    ListElement { FrameRate: "10 fps"; value: 100 }
                    ListElement { FrameRate: "5 fps"; value: 200 }
                    ListElement { FrameRate: "2 fps"; value: 500 }
                    ListElement { FrameRate: "1 fps"; value: 1000 }
                }

                background: Rectangle {
                              color: "#86b88a"
                              border.color: "#76782a"
                              border.width: 1
                }
                onActivated: {
                    console.log("setting mag as a text " + 32)
                    aPieChart.setMagnification(32)
                }
              }

            MyButton {
                annunc: "Export"
                frColor: "#303030"
                bkColor: "#b6a0b0"
                bdColor: "#76782a"
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
                              border.width: 1
                }

                model: ListModel {
                    ListElement { Xport: "BIN"; value: 20 }
                    ListElement { Xport: "ARF"; value: 20 }
                    ListElement { Xport: "BMP"; value: 40 }
                    ListElement { Xport: "AVI"; value: 20 }
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
//    }


    Timer {
        interval: 1000
        running: true
        repeat: true
        triggeredOnStart: true
        onTriggered: time.text = Date().toString()
    }

    Text {
        id: time
//        y: 360
    }

    FileDialog {
        id: fileDialog
        title: "Please choose a file"
        nameFilters: ["Arf files (*.arf)", "Bin files (*.bin)"]
        folder: shortcuts.home
        onAccepted: {
            console.log("You chose: " + fileDialog.fileUrls)
            aPieChart.openIFileName(fileDialog.fileUrls)
        }
        onRejected: {
            console.log("Cancelled")
            Qt.quit()
        }

        Component.onCompleted: visible = false
    }


}
