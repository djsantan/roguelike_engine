#include <stdio.h>
#include "ent.h"

void main(void){	

	entity e;
	e.symbol = '#';
	e.type = PC;

	printf("%c %d\n", e.symbol, (int)e.type);

	return;
}
