#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ncurses.h>
#include <time.h>
#include "types.h"
#include "room.h"
#include "ent.h"

#define PRINTDBG(a) mvprintw(FLOOR_HEIGHT+1, 0, "%s", a); refresh() 
#define PRINTFDBG(fmt, ...) snprintf(__DBGBUF, 1000, fmt, __VA_ARGS__); PRINTDBG(dbgbuf); 

char __DBGBUF[1000];

unsigned int stu_x, stu_y, std_x, std_y;
unsigned int seed;
unsigned int DEBUG_ILLUMINATE = 1;
unsigned int DEBUG_SHOW_VISMAP = 0;


bool coords_inbounds(int x, int y){
	if(x < 0 || x > FLOOR_WIDTH) return false;
	if(y < 0 || y > FLOOR_HEIGHT) return false;
	return true;
}

bool search_connected(floor_t *floor, int st_x, int st_y, int end_x, int end_y){

	if(st_x < 0 || st_x > FLOOR_WIDTH || st_y < 0 || st_y > FLOOR_HEIGHT) return false;
	if(floor->tile_tmp[st_x][st_y].type == WALL_TILE) return false;
	if(floor->tile_tmp[st_x][st_y].type == PLACEHOLDER_TILE) return false;
	if(st_x == end_x && st_y == end_y){
		return true;
	} else {
		floor->tile_tmp[st_x][st_y] = ter_bogus;
		return (search_connected(floor, st_x + 1, st_y, end_x, end_y) ||
			search_connected(floor, st_x - 1, st_y, end_x, end_y) ||
			search_connected(floor, st_x, st_y + 1, end_x, end_y) ||
			search_connected(floor, st_x, st_y - 1, end_x, end_y));
	}

	return false;

}

void gen_box_room(floor_t *floor, int xc, int yc, int width, int height){

	int x, y;

	/* Build the floor of the room */
	for(y = yc; y < yc + width; y++){
		for(x = xc; x < xc + height; x++){
			floor->tile[x][y] = ter_floor;
			if(x == FLOOR_WIDTH - 1 ||
			   x == 0 ||
			   y == FLOOR_HEIGHT - 1 ||
			   y == 0)
			{
				floor->tile[x][y] = ter_wall;
			}
		}
	}
}

void gen_goals(floor_t *floor){
	int x, y;
	while(true){
		x = rand_r(&seed) % FLOOR_WIDTH;
		y = rand_r(&seed) % FLOOR_HEIGHT;
		if(floor->tile[x][y].type == FLOOR_TILE){
			//In a room
			stu_x = x;
			stu_y = y;
			floor->tile[x][y] = ter_stair_up;
			break;
		}
	}
	while(true){
		x = rand_r(&seed) % FLOOR_WIDTH;
		y = rand_r(&seed) % FLOOR_HEIGHT;
		if(floor->tile[x][y].type == FLOOR_TILE){
			//In a room
			std_x = x;
			std_y = y;
			floor->tile[x][y] = ter_stair_down;
			break;
		}
	}
}

