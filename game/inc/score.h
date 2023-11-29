#ifndef __SCORE_H__
#define __SCORE_H__

/* Maintain current score and communicate with 7Seg Display */
#include <stdint.h>
#include "types.h"

/* Print score to 7Seg displays */
void display_score();

/* Get the current score in ASCII */
int get_score(player_t player);

/* Set the score */
void set_score(player_t player, uint8_t val);

/* Display a value on display temporarily */
void display_value(uint8_t val, uint32_t ms);

/* Set a player's displayed score with a custom bitmap (ie. for tennis 'Ad') */
/* The display will remain overridden until cleared */
void override_display(player_t player, uint8_t ldigit, uint8_t rdigit);

/* clear any display override */
void clear_display(player_t player);

/* Adjust a player's score */
void modify_score(player_t player, int delta);

/* declare a winner, TODO: Add sound to this call */
void set_winner(player_t player);

/* Reset the score, TODO: Add sound to this call */
void reset_score();

#endif /* __SCORE_H__ */
