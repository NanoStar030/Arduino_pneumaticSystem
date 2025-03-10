#ifndef ENUM_H
#define ENUM_H

typedef unsigned char uchar;
enum PG_state{
    PG_IDLE,
    PG_MOVEDOWN,
    PG_RELEASE1,
    PG_HOLD1,
    PG_MOVEUP,
    PG_TARE,
    PG_RELEASE2,
    PG_EXIT,
    PG_ERROR
};


// 定義輸送帶狀態列舉型態
enum CB_state {
    CB_IDLE,         // 無物體，等待狀態
    CB_DETECTING,    // 感測到物體，計時物體長度
    CB_TRANSPORTING,  // 輸送物體，依照物體長度進行移動
    CB_GRIPPING,
    CB_EXIT
};

enum KB_state {
    KB_IDLE,
    KB_CHP,
    KB_FN1,
    KB_FN2,
    KB_FN3,
    KB_FN4,
    KB_FN5,
    KB_FN6
};

#endif