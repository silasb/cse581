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

void loadModel(const char *, scene_t *);
void freeModel(scene_t *);

void drawScene(scene_t *s);
void drawPolygon(polygon_t *p);

#endif
