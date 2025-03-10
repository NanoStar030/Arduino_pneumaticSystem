#include "Keyboard.h"
Keyboard::Keyboard(int pin1, int pin2, int pin3, int pin4){
    this->_p1 = pin1;
    this->_p2 = pin2;
    this->_p3 = pin3;
    this->_p4 = pin4;
    this->fnPage = 0;
    this->_detectTime = 0;
    this->state = KB_IDLE;
    this->_lastState1 = false;
    this->_lastState2 = false;
    this->_lastState3 = false;
    this->_lastState4 = false;
}

void Keyboard::Init(){
    pinMode(_p1, INPUT);
    pinMode(_p2, INPUT);
    pinMode(_p3, INPUT);
    pinMode(_p4, INPUT);
}

KB_state Keyboard::Update(){
    if(millis()-_detectTime < 100) return state; // 防止重複讀取
    bool temp;
    temp = digitalRead(_p1);
    if(!_lastState1 && temp){
        fnPage ++;
        if(fnPage > 1)
            fnPage = 0;
        state = KB_CHP;
    }
    _lastState1 = temp;

    temp = digitalRead(_p2);
    if(!_lastState2 && temp){
        switch (fnPage)
        {
            case 0:
                state = KB_FN1;
                break;
            case 1:
                state = KB_FN4;
                break;
        }
    }
    _lastState2 = temp;

    temp = digitalRead(_p3);
    if(!_lastState3 && temp){
        switch (fnPage)
        {
            case 0:
                state = KB_FN2;
                break;
            case 1:
                state = KB_FN5;
                break;
        }
    }
    _lastState3 = temp;

    temp = digitalRead(_p4);
    if(!_lastState4 && temp){
        switch (fnPage)
        {
            case 0:
                state = KB_FN3;
                break;
            case 1:
                state = KB_FN6;
                break;
        }
    }
    _lastState4 = temp;

    if(state != KB_IDLE){ // 狀態被切換，更新切換的時間
        _detectTime = millis();
    }
    return state;
}