void gen_hallways(floor_t *floor){
	int iter = 0;
	int x, y, t, orientation;
	bool connection;
	char buf[255];
	memcpy(floor->tile_tmp, floor->tile, FLOOR_WIDTH*FLOOR_HEIGHT*sizeof(tile_t));
	while(!search_connected(floor, stu_x, stu_y, std_x, std_y)){
		x = rand_r(&seed) % FLOOR_WIDTH;
		y = rand_r(&seed) % FLOOR_HEIGHT;
		if(floor->tile[x][y].type == FLOOR_TILE){
			//In a room
			
			//while(dfloor[x][y] == TER_FLOOR && x < FLOOR_WIDTH){
			//	x++;
			//}
			orientation = rand_r(&seed) % 4;
			connection = false;
			switch(orientation){
			case 0:
				while(x < FLOOR_WIDTH && floor->tile[x][y].type == FLOOR_TILE){
					x++;
				}
				t = x;
				while(t < FLOOR_WIDTH){
					if(floor->tile[t++][y].type == FLOOR_TILE) connection = true;
				}
				if(connection){
					while(x < FLOOR_WIDTH && floor->tile[x][y].type != FLOOR_TILE){
						floor->tile[x][y] = ter_floor;
						x++;
					}
				}
				break;
			case 1:
				while(x > 0 && floor->tile[x][y].type == FLOOR_TILE){
					x--;
				}
				t = x;
				while(t > 0){
					if(floor->tile[t--][y].type == FLOOR_TILE) connection = true;
				}
				if(connection){
					while(x > 0 && floor->tile[x][y].type != FLOOR_TILE){
						floor->tile[x][y] = ter_floor;
						x--;
					}
				}
				break;
			case 2:
				while(y < FLOOR_HEIGHT && floor->tile[x][y].type == FLOOR_TILE){
					y++;
				}
				t = y;
				while(t < FLOOR_HEIGHT){
					if(floor->tile[x][t++].type == FLOOR_TILE) connection = true;
				}
				if(connection){
					while(y < FLOOR_HEIGHT && floor->tile[x][y].type != FLOOR_TILE){
						floor->tile[x][y] = ter_floor;
						y++;
					}
				}
				break;
			case 3:
				while(y > 0 && floor->tile[x][y].type == FLOOR_TILE){
					y--;
				}
				t = y;
				while(t > 0){
					if(floor->tile[x][t--].type == FLOOR_TILE) connection = true;
				}
				if(connection){
					while(y > 0 && floor->tile[x][y].type != FLOOR_TILE){
						floor->tile[x][y] = ter_floor;
						y--;
					}
				}
				break;
			}
			/* Uncomment to watch the hallways be placed */
			//draw(floor);
			//sleep(1);
			memcpy(floor->tile_tmp, floor->tile, FLOOR_WIDTH*FLOOR_HEIGHT*sizeof(tile_t));
		}
	}
}

void gen_rooms(floor_t *floor){

	int i, x, y, w, h, room_count;
	for(y = 0; y < FLOOR_HEIGHT; y++){
		for(x = 0; x < FLOOR_WIDTH; x++){
			floor->tile[x][y] = ter_wall;
		}
	}

	room_count = (rand_r(&seed) % (FLOOR_MAX_ROOMS - FLOOR_MIN_ROOMS)) + FLOOR_MIN_ROOMS;
	
	for(i = 0; i < room_count; i++){
		w = (rand_r(&seed) % (ROOM_MAX_WIDTH - ROOM_MIN_WIDTH)) + ROOM_MIN_WIDTH;
		h = (rand_r(&seed) % (ROOM_MAX_HEIGHT - ROOM_MIN_HEIGHT)) + ROOM_MIN_HEIGHT;
		x = 1 + rand_r(&seed) % (FLOOR_WIDTH - 3 - w);
		y = 1 + rand_r(&seed) % (FLOOR_HEIGHT - 3 - h);
		gen_box_room(floor, x, y, w, h);		
	}
}

void gen_floor(floor_t *floor){

	gen_rooms(floor);
	gen_goals(floor);
	gen_hallways(floor);

}

void draw(floor_t *floor){

	static int framecnt = 0;

	int x, y;
	
	for(y = 0; y < FLOOR_HEIGHT; y++){
		for(x = 0; x < FLOOR_WIDTH; x++){
			if(DEBUG_SHOW_VISMAP){
				mvprintw(y, x, "%d", floor->vis_map[x][y]);
			}
			else if(floor->vis_map[x][y] == 1 || DEBUG_ILLUMINATE){
				mvprintw(y, x, "%c", floor->tile[x][y].symbol);
			} else {
				mvprintw(y, x, "%c", TER_DARKNESS); 
			}
			if(floor->ent_map[x][y] != 0 && floor->vis_map[x][y] == 1)
				mvprintw(y, x, "%c", floor->ent_map[x][y]);	
		}
	}
	refresh();

}

