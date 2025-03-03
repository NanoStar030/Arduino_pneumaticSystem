#include "ConveyorBelt.h"

// 建構子實作，初始化成員變數
ConveyorBelt::ConveyorBelt(int motorPin, int sensorPin, float scale)
{
    this->_motorPin = motorPin;
    this->_sensorPin = sensorPin;
    this->_state = CB_IDLE;
    this->_detectStartTime = 0;
    this->_detectDuration = 0;
    this->_transportScale = scale;
    this->_transportStartTime = 0;
}

// 模組初始化：設定腳位模式，並先停下馬達
void ConveyorBelt::Init() {
    pinMode(_sensorPin, INPUT);   // 感測器腳位設為輸入
    pinMode(_motorPin, OUTPUT);   // 馬達腳位設為輸出
    StopMotor();                  // 初始狀態停止馬達
}

void ConveyorBelt::StartMotor() { // 啟動馬達
    analogWrite(_motorPin, LOW);
}


void ConveyorBelt::StopMotor() { // 停止馬達
    analogWrite(_motorPin, HIGH);
}


void ConveyorBelt::Update() { // 主更新函式：根據感測器讀取值與狀態機進行狀態轉換與操作
    int sensorValue = digitalRead(_sensorPin); // 讀取感測器狀態，0 表示無物體，1 表示有物體
    switch (_state) {
        case CB_IDLE:
        if (sensorValue == HIGH) {  // 在 IDLE 狀態下，若感測器讀到 1，則開始偵測物體
            _state = CB_DETECTING;           // 切換到偵測狀態
            _detectStartTime = millis();  // 記錄偵測開始時間
            StartMotor();                 // 啟動馬達開始移動輸送帶
        }
        break;
      
        case CB_DETECTING:
        if (sensorValue == LOW) {  // 偵測狀態：持續移動直到感測器從 1 轉為 0
            _detectDuration = millis() - _detectStartTime; // 計算物體長度所花費的時間
            _state = CB_TRANSPORTING;                         // 切換到輸送狀態
            _transportStartTime = millis();                // 記錄輸送狀態開始時間
        }
        break;
      
        case CB_TRANSPORTING:
        if (millis() - _transportStartTime > _transportScale*_detectDuration) {  // 輸送狀態：繼續移動直到達到物體長度所代表的時間
            StopMotor();
            _state = CB_IDLE;
        }
        break;
      
        default:  // 任何異常狀態下，先停下馬達並回到 IDLE 狀態
        StopMotor();
        _state = CB_IDLE;
        break;
    }
}
