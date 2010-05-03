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
#include <cmath>

#include "scene.h"

#define WIDTH 512
#define HEIGHT 512

void pip(int, int);

static int _mouseX = 0;
static int _mouseY = 0;
static bool lButtonDown;
static bool rButtonDown;
static float zoom = 1;

bool enabled_pip = true;
GLint viewport[2];

// window idenfitier
static int win;

scene_t test;

static void mMotion(int x, int y)
{
  bool changed = false;

  const int dx = x - _mouseX;
  const int dy = y - _mouseY;

  if(dx == 0 && dy == 0)
    return;

  if(lButtonDown) 
  {
    viewport[0] += dx;
    viewport[1] -= dy;
    
    changed = true;
  }
  else if(rButtonDown)
  {
    zoom += dy * 0.01;

    changed = true;
  }

  _mouseX = x;
  _mouseY = y;

  if(changed)
    glutPostRedisplay();
}

static void mButton(int button, int state, int x, int y)
{
  _mouseX = x;
  _mouseY = y;

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
  glutPostRedisplay();
}

static void resize(int width, int height)
{
  //const float ar = (float) width / (float) height;

  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();

  if(height > width)
    glViewport(viewport[0], viewport[1], width, width);
  else
    glViewport(viewport[0], viewport[1], height, height);
  //        left, right, bottom, top, near, far
  //glOrtho(0,    width, height, 0,   0,    1  );

  // this might be from left to right (0 to width)
  // and bottom to top (0 to height)
  glOrtho(-1 * zoom, 1 * zoom, -1 * zoom, 1 * zoom, -1, 1);

  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
}

void reshape(GLsizei width, GLsizei height)
{
  glutReshapeWindow(width, height);
}

void display()
{
  int width = glutGet(GLUT_WINDOW_WIDTH);
  int height = glutGet(GLUT_WINDOW_HEIGHT);

  glClear(GL_COLOR_BUFFER_BIT);

  if(enabled_pip)
  {
    pip(width, height);
  }

  resize(width, height);

  drawScene(&test);

  glFlush();

  glutSwapBuffers();
}

void
pip(int width, int height)
{
  if(height > width)
    glViewport(0, 0, 0.20 * width, 0.20 * width);
  else
    glViewport(0, 0, 0.20 * height, 0.20 * height);

  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();

  glOrtho(-1, 1, -1, 1, -1, 1);

  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();

  drawScene(&test);
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
    case 'm':
      if(enabled_pip) enabled_pip = false; 
      else enabled_pip = true;
      glutPostRedisplay();
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

void idle(void)
{
  animate(&test);
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
  glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE);
  glutInitWindowSize(WIDTH, HEIGHT);
  win = glutCreateWindow("lab2");

  glutDisplayFunc(display);
  glutReshapeFunc(resize);
  glutIdleFunc(idle);

  // events
  glutKeyboardFunc(keyboard);
  glutSpecialFunc(skeyboard);
  glutMouseFunc(mButton);
  glutMotionFunc(mMotion);

  glutMainLoop();

  freeScene(&test);

  return 0;
}
