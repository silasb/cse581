/*
 * CSE581 - Lab #1
 * Author: Silas Baronda
 * Creation Date: 04/04/10
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
#include <math.h>		// Trig functions
#include <string.h>		// Memory functions such as memset
#include <stdlib.h>
#include "ppm.h"

#define WIDTH 512
#define HEIGHT 512
#define NFRAMES 48

int xrange;

enum color { R, G, B };

const float PI=3.14159265358979;

float XPoint(float,float, float,float, float,float);
float YPoint(float,float, float,float, float,float);
void fractal(float,float,int,const float);

int cur_frame;
unsigned char images[NFRAMES][WIDTH * HEIGHT * 3];

// window idenfitier
static int win;

void display()
{
  printf("displaying frame: %i\n", cur_frame);

  glClear(GL_COLOR_BUFFER_BIT);
  
  glDrawPixels(WIDTH, HEIGHT, GL_RGB, GL_UNSIGNED_BYTE, images[cur_frame]);
  glFlush();
}

void mouse(int button, int state, int x, int y)
{
  if(button == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
  {
    glClear(GL_COLOR_BUFFER_BIT);
    glDrawPixels(WIDTH, HEIGHT, GL_RGB, GL_UNSIGNED_BYTE, images[cur_frame++]);
    glFlush();
  }
}

void keyboard(unsigned char key, int x, int y)
{
  switch(key)
  {
    case 's':
      printf("writing PPM image out to out-%i.ppm\n", cur_frame);
      PPMWriteImage(cur_frame, images[cur_frame], WIDTH, HEIGHT);
      break;
    case 'd':
      cur_frame--;
      if(cur_frame < 0) cur_frame = NFRAMES - 1;
      display();
      break;
    case 'f':
      cur_frame++;
      if(cur_frame >= NFRAMES) cur_frame = 0;
      display();
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

void clean_exit()
{
  puts("Quitting...");
}

int main(int argc, char** argv){

  for(cur_frame = 0; cur_frame < NFRAMES; cur_frame++)
  {
    fractal(0, 0, 20, PI*cur_frame/(NFRAMES * 0.5));
    printf("processing frame: %i complete\n", cur_frame);
  }

  cur_frame = 0;

  glutInit(&argc, argv);
  glutInitWindowSize(WIDTH, HEIGHT);
  win =  glutCreateWindow("Fractal Browser");
  glutDisplayFunc(display);
  glutKeyboardFunc(keyboard);
  glutMouseFunc(mouse);
  glutSpecialFunc(skeyboard);

  atexit(clean_exit);

  glutMainLoop();

  return 0;
}

float XPoint(float xc,float yc, float theta,float scale, float x,float y){
  return ((x*cos(theta)-y*sin(theta))*scale+xc);
}

float YPoint(float xc,float yc, float theta,float scale, float x,float y){
  return ((x*sin(theta)+y*cos(theta))*scale+yc);
}

void fractal(float x, float y, int depth, const float ang) {
  if (depth==0) {
    
    int xc,yc;
    
    xc = int( (x-(-2)) * WIDTH/4.0 );
    yc = int( (y-(-2)) * HEIGHT/4.0 );
    
    int loc = (WIDTH * yc + xc) * 3;

    if(xc < xrange)
    {
      images[cur_frame][loc + R] = xrange;
      images[cur_frame][loc + G] = xrange;
      images[cur_frame][loc + B] = xrange;
    }
    else {
      images[cur_frame][loc + R] = 204;
      images[cur_frame][loc + G] = 51;
      images[cur_frame][loc + B] = 153;
    }
  } else {
    xrange++;
    fractal(XPoint(1,0,0+ang,.70710678,x,y),YPoint(1,0,0+ang,.70710678,x,y),depth-1,ang);
    fractal(XPoint(-1,0,0-ang,.70710678,x,y),YPoint(-1,0,0-ang,.70710678,x,y),depth-1,ang);
  }
}
