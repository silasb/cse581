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


#include <math.h>		// Trig functions
#include <string.h>		// Memory functions such as memset
#include <unistd.h>

#define WIDTH 256
#define HEIGHT 256
#define NFRAMES 48

const float PI=3.14159265358979;

float XPoint(float,float, float,float, float,float);
float YPoint(float,float, float,float, float,float);
void fractal(float,float,int,const float);

unsigned char oneFrame[WIDTH * HEIGHT];

void display()
{
  glClear(GL_COLOR_BUFFER_BIT);
  
  for (unsigned int i = 0; i < NFRAMES; i++)
  {
    memset(oneFrame, 0, WIDTH * HEIGHT);
    fractal(0, 0, 10, PI*i/(NFRAMES * 0.5));
    
    glDrawPixels(WIDTH, HEIGHT, GL_LUMINANCE, GL_UNSIGNED_BYTE, oneFrame);
    glFlush();

    sleep(1);
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
    
    //xc = int( (x-(-2)) * WIDTH/4. );
    xc = (int)( (x-(-2)) * WIDTH/4.0 );
    //yc = int( (y-(-2)) * HEIGHT/4. );
    yc = (int)( (y-(-2)) * HEIGHT/4.0 );
    
    oneFrame[WIDTH * yc + xc] = 255;
    
  } else {
    fractal(XPoint(1,0,0+ang,.70710678,x,y),YPoint(1,0,0+ang,.70710678,x,y),depth-1,ang);
    fractal(XPoint(-1,0,0-ang,.70710678,x,y),YPoint(-1,0,0-ang,.70710678,x,y),depth-1,ang);
  }
}
