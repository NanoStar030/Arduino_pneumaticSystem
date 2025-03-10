#include "main.h"
#include "Keyboard.h"
#include "PneuGripper.h"
#include "ConveyorBelt.h"
#include "LCD12864RSPI.h"
#include <Arduino.h>

uchar emptyChar[] = "                 "; // 17 個空白符號
uchar valueChar[] = "                 ";
uchar pageChar[] = "         page: 0 ";
float value;

bool isAuto = true;
bool isMeasure = false;
CB_state cb_state = CB_IDLE;
KB_state kb_state = KB_IDLE;
PG_state pg_state1 = PG_IDLE;

// CB_state cb_state_o = CB_IDLE;
// PG_state pg_state1_o = PG_IDLE;
LCD12864RSPI LCDA = LCD12864RSPI(53, 51, 49);
ConveyorBelt conveyor(motorPin, motorSensorPin, 1.0);
Keyboard keyboard = Keyboard(PIN_KB1, PIN_KB2, PIN_KB3, PIN_KB4);
PneuGripper gripper1 = PneuGripper(PIN_MV_PG1, PIN_HL_PG1, PIN_DT_PG1, PIN_SCK_PG1, A0, A1);

void setup() {
    Serial.begin(115200);
    keyboard.Init();
    conveyor.Init();
    LCDA.Init();    // 初始化螢幕
    LCDA.Clear();
    LCDA.DisplayFullScreen((uchar*)"System Starting", emptyChar, emptyChar, pageChar);
    gripper1.Init();
    LCDA.DisplayFullScreen((uchar*)"System Ready", emptyChar, emptyChar, pageChar);
}
void AutoSwitch(){
    if(isAuto){
        isAuto = false;
        LCDA.DisplayFullScreen((uchar*)"System Auto: OFF", emptyChar, emptyChar, pageChar);
    }
    else{
        isAuto = true;
        LCDA.DisplayFullScreen((uchar*)"System Auto: ON", emptyChar, emptyChar, pageChar);
    }
}
void loop() {
    kb_state = keyboard.Update();
    cb_state = conveyor.Update(isAuto, pg_state1);
    pg_state1 = gripper1.Update(isAuto, cb_state);
    if(isAuto && pg_state1== PG_ERROR){
        LCDA.DisplayFullScreen(PG_state_str[pg_state1], (uchar*)"Please Restart", (uchar*)"The System", pageChar);
    }
    // Keyboard Control
    if(kb_state != KB_IDLE){ // **可以的話包在 KB 中**
        switch(kb_state){
            case KB_CHP: pageChar[15] = '0' + keyboard.fnPage;
                         LCDA.DisplayFullScreen((uchar*)"Change page      ", emptyChar, emptyChar, pageChar);    
                         break;
            case KB_FN1: AutoSwitch(); break;
            case KB_FN2: value = gripper1.Zero();  
                         dtostrf(value, 14, 4, (char *)valueChar);
                         LCDA.DisplayFullScreen((uchar*)"Zero:            ", (uchar*)valueChar, emptyChar, pageChar);    
                         break;
            case KB_FN3: value = gripper1.Ratio();
                         dtostrf(value, 14, 4, (char *)valueChar);
                         LCDA.DisplayFullScreen((uchar*)"Ratio:           ", (uchar*)valueChar, emptyChar, pageChar);    
                         break;
            case KB_FN4: if(!isAuto){
                            gripper1.Hold();
                            LCDA.DisplayFullScreen((uchar*)"Gripper1 Hold    ", emptyChar, emptyChar, pageChar);
                         } 
                         else{
                            LCDA.DisplayFullScreen((uchar*)"Please Cancel    ", (uchar*)"System Auto      ", emptyChar, pageChar);
                         } break;
            case KB_FN5: if(!isAuto){
                            gripper1.Move();
                            LCDA.DisplayFullScreen((uchar*)"Gripper1 Move    ", emptyChar, emptyChar, pageChar);      
                         } 
                         else{
                            LCDA.DisplayFullScreen((uchar*)"Please Cancel    ", (uchar*)"System Auto      ", emptyChar, pageChar);
                         } break;
            case KB_FN6: if(!isAuto){
                            conveyor.Move();
                            LCDA.DisplayFullScreen((uchar*)"Conveyor Move    ", emptyChar, emptyChar, pageChar);
                         }                          
                         else{
                            LCDA.DisplayFullScreen((uchar*)"Please Cancel    ", (uchar*)"System Auto      ", emptyChar, pageChar);
                         } break;
            default: break;
        }
        keyboard.state = KB_IDLE; // 重置 KB 狀態
    }
    // Gripper1 Control
    int prevValue = 0;
    if(pg_state1 == PG_TARE){
        Serial.println("=== ===");
        int stableCount = 0;  // 計算連續穩定的數據筆數
        long startTime = millis();
        while (stableCount < 3 && millis()-startTime < 1000) {
            float currentValue = gripper1.Tare(1);
            Serial.println(currentValue);
            if (abs(currentValue - prevValue) < 1) {
                stableCount++; // 記錄穩定數據
            } else {
                stableCount = 0; // 如果變動過大，重新計算
            }
            prevValue = currentValue; // 更新前一次數值
        }
        gripper1.Tare(3);
        LCDA.DisplayFullScreen((uchar*)"Measurement:", (uchar*)gripper1.scaleWeight, emptyChar, pageChar);    
        gripper1.Exit(); // 切換下一個狀態
    }
    // Serial Control
    if(Serial){
        if(Serial.available() > 0) {
            char inputChar = Serial.read();
            switch (inputChar){
                case '1':   gripper1.Move();    break; // 移動夾爪
                case '2':   gripper1.Hold();           // 夾爪開合
                            break;
                case '3':   if(isMeasure){
                                isMeasure = false;
                            }
                            else{
                                isMeasure = true;
                            }
                            break;
                case 'z':   Serial.print("Gripper1 zero: "); // 重新歸零(可以不用收爪)
                            Serial.println(gripper1.Zero());
                            break;
                case 'r':   Serial.print("Gripper1 ratio: ");// 計算斜率
                            Serial.println(gripper1.Ratio());
                            break;
                case 'a':   if(isAuto){
                                isAuto = false;
                            }
                            else{
                                isAuto = true;
                            } break;
                case 'b':   conveyor.Move(); break;
                default:    Serial.println("Invalid Input..."); break;
            }
        }
        // Measure Debug
        if(isMeasure){
            Serial.println(gripper1.Tare(1));
            delay(90);
        }
    }
    delay(10);
}
