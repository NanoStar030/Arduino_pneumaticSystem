
#ifndef PneuGripper_h
#define PneuGripper_h
#include "HX711.h"
#include "Arduino.h"

class PneuGripper {
    enum PG_status{

    };
    public:
        PneuGripper(int m, int h, int d, int s);
        unsigned char scaleWeight[17];
        void Init(void);    //初始化
        void Move(void);    //切換夾爪上或下
        void Hold(void);    //切換夾爪開或合
        float Tare(void);   //使用夾爪的重量感測器量測
        float Zero(void);   //重量感測器歸零
        float Ratio(void);  //計算重量感測器斜率
    private:
        HX711 scale;
        float scaleZero = 3*304.63;
        float scaleRatio = 304.63;
        float scaleSampleWeight = 69.70; //89.05

        int DT_Pin;   //重量感測器
        int SCK_Pin;  //重量感測器
        int MOVE_Pin; //控制夾爪上下移動的Pin
        int HOLD_Pin; //控制夾爪夾放開合的Pin
};
#endif