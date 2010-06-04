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

#include "common.h"
#include "drawing.h"
#include "events.h"
#include "loader/obj_loader.h"
#include "camera.h"

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

bool_t tGndTex=true;
bool_t tLinear=false;
bool_t trackballMove=false;

int frame=0;
int movie[300];

float rot = 0;

extern camera_t *c;
extern light_t *light0;
extern light_t *light1;
extern light_t *light2;
extern light_t *light3;

/* prototypes */
void PPMWriteImage(unsigned char *imgBuffer, const int nx, const int ny);
void PPMReadImage(const char *, GLubyte **, int *, int *);
GLuint loadTexture(const char *filename, int wrap);

void
display()
{
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  zoomFactor = 2.5;
  if(ortho)
    setup_ortho_matrix();
  else
    setup_projection_matrix();

  c->pos.x = 20;
  c->pos.y = 20;
  c->coi.x = 0;
  c->coi.y = 5;
  c->coi.z = 0;
  set_camera_perspective(c);

  glPushMatrix();
  /*
   * Directional light source (w = 0)
   * The light source is at an infinite distance,
   * all the ray are parallel and have the direction (x, y, z).
   */
  draw_light(light0);

  /*
   * Positional light source (w = 1)
   * The light source is positioned at (x, y, z).
   * The ray come from this particular location (x, y, z) and goes towards all directions.
   */
  draw_light(light1);
  
  /*
   * Spotlight
   */
  draw_light(light2);

  /*
   * helmet cam
   */
  light3->pos[0] = c->pos.x;
  light3->pos[1] = c->pos.y;
  light3->pos[2] = c->pos.z;
  light3->spot.direction[0] = c->coi.x;
  light3->spot.direction[1] = c->coi.y;
  light3->spot.direction[2] = c->coi.z;
  draw_light(light3);

  glPopMatrix();

  //glPushMatrix();
  //GLfloat light_position[] = {0.0, 20.0, 10.0, 1.0};
  //glLightfv(GL_LIGHT0, GL_POSITION, light_position);
  //glColor3f(1, 1, 1);
  //glTranslatef(light_position[0], light_position[1], light_position[2]);
  //glutWireCube(0.5);
  //glPopMatrix();

  /*
  entity_t e;
  obj_load("data/cube3.obj", &e);
  glColor3f(1, .5, 0);
  draw_entity(&e);
  */

  // animation
  glRotatef(rot, 0, 1, 0);
  glTranslatef(2, 3, 0);

  glPushMatrix();
    glColor3f(1, 0, 0);
    glutSolidCube(3);
  glPopMatrix();

  glTranslatef(2, 3, 0);

  glPushMatrix();
    glColor3f(1, 1, 0);
    glutSolidCube(3);
  glPopMatrix();

  glTranslatef(2, 3, 0);

  glPushMatrix();
    glColor3f(1, 1, 1);
    glutSolidCube(3);
  glPopMatrix();

  glTranslatef(2, 3, 0);

  glPushMatrix();
    glColor3f(.5, .5, .5);
    glutSolidCube(3);
  glPopMatrix();

  glutSwapBuffers();

  //free(&e);
}

void idle()
{
  rot += .2;
  glutPostRedisplay();
}

void
resize(int width, int height)
{

  if(height > width)
    glViewport(0, height-width, width, width);
  else
    glViewport(0, 0, height, height);

  winWidth = width;
  winHeight = height;
}

void
setup_projection_matrix()
{
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();

  gluPerspective(60/zoomFactor, 1, 1.0f, 100.0f);
}

void
setup_ortho_matrix()
{
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  glOrtho(-26 / zoomFactor, 26 / zoomFactor, -26 / zoomFactor, 
          26 / zoomFactor, 1, 100);
}

/* export PPM */
void
exportPPM(void)
{
  int width = glutGet(GLUT_WINDOW_WIDTH);
  int height = glutGet(GLUT_WINDOW_HEIGHT);
  unsigned char *image = (unsigned char *)malloc(width * height * 3 * sizeof(unsigned char *));
  glReadPixels(0, 0, width, height, GL_RGB, GL_UNSIGNED_BYTE, image);
  PPMWriteImage(image, width, height);
  free(image);
}

GLuint buildTexture(GLubyte *data, int width, int height, int wrap)
{
  GLuint texture;

  // alloc tex name
  glGenTextures(1, &texture);

  // select texture
  glBindTexture( GL_TEXTURE_2D, texture );


  // mix texture with color for shading
  glTexEnvf( GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
  //glTexEnvf( GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);

  if(tLinear) {
    glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
    glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
  } else {
    glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST );
    glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST );
  }

  // if wrap is true, the texture wraps over at the edges (repeat)
  //       ... false, the texture ends at the edges (clamp)
  glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S,
                   wrap ? GL_REPEAT : GL_CLAMP );
  glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T,
                   wrap ? GL_REPEAT : GL_CLAMP );

  glTexImage2D(GL_TEXTURE_2D, 0, 3, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);

  return texture;
}

GLuint loadTexture(const char *filename, int wrap)
{
  GLubyte *image;
  GLuint texture;
  int width, height;

  PPMReadImage(filename, &image, &width, &height);
  texture = buildTexture(image, width, height, wrap);
  free(image); // opengl has the image now

  return texture;
}

void unloadTexture(GLuint texture) {
  free(texture);
}
