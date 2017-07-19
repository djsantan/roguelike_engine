#ifndef __ENT_H__

#include "types.h"

#define PC_SYM '@'
#define PC 0


/* Function Prototypes */
void ent_update(floor_t *, unsigned int);
void ent_gen_map_layer(floor_t *, unsigned int, unsigned int, unsigned int);
void ent_init_list(floor_t *, unsigned int, unsigned int, unsigned int);

#define __ENT_H__
#endif
