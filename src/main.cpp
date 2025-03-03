#include "main.h"
#include "Keyboard.h"
#include "ConveyorBelt.h"
#include "PneuGripper.h"
#include "LCD12864RSPI.h"
#include <Arduino.h>

typedef unsigned char uchar;
uchar lcd_char[] = "                 "; // 17 個空白符號

bool isMeasure = false;
LCD12864RSPI LCDA = LCD12864RSPI(13, 12, 11);
ConveyorBelt conveyor(motorPin, motorSensorPin, 1.0);
Keyboard keyboard = Keyboard(PIN_KB1, PIN_KB2, PIN_KB3, PIN_KB4);
PneuGripper gripper1 = PneuGripper(PIN_MV_PG1, PIN_HL_PG1, PIN_DT_PG1, PIN_SCK_PG1);

void setup() {
    Serial.begin(115200);
    keyboard.Init();
    conveyor.Init();
    LCDA.Init();    // 初始化螢幕
    LCDA.Clear();
    LCDA.DisplayFullScreen((uchar*)"System-Starting  ", lcd_char, lcd_char, lcd_char);
    gripper1.Init();
    LCDA.DisplayFullScreen((uchar*)"System-Ready     ", lcd_char, lcd_char, lcd_char);
}
void ConveyorMove(int time){
    digitalWrite(motorPin, !digitalRead(motorPin));        // 時間控制傳輸帶
    delay(time);
    digitalWrite(motorPin, !digitalRead(motorPin));
}
void Gripper1Auto(){
    gripper1.Move();    // 夾爪自動跑流程
    delay(1000);
    gripper1.Hold();
    delay(100);
    gripper1.Move();
    delay(1000);
    Serial.println("=== ===");
    for(int i = 0;i<=10;i++){ // 一次大約 delay(100)
        Serial.println(gripper1.Tare());
    }
    // 只顯示最後一次的讀值(最後一次比較符合) 暫定
    LCDA.DisplayFullScreen((uchar*)"Measurement:     ", (uchar*)gripper1.scaleWeight, lcd_char, lcd_char);    
    gripper1.Hold();
}
void loop() {
    keyboard.Update();
    conveyor.Update();
    if(keyboard.state != KB_IDLE){
        switch (keyboard.state){
            case KB_BTN1: gripper1.Zero(); break;
            case KB_BTN2: ConveyorMove(3000); break;
            case KB_BTN3: Gripper1Auto(); break;
            case KB_BTN4: break;
        }
    }
    if(Serial){
        if(Serial.available() > 0) {
            char inputChar = Serial.read();
            switch (inputChar){
                case '1':   gripper1.Move();    break; // 移動夾爪
                case '2':   gripper1.Hold();           // 夾爪開合
                            if(isMeasure){
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
                case 'a':   ConveyorMove(3000); break;
                case 'b':   Gripper1Auto(); break;
                default:    Serial.println("Invalid Input..."); break;
            }
        }
    }
    if(isMeasure){
        Serial.println(gripper1.Tare());
    }
    delay(100);
}
