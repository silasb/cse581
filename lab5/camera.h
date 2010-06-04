#ifndef _CAMERA_H_
#define _CAMERA_H_

struct vector {
  float x, y, z;
};

typedef struct {
  struct vector pos;
  struct vector coi;
  struct vector up;

  int fov;
  int clip_n, clip_f;
  
  //float projection_matrix[16];
  //float modelview_matrix[16];
} camera_t;

camera_t* init_camera();
void set_camera_perspective(camera_t *c);

#endif
