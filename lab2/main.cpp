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
#include "ppm.h"

#define WIDTH 512
#define HEIGHT 512


static int _mouseX = 0;
static int _mouseY = 0;
static bool lButtonDown;
static bool rButtonDown;
static float zoom = 1;
bool enabled_pip = true;
GLfloat viewport[2];
scene_t scene;

// window idenfitier
static int win;

// internal prototypes
void exportPPM();
void pip(int, int);

static void
mMotion(int x, int y)
{
  bool changed = false;

  const int dx = x - _mouseX;
  const int dy = y - _mouseY;

  if(dx == 0 && dy == 0)
    return;

  if(lButtonDown) 
  {
    viewport[0] -= dx * 0.005;
    viewport[1] += dy * 0.005;

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

static void
mButton(int button, int state, int x, int y)
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
    if(state == GLUT_DOWN)
      rButtonDown = true;
    else
      rButtonDown = false;
}

static void
resize(int width, int height)
{
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();

  if(height > width)
    glViewport(0, height-width, width, width);
  else
    glViewport(0, 0, height, height);

  glOrtho(-1, 1, -1, 1, -1, 1);

  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
}

void
reshape(GLsizei width, GLsizei height)
{
  glutReshapeWindow(width, height);
}

void
display()
{
  int width = glutGet(GLUT_WINDOW_WIDTH);
  int height = glutGet(GLUT_WINDOW_HEIGHT);

  glClear(GL_COLOR_BUFFER_BIT);

  if(enabled_pip)
    pip(width, height);

  if(height > width)
    glViewport(0, height-width, width, width);
  else
    glViewport(0, 0, height, height);

  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();

  glOrtho(-1 * zoom + viewport[0], 1 * zoom + viewport[0], -1 * zoom + viewport[1], 1 * zoom + viewport[1], -1, 1);

  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();

  drawScene(&scene);

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

  drawScene(&scene);
}

void
keyboard(unsigned char key, int x, int y)
{
  switch(key)
  {
    case 's':
      exportPPM();
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
      viewport[0] = 0; viewport[1] = 0; zoom = 1;
      break;
    case 'm':
      if(enabled_pip) enabled_pip = false; 
      else enabled_pip = true;
      break;
    case 'q':
    case 27:
      glutDestroyWindow(win);
      exit(0);
  }
}

void
skeyboard(int key, int x, int y)
{
  switch(key) {
    case GLUT_KEY_F1:
      printf("HIT F1 key\n");
      break;
  }
}

void
idle(void)
{
  animate(&scene);
}

int
main(int argc, char** argv)
{
  if( argc < 2 )
  {
    fprintf(stderr, "You didn't specify enough arguments\n");
    exit(1);
  }

  char *fileName = argv[1];

  loadScene(fileName, &scene);

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

  freeScene(&scene);

  return 0;
}

void
exportPPM()
{
  int width = glutGet(GLUT_WINDOW_WIDTH);
  int height = glutGet(GLUT_WINDOW_HEIGHT);
  unsigned char *image = (unsigned char *)malloc(height * width * 3 * sizeof(unsigned char *));
  //unsigned char image[width * height * 3];
  glReadPixels(0, 0, width, height, GL_RGB, GL_UNSIGNED_BYTE, image);
  PPMWriteImage(image, width, height);
}