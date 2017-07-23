#ifndef __ROOM_H__

#include "types.h"

#define FLOOR_MIN_ROOMS 28
#define FLOOR_MAX_ROOMS 30 
#define ROOM_MIN_WIDTH 3
#define ROOM_MIN_HEIGHT 3 
#define ROOM_MAX_WIDTH 8 
#define ROOM_MAX_HEIGHT 8
#define PC_SYM '@'
#define PC_VIS 6 
#define NPC_MIN 4
#define NPC_MAX 10

#define STAT_WIDTH 24
#define STAT_HEIGHT 12

#define TER_DARKNESS ' '
#define TER_FLOOR '.'
#define TER_WALL ' ' 
#define TER_STAIR_UP '<'
#define TER_STAIR_DOWN '>'

extern const tile_t ter_bogus;
extern const tile_t ter_floor;
extern const tile_t ter_wall;
extern const tile_t ter_stair_up;
extern const tile_t ter_stair_down;

/* Function Prototypes */
bool check_occupied(floor_t *, int, int);
void draw(floor_t *floor);
void fill_vismap(floor_t *floor, int stx, int sty, int r);
void win_init();


#define __ROOM_H__
#endif
