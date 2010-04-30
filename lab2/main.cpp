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

#include <cstdio>
#include <cstdlib>

#include "scene.h"

#define WIDTH 512
#define HEIGHT 512

static bool lButtonDown;
static bool rButtonDown;

// window idenfitier
static int win;

scene_t test;

static void mMotion(int x, int y)
{
  bool changed = false;
  const int dx = x;
  const int dy = y;

  if(lButtonDown)
    printf("moved to %i, %i\n", x, y);
}

static void mButton(int button, int state, int x, int y)
{
  if(button == GLUT_LEFT_BUTTON)
  {
    if(state == GLUT_DOWN)
      lButtonDown = true;
    else
      lButtonDown = false;
  }
  else if(button == GLUT_RIGHT_BUTTON)
  {
    if(state == GLUT_DOWN)
      rButtonDown = true;
    else
      rButtonDown = false;
  }
}

static void resize(int width, int height)
{
  //const float ar = (float) width / (float) height;

  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  glViewport(0, 0, width, height);
  //        left, right, bottom, top, near, far
  //glOrtho(0,    width, height, 0,   0,    1  );

  // this might be from left to right (0 to width)
  // and bottom to top (0 to height)
  gluOrtho2D(-5, 5, -5, 5);

  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
}

void reshape(GLsizei width, GLsizei height)
{
  glutReshapeWindow(width, height);
}

void display()
{
  glClear(GL_COLOR_BUFFER_BIT);

  glPushMatrix();
  drawScene(&test);
  glPopMatrix();

  glutSwapBuffers();
}

void keyboard(unsigned char key, int x, int y)
{
  switch(key)
  {
    case 's':
      break;
    case 'j':
      reshape(256, 256);
      break;
    case 'k':
      reshape(512, 512);
      break;
    case 'l':
      reshape(1024, 1024);
      break;
    case 'n':
      break;
    case 'q':
      glutDestroyWindow(win);
      exit(0);
  }
}

void skeyboard(int key, int x, int y)
{
  switch(key) {
    case GLUT_KEY_F1:
      printf("HIT F1 key\n");
      break;
  }
}

int main(int argc, char** argv)
{

  if( argc < 2 )
  {
    fprintf(stderr, "You didn't specify enough arguments\n");
    exit(1);
  }

  char *fileName = argv[1];

  loadScene(fileName, &test);

  //if(processFile(fileName))
  /*
  {
    fprintf(stderr, "Error processing file\n");
    exit(1);
  }
  */

  glutInit(&argc, argv);
  glutInitWindowSize(WIDTH, HEIGHT);
  win = glutCreateWindow("lab2");

  glutDisplayFunc(display);
  glutReshapeFunc(resize);

  // events
  glutKeyboardFunc(keyboard);
  glutSpecialFunc(skeyboard);
  glutMouseFunc(mButton);
  glutMotionFunc(mMotion);

  glutMainLoop();

  freeScene(&test);

  return 0;
}
