#include "scene.h"
#include <cstdio>
#include <cstdlib>

#if defined(_WIN32) || defined(__WIN32__)
#include "windows.h"
#else
#include <unistd.h>
#endif

#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

#ifdef _WIN32
#define fscanf fscanf_s
#endif

#include <cmath>

bool scale_up = true;

void loadPolygon(FILE **file, polygon_t *polygon, vertex_s *vList);
void getColorList(FILE *file, int nVertices, polygon_t *polygon);
void drawPolygon(polygon_t *p);
void check_format(int nVertices, vertex_t *vList);
vector_t norm_vect(vector_t *v);
int length_vect(vector_t *v);
int dot(vector_t *v1, vector_t *v2);
vector_t cross(vector_t *v1, vector_t *v2);

void
loadScene(const char *fileName, scene_t *scene)
{
  printf("Opening: %s\n", fileName);

  FILE *file = fopen(fileName, "r");

  if(file != NULL)
  {
    // fscanf the number of vertices
    fscanf(file, "%i", &scene->nVertices);

    if(scene->nVertices < 3) {
      fprintf(stderr, "Need more than three vertices defined\n");
      exit(1);
    }

    // alloc scene vertice list
    scene->vList = (vertex_t *)malloc(scene->nVertices * sizeof(vertex_t));

    // get all vertices in the scene
    for(int i = 0; i < scene->nVertices; i++)
    {
      fscanf(file, "%f %f", 
          &scene->vList[i].x, 
          &scene->vList[i].y);
    }

    check_format(scene->nVertices, scene->vList);

    // get number of polygons in scene
    fscanf(file, "%i", &scene->nPolygons);

    scene->pList = (polygon_t *)malloc(scene->nPolygons * sizeof(polygon_t));

    for(int i = 0; i < scene->nPolygons; i++)
    {
      loadPolygon(&file, &scene->pList[i], scene->vList);

#ifdef __DEBUG__
      printf("polygon[%i]", i);
      for(int j = 0; j < scene->pList[i].nVertices; j++)
      {
        printf(" (%f, %f)\n", scene->pList[i].vList[j].x, scene->pList[i].vList[j].y);
      }
#endif
    }
  }
  else
  {
    fprintf(stderr, "Couldn't load file %s\n", fileName);
  }

}

void
freeScene(scene_t *scene)
{
  if(scene->vList)
  {
    free(scene->vList);
    for(int i = 0; i < scene->nPolygons; i++)
    {
      free(scene->pList[i].vList);
      scene->pList[i].vList = NULL;
      free(scene->pList[i].tList);
      scene->pList[i].tList = NULL;
    }
    scene->pList = NULL;
    scene->vList = NULL;
  }
}

void
loadPolygon(FILE **file, polygon_t *polygon, vertex_t *vList)
{
  fscanf(*file, "%i", &polygon->nVertices);

  polygon->vList = (vertex_t *)malloc(polygon->nVertices * sizeof(vertex_t));

#ifdef __DEBUG__
  printf("nVertices: %i\n", polygon->nVertices);
#endif

#ifdef __APPLE__
  // skip the space
  fseek(*file, 1, SEEK_CUR);
#endif

  // copy vertices from scene vlist to polygon vlist
  int i = 0;
  while(i < polygon->nVertices)
  {
    char ch = fgetc(*file);
    if((ch >= 0x30 && ch <= 0x39)) 
    { 
      int mast_pos = atoi(&ch);
      polygon->vList[i].x = vList[mast_pos].x;
      polygon->vList[i].y = vList[mast_pos].y;
      i++;
    }
  }

  getColorList(*file, polygon->nVertices, polygon);

  fscanf(*file, "%i", &polygon->nTrans);
#ifdef __DEBUG__
  printf("nTrans: %i\n", polygon->nTrans);
#endif

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
    if(type == 't' || type == 's')
      fscanf(*file, "%f %f", &polygon->tList[i].x, &polygon->tList[i].y);
    else
      fscanf(*file, "%i", &polygon->tList[i].d);

#ifdef __DEBUG__
    printf("trans type: %c -> ", polygon->tList[i].type);
    if(type == 't' || type == 's')
      printf("(%f, %f)\n", polygon->tList[i].x, polygon->tList[i].y);
    else
      printf("%i\n", polygon->tList[i].d);
#endif

    while((ch = fgetc(*file)) == 10)
      ;
    ungetc(ch, *file);
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
    polygon->fill = true;

#ifdef __DEBUG__
  printf("fill: %i\n", polygon->fill);
  printf("lWidth: %f\n", polygon->lWidth);
#endif

  while((ch = fgetc(*file)) == 10)
    ;
  ungetc(ch, *file);

  fscanf(*file, "%c", &polygon->scale); // scale animation
  fscanf(*file, "%i", &polygon->lRotate); // local rotate
  fscanf(*file, "%i", &polygon->gRotate); // global rotate

  polygon->scale_number = 1;
}

