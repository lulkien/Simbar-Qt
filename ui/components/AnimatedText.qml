import QtQuick
import Simbar

BaseText {
    id: root

    property int duration: 300
    property var contentCached: [] // cache all state of text

    QtObject {
        id: self
        property int updateInterval: 50
        property int cachedIndex: 0
        readonly property int maxStep: 10

        function createTextState(currentText, targetText) {
            root.contentCached = [currentText]
            cachedIndex = 0

            var fromSize = currentText.length
            var toSize = targetText.length

            var diffSize = Math.abs(toSize - fromSize)
            var diffStep = Math.ceil(diffSize / Math.min(diffSize, maxStep))

            self.updateInterval = Math.floor(
                        root.duration / (diffSize / diffStep))

            var i = 0

            if (fromSize < toSize) {
                for (i = fromSize + diffStep; i < toSize; i += diffStep) {
                    root.contentCached.push(getRandomString(i))
                }
            } else if (fromSize > toSize) {
                for (i = fromSize - diffStep; i > toSize; i -= diffStep) {
                    root.contentCached.push(getRandomString(i))
                }
            }

            root.contentCached.push(targetText)
        }

        function getRandomString(length) {
            const chars = 'ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789!@#$%^&*()'
            let result = ''
            for (var i = 0; i < length; i++) {
                result += chars.charAt(Math.floor(Math.random() * chars.length))
            }
            return result
        }
    }

    Timer {
        id: textUpdateTimer
        running: false
        repeat: true
        interval: self.updateInterval
        triggeredOnStart: true
        onTriggered: {
            if (self.cachedIndex < root.contentCached.length) {
                root.text = root.contentCached[self.cachedIndex]
                self.cachedIndex += 1
                return
            }

            root.contentCached = []
            self.cachedIndex = 0
            self.updateInterval = 50
            running = false
        }
    }

    function updateText(newText) {
        textUpdateTimer.stop()

        if (newText === root.text) {
            return
        }

        var step = Math.floor(root.duration / self.updateInterval)

        self.createTextState(root.text, newText)

        textUpdateTimer.start()
    }

    function instantUpdateText(newText) {
        root.text = newText
    }
}
