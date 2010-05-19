#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif
#include <stdlib.h>
#include <math.h>

#include "common.h"
#include "drawing.h"

float zoomFactor=1.0;

int mouseX = 0;
int mouseY = 0;
bool_t rButtonDown;

int rAng = 0;

int face=0;
int flip=1;

void
keyboard(unsigned char key, int x, int y)
{
  switch(key)
  {
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
      break;
    case '5':
      // eye loc (25, 25, 0)
      // coi (-25, 25, 0)
      eye[0] = 25.0f; eye[1] = 25.0f; eye[2] = 0.0f;
      coi[0] = -25.0f; coi[1] = 25.0f; coi[2] = 0.0f;
      face=2;
      flip=-1;
      break;
    case '6':
      // eye loc (0, 25, 25)
      // coi (0, 25, -25)
      eye[0] = 0.0f; eye[1] = 25.0f; eye[2] = 25.0f;
      coi[0] = 0.0f; coi[1] = 25.0f; coi[2] = -25.0f;
      face=0;
      flip=1;
      break;
    case '7':
      // eye loc (-25, 25, 0)
      // coi (25, 25, 0)
      eye[0] = -25.0f; eye[1] = 25.0f; eye[2] = 0.0f;
      coi[0] = 25.0f; coi[1] = 25.0f; coi[2] = 0.0f;
      face=2;
      flip=1;
      break;
    case '8':
      // eye loc (0, 25, -25)
      // coi (0, 25, 25)
      eye[0] = 0.0f; eye[1] = 25.0f; eye[2] = -25.0f;
      coi[0] = 0.0f; coi[1] = 25.0f; coi[2] = 25.0f;
      face=0;
      flip=-1;
      break;
    case 'd': // cam left 5
      if(eye[0 + face] != -25 * flip)
        eye[0 + face] -= flip * 5;
      break;
    case 'g': // cam right 5
      if(eye[0 + face] != 25 * flip)
        eye[0 + face] += flip * 5;
      break;
    case 'r': // cam up 5
      if(eye[1] != 25)
        eye[1] += 5;
      break;
    case 'f': // cam down 5
      if(eye[1] != 0)
        eye[1] -= 5;
      break;
    case 'j': // coi left 5
      if(coi[0+face] != -25 * flip)
        coi[0+face] -= flip * 5;
      break;
    case 'l': // coi right 5
      if(coi[0+face] != 25 * flip)
        coi[0+face] += flip * 5;
      break;
    case 'i': // coi up 5
      if(coi[1] != 50)
        coi[1] += 5;
      break;
    case 'k': // coi down 5 or along the ground plane
      if(coi[1] != 0)
      coi[1] -= 5;
      break;
    case 'b': // move eye and coi left 5, if no bound restrictions
      if((coi[0 + face] != -25 * flip && eye[0 + face] != -25 * flip)) {
        coi[0 + face] -= flip * 5;
        eye[0 + face] -= flip * 5;
      }  
      break;
    case 'm': // move eye and coi right 5, if no bound restrictions
      if((coi[0 + face] != 25 * flip && eye[0 + face] != 25 * flip)) {
        coi[0 + face] += flip * 5;
        eye[0 + face] += flip * 5;
      }  
      break;
    case 'h': // move eye and coi up 5, if no bound restrictions
      if((coi[1] != 50 && eye[1] != 50)) {
        coi[1] += 5;
        eye[1] += 5;
      }
      break;
    case 'n': // move eye and coi down 5, if no bound restrictions
      if((coi[1] != 0 && eye[1] != 0)) {
        coi[1] -= 5;
        eye[1] -= 5;
      }
      break;
    case '.': // rotate cam up-vector clockwise 5 degrees
      rAng += 5;
      up[1] = cos(rAng * M_PI*2/360);
      up[0] = sin(rAng * M_PI*2/360);
      break;
    case ',': // rotate cam up-vector counter-clockwise 5 degrees
      rAng -= 5;
      up[1] = cos(rAng * M_PI*2/360);
      up[0] = sin(rAng * M_PI*2/360);
      break;
    case 'o': // orthogonal projection
      glMatrixMode(GL_PROJECTION);
      glLoadIdentity();
      //glOrtho(-12, 12, -12, 12, 1, 100);
      glOrtho(-50, 50, -50, 50, 1, 100);
      break;
    case 'p': // perspective projectection
      setup_projection_matrix();
      break;
    case 'w': // wireframe
      wireframe = wireframe == true ? false : true;
      glPolygonMode(GL_FRONT_AND_BACK, wireframe == true ? GL_LINE : GL_FILL);
      break;
    case 'a': // show location of coi
      if(pointer)
        pointer=false;
      else
        pointer=true;
      break;
    case 'z': // hidden surface removeal
      break;
    case 'x': // back face culling
      break;
    case 'c': // draw imaginary bounding box
      if(bounding_box)
        bounding_box=false;
      else
        bounding_box=true;
      break;
    case 27:
    case 'q': // quit
      glutDestroyWindow(win);
      exit(0);
  }
  glutPostRedisplay();
}

void
skeyboard(int key, int x, int y)
{
  switch(key) {
    case GLUT_KEY_F1:
      break;
  }
}

void
mButton(int button, int state, int x, int y)
{
  mouseX = x;
  mouseY = y;

  if(button == GLUT_RIGHT_BUTTON)
  {
    if(state == GLUT_DOWN)
      rButtonDown = true;
    else
      rButtonDown = false;
  }
}

void
mMotion(int x, int y)
{
  bool_t changed = true;

  int dx = x - mouseX;
  int dy = y - mouseY;

  if(dx == 0 && dy == 0)
    return;

  if(rButtonDown)
  {
    if(abs(dy) > abs(dx)) {
      zoomFactor -= dy/100.0;
      if(zoomFactor < 0.5) zoomFactor = 0.5;
    }
    changed = true;
  }

  mouseX = x;
  mouseY = y;

  if(changed) {
    setup_projection_matrix();
    glutPostRedisplay();
  }
}
