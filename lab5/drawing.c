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

extern camera_t *c;

/* prototypes */
void PPMWriteImage(unsigned char *imgBuffer, const int nx, const int ny);
void PPMReadImage(const char *, GLubyte **, int *, int *);
GLuint loadTexture(const char *filename, int wrap);

void
display()
{
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  if(ortho)
    setup_ortho_matrix();
  else
    setup_projection_matrix();

  set_camera_perspective(c);

  glPushMatrix();
  GLfloat light_position[] = {0.0, 20.0, 10.0, 1.0};
  glLightfv(GL_LIGHT0, GL_POSITION, light_position);
  glColor3f(1, 1, 1);
  glTranslatef(light_position[0], light_position[1], light_position[2]);
  glutWireCube(0.5);
  glPopMatrix();

  entity_t e;
  obj_load("data/cube3.obj", &e);
  glColor3f(1, 1, 0);
  draw_entity(&e);
   
  /* draw teapot */
  glPushMatrix();
  glColor3f(1.0, 0.2, 0.2);
  glTranslatef(0, 3, 0);
  glutSolidTeapot(3);
  glPopMatrix();

  glutSwapBuffers();

  //free(&e);
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
