#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ncurses.h>
#include <time.h>
#include "room.h"
#include "ent.h"

/* This is intentional so that PRNG can easily be manipulated to test things. */
extern unsigned int seed;

void ent_ai_wander(floor_t *floor, entity_t *ent){
	int dir;

	dir = rand_r(&seed) % 4;
	switch(dir){
		case 0:
			if(!check_occupied(floor, ent->x - 1, ent->y)){
				ent->x -= 1;
			}
			break;
		case 1:
			if(!check_occupied(floor, ent->x + 1, ent->y)){
				ent->x += 1 ; 
			}
			break;
		case 2:
			if(!check_occupied(floor, ent->x, ent->y - 1)){
				ent->y -= 1;
			}
			break;	
		case 3:
			if(!check_occupied(floor, ent->x, ent->y + 1)){
				ent->y += 1;
			}
			break;
	}
}

void ent_add_rand(floor_t *floor, unsigned int len){
	
	unsigned int i, x, y;
	entity_t *list = floor->entities;
	for(i = 0; i < len; i++){
		if(list[i].type == ETYPE_NONE){
			while(true){
				x = rand_r(&seed) % FLOOR_WIDTH;
				y = rand_r(&seed) % FLOOR_HEIGHT;
				if(floor->tile[x][y].type == FLOOR_TILE){
					//In a room
					list[i].x = x;
					list[i].y = y;
					list[i].type = ETYPE_NPC;
					list[i].symbol = 'B';
					list[i].update = ent_ai_wander;
					break;
				}
			}
			break;
		}
	}
}

void ent_init_list(floor_t *floor, unsigned int px, unsigned int py, unsigned int len){
	unsigned int i, npc_count;

	entity_t *list = floor->entities;
	
	// PC is index 0
	list[PC].symbol = PC_SYM;
	list[PC].type = ETYPE_PC;
	list[PC].x = px;
	list[PC].y = py;
	

	for(i = PC + 1; i < len; i++){
		list[i].type = ETYPE_NONE;
	}

	npc_count = NPC_MIN + rand_r(&seed) % (NPC_MAX - NPC_MIN);
	for(i = 0; i < npc_count; i++){
		ent_add_rand(floor, len);
	}
}

/* Tell each entity in the list to do its event function */
void ent_update(floor_t *floor, unsigned int len){
	unsigned int i;
	for(i = PC + 1; i < len; i++){
		if(floor->entities[i].type == ETYPE_NPC){

			floor->entities[i].update(floor, &(floor->entities[i]));	

		}
	}
}

void ent_gen_map_layer(floor_t *floor, unsigned int len, unsigned int map_width, unsigned int map_height){

	unsigned int i, x, y;
	entity_t *list = floor->entities;

	memset(floor->ent_map, 0, sizeof(char) * FLOOR_WIDTH * FLOOR_HEIGHT);

	for(i = 0; i < len; i++){
		if(list[i].type != ETYPE_NONE){
			x = list[i].x;
			y = list[i].y;
			floor->ent_map[x][y] = list[i].symbol;	
		}
	}

}
