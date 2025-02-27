#include "main.h"
#include "ConveyorBelt.h"
#include "PneuGripper.h"
#include "LCD12864RSPI.h"
#include <Arduino.h>

typedef unsigned char uchar;
const int motorPWMPin = 1;
const int motorSensorPin = 1;
const int motorSpeed = 128;
const int relayPin = 39; 

uchar lcd_char[] = "                 "; // 17 個空白符號

bool isMeasure = false;
LCD12864RSPI LCDA = LCD12864RSPI(13, 12, 11);
ConveyorBelt conveyor(motorPWMPin, motorSensorPin, motorSpeed);
PneuGripper gripper1 = PneuGripper(PIN_MV_PG1, PIN_HL_PG1, PIN_DT_PG1, PIN_SCK_PG1);

void setup() {
    pinMode(relayPin, OUTPUT);
    digitalWrite(relayPin, HIGH);
    Serial.begin(115200);
    conveyor.begin();
    LCDA.Init();    // 初始化螢幕
    LCDA.Clear();
    LCDA.DisplayFullScreen((uchar*)"System-Starting  ", lcd_char, lcd_char, lcd_char);
    gripper1.Init();
    LCDA.DisplayFullScreen((uchar*)"System-Ready     ", lcd_char, lcd_char, lcd_char);
}

void loop() {
    conveyor.update();
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
                case 'q':   digitalWrite(relayPin, !digitalRead(relayPin)); break; // 手動控制傳輸帶
                case 'a':
                            digitalWrite(relayPin, !digitalRead(relayPin));        // 時間控制傳輸帶
                            delay(3000);
                            digitalWrite(relayPin, !digitalRead(relayPin));
                            break;
                case 'b':   gripper1.Move();    // 夾爪自動跑流程
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
                            break;
                default:    Serial.println("Invalid Input..."); break;
            }
        }
    }
    if(isMeasure){
        Serial.println(gripper1.Tare());
    }
    delay(100);
}
