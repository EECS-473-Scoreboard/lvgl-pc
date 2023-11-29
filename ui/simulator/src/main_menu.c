#include "main_menu.h"
#include "common.h"

#include "lvgl/src/core/lv_event.h"
#include "lvgl/src/core/lv_obj.h"
#include "lvgl/src/core/lv_obj_pos.h"
#include "lvgl/src/core/lv_obj_tree.h"
#include "lvgl/src/misc/lv_area.h"
#include "lvgl/src/misc/lv_color.h"
#include "lvgl/src/misc/lv_txt.h"
#include "lvgl/src/widgets/lv_btn.h"
#include "lvgl/src/widgets/lv_label.h"
#include "lvgl/src/widgets/lv_textarea.h"

static main_menu_state_t state;

static enum screen_state_t {
    IDLE,
    P1_PARING,
    P2_PARING,
} menu_state;

static lv_obj_t *p1_textarea;
static lv_obj_t *p1_pair_btn;
static lv_obj_t *p1_pair_lbl;
static lv_obj_t *p2_textarea;
static lv_obj_t *p2_pair_btn;
static lv_obj_t *p2_pair_lbl;
static lv_obj_t *start_btn_lbl;

static void start_btn_pressed(lv_event_t *_) {
    if (menu_state != IDLE) {
        lv_label_set_text(start_btn_lbl, "Still pairing");
        return;
    }

    if (state.wearable_1 == NO_WEARABLE || state.wearable_2 == NO_WEARABLE) {
        lv_label_set_text(start_btn_lbl, "Device not paired");
        return;
    }

    if (state.p1_name[0] == 0 || state.p2_name[0] == 0) {
        lv_label_set_text(start_btn_lbl, "Player name missing");
        return;
    }

    for (size_t i = 0; i < MAX_NAME_LEN; i++) {
        if (state.p1_name[i] != state.p2_name[i]) {
            // names field populated and different, everything ready
            state.ready_state = MAIN_MENU_GO_GAME;
            return;
        }
        if (state.p1_name[i] == 0) {
            // every char is the same till \0, duplicate names
            break;
        }
    }
    lv_label_set_text(start_btn_lbl, "Player names identical");
}

static void wearable_packet_rcvd(lv_event_t *e) {
    // on the 32-bit target:
    // wearable_event_t event = (wearable_event_t)e->param;

    bool is_first_wearable = e->param == (void *)0xDEAD;

    switch (menu_state) {
    case IDLE:
        return;
    case P1_PARING:
        lv_label_set_text(lv_obj_get_child(p1_pair_btn, 0), "Pair");
        lv_obj_clear_state(p1_pair_btn, LV_STATE_CHECKED);
        lv_label_set_text(p1_pair_lbl, is_first_wearable ? "0xDEAD" : "0xBEEF");
        state.wearable_1 = is_first_wearable ? 0xDEAD : 0xBEEF;
        break;
    case P2_PARING:
        lv_label_set_text(lv_obj_get_child(p2_pair_btn, 0), "Pair");
        lv_obj_clear_state(p2_pair_btn, LV_STATE_CHECKED);
        lv_label_set_text(p2_pair_lbl, is_first_wearable ? "0xDEAD" : "0xBEEF");
        state.wearable_2 = is_first_wearable ? 0xDEAD : 0xBEEF;
        break;
    }

    // the same wearable paired to two players
    if (state.wearable_1 == state.wearable_2) {
        // discard the previous pair
        if (menu_state == P1_PARING) {
            lv_label_set_text(p2_pair_lbl, "<NONE>");
            state.wearable_2 = NO_WEARABLE;
        } else {
            lv_label_set_text(p1_pair_lbl, "<NONE>");
            state.wearable_1 = NO_WEARABLE;
        }
    }

    menu_state = IDLE;
}

static void pair_btn_pressed(lv_event_t *e) {
    lv_obj_t *btn = lv_event_get_target(e);
    switch (menu_state) {
    case IDLE:
        lv_label_set_text(lv_obj_get_child(btn, 0), "Pairing");
        menu_state = btn == p1_pair_btn ? P1_PARING : P2_PARING;
        break;
    case P1_PARING:
        if (btn == p1_pair_btn) {
            lv_label_set_text(lv_obj_get_child(p1_pair_btn, 0), "Pair");
            menu_state = IDLE;
        } else {
            lv_label_set_text(lv_obj_get_child(p1_pair_btn, 0), "Pair");
            lv_obj_clear_state(p1_pair_btn, LV_STATE_CHECKED);
            lv_label_set_text(lv_obj_get_child(p2_pair_btn, 0), "Pairing");
            menu_state = P2_PARING;
        }
        break;
    case P2_PARING:
        if (btn == p2_pair_btn) {
            lv_label_set_text(lv_obj_get_child(p2_pair_btn, 0), "Pair");
            menu_state = IDLE;
        } else {
            lv_label_set_text(lv_obj_get_child(p2_pair_btn, 0), "Pair");
            lv_obj_clear_state(p2_pair_btn, LV_STATE_CHECKED);
            lv_label_set_text(lv_obj_get_child(p1_pair_btn, 0), "Pairing");
            menu_state = P1_PARING;
        }
        break;
    }
}

