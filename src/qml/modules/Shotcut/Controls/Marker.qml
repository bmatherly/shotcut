/*
 * Copyright (c) 2021 Meltytech, LLC
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

import QtQuick 2.12
import QtQuick.Shapes 1.12

Item {
    id: root
    property real timeScale: 1.0;
    property var start: 0
    property var end: 0
    property var markerColor: 'black'
    property var text: ""
    x: 0
    width: parent.width
    height: 17

    SystemPalette { id: activePalette }

    Shape {
        id: markerStart
        width: 7
        height: 17
        x: (start * timeScale) - 7
        antialiasing: true
        ShapePath {
            strokeWidth: 1
            strokeColor: 'transparent'
            fillColor: root.markerColor
            startX: 0
            startY: 0
            PathLine { x: 0; y: 10 }
            PathLine { x: 7; y: 17 }
            PathLine { x: 7; y: 0 }
            PathLine { x: 0; y: 0 }
        }

        MouseArea {
            anchors.fill: parent
            acceptedButtons: Qt.LeftButton
            drag {
                target: parent
                axis: Drag.XAxis
                threshold: 0
                minimumX: 0
                maximumX: (end * timeScale) - 7
            }
            cursorShape: Qt.PointingHandCursor
        }
    }

    Shape {
        id: markerEnd
        width: 7
        height: 17
        x: end * timeScale
        antialiasing: true
        ShapePath {
            strokeWidth: 1
            strokeColor: 'transparent'
            fillColor: root.markerColor
            startX: 0
            startY: 0
            PathLine { x: 0; y: 17 }
            PathLine { x: 7; y: 10 }
            PathLine { x: 7; y: 0 }
            PathLine { x: 0; y: 0 }
        }
        MouseArea {
            anchors.fill: parent
            acceptedButtons: Qt.LeftButton
            drag {
                target: parent
                axis: Drag.XAxis
                threshold: 0
                minimumX: start * timeScale
                maximumX: root.width
            }
            cursorShape: Qt.PointingHandCursor
        }
    }

    Rectangle {
        anchors.left: markerStart.right
        anchors.right: markerEnd.left
        height: 7
        color: root.markerColor
    }
}
