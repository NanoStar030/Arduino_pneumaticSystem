#ifndef KEYBOARD_H
#define KEYBOARD_H

#include <Arduino.h>
enum KeyboardState {
    KB_BTN1,
    KB_BTN2,
    KB_BTN3,
    KB_BTN4,
    KB_IDLE
};

class Keyboard {
public:
    // 建構子
    Keyboard(int p1, int p2, int p3, int p4);
    void Init();         // 模組初始化，設定相關腳位
    void Update();       // 主更新函式，需放在 loop() 中持續呼叫以更新狀態
    KeyboardState state; // 當前狀態

private:
    int _p1;
    int _p2;
    int _p3;
    int _p4;
};

#endif