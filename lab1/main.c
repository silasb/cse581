/*
 * CSE581 - Lab #1
 * Author: Silas Baronda
 * Creation Date: 04/04/10
 *
 */

#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif


#include <stdio.h>
#include <math.h>		// Trig functions
#include <string.h>		// Memory functions such as memset
#include <unistd.h>
#include <stdlib.h>
#include "ppm.h"

#define WIDTH 128
#define HEIGHT 128
#define NFRAMES 48

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
    for(cur_frame = 0; cur_frame < NFRAMES; cur_frame++)
    {
      glClear(GL_COLOR_BUFFER_BIT);
      glDrawPixels(WIDTH, HEIGHT, GL_RGB, GL_UNSIGNED_BYTE, images[cur_frame]);
      sleep(1);
      glFlush();
    }
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

int main(int argc, char** argv){

  for(cur_frame = 0; cur_frame < NFRAMES; cur_frame++)
  {
    memset(images[cur_frame], 0, WIDTH * HEIGHT * 3);
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
    
    xc = int( (x-(-2)) * WIDTH/4. );
    //xc = (int)( (x-(-2)) * WIDTH/4.0 );
    yc = int( (y-(-2)) * HEIGHT/4. );
    //yc = (int)( (y-(-2)) * HEIGHT/4.0 );
    
    int loc = (WIDTH * yc + xc) * 3;
    images[cur_frame][loc + R] = 204;
    images[cur_frame][loc + G] = 51;
    images[cur_frame][loc + B] = 153;
  } else {
    fractal(XPoint(1,0,0+ang,.70710678,x,y),YPoint(1,0,0+ang,.70710678,x,y),depth-1,ang);
    fractal(XPoint(-1,0,0-ang,.70710678,x,y),YPoint(-1,0,0-ang,.70710678,x,y),depth-1,ang);
  }
}