void
getColorList(FILE *file, int nVertices, polygon_t *polygon)
{
  for(int i = 0; i < nVertices; i++)
    fscanf(file, "%f %f %f", 
        &polygon->vList[i].r,
        &polygon->vList[i].g,
        &polygon->vList[i].b);
}

void
drawScene(scene_t *s)
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


void
drawPolygon(polygon_t *p)
{
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();

  if(p->gRotate == 1 || p->gRotate == 2)
    glRotatef(p->g_angle, 0.0f, 0.0f, 1.0f);

  for(int j = 0; j < p->nTrans; j++)
  {
    if(p->tList[j].type == 'r')
      glRotatef(p->tList[j].d, 0.0f, 0.0f, 1.0f);
    else if(p->tList[j].type == 't')
      glTranslatef(p->tList[j].x, p->tList[j].y, 0);
    else if(p->tList[j].type == 's')
      glScalef(p->tList[j].x, p->tList[j].y, 0);
  }

  if(p->scale == 'y') 
    glScalef(p->scale_number, p->scale_number, 0);
  if(p->lRotate == 1 || p->lRotate == 2) {
    glRotatef(p->l_angle, 0.0f, 0.0f, 1.0f);
  }
  if(p->gRotate == 1 || p->gRotate == 2)
    glRotatef(-p->g_angle, 0.0f, 0.0f, 1.0f);

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

void
animate(scene_t *s)
{
  for(int i = 0; i < s->nPolygons; i++)
  {
    if(s->pList[i].scale == 'y') 
    {
      if(scale_up)
        s->pList[i].scale_number += grow_rate;
      else
        s->pList[i].scale_number -= grow_rate;

      if(s->pList[i].scale_number >= 2)
        scale_up = false;
      else if(s->pList[i].scale_number <= 0.5)
        scale_up = true;
    }

    if(s->pList[i].lRotate == 1)
      s->pList[i].l_angle -= grow_rate * 100;
    else if(s->pList[i].lRotate == 2)
      s->pList[i].l_angle += grow_rate * 100;

    if(s->pList[i].gRotate == 1)
      s->pList[i].g_angle -= grow_rate * 100;
    else if(s->pList[i].gRotate == 2)
      s->pList[i].g_angle += grow_rate * 100;
  }
  glutPostRedisplay();
}

void
check_format(int nVertices, vertex_t *vList)
{
  for(int i = 1; i < nVertices - 1; i++)
  {
    vertex_t a = vList[i-1];
    vertex_t b = vList[i];
    vertex_t c;
    if(i < nVertices - 1)
      c = vList[i + 1];
    else
      c = vList[0];
    vector_t vec1 = {c.x - b.x, c.y - b.y, 0};
    vector_t vec2 = {a.x - b.x, a.y - b.y, 0};
    vector_t normed_v1 = norm_vect(&vec1);
    vector_t normed_v2 = norm_vect(&vec2);

    if(dot(&normed_v1, &normed_v2) == -1)
    {
      fprintf(stderr, "Vertices are collinear\n");
      exit(1);
    }

    vector_t vec3 = cross(&vec1, &vec2);
    if(vec3.z < 0)
    {
      fprintf(stderr, "Polygon is either concave, clockwise, or intersecting\n");
      exit(1);
    }
  }
}

/* Vector math */

vector_t
norm_vect(vector_t *v)
{
  int len = length_vect(v);
  vector_t temp;
  temp.x = v->x / len;
  temp.y = v->y / len;
  temp.z = v->z / len;
  return temp;
}

int
length_vect(vector_t *v)
{
  return sqrt(pow(v->x, 2) + pow(v->y, 2) + pow(v->z, 2));
}

int
dot(vector_t *v1, vector_t *v2)
{
  return(v1->x * v2->x +
         v1->y * v2->y +
         v1->z * v2->z);
}

vector_t
cross(vector_t *v1, vector_t *v2)
{
  vector_t temp;
  temp.x = v1->y * v2->z - v1->z * v2->y;
  temp.y = v1->z * v2->x - v1->x * v2->z;
  temp.z = v1->x * v2->y - v1->y * v2->x;
  return temp;
}