bool check_los(floor_t* floor, int st_x, int st_y, int end_x, int end_y){

	int x = st_x;
	int y = st_y;
	int i;
	int deltax = end_x - st_x;
	int deltay = end_y - st_y;
	int dx1 = 0, dy1 = 0, dx2 = 0, dy2 = 0;
	int numerator;
	int longer = abs(deltax), shorter = abs(deltay);
	int wall_count = 0;
	if(deltax<0) dx1 = -1; else if (deltax>0) dx1 = 1;
	if(deltay<0) dy1 = -1; else if (deltay>0) dy1 = 1;
	if(deltax<0) dx2 = -1; else if (deltax>0) dx2 = 1;
	
	if(longer<shorter){
		longer = abs(deltay);
		shorter = abs(deltax);
		if(deltay < 0) dy2 = -1; else if(deltay>0) dy2 = 1;
		dx2 = 0;
	}
	numerator = longer >> 1;
	for(i = 0; i <= longer; i++){
		if(wall_count >= 1) return false;
		if(floor->tile[x][y].type == WALL_TILE) wall_count++;
		numerator += shorter;
		if(numerator > longer){
			numerator -= longer;
			x += dx1;
			y += dy1;
		} else {
			x += dx2;
			y += dy2;
		}
	}
	return true;
}

void update_vismap(floor_t *floor, int x, int y, int r){
	int i;
	for(i = 0; i < r; i++){
		fill_vismap(floor, x, y, i);
	}
	//fill_vismap(x, y, r);
	for(y = 0; y < FLOOR_HEIGHT; y++){
		for(x = 0; x < FLOOR_WIDTH; x++){
			if(floor->vis_map[x][y] == 2)
				floor->vis_map[x][y] = 1;
		}
	}
}

void fill_vismap(floor_t *floor, int stx, int sty, int r){
   	int t = 0; 
	int x = r;
	int y = 0;
	int err = 0;
	while (x >= y)
	{
		if(coords_inbounds(stx+x, sty+y) && check_los(floor, floor->entities[PC].x, floor->entities[PC].y, stx+x, sty+y)){
			floor->vis_map[stx+x][sty+y] = 2;
		}
		if(coords_inbounds(stx+y, sty+x) && check_los(floor, floor->entities[PC].x, floor->entities[PC].y, stx+y, sty+x)){
			floor->vis_map[stx+y][sty+x] = 2;
		}
		if(coords_inbounds(stx-y, sty+x) && check_los(floor, floor->entities[PC].x, floor->entities[PC].y, stx-y, sty+x)){
			floor->vis_map[stx-y][sty+x] = 2;
		}
		if(coords_inbounds(stx-x, sty+y) && check_los(floor, floor->entities[PC].x, floor->entities[PC].y, stx-x, sty+y)){
			floor->vis_map[stx-x][sty+y] = 2;
		}
		if(coords_inbounds(stx-x, sty-y) && check_los(floor, floor->entities[PC].x, floor->entities[PC].y, stx-x, sty-y)){
			floor->vis_map[stx-x][sty-y] = 2;
		}
		if(coords_inbounds(stx-y, sty-x) && check_los(floor, floor->entities[PC].x, floor->entities[PC].y, stx-y, sty-x)){
			floor->vis_map[stx-y][sty-x] = 2;
		}
		if(coords_inbounds(stx+y, sty-x) && check_los(floor, floor->entities[PC].x, floor->entities[PC].y, stx+y, sty-x)){
			floor->vis_map[stx+y][sty-x] = 2;
		}
		if(coords_inbounds(stx+x, sty-y) && check_los(floor, floor->entities[PC].x, floor->entities[PC].y, stx+x, sty-y)){
			floor->vis_map[stx+x][sty-y] = 2;
		}
		
		y += 1;
		err += 1 + 2*y;
		if (2*(err-x) + 1 > 0){
			x -= 1;
			err += 1 - 2*x;
		}
	}
}

