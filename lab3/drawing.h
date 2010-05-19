#ifndef _DRAWING_H
#define _DRAWING_H

#include "shared.h"

void display();
void resize(int width, int height);
void setup_projection_matrix();
void exportPPM(void);

float eye[3];
float coi[3];
float  up[3];

bool_t pointer;
bool_t bounding_box;

bool_t wireframe;

#endif
