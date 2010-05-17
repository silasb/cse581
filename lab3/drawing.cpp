#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

#include "common.h"
#include "drawing.h"

bool pointer=false;
bool bounding_box=false;

void draw_floor();
void draw_pointer();
void draw_bounding_box();

void
display()
{
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
  gluLookAt(eye[0], eye[1], eye[2],
            coi[0], coi[1], coi[2],
             up[0],  up[1],  up[2]);

  if(bounding_box)
    draw_bounding_box();

  if(pointer)
    draw_pointer();
  draw_floor();
  
  glFlush();
  glutSwapBuffers();
}

void
draw_pointer()
{
  glPushMatrix();
  glTranslatef(coi[0], coi[1], coi[2]);
  glutWireCube(1.0f);
  glPopMatrix();
}

void
draw_bounding_box()
{
  glPushMatrix();
  glTranslatef(0, 25, 0);
  glutWireCube(50.0f);
  glPopMatrix();
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

  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  gluPerspective(60.0f*zoomFactor, 1, 1.0f, 100.0f);

  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
  gluLookAt(eye[0], eye[1], eye[2],
            coi[0], coi[1], coi[2],
             up[0],  up[1],  up[2]);

  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
}
