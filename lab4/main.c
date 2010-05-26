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

bool_t pointer=false;
bool_t bounding_box=false;
bool_t wireframe=true;

void initGL();

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
  glutMouseFunc(mButton);
  glutMotionFunc(mMotion);
  glutSpecialFunc(skeyboard);

  initGL();

  glutMainLoop();

  return 0;
}

/* opengl defaults */
void
initGL() {
  glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

  glEnable(GL_DEPTH_TEST);
  glCullFace(GL_BACK);

  glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

  /* setup initial eye */
  eye[0] = 0.0f; eye[1] = 25.0f; eye[2] = 25.0f;
  coi[0] = 0.0f; coi[1] = 25.0f; coi[2] = -25.0f;
  up[0]  = 0.0f; up[1]  = 1.0f;  up[2]  = 0.0f;
}
