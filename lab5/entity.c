#include "entity.h"
#include "OpenGL/GL.h"
#include <stdio.h>

entity_t* create_entity(const char *name)
{
  entity_t *e;

  //e = (entity_t *)malloc(s)

  return e;
}

void draw_entity(entity_t *e)
{
  glPushMatrix();
  printf("%d\n", e->nPolygons);
  for(int i = 0; i < e->nPolygons; i++)
  {
  glBegin(GL_TRIANGLES);
    glVertex3f(e->pList[i].v[0].x, e->pList[i].v[0].y, e->pList[i].v[0].z);
    glVertex3f(e->pList[i].v[1].x, e->pList[i].v[1].y, e->pList[i].v[1].z);
    glVertex3f(e->pList[i].v[2].x, e->pList[i].v[2].y, e->pList[i].v[2].z);
  glPopMatrix();
  }
  glEnd();
}

/*
void set_entity_model(entity_t *e, model_t *m)
{
}
*/
