/*
 * structure taken from:
 * http://www.falloutsoftware.com/tutorials/gl/gl6.htm
 */

#ifndef _MODEL_H_
#define _MODEL_H_

#if defined(_WIN32) || defined(__WIN32__)
#include "windows.h"
#endif

#ifdef __APPLE__
#include <OpenGL/gl.h>
#else
#include <GL/gl.h>
#endif

static double grow_rate = 0.001;

typedef struct vertex_s
{
  // 2d
  GLfloat x, y;
  // colors
  GLfloat r, g, b;
} vertex_t;

typedef struct vector_s
{
  GLfloat x, y, z;
} vector_t;

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
  GLfloat l_angle, g_angle;
} polygon_t;

#endif
