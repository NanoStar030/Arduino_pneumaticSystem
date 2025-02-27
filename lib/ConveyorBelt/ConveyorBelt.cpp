#include "ConveyorBelt.h"

// 建構子實作，初始化成員變數
ConveyorBelt::ConveyorBelt(int motorPWMPin, int sensorPin, int motorSpeed)
: _motorPWMPin(motorPWMPin), _sensorPin(sensorPin), _motorSpeed(motorSpeed),
  _state(IDLE), _detectStartTime(0), _detectDuration(0), _transportStartTime(0)
{
}

// 模組初始化：設定腳位模式，並先停下馬達
void ConveyorBelt::begin() {
  pinMode(_motorPWMPin, OUTPUT);   // 馬達腳位設為輸出
  pinMode(_sensorPin, INPUT);      // 感測器腳位設為輸入
  stopMotor();                     // 初始狀態停止馬達
}

// 啟動馬達：以指定 PWM 速度開始運轉
void ConveyorBelt::startMotor() {
  analogWrite(_motorPWMPin, _motorSpeed);
}

// 停止馬達：將 PWM 輸出設為 0，使馬達停止
void ConveyorBelt::stopMotor() {
  analogWrite(_motorPWMPin, 0);
}

// 主更新函式：根據感測器讀取值與狀態機進行狀態轉換與操作
void ConveyorBelt::update() {
  int sensorValue = digitalRead(_sensorPin); // 讀取感測器狀態，0 表示無物體，1 表示有物體

  switch (_state) {
    case IDLE:
      // 在 IDLE 狀態下，若感測器讀到 1，則開始偵測物體
      if (sensorValue == HIGH) {
        _state = DETECTING;         // 切換到偵測狀態
        _detectStartTime = millis();  // 記錄偵測開始時間
        startMotor();               // 啟動馬達開始移動輸送帶
      }
      break;
      
    case DETECTING:
      // 偵測狀態：持續移動直到感測器從 1 轉為 0
      if (sensorValue == HIGH) {
        // 持續移動，等待物體完全通過感測區
      } else {
        // 當感測器回到 0，代表物體通過結束
        _detectDuration = millis() - _detectStartTime; // 計算物體長度所花費的時間
        _state = TRANSPORTING;   // 切換到輸送狀態
        _transportStartTime = millis(); // 記錄輸送狀態開始時間
      }
      break;
      
    case TRANSPORTING:
      // 輸送狀態：繼續移動直到達到物體長度所代表的時間
      if (millis() - _transportStartTime < _detectDuration) {
        // 仍在輸送中，持續運行馬達
      } else {
        // 達到目標時間後停止馬達，回到 IDLE 狀態
        stopMotor();
        _state = IDLE;
      }
      break;
      
    default:
      // 任何異常狀態下，先停下馬達並回到 IDLE 狀態
      stopMotor();
      _state = IDLE;
      break;
  }
}
