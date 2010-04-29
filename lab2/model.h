/*
 * structure taken from:
 * http://www.falloutsoftware.com/tutorials/gl/gl6.htm
 */

#ifndef _MODEL_H_
#define _MODEL_H_

typedef struct vertex_s
{
  // 2d
  float x, y;
  // colors
  float r, g, b;
} vertex_t;

typedef struct transformation_s
{
  int type;
  union {
    struct { float x, y; };
    struct { int d; };
  };
} trans_t;

typedef struct polygon_s
{
  int nVertices;
  int nTrans; 
  vertex_t *vList;
  trans_t *tList;

  int fill;
  float lWidth;
  char scale;
  int lRotate;
  int gRotate;
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
