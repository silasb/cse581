/*
 * Author: Silas Baronda
 *
 */

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

#include <stdio.h>
#include <stdlib.h>

#include "common.h"
#include "events.h"
#include "drawing.h"

void initGL();

float eye[3];
float coi[3];
float up[3];

int main(int argc, char** argv)
{
  glutInit(&argc, argv);
  glutInitWindowSize(WIDTH, HEIGHT); // initially 512 x 512
  win = glutCreateWindow("lab3");
  glutInitDisplayMode(GLUT_DEPTH | GLUT_RGB | GLUT_DOUBLE);

  // callbacks
  glutDisplayFunc(display);
  glutReshapeFunc(resize);
  glutKeyboardFunc(keyboard);
  glutMouseFunc(mouse);
  glutSpecialFunc(skeyboard);

  initGL();

  glutMainLoop();

  return 0;
}

void
initGL() {
  glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

  glEnable(GL_DEPTH_TEST);

  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  gluPerspective(60.0f*zoomFactor, 1, 1.0f, 100.0f);

  eye[0] = 0.0f; eye[1] = 25.0f; eye[2] = 25.0f;
  coi[0] = 0.0f; coi[1] = 25.0f; coi[2] = -25.0f;
  up[0]  = 0.0f; up[1]  = 1.0f;  up[2]  = 0.0f;
  //eye = {0.0f, 25.0f,  25.0f};
  //coi = {0.0f, 25.0f, -25.0f};
  //up =  {0.0f,  1.0f,   0.0f};

  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
  gluLookAt(eye[0], eye[1], eye[2],
            coi[0], coi[1], coi[2],
             up[0],  up[1],  up[2]);
}
