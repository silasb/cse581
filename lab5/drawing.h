#ifndef _DRAWING_H
#define _DRAWING_H

#include "shared.h"

void display();
void resize(int width, int height);
void setup_projection_matrix();
void setup_ortho_matrix();
void exportPPM(void);
GLuint loadTexture(const char *filename, int wrap);
void idle();

int nearClip, farClip;
bool_t ortho;

int gndTexture;
int wallTex;
int pictureTex;

#endif
