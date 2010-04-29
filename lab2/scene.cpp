#include "scene.h"
#include <cstdio>
#include <cstdlib>
#include <GLUT/glut.h>

void loadPolygon(FILE **file, polygon_t *polygon, vertex_s *vList);
void getColorList(FILE *file, int nVertices, polygon_t *polygon);
void drawPolygon(polygon_t *p);

void loadScene(const char *fileName, scene_t *scene)
{
  printf("Opening: %s\n", fileName);

  FILE *file = fopen(fileName, "r");

  if(file != NULL)
  {
    // fscanf the number of vertices
    fscanf(file, "%i", &scene->nVertices);

    // alloc scene vertice list
    scene->vList = (vertex_t *)malloc(scene->nVertices * sizeof(vertex_t));

    // get all vertices in the scene
    for(int i = 0; i < scene->nVertices; i++)
    {
      fscanf(file, "%f %f", 
          &scene->vList[i].x, 
          &scene->vList[i].y);
    }

    // get number of polygons in scene
    fscanf(file, "%i", &scene->nPolygons);

    scene->pList = (polygon_t *)malloc(scene->nPolygons * sizeof(polygon_t));

    for(int i = 0; i < scene->nPolygons; i++)
    {
      loadPolygon(&file, &scene->pList[i], scene->vList);

      printf("polygon[%i]", i);
      for(int j = 0; j < scene->pList[i].nVertices; j++)
      {
        printf(" (%f, %f)\n", scene->pList[i].vList[j].x, scene->pList[i].vList[j].y);
      }
    }
  }
  else
  {
    fprintf(stderr, "Couldn't load file %s\n", fileName);
  }

}

void freeScene(scene_t *scene)
{
  if( scene->vList)
  {
    free(scene->vList);
    scene->vList = NULL;
  }
}

void loadPolygon(FILE **file, polygon_t *polygon, vertex_t *vList)
{
  fscanf(*file, "%i", &polygon->nVertices);

  polygon->vList = (vertex_t *)malloc(polygon->nVertices * sizeof(vertex_t));

  printf("nVertices: %i\n", polygon->nVertices);

  // skip the space
  fseek(*file, 1, SEEK_CUR);

  int i = 0;
  while(i < polygon->nVertices)
  {
    int ch = fgetc(*file);
    if((ch >= 0x30 && ch <= 0x39)) 
    { 
      polygon->vList[i].x = vList[i].x;
      polygon->vList[i].y = vList[i].y;
      i++;
    }
  }

  getColorList(*file, polygon->nVertices, polygon);

  fscanf(*file, "%i", &polygon->nTrans);
  printf("nTrans: %i\n", polygon->nTrans);

  polygon->tList = (trans_t*)malloc(polygon->nTrans * sizeof(trans_t));

  int ch;
  while((ch = fgetc(*file)) == 10)
    ;
  ungetc(ch, *file);

  for(i = 0; i < polygon->nTrans; i++)
  {
    char type;
    fscanf(*file, "%c", &type);

    polygon->tList[i].type = type;
    if(type == 't' or type == 's')
      fscanf(*file, "%f %f", &polygon->tList[i].x, &polygon->tList[i].y);
    else
      fscanf(*file, "%i", &polygon->tList[i].d);
    printf("trans type: %c -> ", polygon->tList[i].type);
    printf("%i\n", polygon->tList[i].d);
  }

  while((ch = fgetc(*file)) == 10)
    ;
  ungetc(ch, *file);

  char fill;
  fscanf(*file, "%c", &fill);
  if(fill == 'n') {
    polygon->fill = 0;
    fscanf(*file, "%f", &polygon->lWidth);
  }
  else
    polygon->fill = 1;

  printf("fill: %i\n", polygon->fill);
  printf("lWidth: %f\n", polygon->lWidth);

  fscanf(*file, "%c", &polygon->scale); // scale animation
  fscanf(*file, "%i", &polygon->lRotate); // local rotate
  fscanf(*file, "%i", &polygon->gRotate); // global rotate
}

void getColorList(FILE *file, int nVertices, polygon_t *polygon)
{
  for(int i = 0; i < nVertices; i++)
    fscanf(file, "%f %f %f", 
        &polygon->vList[i].r,
        &polygon->vList[i].g,
        &polygon->vList[i].b);
}

void drawScene(scene_t *s)
{
  for(int i = 0; i < s->nPolygons; i++)
  {
    drawPolygon(&s->pList[i]);
  }

}

/*
 * Draw this order using a triangle strip
 *   2---4---6
 *  / \ / \ / \
 * 1---3---5---7
 *
 * http://www.google.com/url?sa=t&source=web&ct=res&cd=5&ved=0CCEQFjAE&url=http%3A%2F%2Fwww.cs.umd.edu%2Fprojects%2Fgvil%2Fpapers%2Fav_ts.pdf&ei=8-fYS-TlIsOC8gbJ3sWhCg&usg=AFQjCNHPgAQ3_MZurIVRWGondJ2HDgIX4Q&sig2=MccnC_bpBbMGTaNhC-kA9Q
 *
 */


void drawPolygon(polygon_t *p)
{
  if(p->fill)
    glBegin(GL_POLYGON);
  else {
    glLineWidth(p->lWidth);
    glBegin(GL_LINE_LOOP);
  }

  for(int i = 0; i < p->nVertices; i++)
  {
    glColor3f(p->vList[i].r, p->vList[i].g, p->vList[i].b);
    glVertex2f(p->vList[i].x, p->vList[i].y);
  }
  glEnd();
}
