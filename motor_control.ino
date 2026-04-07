#define BLINKER_WIFI
#include <Blinker.h>
#include <ESP32Servo.h>

char auth[] = "b9ce166f9ec0";       // App 中获取的授权码
char ssid[] = "你のWIFI";      // WiFi 名称
char pswd[] = "zq219118";  // WiFi 密码

Servo myServo;
const int servoPin = 21;    // 舵机引脚
const int triggerPin = 22;  // 连向 CAM 的触发引脚

BlinkerButton Button1("btn-power"); // 假设你在 App 里的键名是这个

void button1_callback(const String & state) {
    BLINKER_LOG("Get button state: ", state);
    
    // 1. 舵机动作：按压开机键
    myServo.write(60); 
    delay(600);
    myServo.write(0);
    
    // 2. 触发摄像头：给 GPIO 22 一个高电平脉冲
    digitalWrite(triggerPin, HIGH);
    delay(500); // 持续半秒
    digitalWrite(triggerPin, LOW);
    
    Blinker.vibrate(); 
}

void setup() {
    Serial.begin(115200);
    
    myServo.attach(servoPin);
    myServo.write(0);
    
    pinMode(triggerPin, OUTPUT);
    digitalWrite(triggerPin, LOW); // 默认低电平

    Blinker.begin(auth, ssid, pswd);
    Button1.attach(button1_callback);
}

void loop() {
    Blinker.run();
}