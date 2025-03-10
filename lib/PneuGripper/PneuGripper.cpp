#include "PneuGripper.h"

//constructor
PneuGripper::PneuGripper(int movePin, int holdPin, int dtPin, int sckPin, uint8_t lowPin, uint8_t topPin)
{
    this->MOVE_Pin = movePin;
    this->HOLD_Pin = holdPin;
    this->DT_Pin = dtPin;
    this->SCK_Pin = sckPin;
    this->_state = PG_IDLE;
    this->_senValue = 1024;
    this->LOW_Sen_Pin = lowPin;
    this->TOP_Sen_Pin = topPin;
    this->scale.begin(this->DT_Pin, this->SCK_Pin);
}
void PneuGripper::Init(void) { // 初始化
    pinMode(MOVE_Pin, OUTPUT);
    pinMode(HOLD_Pin, OUTPUT);
    digitalWrite(MOVE_Pin, HIGH);
    digitalWrite(HOLD_Pin, HIGH);
    delay(500); // 等待夾爪穩定後再啟用重量感測器
    scale.set_scale();
    scale.tare();
}
void PneuGripper::Move(void){ // 切換夾爪上或下
    digitalWrite(MOVE_Pin, !digitalRead(MOVE_Pin));
}
void PneuGripper::Hold(void){ // 切換夾爪開或合
    digitalWrite(HOLD_Pin, !digitalRead(HOLD_Pin));
}
void PneuGripper::Exit(void){
    Hold();
    _state = PG_RELEASE2;
    _startTime = millis();
}
PG_state PneuGripper::Update(bool isAuto, CB_state cb_state){ //自動流程
    // if(cb_state == CB_DETECTING) Zero();
    if(!isAuto || cb_state != CB_GRIPPING) return PG_IDLE;
    switch (_state)
    {
        case PG_IDLE:
            Move();                     // 夾爪上移
            _state = PG_MOVEDOWN;       // 更新狀態
            _startTime = millis();      // 更新時間
            break;

        case PG_MOVEDOWN:
            _senValue = analogRead(LOW_Sen_Pin);
            if(_senValue < 500){  // 經過中間位置感測器時
                Hold();                 // 夾爪放開
                _state = PG_RELEASE1;   // 更新狀態
                _startTime = millis();  // 更新時間
            }
            else if(millis() - _startTime > _timeoutTime){  // 超過正常的時間
                _state = PG_ERROR;      // **未來處置**
            } break;

        case PG_RELEASE1:
            if(millis() - _startTime > 500){ // 等待到達底部
                Hold();                 // 夾爪收起
                _state = PG_HOLD1;      // 更新狀態
                _startTime = millis();  // 更新時間
            } break;

        case PG_HOLD1:
            if(millis() - _startTime > 500){ // 等待夾爪收起
                Move();                 // 夾爪上移
                _state = PG_MOVEUP;     // 更新狀態
                _startTime = millis();  // 更新時間
            } break;

        case PG_MOVEUP: 
            _senValue = analogRead(TOP_Sen_Pin);
            if(_senValue < 500){  // 經過頂端位置感測器時
                _state = PG_TARE;       // 更新狀態 
                _startTime = millis();  // 更新時間
            }
            else if(millis()-_startTime>_timeoutTime){  // 超過 正常的時間
                _state = PG_ERROR;      // **未來處置**
            } break;

        case PG_RELEASE2:
            if(millis() - _startTime > 100){  // 等待夾爪張開
                Hold();                 // 夾爪收起
                _state = PG_EXIT;       // 更新狀態 (由 main 讀值並顯示，之後呼叫 Exit())
                _startTime = millis();  // 更新時間
            } break;

        case PG_EXIT:
            if(millis() - _startTime > 100){  // 等待夾爪張開
                _state = PG_IDLE;       // 更新狀態
            } break;
    }
    return _state;
}
float PneuGripper::Zero(void){  // 重新歸零
    float value = scale.get_units(10);
    scaleZero = value;          // 移動跟靜止狀態約差 3 g   + 3*scaleRatio
    return scaleZero;
}
float PneuGripper::Ratio(void){ // 計算斜率
    float value = scale.get_units(10);
    scaleRatio = (value-scaleZero)/scaleSampleWeight;
    return scaleRatio;
}
float PneuGripper::Tare(int times){  // 使用夾爪的重量感測器量測
    float value = scale.get_units(times);
    float weight = (value-scaleZero)/scaleRatio;
    dtostrf(weight, 14, 2, (char *)scaleWeight);
    strcat((char*)scaleWeight, " g");
    return weight;
}