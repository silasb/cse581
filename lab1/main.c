#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

#include <math.h>
#include <string.h>
#include <stdio.h>
#include "ppm.h"

#define HEIGHT 512
#define WIDTH 512
#define NFRAMES 48

const float PI = 3.14159265358979;

unsigned char oneFrame[WIDTH * HEIGHT * 3];

float XPoint(float xc, float yc, float theta, float scale, float x, float y);
float YPoint(float xc, float yc, float theta, float scale, float x, float y);
void fractal(float x, float y, int depth, const float ang);

void display() {
  glClear(GL_COLOR_BUFFER_BIT);

  for(unsigned int i = 0; i < NFRAMES; i++)
  {
    memset(oneFrame, 0, WIDTH * HEIGHT * 3);

    printf("frame: %i\n", i);
    fractal(0, 0, 20, PI * i/(NFRAMES * 0.5));

    //PPMWriteImage(oneFrame, WIDTH, HEIGHT);
    glDrawPixels(WIDTH, HEIGHT, GL_RGB, GL_UNSIGNED_BYTE, oneFrame);
    glFlush();
  }
  //PPMWriteImage(oneFrame, WIDTH, HEIGHT);
  
}

void mouse(int button, int state, int x, int y)
{
  
}

void keyboard(unsigned char key, int x, int y)
{

}

int main (int argc, char ** argv) {
  glutInit(&argc, argv);
  glutInitWindowSize(WIDTH, HEIGHT);
  glutCreateWindow("Fractal Browswer");
  glutDisplayFunc(display);
  
  glutMainLoop();
  
  return 0;
}

float XPoint(float xc, float yc, float theta, float scale, float x, float y)
{
  return ((x*cos(theta)-y*sin(theta))*scale+xc);
}

float YPoint(float xc, float yc, float theta, float scale, float x, float y)
{
  return ((x*sin(theta)+y*cos(theta))*scale+yc);
}

void fractal(float x, float y, int depth, const float ang)
{
  printf("depth: %i\n", depth);
  if(depth == 0)
  {
    int xc, yc;
    
    xc = (int) ((x-(-2)) * WIDTH / 4.0);
    yc = (int) ((y-(-2)) * HEIGHT / 4.0);
    
    if(xc > WIDTH || xc < 0)
      return;
    
    if(yc > HEIGHT || yc < 0)
      return;
    
    // rgb
    oneFrame[WIDTH * yc + xc] = 204;
    oneFrame[WIDTH * yc + xc + 1] = 51;
    oneFrame[WIDTH * yc + xc + 2] = 153;
  } else {
    fractal(XPoint( 1, 0, 0+ang, .70710678, x, y), YPoint( 1, 0, 0+ang, .70710678, x, y), depth-1, ang);
    fractal(XPoint(-1, 0, 0-ang, .70710678, x, y), YPoint(-1, 0, 0-ang, .70710678, x, y), depth-1, ang);
  }
}
