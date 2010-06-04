#include "camera.h"
#include <GLUT/glut.h>
#include <stdlib.h>

camera_t* init_camera()
{
  camera_t *c;
  c = (camera_t *)malloc(sizeof(camera_t));

  c->pos.x = 0;
  c->pos.y = 25;
  c->pos.z = 25;
  c->coi.x = 0;
  c->coi.y = 25;
  c->coi.z = -25;
  c->up.x = 0;
  c->up.y = 1;
  c->up.z = 0;

  c->fov = 30;
  c->clip_n = 1.0f;
  c->clip_f = 100.0f;

  return c;
}

void set_camera_perspective(camera_t *c)
{
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
  gluLookAt(c->pos.x, c->pos.y, c->pos.z,
            c->coi.x, c->coi.y, c->coi.z,
            c->up.x, c->up.y, c->up.z);
  /*
  printf("cam pos: %f %f %f\n", c->pos.x, c->pos.y, c->pos.z);
  printf("cam coi: %f %f %f\n", c->coi.x, c->coi.y, c->coi.z);
  printf("cam up: %f %f %f\n", c->up.x, c->up.y, c->up.z);
  */
}
