#include "ConveyorBelt.h"

// 定義馬達 PWM 腳位、感測器腳位及馬達速度
// 請根據實際接線更改這些腳位號碼
const int motorPWMPin = 9;    // 例如，使用 9 號 PWM 腳位
const int sensorPin = 2;      // 例如，使用 2 號數位腳位讀取感測器
const int motorSpeed = 128;   // 馬達 PWM 速度 (0 ~ 255)，可根據需求調整

// 建立一個 ConveyorBelt 物件
ConveyorBelt conveyor(motorPWMPin, sensorPin, motorSpeed);

void setup() {
  Serial.begin(9600);   // 初始化序列埠，方便除錯訊息輸出
  conveyor.begin();     // 初始化輸送帶模組，設定腳位
}

void loop() {
  // 持續更新模組狀態，讓輸送帶根據感測器狀態自動進行相應動作
  conveyor.update();
}
