#include "room.h"

const tile_t ter_bogus = {
	'%',
	0x00,
	0x00,
	0,
	PLACEHOLDER_TILE,
	""
};
const tile_t ter_floor = {
	'.',
	0x00,
	0xFF,
	0,
	FLOOR_TILE,
	""
};

const tile_t ter_wall = {
	'#',
	0xFF,
	0x00,
	0,
	WALL_TILE,
	""
};

const tile_t ter_stair_up = {
	'<',
	0xFF,
	0x00,
	0,
	STAIR_UP_TILE,
	""
};

const tile_t ter_stair_down = {
	'>',
	0xFF,
	0x00,
	0,
	STAIR_DOWN_TILE,
	""
};
