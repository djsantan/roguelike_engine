#ifndef __ROGUE_TYPES_H__

#ifndef bool
	#define bool _Bool
#endif

#define FLOOR_WIDTH 80 
#define FLOOR_HEIGHT 24
#define ENT_MAX 255

typedef enum{

	ETYPE_NONE = 0,
	ETYPE_PC = 1,
	ETYPE_NPC = 2,

} entity_type_t;

typedef struct{

	char symbol;
	entity_type_t type;	
	int x;
	int y;
	void (*update)(); // Entity behavior function

} entity_t;

typedef char visibility_t;

typedef enum{

	FLOOR_TILE,
	WALL_TILE,
	STAIR_UP_TILE,
	STAIR_DOWN_TILE,
	AIR_TILE,
	WATER_TILE,
	MAGMA_TILE,
	PLACEHOLDER_TILE

}tile_type;

typedef struct{

	char symbol;
	short fg_color;
	short bg_color;
	short vis;
	tile_type type;
	char *data;

}tile_t;

typedef struct{

	visibility_t vis_map[FLOOR_WIDTH][FLOOR_HEIGHT];
	char ent_map[FLOOR_WIDTH][FLOOR_HEIGHT];
	entity_t entities[ENT_MAX];
	tile_t tile[FLOOR_WIDTH][FLOOR_HEIGHT];
	tile_t tile_tmp[FLOOR_WIDTH][FLOOR_HEIGHT];

}floor_t;

#define __ROGUE_TYPES_H__
#endif
