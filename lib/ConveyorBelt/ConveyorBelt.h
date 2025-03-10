#ifndef CONVEYOR_BELT_H
#define CONVEYOR_BELT_H

#include <Arduino.h>
#include "enum.h"


// ConveyorBelt 類別，用來封裝輸送帶控制模組
class ConveyorBelt {
public:
    // 建構子：傳入馬達 PWM 腳位、感測器腳位以及馬達速度 (0~255)
    ConveyorBelt(int motorPWMPin, int sensorPin, float scale);
    void Init();  // 模組初始化，設定相關腳位
    void Exit();
    void Move();
    CB_state Update(bool isAuto, PG_state pg_state); // 主更新函式，需放在 loop() 中持續呼叫以更新狀態
    
private:
    int _sensorPin;     // 感測器輸入腳位
    int _motorPin;      // 馬達控制腳位
    CB_state _state;              // 當前狀態
    //int _motorSpeed;    // 馬達運轉的 PWM 速度
    unsigned long _timeoutTime = 20000; // 超過就當作 error

    unsigned long _detectStartTime;    // 進入偵測狀態時記錄的時間
    unsigned long _detectDuration;     // 感測到物體持續的時間 (代表物體長度)

    unsigned int _transportScale;     // 想要輸送的距離(物體長度的多少倍)
    unsigned long _transportStartTime; // 輸送狀態開始時間
    
    void StartMotor(); // 啟動馬達，開始移動輸送帶
    void StopMotor();  // 停止馬達，使輸送帶停止移動
};

#endif // CONVEYOR_BELT_H
