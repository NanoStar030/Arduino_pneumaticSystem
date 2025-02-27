#include "PneuGripper.h"

//constructor
PneuGripper::PneuGripper(int movePin, int holdPin, int dtPin, int sckPin)
{
    this->MOVE_Pin = movePin;
    this->HOLD_Pin = holdPin;
    this->DT_Pin = dtPin;
    this->SCK_Pin = sckPin;
    this->scale.begin(this->DT_Pin, this->SCK_Pin);
}
void PneuGripper::Init(void) { //初始化
    pinMode(MOVE_Pin, OUTPUT);
    pinMode(HOLD_Pin, OUTPUT);
    digitalWrite(MOVE_Pin, HIGH);
    digitalWrite(HOLD_Pin, LOW);
    delay(500); //等待夾爪穩定後再啟用重量感測器
    scale.set_scale();
    scale.tare();
}
void PneuGripper::Move(void){ //切換夾爪上或下
    digitalWrite(MOVE_Pin, !digitalRead(MOVE_Pin));
}
void PneuGripper::Hold(void){ //切換夾爪開或合
    digitalWrite(HOLD_Pin, !digitalRead(HOLD_Pin));
}
float PneuGripper::Zero(void){ //重新歸零
    float currentValue = scale.get_units(10);
    scaleZero = currentValue + 3*scaleRatio; //移動跟靜止狀態約差 3 g
    return scaleZero;
}
float PneuGripper::Ratio(void){//計算斜率
    float currentValue = scale.get_units(10);
    scaleRatio = (currentValue-scaleZero)/scaleSampleWeight;
    return scaleRatio;
}
float PneuGripper::Tare(void){ //使用夾爪的重量感測器量測
    float currentValue = scale.get_units(1);
    float currentWeight = (currentValue-scaleZero)/scaleRatio;
    dtostrf(currentWeight, 14, 2, (char *)scaleWeight);
    strcat((char*)scaleWeight, " g");
    return currentWeight;
}