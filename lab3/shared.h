#ifndef _SHARED_H
#define _SHARED_H

typedef float vec_t;
typedef vec_t vec3_t[3];

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

#define VectorCopy(a,b) (a[0]=b[0],a[1]=b[1],a[2]=b[2])
#define DotProduct(a,b) (a[0]*b[0] + a[1]*b[1] + a[2]*b[2])

void rotate(int, float, vec_t [][4]);
void vec_mul_matrix(vec3_t, vec_t [4][4], vec3_t);

#endif
