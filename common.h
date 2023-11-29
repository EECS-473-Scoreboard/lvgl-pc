#pragma once

#include "lvgl/lvgl.h"

#define MAX_NAME_LEN 12

typedef uint16_t wearable_id_t;
typedef uint16_t wearable_act_t;
typedef union {
    struct {
        wearable_id_t id;
        wearable_act_t act;
    } fields;
    uint32_t bits;
} wearable_event_t;
#define NO_WEARABLE 0

#define WEARABLE_ACT_0 '2'
#define WEARABLE_ACT_1 '3'
#define WEARABLE_ACT_2 '4'
#define WEARABLE_ACT_3 '5'

typedef enum : char {
    PINGPONG,
    CORNHOLE,
    TENNIS
} game_t;

#define COLOR_GREY lv_palette_main(LV_PALETTE_GREY)
