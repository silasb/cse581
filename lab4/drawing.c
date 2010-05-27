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

#include <stdlib.h>
#include <math.h>

/* prototypes */
void draw_floor();

void PPMWriteImage(unsigned char *imgBuffer, const int nx, const int ny);

void
display()
{
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
  gluLookAt(0, 0, -25,
            0, 0, 25,
            0,  1, 0);

  /* draw teapot */
  glColor3f(1, 1, .2);
  glPushMatrix();
  glTranslatef(0, 0, 0);
  glutSolidTeapot(3);
  glPopMatrix();

  draw_floor();
  
  glFlush();
  glutSwapBuffers();
}

void
draw_floor()
{
  glPushMatrix();
  glBegin(GL_QUADS);
  glColor3f(0,0,1);
  glVertex3f(-25, 0, 25);
  glColor3f(1,0,1);
  glVertex3f(25, 0, 25);
  glColor3f(0,1,0);
  glVertex3f(25, 0, -25);
  glColor3f(1,1,0);
  glVertex3f(-25, 0, -25);
  glEnd();
  glPopMatrix();
}

void
resize(int width, int height)
{

  if(height > width)
    glViewport(0, height-width, width, width);
  else
    glViewport(0, 0, height, height);

  setup_projection_matrix();

  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
  gluLookAt(0, 0, -25,
            0, 0, 25,
            0,  1, 0);
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
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
