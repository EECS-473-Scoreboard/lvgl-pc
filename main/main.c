
/**
 * @file main
 *
 */

/*********************
 *      INCLUDES
 *********************/
#include "common.h"
#include "game_screen.h"
#include "main_menu.h"
#include "score_screen.h"
#include "sound_screen.h"

#include "lvgl/src/hal/lv_hal_tick.h"
#include <stdlib.h>
#include <unistd.h>
#define SDL_MAIN_HANDLED /*To fix SDL's "undefined reference to WinMain"                                               \
                            issue*/
#include "lv_drivers/sdl/sdl.h"
#include "lvgl/demos/lv_demos.h"
#include "lvgl/examples/lv_examples.h"
#include "lvgl/lvgl.h"
#include <SDL2/SDL.h>

/*********************
 *      DEFINES
 *********************/

/**********************
 *      TYPEDEFS
 **********************/

/**********************
 *  STATIC PROTOTYPES
 **********************/
static void hal_init(void);

/**********************
 *  STATIC VARIABLES
 **********************/

static lv_obj_t *main_menu;
static lv_obj_t *sound_screen;
static lv_obj_t *score_screen;
static lv_obj_t *game_screen;

static enum : char { MAIN_MENU, SOUND_SCR, GAME_SCR, SCORE_SCR } current_scr = MAIN_MENU;

/**********************
 *      MACROS
 **********************/

/**********************
 *   GLOBAL FUNCTIONS
 **********************/

/*********************
 *      DEFINES
 *********************/

/**********************
 *      TYPEDEFS
 **********************/

/**********************
 *      VARIABLES
 **********************/

uint32_t SC_EVENT_WEARABLE;

/**********************
 *  STATIC PROTOTYPES
 **********************/

/**********************
 *   GLOBAL FUNCTIONS
 **********************/

int main(int argc, char **argv) {
    (void)argc; /*Unused*/
    (void)argv; /*Unused*/

    /*Initialize LVGL*/
    lv_init();

    /*Initialize the HAL (display, input devices, tick) for LVGL*/
    hal_init();

    main_menu = lv_scr_act();
    main_menu_build(main_menu);
    sound_screen = lv_obj_create(NULL);
    sound_screen_build(sound_screen);
    score_screen = lv_obj_create(NULL);
    score_screen_build(score_screen);
    game_screen = lv_obj_create(NULL);
    game_screen_build(game_screen);

    while (1) {
        main_menu_state_t *main_menu_state;
        game_screen_state_t *game_screen_state;
        switch (current_scr) {
        case MAIN_MENU:
            main_menu_state = main_menu_ready();
            if (main_menu_state->ready_state == MAIN_MENU_GO_GAME) {
                game_screen_init(main_menu_state);
                lv_scr_load(game_screen);
                main_menu_reset();
                main_menu_state->ready_state = MAIN_MENU_STAY;
                current_scr = GAME_SCR;
            } else if (main_menu_state->ready_state == MAIN_MENU_GO_SOUND) {
                lv_scr_load(sound_screen);
                main_menu_state->ready_state = MAIN_MENU_STAY;
                current_scr = SOUND_SCR;
            }
            break;
        case SOUND_SCR:
        case GAME_SCR:
            game_screen_state = game_screen_ready();
            if (game_screen_state->ready_state == GAME_SCR_GO_MENU) {
                lv_scr_load(main_menu);
                game_screen_state->ready_state = GAME_SCR_STAY;
                current_scr = MAIN_MENU;
            } else if (game_screen_state->ready_state == GAME_SCR_GO_SCORE) {
                lv_scr_load(score_screen);
                game_screen_state->ready_state = GAME_SCR_STAY;
                current_scr = SCORE_SCR;
            }
            break;
        case SCORE_SCR:
            break;
        }

        /* Periodically call the lv_task handler.
         * It could be done in a timer interrupt or an OS task too.*/
        lv_timer_handler();
        usleep(5 * 1000);
        lv_tick_inc(5);
    }

    return 0;
}

/**********************
 *   STATIC FUNCTIONS
 **********************/

