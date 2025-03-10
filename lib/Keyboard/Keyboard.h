#ifndef KEYBOARD_H
#define KEYBOARD_H

#include "enum.h"
#include <Arduino.h>
class Keyboard {
public:
    // 建構子
    Keyboard(int p1, int p2, int p3, int p4);
    void Init();         // 模組初始化，設定相關腳位
    KB_state Update();       // 主更新函式，需放在 loop() 中持續呼叫以更新狀態
    KB_state state; // 當前狀態
    int fnPage;

private:
    unsigned long _detectTime;

    int _p1;
    int _p2;
    int _p3;
    int _p4;

    bool _lastState1;
    bool _lastState2;
    bool _lastState3;
    bool _lastState4;
};

#endif