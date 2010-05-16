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

#define WIDTH 512
#define HEIGHT 512
#define NFRAMES 48

// window idenfitier
static int win;

void display()
{
  glClear(GL_COLOR_BUFFER_BIT);
  
  glFlush();
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
    case 'd':
      break;
    case 'f':
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
  glutInit(&argc, argv);
  glutInitWindowSize(WIDTH, HEIGHT);
  win = glutCreateWindow("lab3");
  glutDisplayFunc(display);
  glutKeyboardFunc(keyboard);
  glutMouseFunc(mouse);
  glutSpecialFunc(skeyboard);

  glutMainLoop();

  return 0;
}
