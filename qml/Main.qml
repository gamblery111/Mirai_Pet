import QtQuick 6.5
import QtQuick.Window 6.5

Window {
    // **给 Window 设置一个 id**
    id: rootWindow

    width: 300
    height: 300
    visible: true
    title: "Desktop Pet"

    // --- 核心魔法区域 Start ---
    
    // 1. 背景色设为透明
    color: "transparent" 
    
    // 2. 设置窗口标志位：
    // FramelessWindowHint: 去掉标题栏和边框
    // WindowStaysOnTopHint: 始终在其他窗口上面
    flags: Qt.FramelessWindowHint | Qt.WindowStaysOnTopHint

    // 拖动区域 (覆盖整个窗口)
    MouseArea {
        anchors.fill: parent // 覆盖整个窗口

        onPressed: {
            rootWindow.startSystemMove()
        }
    }


    // 这是一个占位符，代表你的宠物
    // 只要能看到这个红块浮在桌面上，且四周没有黑底/白底，你就成功了！
    Image {
        id: petImage
        width: 100
        height: 100
        source: "qrc:/resources/picture/pet.jpg"
        anchors.centerIn: parent
        
        // 简单的动画：让它呼吸一下，证明它是活的
        SequentialAnimation on scale {
            loops: Animation.Infinite
            NumberAnimation { to: 1.2; duration: 1000; easing.type: Easing.InOutQuad }
            NumberAnimation { to: 1.0; duration: 1000; easing.type: Easing.InOutQuad }
        }
    }
    
    // 加个文字
    Text {
        anchors.top: petImage.bottom
        anchors.horizontalCenter: parent.horizontalCenter
        text: "我是初始状态"
        color: "white"
        style: Text.Outline
        styleColor: "black"
    }
}
