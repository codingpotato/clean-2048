import QtQuick 2.0

Rectangle {
    id: cell

    property var colors: { "": "white", "2": "#eee4da", "4": "#ece0c8", "8": "#f2b179", "16": "#f59563",
        "32": "#f57c5f", "64": "#f95c3b", "128": "#edce71", "256": "#eecc61",
        "512": "#ebc850", "1024": "#edc53f" , "2048": "#eec22e", "4096": "#3d3a33",
        "8192": "#3d3a33", "16384": "#3d3a33" }

    property var fontSize: { 0: 1, 1: 45, 2: 40, 3: 35, 4: 30 }

    property alias value: lable.text
    property size size: Qt.size(20,20)
    property bool animMoveEnable: false
    property bool animResizeEnable: false

    width: parent.width / 4
    height: parent.height / 4
    color: "transparent"
    
    Rectangle {
        id: styleBtn

        width: size.width
        height: size.height
        color: colors[value]
        radius: 3
        anchors.centerIn: parent

        Text {
            id: lable
            color: value == "2" || value == "4" ? "#786f66" : "#f7f8f0"
            text: value
            style: Text.Normal
            font.family: "Tahoma"
            font.bold: true
            anchors.centerIn: parent
            font.pixelSize: fontSize[value.length]
            horizontalAlignment: Text.AlignHCenter
        }

        Behavior on width {
            enabled: animResizeEnable
            NumberAnimation {
                duration: 100
            }
        }

        Behavior on height {
            enabled: animResizeEnable
            NumberAnimation {
                duration: 100
            }
        }
    }
    Behavior on x {
        enabled: animMoveEnable
        NumberAnimation {
            duration: 100
        }
    }

    Behavior on y {
        enabled: animMoveEnable
        NumberAnimation {
            duration: 100
        }
    }
}