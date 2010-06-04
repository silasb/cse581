#ifndef _LIGHT_
#define _LIGHT_

struct spot {
  float direction[3];
  float cutoff;
  float concentration;
};

typedef struct {
  float pos[4];
  float ambient[4];
  float diffuse[4];
  float specular[4];
  struct spot spot;
  int type;
  int name;
} light_t;

light_t* init_light(int name, int type);
void draw_light(light_t *l);

#endif
