
#ifndef PNEU_GRIPPER_H
#define PNEU_GRIPPER_H
#include "enum.h"
#include "HX711.h"
#include "Arduino.h"
class PneuGripper {
    public:
        PneuGripper(int m, int h, int d, int s, uint8_t lp, uint8_t tp);
        unsigned char scaleWeight[17];
        void Init(void);    //初始化
        void Move(void);    //切換夾爪上或下
        void Hold(void);    //切換夾爪開或合
        void Exit(void);
        PG_state Update(bool isAuto, CB_state cb_state);
        float Tare(int times);   //使用夾爪的重量感測器量測
        float Zero(void);   //重量感測器歸零
        float Ratio(void);  //計算重量感測器斜率
    private:
        HX711 scale;
        PG_state _state;
        float scaleZero = 0;//-1325
        float scaleRatio = 4.9;
        float scaleSampleWeight = 94.63; //94.63; 

        unsigned long _startTime;
        unsigned long _timeoutTime = 2000; // 超過代表發生 error
        int _senValue;

        int DT_Pin;   //重量感測器
        int SCK_Pin;  //重量感測器
        uint8_t TOP_Sen_Pin;  //頂端的位置偵測 pin
        uint8_t LOW_Sen_Pin;  //中間的位置偵測 pin
        int MOVE_Pin; //控制夾爪上下移動的Pin
        int HOLD_Pin; //控制夾爪夾放開合的Pin
};
#endif