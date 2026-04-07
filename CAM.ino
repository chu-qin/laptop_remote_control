#include <WiFi.h>
#include "esp_camera.h"
#include <HTTPClient.h>

const char* ssid = "你のWIFI";
const char* password = "password";
const char* uid = "uid"; 
const char* topic = "monitor006"; 

const int inputPin = 13; // 接收 WROOM 信号的引脚

// 摄像头引脚定义 (AI Thinker)
#define PWDN_GPIO_NUM     32
#define RESET_GPIO_NUM    -1
#define XCLK_GPIO_NUM      0
#define SIOD_GPIO_NUM     26
#define SIOC_GPIO_NUM     27
#define Y9_GPIO_NUM       35
#define Y8_GPIO_NUM       34
#define Y7_GPIO_NUM       39
#define Y6_GPIO_NUM       36
#define Y5_GPIO_NUM       21
#define Y4_GPIO_NUM       19
#define Y3_GPIO_NUM       18
#define Y2_GPIO_NUM        5
#define VSYNC_GPIO_NUM    25
#define HREF_GPIO_NUM     23
#define PCLK_GPIO_NUM     22

void captureAndUpload() {
    // 1. 冲刷缓冲区：连续抓取并丢弃 3 帧旧数据
    for (int i = 0; i < 3; i++) {
        camera_fb_t * fb_temp = esp_camera_fb_get();
        if (fb_temp) {
            esp_camera_fb_return(fb_temp);
            delay(100); // 给传感器一点响应时间
        }
    }

    // 2. 抓取真正的当前帧
    camera_fb_t * fb = esp_camera_fb_get();
    if (!fb) {
        Serial.println("Camera capture failed");
        return;
    }

    Serial.printf("Uploading fresh photo, size: %zu bytes\n", fb->len);

    // 3. 上传到巴法云 (会自动覆盖旧图)
    HTTPClient http;
    http.begin("http://images.bemfa.com/upload/v1/upimages.php");
    http.addHeader("Content-Type", "image/jpeg");
    http.addHeader("Authorization", uid);
    http.addHeader("Authtopic", topic);

    int httpCode = http.POST(fb->buf, fb->len);
    
    if (httpCode == 200) {
        Serial.println("Success: Photo updated on Bemfa.");
    } else {
        Serial.printf("Failed, error code: %d\n", httpCode);
    }

    http.end();
    esp_camera_fb_return(fb); // 释放内存
}

void setup() {
  Serial.begin(115200);
  pinMode(inputPin, INPUT_PULLDOWN);

  // 初始化摄像头
  camera_config_t config;
  config.ledc_channel = LEDC_CHANNEL_0;
  config.ledc_timer = LEDC_TIMER_0;
  config.pin_d0 = Y2_GPIO_NUM; config.pin_d1 = Y3_GPIO_NUM;
  config.pin_d2 = Y4_GPIO_NUM; config.pin_d3 = Y5_GPIO_NUM;
  config.pin_d4 = Y6_GPIO_NUM; config.pin_d5 = Y7_GPIO_NUM;
  config.pin_d6 = Y8_GPIO_NUM; config.pin_d7 = Y9_GPIO_NUM;
  config.pin_xclk = XCLK_GPIO_NUM; config.pin_pclk = PCLK_GPIO_NUM;
  config.pin_vsync = VSYNC_GPIO_NUM; config.pin_href = HREF_GPIO_NUM;
  config.pin_sscb_sda = SIOD_GPIO_NUM; config.pin_sscb_scl = SIOC_GPIO_NUM;
  config.pin_pwdn = PWDN_GPIO_NUM; config.pin_reset = RESET_GPIO_NUM;
  config.xclk_freq_hz = 20000000; config.pixel_format = PIXFORMAT_JPEG;
  config.frame_size = FRAMESIZE_VGA; config.jpeg_quality = 12; config.fb_count = 1;

  esp_camera_init(&config);

  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) delay(500);
}

void loop() {
  // 检查是否收到 WROOM 的拍照指令
  if (digitalRead(inputPin) == HIGH) {
    Serial.println("Triggered by WROOM!");
    delay(10000); // 等待 3 秒让电脑屏幕亮起
    captureAndUpload();
    while(digitalRead(inputPin) == HIGH) delay(100); // 防止重复触发
  }
}
