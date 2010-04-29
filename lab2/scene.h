#ifndef _SCENE_H_
#define _SCENE_H_

#include "model.h"

typedef struct scene_s
{
  int nVertices;
  int nPolygons;
  vertex_t *vList;
  polygon_t *pList;
} scene_t;

void loadScene(const char *, scene_t *);
void freeScene(scene_t *);

void drawScene(scene_t *s);

#endif
