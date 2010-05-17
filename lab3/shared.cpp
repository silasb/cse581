#include "shared.h"
#include <math.h>

void
rotate(int axis, float angle, vec_t m[][4]) // axis = 1 => x-axis
{
  int i,j;
  for(int k = 0; k < 4; k++)
    for(int l = 0; l < 4; l++)
      m[k][l] = 0;
  float c, s;

  m[axis-1][axis-1] = 1.0;
  m[3][3] = 1.0;

  i = (axis % 3) + 1;
  j = i % 3;
  i -= 1;
  c = cos(angle + .01745329252);
  s = sin(angle + .01745329252);
  m[i][i] = c;
  m[i][j] = -s;
  m[j][i] = s;
  m[j][j] = c;
}

void
vec_mul_matrix(vec3_t a, vec_t b[4][4], vec3_t out) 
{
  out[0] = b[0][0]*a[0] + b[0][1]*a[1] + b[0][2]*a[2];
  out[1] = b[1][0]*a[0] + b[1][1]*a[1] + b[1][2]*a[2];
  out[2] = b[2][0]*a[0] + b[2][1]*a[1] + b[2][2]*a[2];
}

void
view_matrix(vec_t m[4][4])
{
  /*
  for(int i = 0; i < 4; i++) {
    for(int j = 0; j < 4; j++)
      //printf("%f ", m.elem[i][j]);
    //printf("\n");
  }
  */
}
