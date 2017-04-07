import QtQuick 2.7
import QtQuick.Controls 2.0
import QtQml 2.2

Button {
    id: button
    property string annunc: "hello"
    property color frColor: "#050505"
    property color bkColor: "#d5d5d5"
    property color bdColor: "#b0b0b0"
    property int   bHeight: 32
    signal activated(real xPosition)

    text: annunc
    width: Text.implicitWidth
    height: bHeight
    font.pixelSize: bHeight * 5/8

    contentItem: Text {
        font: button.font
        color: frColor
        text: button.text
        horizontalAlignment: Text.AlignHCenter
        verticalAlignment: Text.AlignVCenter
        elide: Text.ElideRight
    }

    background: Rectangle {
        width: button.width
        height: button.height
        color: button.pressed ? Qt.darker(bkColor) : bkColor
        border.color: bdColor
        border.width: 1
        radius: 1
    }

    onPressed: button.activated(1)
}
