#define PIN_DT_PG1   34 //重量感測器 DT
#define PIN_SCK_PG1  35 //重量感測器 SCK

#define PIN_HL_PG1 38
#define PIN_MV_PG1 40
#define PIN_MV_PG2 42
#define PIN_HL_PG2 44
#define PIN_MV_PG3 46
#define PIN_HL_PG3 48
#define PIN_MV_PG4 50
#define PIN_HL_PG4 52

#define motorPin 47
#define motorSensorPin 37

#define PIN_KB1 39
#define PIN_KB2 41
#define PIN_KB3 43
#define PIN_KB4 45

unsigned char PG_state_str[][18] = {
    "PG_IDLE          ",
    "PG_MOVEDOWN      ",
    "PG_RELEASE1      ",
    "PG_HOLD1         ",
    "PG_MOVEUP        ",
    "PG_TARE          ",
    "PG_RELEASE2      ",
    "PG_EXIT          ",
    "PG_ERROR         "
};