#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

#include "common.h"
#include "drawing.h"

bool pointer=false;

void draw_floor();
void draw_pointer();

void display()
{
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  gluPerspective(60.0f*zoomFactor, 1, 1.0f, 100.0f);

  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
  gluLookAt(eye[0], eye[1], eye[2],
            coi[0], coi[1], coi[2],
             up[0],  up[1],  up[2]);

  glPushMatrix();

  if(pointer) {
    draw_pointer();
    glPopMatrix();
  }
  draw_floor();
  
  glFlush();
  glutSwapBuffers();
}

void draw_pointer()
{
  glTranslatef(coi[0], coi[1], coi[2]);
  glutWireCube(1.0f);
}

void draw_floor()
{
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
  //glutWireCube(2.0f);
}