static void keyboard_cb(struct _lv_indev_drv_t *_, uint8_t code) {
    if (code != LV_EVENT_KEY) return;
    uint32_t k = lv_indev_get_key(lv_indev_get_act());

    lv_obj_t *target_scr;
    switch (current_scr) {
    case MAIN_MENU:
        target_scr = main_menu;
        break;
    case SOUND_SCR:
        target_scr = sound_screen;
        break;
    case GAME_SCR:
        target_scr = game_screen;
        break;
    case SCORE_SCR:
        target_scr = score_screen;
        break;
    }

    wearable_event_t event = {0};
    // simulate two wearables
    switch (k) {
    case SDLK_1:
        event.fields.id = 0xDEAD;
        event.fields.act = WEARABLE_ACT_0;
        break;
    case SDLK_2:
        event.fields.id = 0xDEAD;
        event.fields.act = WEARABLE_ACT_1;
        break;
    case SDLK_3:
        event.fields.id = 0xDEAD;
        event.fields.act = WEARABLE_ACT_2;
        break;
    case SDLK_4:
        event.fields.id = 0xDEAD;
        event.fields.act = WEARABLE_ACT_3;
        break;
    case SDLK_7:
        event.fields.id = 0xBEEF;
        event.fields.act = WEARABLE_ACT_0;
        break;
    case SDLK_8:
        event.fields.id = 0xBEEF;
        event.fields.act = WEARABLE_ACT_1;
        break;
    case SDLK_9:
        event.fields.id = 0xBEEF;
        event.fields.act = WEARABLE_ACT_2;
        break;
    case SDLK_0:
        event.fields.id = 0xBEEF;
        event.fields.act = WEARABLE_ACT_3;
        break;
    default:
        return;
    }
    lv_event_send(target_scr, SC_EVENT_WEARABLE, (void *)(uint64_t)event.bits);
}

/**
 * Initialize the Hardware Abstraction Layer (HAL) for the LVGL graphics
 * library
 */
static void hal_init(void) {
    /* Use the 'monitor' driver which creates window on PC's monitor to simulate a
     * display*/
    sdl_init();

    /*Create a display buffer*/
    static lv_disp_draw_buf_t disp_buf1;
    static lv_color_t buf1_1[SDL_HOR_RES * 100];
    lv_disp_draw_buf_init(&disp_buf1, buf1_1, NULL, SDL_HOR_RES * 100);

    /*Create a display*/
    static lv_disp_drv_t disp_drv;
    lv_disp_drv_init(&disp_drv); /*Basic initialization*/
    disp_drv.draw_buf = &disp_buf1;
    disp_drv.flush_cb = sdl_display_flush;
    disp_drv.hor_res = SDL_HOR_RES;
    disp_drv.ver_res = SDL_VER_RES;

    lv_disp_t *disp = lv_disp_drv_register(&disp_drv);

    lv_theme_t *th = lv_theme_default_init(disp, lv_palette_main(LV_PALETTE_BLUE), lv_palette_main(LV_PALETTE_RED),
                                           LV_THEME_DEFAULT_DARK, LV_FONT_DEFAULT);
    lv_disp_set_theme(disp, th);

    lv_group_t *g = lv_group_create();
    lv_group_set_default(g);

    /* Add the mouse as input device
     * Use the 'mouse' driver which reads the PC's mouse*/
    static lv_indev_drv_t indev_drv_1;
    lv_indev_drv_init(&indev_drv_1); /*Basic initialization*/
    indev_drv_1.type = LV_INDEV_TYPE_POINTER;

    /*This function will be called periodically (by the library) to get the mouse
     * position and state*/
    indev_drv_1.read_cb = sdl_mouse_read;
    lv_indev_t *mouse_indev = lv_indev_drv_register(&indev_drv_1);

    static lv_indev_drv_t indev_drv_2;
    lv_indev_drv_init(&indev_drv_2); /*Basic initialization*/
    indev_drv_2.type = LV_INDEV_TYPE_KEYPAD;
    indev_drv_2.read_cb = sdl_keyboard_read;

    SC_EVENT_WEARABLE = lv_event_register_id();
    indev_drv_2.feedback_cb = keyboard_cb;

    lv_indev_t *kb_indev = lv_indev_drv_register(&indev_drv_2);
    lv_indev_set_group(kb_indev, g);

    static lv_indev_drv_t indev_drv_3;
    lv_indev_drv_init(&indev_drv_3); /*Basic initialization*/
    indev_drv_3.type = LV_INDEV_TYPE_ENCODER;
    indev_drv_3.read_cb = sdl_mousewheel_read;
    lv_indev_t *enc_indev = lv_indev_drv_register(&indev_drv_3);
    lv_indev_set_group(enc_indev, g);

    // /*Set a cursor for the mouse*/
    // LV_IMG_DECLARE(mouse_cursor_icon);                  /*Declare the image file.*/
    // lv_obj_t *cursor_obj = lv_img_create(lv_scr_act()); /*Create an image object for the cursor */
    // lv_img_set_src(cursor_obj, &mouse_cursor_icon);     /*Set the image source*/
    // lv_indev_set_cursor(mouse_indev, cursor_obj);       /*Connect the image  object to the driver*/
}
