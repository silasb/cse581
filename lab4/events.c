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
#include <stdlib.h>
#include <math.h>

#include "common.h"
#include "drawing.h"
#include "events.h"

#include <stdio.h>

/* global varibles */
GLfloat zoomFactor = 1;
bool_t ortho=false;
float trans[3];
bool_t 	trackingMouse = false;

float lastPos[3] = {0.0F, 0.0F, 0.0F};
int curx, cury;
int startX, startY;

void trackball_ptov(int x, int y, int width, int height, float v[3]);
void stopMotion(int x, int y);
void startMotion(int x, int y);

void
keyboard(unsigned char key, int x, int y)
{
  switch(key)
  {
    // textures
    case 'c':
      tGndTex=false;
      break;
    case 'v':
      tGndTex=true;
      break;
    case 'n':
      tLinear=false;
      break;
    case 'b':
      tLinear=true;
      break;

    // clipping plane
    case 'a': // near clipping plane away from eye
      break;
    case 'z': // near clipping plane closer to eye
      break;
    case 'k': // far clipping plane away from eye
      break;
    case 'm': // move the far clipping plane closer to eye
      break;
      
    // shading
    case 'f': // flat shading
      glShadeModel(GL_FLAT);
      break;
    case 'g': // gouraud shading
      glShadeModel(GL_SMOOTH);
      break;
      
    // general  
    case '1': // window 256 x 256
      glutReshapeWindow(256, 256);
      break;
    case '2': // window 512 x 512
      glutReshapeWindow(512, 512);
      break;
    case '3': // window 1024 x 1024
      glutReshapeWindow(1024, 1024);
      break;
    case 's': // save to ppm
      exportPPM();
      break;
    case 'o': // orthogonal projection
      zoomFactor=1.0f;
      setup_ortho_matrix();
      ortho=true;
      break;
    case 'p': // perspective projectection
      zoomFactor=1.0f;
      setup_projection_matrix();
      ortho=false;
      break;
    case 27:
    case 'q': // quit
      glutDestroyWindow(win);
      exit(0);
      
    default:
      printf("key not bound\n");
  }
  glutPostRedisplay();
}

void
trackball_ptov(int x, int y, int width, int height, float v[3])
{
  float d, a;

  /* project x,y onto a hemi-sphere centered within width, height */
  v[0] = (2.0F*x - width) / width;
  v[1] = (height - 2.0F*y) / height;
  d = (float) sqrt(v[0]*v[0] + v[1]*v[1]);
  v[2] = (float) cos((M_PI/2.0F) * ((d < 1.0F) ? d : 1.0F));
  a = 1.0F / (float) sqrt(v[0]*v[0] + v[1]*v[1] + v[2]*v[2]);
  v[0] *= a;
  v[1] *= a;
  v[2] *= a;
}

void
mouseMotion(int x, int y)
{
  float curPos[3], dx, dy, dz;

  trackball_ptov(x, y, winWidth, winHeight, curPos);
  if(trackingMouse)
  {
    dx = curPos[0] - lastPos[0];
    dy = curPos[1] - lastPos[1];
    dz = curPos[2] - lastPos[2];

    if (dx || dy || dz) {
      angle = 180.0 * sqrt(dx*dx + dy*dy + dz*dz)/M_PI;

      axis[0] = lastPos[1]*curPos[2] - lastPos[2]*curPos[1];
      axis[1] = lastPos[2]*curPos[0] - lastPos[0]*curPos[2];
      axis[2] = lastPos[0]*curPos[1] - lastPos[1]*curPos[0];

      lastPos[0] = curPos[0];
      lastPos[1] = curPos[1];
      lastPos[2] = curPos[2];
    }

    glMatrixMode(GL_MODELVIEW);
    GLfloat mod[16];
    glGetFloatv(GL_MODELVIEW_MATRIX, mod);

    glLoadIdentity();
    glRotatef(angle, axis[0], axis[1], axis[2]);
    glMultMatrixf(mod);
    glPushMatrix();
    glGetFloatv(GL_MODELVIEW_MATRIX, mod);

    glLoadIdentity();
    glTranslatef(0, 0, -16);
    glMultMatrixf(mod);
  }
  glutPostRedisplay();
}

void
startMotion(int x, int y)
{
  trackingMouse = true;
  //redrawContinue = false;
  startX = x; startY = y;
  curx = x; cury = y;
  trackball_ptov(x, y, winWidth, winHeight, lastPos);
  trackballMove=true;
}

void
stopMotion(int x, int y)
{
  trackingMouse = false;

  if (startX != x || startY != y) {
    //redrawContinue = true;
  } else {
    angle = 0.0F;
    //redrawContinue = false;
    trackballMove = false;
  }
}

void
mouseButton(int button, int state, int x, int y)
{
  if(button==GLUT_LEFT_BUTTON) switch(state)
  {
    case GLUT_DOWN:
      startMotion( x,y);
      break;
    case GLUT_UP:
      stopMotion( x,y);
      break;
  }
}


