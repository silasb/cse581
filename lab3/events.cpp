#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

#include "common.h"
#include <stdlib.h>

void
mouse(int button, int state, int x, int y)
{
  if(button == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
  {
  }
}

void
keyboard(unsigned char key, int x, int y)
{
  switch(key)
  {
    case '1': // window 256 x 256
      break;
    case '2': // window 512 x 512
      break;
    case '3': // window 1024 x 1024
      break;
    case 's': // save to ppm
      break;
    case '5':
      // eye loc (25, 25, 0)
      // coi (-25, 25, 0)
      break;
    case '6':
      // eye loc (0, 25, 25)
      // coi (0, 25, -25)
      break;
    case '7':
      // eye loc (-25, 25, 0)
      // coi (25, 25, 0)
      break;
    case '8':
      // eye loc (0, 25, -25)
      // coi (0, 25, 25)
      break;
    case 'd': // cam left 5
      break;
    case 'g': // cam right 5
      break;
    case 'r': // cam up 5
      break;
    case 'f': // cam down 5
      break;
    case 'j': // coi left 5
      break;
    case 'l': // coi right 5
      break;
    case 'i': // coi up 5
      break;
    case 'k': // coi down 5 or along the ground plane
      break;
    case 'b': // move eye and coi left 5, if no bound restrictions
      break;
    case 'm': // move eye and coi right 5, if no bound restrictions
      break;
    case 'h': // move eye and coi up 5, if no bound restrictions
      break;
    case 'n': // move eye and coi down 5, if no bound restrictions
      break;
    case '.': // rotate cam up-vector clockwise 5 degrees
      break;
    case ',': // rotate cam up-vector counter-clockwise 5 degrees
      break;
    case 'o': // orthogonal projection
      break;
    case 'p': // perspective projectection
      break;
    case 'w': // wireframe
      break;
    case 'a': // show location of coi
      break;
    case 'z': // hidden surface removeal
      break;
    case 'x': // back face culling
      break;
    case 'c': // draw imaginary bounding box
      break;
    case 'q': // quit
      glutDestroyWindow(win);
      exit(0);
  }
}

void
skeyboard(int key, int x, int y)
{
  switch(key) {
    case GLUT_KEY_F1:
      break;
  }
}
