#ifndef _DRAWING_H
#define _DRAWING_H

void display();
void resize(int width, int height);

extern float eye[3];
extern float coi[3];
extern float  up[3];

extern bool pointer;
extern bool bounding_box;

#endif
