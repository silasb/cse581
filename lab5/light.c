#include "light.h"
#include <GLUT/glut.h>
#include <stdlib.h>

light_t* init_light(int name, int type)
{
  light_t *light;
  light = (light_t *)malloc(sizeof(light_t));

  light->pos[0] = 0;
  light->pos[1] = 0;
  light->pos[2] = 0;
  light->pos[3] = 0;

  light->ambient[0] = 0.0f;
  light->ambient[1] = 0.0f;
  light->ambient[2] = 0.0f;
  light->ambient[3] = 0.0f;

  light->diffuse[0] = 0.0f;
  light->diffuse[1] = 0.0f;
  light->diffuse[2] = 0.0f;
  light->diffuse[3] = 0.0f;

  light->specular[0] = 0;
  light->specular[1] = 0;
  light->specular[2] = 0;
  light->specular[3] = 0;

  light->name = name;

  if(type)
    light->type = type;

  return light;
}

void draw_light(light_t *l)
{
  glLightfv(l->name, GL_AMBIENT, l->ambient);
  glLightfv(l->name, GL_DIFFUSE, l->diffuse);
  glLightfv(l->name, GL_SPECULAR, l->specular);

  glLightfv(l->name, GL_POSITION, l->pos);

  if(l->type == GL_SPOT_DIRECTION)
  {
    //printf("%f\n", l->spot.cutoff);
    //printf("%f\n", l->spot.concentration);
    glLightfv(l->name, l->type, l->spot.direction);
    glLightf(l->name, GL_SPOT_CUTOFF, l->spot.cutoff);
    glLightf(l->name, GL_SPOT_EXPONENT, l->spot.concentration);
    //glLightf(l->name, GL_CONSTANT_ATTENUATION, 1.0f);
    //glLightf(l->name, GL_LINEAR_ATTENUATION, 0.0f);
    //glLightf(l->name, GL_QUADRATIC_ATTENUATION, 0.0f);
  }
}