inline static void set_textarea_style(lv_obj_t *ta) {
    lv_textarea_set_one_line(ta, true);
    lv_textarea_set_max_length(ta, MAX_NAME_LEN);
    lv_obj_set_width(ta, 216);
    lv_obj_set_height(ta, 42);
    lv_obj_set_style_text_align(ta, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN);
}

inline static void set_pair_btn_style(lv_obj_t *btn) {
    lv_obj_set_width(btn, 116);
    lv_obj_set_height(btn, 42);
    lv_obj_add_flag(btn, LV_OBJ_FLAG_CHECKABLE);

    lv_obj_t *lbl = lv_label_create(btn);
    lv_obj_center(lbl);
    lv_label_set_text(lbl, "Pair");
}

void main_menu_build(lv_obj_t *scr) {
    state.wearable_1 = NO_WEARABLE;
    state.wearable_2 = NO_WEARABLE;
    state.ready_state = MAIN_MENU_STAY;
    state.game = CORNHOLE;
    menu_state = IDLE;

    lv_obj_set_style_bg_color(scr, COLOR_GREY, LV_PART_MAIN);

    lv_obj_add_event_cb(scr, wearable_packet_rcvd, SC_EVENT_WEARABLE, NULL);

    lv_obj_t *title = lv_label_create(scr);
    lv_label_set_text(title, "SCORE++ | Scoreboard");
    lv_obj_set_style_text_align(title, LV_TEXT_ALIGN_CENTER, 0);
    lv_obj_align(title, LV_ALIGN_CENTER, 0, -360);

    p1_textarea = lv_textarea_create(scr);
    state.p1_name = lv_textarea_get_text(p1_textarea);
    set_textarea_style(p1_textarea);
    lv_obj_set_pos(p1_textarea, 12, 84);
    lv_obj_t *p1_label = lv_label_create(scr);
    lv_label_set_text(p1_label, "Player 1");
    lv_obj_align_to(p1_label, p1_textarea, LV_ALIGN_BOTTOM_MID, 0, 40);

    p1_pair_btn = lv_btn_create(scr);
    lv_obj_add_event_cb(p1_pair_btn, pair_btn_pressed, LV_EVENT_CLICKED, NULL);
    set_pair_btn_style(p1_pair_btn);
    lv_obj_set_pos(p1_pair_btn, 256, 84);
    p1_pair_lbl = lv_label_create(scr);
    lv_label_set_text(p1_pair_lbl, "<NONE>");
    lv_obj_align_to(p1_pair_lbl, p1_pair_btn, LV_ALIGN_BOTTOM_MID, 0, 40);

    p2_textarea = lv_textarea_create(scr);
    state.p2_name = lv_textarea_get_text(p2_textarea);
    set_textarea_style(p2_textarea);
    lv_obj_set_pos(p2_textarea, 12, 186);
    lv_obj_t *p2_label = lv_label_create(scr);
    lv_label_set_text(p2_label, "Player 2");
    lv_obj_align_to(p2_label, p2_textarea, LV_ALIGN_BOTTOM_MID, 0, 40);

    p2_pair_btn = lv_btn_create(scr);
    lv_obj_add_event_cb(p2_pair_btn, pair_btn_pressed, LV_EVENT_CLICKED, NULL);
    set_pair_btn_style(p2_pair_btn);
    lv_obj_set_pos(p2_pair_btn, 256, 186);
    p2_pair_lbl = lv_label_create(scr);
    lv_label_set_text(p2_pair_lbl, "<NONE>");
    lv_obj_align_to(p2_pair_lbl, p2_pair_btn, LV_ALIGN_BOTTOM_MID, 0, 40);

    lv_obj_t *start_btn = lv_btn_create(scr);
    lv_obj_add_event_cb(start_btn, start_btn_pressed, LV_EVENT_CLICKED, NULL);
    lv_obj_set_width(start_btn, 360);
    lv_obj_set_height(start_btn, 200);
    lv_obj_set_pos(start_btn, 20, 580);
    start_btn_lbl = lv_label_create(start_btn);
    lv_obj_center(start_btn_lbl);
    lv_label_set_text(start_btn_lbl, "START!!!");
}

void main_menu_reset() {
    state.ready_state = MAIN_MENU_STAY;
    menu_state = IDLE;
    lv_label_set_text(start_btn_lbl, "START!!!");
}

main_menu_state_t *main_menu_ready() {
    return &state;
}