bool check_occupied(floor_t *floor, int x, int y){
	if(coords_inbounds(x, y) &&
	   floor->ent_map[x][y] == 0 &&
           floor->tile[x][y].type != WALL_TILE){
	
		return false;
	} else {
		return true;
	}
}

int main(void){

	int c, x, y;
	char buf[255];
	bool esc = false;
	seed = time(NULL);

	floor_t dungeon;

	initscr();
	if(!has_colors()){
		printf("Your terminal doesn't support colors! Aborting...\n");
		exit(1);
	}
	start_color();

	noecho();
	cbreak();
	curs_set(0);	
	gen_floor(&dungeon);
	ent_init_list(&dungeon, stu_x, stu_y, ENT_MAX);
	update_vismap(&dungeon, dungeon.entities[PC].x, dungeon.entities[PC].y, PC_VIS);
	draw(&dungeon);
	while(!esc){

		/* Control needs to be moved into some function somewhere else but not in ent.c. If I plan to implement some
		netcode, I should probably make this in a file called client.h or something along with the drawing. */
		c = getch();
		switch(c){

		case 'k':
			if(!check_occupied(&dungeon, dungeon.entities[PC].x, dungeon.entities[PC].y-1)){
				dungeon.ent_map[dungeon.entities[PC].x][dungeon.entities[PC].y] = 0;
				dungeon.entities[PC].y--;
			}
			break;
		case 'j':
			if(!check_occupied(&dungeon, dungeon.entities[PC].x, dungeon.entities[PC].y+1)){
				dungeon.ent_map[dungeon.entities[PC].x][dungeon.entities[PC].y] = 0;
				dungeon.entities[PC].y++;
			}
			break;
		case 'h':
			if(!check_occupied(&dungeon, dungeon.entities[PC].x-1, dungeon.entities[PC].y)){
				dungeon.ent_map[dungeon.entities[PC].x][dungeon.entities[PC].y] = 0;
				dungeon.entities[PC].x--;
			}
			break;
		case 'l':
			if(!check_occupied(&dungeon, dungeon.entities[PC].x+1, dungeon.entities[PC].y)){
				dungeon.ent_map[dungeon.entities[PC].x][dungeon.entities[PC].y] = 0;
				dungeon.entities[PC].x++;
			}
			break;
		case 'b':
			if(DEBUG_ILLUMINATE)
				DEBUG_ILLUMINATE = 0;
			else
				DEBUG_ILLUMINATE = 1;
			break;
		case 't':
			if(DEBUG_SHOW_VISMAP)
				DEBUG_SHOW_VISMAP = 0;
			else
				DEBUG_SHOW_VISMAP = 1;
			break;
		case '>':
			if(dungeon.entities[PC].x == std_x && dungeon.entities[PC].y == std_y)
				esc = true;
			break;
		case 'v':
			for(y = 0; y < FLOOR_HEIGHT; y++){
				for(x = 0; x < FLOOR_WIDTH; x++){
					dungeon.vis_map[x][y] = 2;
				}
			}
			break;
		case 127:
			esc = true;	
			break;

		}
		// Replace thi sline with a function that will upadte every other entity
		dungeon.ent_map[dungeon.entities[PC].x][dungeon.entities[PC].y] = PC_SYM;
		ent_gen_map_layer(&dungeon, ENT_MAX, FLOOR_WIDTH, FLOOR_HEIGHT);
		ent_update(&dungeon, ENT_MAX);
		update_vismap(&dungeon, dungeon.entities[PC].x, dungeon.entities[PC].y, PC_VIS);
		draw(&dungeon);	
		
	}

	endwin();

}
