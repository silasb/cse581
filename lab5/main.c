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

  // light1 - directional
  light0 = init_light(GL_LIGHT0, 0);
  light0->ambient[4] = 1;
  light0->diffuse[0] = 1;
  light0->diffuse[1] = 1;
  light0->diffuse[2] = 1;
  light0->diffuse[3] = 1;
  light0->pos[1] = 50.0f;

  // light2 - point
  light1 = init_light(GL_LIGHT1, 0);
  light1->ambient[0] = 1;
  light1->ambient[1] = 0;
  light1->ambient[3] = 1;

  light1->diffuse[0] = light1->ambient[0];
  light1->diffuse[1] = light1->ambient[1];
  light1->diffuse[3] = light1->ambient[3];

  light1->pos[0] = 0;
  light1->pos[1] = 0;
  light1->pos[2] = 0;
  light1->pos[3] = 1;

  // light3 - spot
  light2 = init_light(GL_LIGHT2, GL_SPOT_DIRECTION);
  light2->pos[3] = 25;
  light2->pos[4] = 1.0f;

  light2->ambient[2] = .2;
  light2->ambient[3] = 1.0f;

  light2->diffuse[2] = 1;
  light2->diffuse[3] = 1;

  light2->spot.concentration = 10.0f;
  light2->spot.direction[0] = 1;
  light2->spot.direction[1] = 2;
  light2->spot.direction[2] = 0;

  light2->spot.cutoff = 20;

  // light4
  light3 = init_light(GL_LIGHT3, GL_SPOT_DIRECTION);
  light3->pos[4] = 1;

  light3->ambient[2] = .2;
  light3->ambient[3] = 0;

  light3->diffuse[2] = 1;

  light3->spot.concentration = 1.0f;
  light3->spot.cutoff = 10.2;

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
  glEnable(GL_LIGHTING);
  glEnable(GL_LIGHT0);
  glEnable(GL_LIGHT1);
  glEnable(GL_LIGHT2);
  glEnable(GL_LIGHT3);

  float matAmbient[] = { 1.0f, 1.0f, 1.0f, 1.0f };
  float matDiffuse[] = { 1.0f, 1.0f, 1.0f, 1.0f };
  float matSpecular[] = { 1.0f, 1.0f, 1.0f, 1.0f };

  glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, matAmbient);
  glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, matDiffuse);
  glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, matSpecular);
  glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, 20.0f);

}
