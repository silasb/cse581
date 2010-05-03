/*
 * structure taken from:
 * http://www.falloutsoftware.com/tutorials/gl/gl6.htm
 */

#ifndef _MODEL_H_
#define _MODEL_H_

#ifdef __APPLE__
#include <OpenGL/gl.h>
#else
#include <GL/gl.h>
#endif

static float grow_rate = 0.001;

typedef struct vertex_s
{
  // 2d
  GLfloat x, y;
  // colors
  GLfloat r, g, b;
} vertex_t;

typedef struct transformation_s
{
  int type;
  union {
    struct { GLfloat x, y; };
    struct { GLint d; };
  };
} trans_t;

typedef struct polygon_s
{
  int nVertices;
  int nTrans; 
  vertex_t *vList;
  trans_t *tList;

  GLboolean fill;
  GLfloat lWidth;
  char scale;
  int lRotate;
  int gRotate;
  GLfloat scale_number;
  bool clockwise;
  GLfloat current_angle;
} polygon_t;

typedef struct model_s
{
  int nPolygons;
  polygon_t *pList;
} model_t;

//extern model_t sampleModel;

//void drawPolygon(polygon_t *p);

//void loadModel(const char *, model_t *);
//void freeModel(model_t *);

#endif
