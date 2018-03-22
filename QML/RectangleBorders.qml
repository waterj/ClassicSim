import QtQuick 2.0

Rectangle {
    property string outerBorderColor: root.gray
    property string innerBorderColor: "black"
    property color rectColor
    property int paddingInner: 1
    property string text: "Missing"

    signal rectangleClicked()

    color: "black"

    Rectangle {
        anchors {
            fill: parent
            margins: paddingInner
        }

        color: outerBorderColor

        Rectangle {
            anchors {
                fill: parent
                margins: paddingInner
            }

            color: innerBorderColor

            Rectangle {
                anchors {
                    fill: parent
                    margins:paddingInner
                }

                color: rectColor

                MouseArea {
                    anchors.fill: parent
                    onClicked: rectangleClicked()
                }
            }
        }
    }
}