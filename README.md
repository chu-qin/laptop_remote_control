# laptop_remote_control
ESP32配合舵机实现远程物理按键开机
1. App 端设置
在写代码之前，先在手机上配置好控制界面：

下载 Blinker App 并注册。

点击 添加设备 -> 独立设备 -> 网络接入。

选择你的设备（通常选 ESP32），系统会生成一个 Secret Key（授权码），复制并保存好，代码里要用。

进入设备控制面板，点击 右上角修改 -> 添加按键。

重要： 点击这个按键，在“组件键名”处输入 btn-power（这个键名必须和代码里一致）。

保存退出，现在你的 App 上就有了一个控制按钮。

2. 准备库文件
在 Arduino IDE 中，点击 工具 -> 管理库，搜索并安装：

Blinker (作者: blinker-iot)

ESP32Servo

3. 程序代码
将以下代码中的 Your_Auth_Key、Your_WiFi_SSID 和 Your_WiFi_Password 替换为你自己的信息。
共地：用杜邦线连接 WROOM 的 GND 和 CAM 的 GND（极其重要，否则信号无法传递）。

触发线：将 WROOM 的 D22 连接到 CAM 的 D13。

舵机：依然接在 WROOM 的 D21。
