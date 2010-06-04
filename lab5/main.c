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

GLuint gndTexture;

void initGL();

int main(int argc, char** argv)
{
  glutInit(&argc, argv);
  glutInitWindowSize(WIDTH, HEIGHT); // initially 512 x 512
  win = glutCreateWindow("lab4");
  glutInitDisplayMode(GLUT_DEPTH | GLUT_RGB | GLUT_DOUBLE);

  // callbacks
  glutDisplayFunc(display);
  glutReshapeFunc(resize);
  glutKeyboardFunc(keyboard);
  glutIdleFunc(idle);

  initGL();

  //gndTexture = loadTexture("Untitled.ppm", 1);

  c = init_camera();
  set_camera_perspective(c);

  glutMainLoop();

  return 0;
}

/* opengl defaults */
void
initGL() {
  glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

  glEnable(GL_DEPTH_TEST);
  glCullFace(GL_BACK);

  // get proper shading
  glShadeModel(GL_SMOOTH);

  // setup camera
  //setup_ortho_matrix();

  // lighting
  //glEnable(GL_LIGHTING);
  //glEnable(GL_LIGHT0);
}
