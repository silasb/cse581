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
#define NFRAMES 48

static bool lButtonDown;

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
}

static void resize(int width, int height)
{
  //const float ar = (float) width / (float) height;

  glViewport(0, 0, width, height);
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  //gluPerspective(45.0f, (float)width/(float)height, 0.2f, 255.0f);
  glOrtho(0, width, height, 0, 0, 1);

  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();

  glTranslatef (0.375, 0.375, 0);
}

void reshape(GLsizei width, GLsizei height)
{
  glutReshapeWindow(width, height);
}

void display()
{
  glClear(GL_COLOR_BUFFER_BIT);

  drawScene(&test);

  glutSwapBuffers();
}

void mouse(int button, int state, int x, int y)
{
  if(button == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
  {
  }
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
  //glutReshapeFunc(resize);
  glutKeyboardFunc(keyboard);
  glutMouseFunc(mouse);
  glutSpecialFunc(skeyboard);

  glutMouseFunc(mButton);
  glutMotionFunc(mMotion);

  /*
  glClearColor(0, 0, 0, 0);
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  gluOrtho2D(0, 100, 0, 100);

  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
  */

  glutMainLoop();

  freeScene(&test);

  return 0;
}
