#ifndef CONVEYOR_BELT_H
#define CONVEYOR_BELT_H

#include <Arduino.h>

// 定義輸送帶狀態列舉型態
enum ConveyorState {
  IDLE,         // 無物體，等待狀態
  DETECTING,    // 感測到物體，計時物體長度
  TRANSPORTING  // 輸送物體，依照物體長度進行移動
};

// ConveyorBelt 類別，用來封裝輸送帶控制模組
class ConveyorBelt {
public:
  // 建構子：傳入馬達 PWM 腳位、感測器腳位以及馬達速度 (0~255)
  ConveyorBelt(int motorPWMPin, int sensorPin, int motorSpeed);
  
  // 模組初始化，設定相關腳位
  void begin();
  
  // 主更新函式，需放在 loop() 中持續呼叫以更新狀態
  void update();
  
private:
  int _motorPWMPin;   // 馬達 PWM 控制腳位
  int _sensorPin;     // 感測器輸入腳位
  int _motorSpeed;    // 馬達運轉的 PWM 速度
  
  ConveyorState _state;         // 當前狀態
  unsigned long _detectStartTime;   // 進入偵測狀態時記錄的時間
  unsigned long _detectDuration;    // 感測到物體持續的時間 (代表物體長度)
  unsigned long _transportStartTime; // 輸送狀態開始時間
  
  // 啟動馬達，開始移動輸送帶
  void startMotor();
  
  // 停止馬達，使輸送帶停止移動
  void stopMotor();
};

#endif // CONVEYOR_BELT_H
