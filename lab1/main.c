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

#define WIDTH 128
#define HEIGHT 128
#define NFRAMES 48

enum color { R, G, B };

const float PI=3.14159265358979;

float XPoint(float,float, float,float, float,float);
float YPoint(float,float, float,float, float,float);
void fractal(float,float,int,const float);

int cur_frame;
unsigned char oneFrame[WIDTH * HEIGHT * 3];
unsigned char images[NFRAMES][WIDTH * HEIGHT * 3];

void display()
{
  glClear(GL_COLOR_BUFFER_BIT);
  
  for (unsigned int i = 0; i < NFRAMES; i++)
  {
    //memset(oneFrame, 0, WIDTH * HEIGHT * 3);
    //fractal(0, 0, 20, PI*i/(NFRAMES * 0.5));
    
    glDrawPixels(WIDTH, HEIGHT, GL_RGB, GL_UNSIGNED_BYTE, images[i]);
    sleep(1);
    glFlush();
    printf("finished displaying frame: %i\n", i);
    // insert code for sleeping
  }
}

void mouse(int button, int state, int x, int y)
{
  // insert code
}

void keyboard(unsigned char key, int x, int y)
{
  // insert code
}


int main(int argc, char** argv){

  for(cur_frame = 0; cur_frame < NFRAMES; cur_frame++)
  {
    fractal(0, 0, 20, PI*cur_frame/(NFRAMES * 0.5));
    printf("frame: %i complete\n", cur_frame);

  }

  glutInit(&argc, argv);
  glutInitWindowSize(WIDTH, HEIGHT);
  glutCreateWindow("Fractal Browser");
  glutDisplayFunc(display);

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
