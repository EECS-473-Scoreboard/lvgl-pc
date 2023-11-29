#include "common.h"
#include "game_screen.h"


static lv_obj_t* ScoreScreenBtn;
static lv_obj_t* RetBtn;
static lv_obj_t* BTN_1;
static lv_obj_t* BTN_2;
static lv_obj_t* BTN_3;
static lv_obj_t* BTN_4;



inline static void set_ret_btn_style(lv_obj_t *btn){
    lv_obj_set_width(btn,150);
    lv_obj_set_height(btn,50);
    lv_obj_add_flag(btn, LV_OBJ_FLAG_CHECKABLE);
    
    lv_obj_t *retLabel = lv_label_create(btn);    
    lv_obj_center(retLabel);
    lv_label_set_text(retLabel, "Return");
}

inline static void set_act_btn_style(lv_obj_t *btn, char name[10]){
    lv_obj_set_width(btn,150);
    lv_obj_set_height(btn,50);
    lv_obj_add_flag(btn, LV_OBJ_FLAG_CHECKABLE);
    
    lv_obj_t *retLabel = lv_label_create(btn);    
    lv_obj_center(retLabel);
    lv_label_set_text(retLabel, name);
}

inline static void set_scor_btn_style(lv_obj_t *btn){
    lv_obj_set_width(btn,150);
    lv_obj_set_height(btn,50);
    lv_obj_add_flag(btn, LV_OBJ_FLAG_CHECKABLE);
    
    lv_obj_t *retLabel = lv_label_create(btn);    
    lv_obj_center(retLabel);
    lv_label_set_text(retLabel, "Scores");
}

void game_screen_build(lv_obj_t* scr){
    lv_obj_set_style_bg_color(scr,COLOR_GREY,LV_PART_MAIN);
    lv_obj_t *title = lv_label_create(scr);
    lv_label_set_text(title, "Current Game"); // Need to find a way to get current game as char[]
    lv_obj_set_style_text_align(title, LV_TEXT_ALIGN_CENTER, 0);
    lv_obj_align(title, LV_ALIGN_CENTER, 0, -360);
    
    lv_obj_t *P1_name = lv_label_create(scr);
    lv_label_set_text(P1_name,"Player 1"); //Need to get the player's name
    lv_obj_align(P1_name,LV_ALIGN_CENTER,0,-300);
    lv_obj_t *p1Score = lv_table_create(scr);
    lv_table_set_cell_value(p1Score,0,0,"Score1");
    lv_obj_align(p1Score,LV_ALIGN_CENTER,0,-260);

    lv_obj_t *P2_name = lv_label_create(scr);
    lv_label_set_text(P2_name,"Player 2"); //Need to get the player's name
    lv_obj_align(P2_name,LV_ALIGN_CENTER,0,-200);
    lv_obj_t *p2Score = lv_table_create(scr);
    lv_table_set_cell_value(p2Score,0,0,"Score2");
    lv_obj_align(p2Score,LV_ALIGN_CENTER,0,-160);

    RetBtn = lv_btn_create(scr);
    lv_obj_align(RetBtn,LV_ALIGN_CENTER,-80,100);
    set_ret_btn_style(RetBtn);

    ScoreScreenBtn = lv_btn_create(scr);
    lv_obj_align(ScoreScreenBtn,LV_ALIGN_CENTER,80,100);
    set_scor_btn_style(ScoreScreenBtn);



    BTN_1 = lv_btn_create(scr);
    lv_obj_align(BTN_1,LV_ALIGN_CENTER,0,160);
    set_act_btn_style(BTN_1,"Button1");

     BTN_2 = lv_btn_create(scr);
    lv_obj_align(BTN_2,LV_ALIGN_CENTER,0,220);
    set_act_btn_style(BTN_2,"Button2");
    
     BTN_3 = lv_btn_create(scr);
    lv_obj_align(BTN_3,LV_ALIGN_CENTER,0,280);
    set_act_btn_style(BTN_3,"Button3");
    
     BTN_4 = lv_btn_create(scr);
    lv_obj_align(BTN_4,LV_ALIGN_CENTER,0,340);
    set_act_btn_style(BTN_4,"Button4");
    
    

    
}