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
    digitalWrite(_motorPin, LOW);
}

void ConveyorBelt::StopMotor() { // 停止馬達
    digitalWrite(_motorPin, HIGH);
}

void ConveyorBelt::Exit(){
    StartMotor();
    _state = CB_EXIT;
    _transportStartTime = millis();
}

void ConveyorBelt::Move(){
    digitalWrite(_motorPin, !digitalRead(_motorPin));        // 控制傳輸帶
}

CB_state ConveyorBelt::Update(bool isAuto, PG_state pg_state) { // 主更新函式：根據感測器讀取值與狀態機進行狀態轉換與操作
    if(!isAuto) return CB_IDLE;
    if(pg_state==PG_EXIT){
        Exit();    
    }    
    else if(pg_state!=PG_IDLE) return CB_GRIPPING;

    int sensorValue = digitalRead(_sensorPin); // 讀取感測器狀態，1 表示無物體，0 表示有物體
    switch (_state) {
        case CB_IDLE:
        if (sensorValue == LOW) {  // IDLE: 感測器讀到 0，則開始偵測物體
            _state = CB_DETECTING;        // 切換 CB_DETECTING
            _detectStartTime = millis();  // 記錄 偵測開始時間
            StartMotor();                 // 啟動 馬達
        } break;
      
        case CB_DETECTING:
        if (sensorValue == HIGH) {  // DETECTING: 持續移動直到感測器從 0 轉為 1
            _detectDuration = millis() - _detectStartTime; // 計算物體長度所花費的時間
            _state = CB_TRANSPORTING;                         // 切換到輸送狀態
            _transportStartTime = millis();                // 記錄輸送狀態開始時間
        } 
        else if(millis()-_detectStartTime>_timeoutTime){
            _state = CB_IDLE;
            StopMotor();
        }
        break;
      
        case CB_TRANSPORTING:
        if (millis() - _transportStartTime > 0.2*_detectDuration) {  // 輸送狀態：繼續移動直到達到物體長度所代表的時間
            StopMotor();
            _state = CB_GRIPPING;
        } break;
        
        case CB_EXIT:
        if (millis() - _transportStartTime > 1.5*_detectDuration) {  // 輸送狀態：繼續移動直到達到物體長度所代表的時間
            StopMotor();
            _state = CB_IDLE;
        } break;

        // default:  // 任何異常狀態下，先停下馬達並回到 IDLE 狀態
        // StopMotor();
        // _state = CB_IDLE;
        // break;
    }
    return _state;
}
