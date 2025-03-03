#include "Keyboard.h"
Keyboard::Keyboard(int pin1, int pin2, int pin3, int pin4){
    this->_p1 = pin1;
    this->_p2 = pin2;
    this->_p3 = pin3;
    this->_p4 = pin4;
    this->state = KB_IDLE;
}

void Keyboard::Init(){
    pinMode(_p1, INPUT);
    pinMode(_p2, INPUT);
    pinMode(_p3, INPUT);
    pinMode(_p4, INPUT);
}

void Keyboard::Update(){
    if(digitalRead(_p1)){
        state = KB_BTN1;
    }
    if(digitalRead(_p2)){
        state = KB_BTN2;
    }
    if(digitalRead(_p3)){
        state = KB_BTN3;
    }
    if(digitalRead(_p4)){
        state = KB_BTN4;
    }
    state = KB_IDLE;
}