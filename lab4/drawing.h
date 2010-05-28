#ifndef _DRAWING_H
#define _DRAWING_H

#include "shared.h"

void display();
void resize(int width, int height);
void setup_projection_matrix();
void setup_ortho_matrix();
void exportPPM(void);

bool_t tGndTex;
bool_t tFlatShd;
bool_t tLinear;

#endif
