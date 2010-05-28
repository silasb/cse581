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

#include <stdio.h>

/* global varibles */
GLfloat zoomFactor = 1;
bool_t ortho=false;

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
      tFlatShd=true;
      break;
    case 'g': // gouraud shading
      tFlatShd=false;
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
