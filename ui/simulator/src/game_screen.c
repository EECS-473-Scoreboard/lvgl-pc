#include "game_screen.h"
#include "common.h"

static game_screen_state_t state;

static lv_obj_t *game_name_lbl;
static lv_obj_t *p1_name_lbl;
static lv_obj_t *p2_name_lbl;
static lv_obj_t *p1_score_table;
static lv_obj_t *p2_score_table;
static lv_obj_t *ScoreScreenBtn;
static lv_obj_t *RetBtn;
static lv_obj_t *BTN_1;
static lv_obj_t *BTN_2;
static lv_obj_t *BTN_3;
static lv_obj_t *BTN_4;
static lv_obj_t *BTN_5;

void game_screen_init(main_menu_state_t *menu_state) {
    state.main_menu_state = menu_state;
    state.ready_state = GAME_SCR_STAY;

    switch (menu_state->game) {
    case PINGPONG:
        lv_label_set_text(game_name_lbl, "PING PONG");
        break;
    case CORNHOLE:
        lv_label_set_text(game_name_lbl, "CORNHOLE");
        break;
    case TENNIS:
        lv_label_set_text(game_name_lbl, "TENNIS");
        break;
    }
    lv_label_set_text(p1_name_lbl, menu_state->p1_name);
    lv_label_set_text(p2_name_lbl, menu_state->p2_name);

    // TODO: game init

    lv_table_set_cell_value(p1_score_table, 0, 0, "--");
    lv_table_set_cell_value(p2_score_table, 0, 0, "--");
}

static void btn_pressed(lv_event_t *e) {
    lv_obj_t *btn = lv_event_get_target(e);

    if (btn == RetBtn) {
        state.ready_state = GAME_SCR_GO_MENU;
    }
}

inline static void set_ret_btn_style(lv_obj_t *btn) {
    lv_obj_set_width(btn, 150);
    lv_obj_set_height(btn, 50);

    lv_obj_t *retLabel = lv_label_create(btn);
    lv_obj_center(retLabel);
    lv_label_set_text(retLabel, "Return");
}

inline static void set_act_btn_style(lv_obj_t *btn, char *name) {
    lv_obj_set_width(btn, 150);
    lv_obj_set_height(btn, 50);

    lv_obj_t *retLabel = lv_label_create(btn);
    lv_obj_center(retLabel);
    lv_label_set_text(retLabel, name);
}

inline static void set_scor_btn_style(lv_obj_t *btn) {
    lv_obj_set_width(btn, 150);
    lv_obj_set_height(btn, 50);
    lv_obj_add_flag(btn, LV_OBJ_FLAG_CHECKABLE);

    lv_obj_t *retLabel = lv_label_create(btn);
    lv_obj_center(retLabel);
    lv_label_set_text(retLabel, "Scores");
}

void game_screen_build(lv_obj_t *scr) {
    lv_obj_set_style_bg_color(scr, COLOR_GREY, LV_PART_MAIN);
    game_name_lbl = lv_label_create(scr);
    lv_obj_set_style_text_align(game_name_lbl, LV_TEXT_ALIGN_CENTER, 0);
    lv_obj_align(game_name_lbl, LV_ALIGN_CENTER, 0, -360);

    p1_name_lbl = lv_label_create(scr);
    lv_obj_align(p1_name_lbl, LV_ALIGN_CENTER, 0, -300);
    p1_score_table = lv_table_create(scr);
    lv_obj_align(p1_score_table, LV_ALIGN_CENTER, 0, -260);

    p2_name_lbl = lv_label_create(scr);
    lv_obj_align(p2_name_lbl, LV_ALIGN_CENTER, 0, -200);
    p2_score_table = lv_table_create(scr);
    lv_obj_align(p2_score_table, LV_ALIGN_CENTER, 0, -160);

    RetBtn = lv_btn_create(scr);
    lv_obj_align(RetBtn, LV_ALIGN_CENTER, -80, 40);
    set_ret_btn_style(RetBtn);

    ScoreScreenBtn = lv_btn_create(scr);
    lv_obj_align(ScoreScreenBtn, LV_ALIGN_CENTER, 80, 40);
    set_scor_btn_style(ScoreScreenBtn);

    BTN_1 = lv_btn_create(scr);
    lv_obj_align(BTN_1, LV_ALIGN_CENTER, -80, 100);
    set_act_btn_style(BTN_1, "P1+");

    BTN_2 = lv_btn_create(scr);
    lv_obj_align(BTN_2, LV_ALIGN_CENTER, -80, 160);
    set_act_btn_style(BTN_2, "P1-");

    BTN_3 = lv_btn_create(scr);
    lv_obj_align(BTN_3, LV_ALIGN_CENTER, 80, 100);
    set_act_btn_style(BTN_3, "P2+");

    BTN_4 = lv_btn_create(scr);
    lv_obj_align(BTN_4, LV_ALIGN_CENTER, 80, 160);
    set_act_btn_style(BTN_4, "P2-");

    BTN_5 = lv_btn_create(scr);
    lv_obj_align(BTN_5, LV_ALIGN_CENTER, 0, 220);
    set_act_btn_style(BTN_5, "Reset");

    lv_obj_add_event_cb(RetBtn, btn_pressed, LV_EVENT_CLICKED, NULL);
    lv_obj_add_event_cb(ScoreScreenBtn, btn_pressed, LV_EVENT_CLICKED, NULL);
    lv_obj_add_event_cb(BTN_1, btn_pressed, LV_EVENT_CLICKED, NULL);
    lv_obj_add_event_cb(BTN_2, btn_pressed, LV_EVENT_CLICKED, NULL);
    lv_obj_add_event_cb(BTN_3, btn_pressed, LV_EVENT_CLICKED, NULL);
    lv_obj_add_event_cb(BTN_4, btn_pressed, LV_EVENT_CLICKED, NULL);
    lv_obj_add_event_cb(BTN_5, btn_pressed, LV_EVENT_CLICKED, NULL);
}

game_screen_state_t *game_screen_ready() {
    return &state;
}