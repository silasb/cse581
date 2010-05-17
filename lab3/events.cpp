#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif
#include <stdlib.h>
#include <math.h>
#include <stdio.h>

#include "common.h"
#include "drawing.h"
#include "shared.h"

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
      break;
    case '6':
      // eye loc (0, 25, 25)
      // coi (0, 25, -25)
      eye[0] = 0.0f; eye[1] = 25.0f; eye[2] = 25.0f;
      coi[0] = 0.0f; coi[1] = 25.0f; coi[2] = -25.0f;
      break;
    case '7':
      // eye loc (-25, 25, 0)
      // coi (25, 25, 0)
      eye[0] = -25.0f; eye[1] = 25.0f; eye[2] = 0.0f;
      coi[0] = 25.0f; coi[1] = 25.0f; coi[2] = 0.0f;
      break;
    case '8':
      // eye loc (0, 25, -25)
      // coi (0, 25, 25)
      eye[0] = 0.0f; eye[1] = 25.0f; eye[2] = -25.0f;
      coi[0] = 0.0f; coi[1] = 25.0f; coi[2] = 25.0f;
      break;
    case 'd': // cam left 5
      if(eye[0] != -25)
        eye[0] -= 5;
      break;
    case 'g': // cam right 5
      if(eye[0] != 25)
        eye[0] += 5;
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
      if(coi[0] != -25)
        coi[0] -= 5;
      break;
    case 'l': // coi right 5
      if(coi[0] != 25)
      coi[0] += 5;
      break;
    case 'i': // coi up 5
      coi[1] += 5;
      break;
    case 'k': // coi down 5 or along the ground plane
      coi[1] -= 5;
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
      vec_t m[4][4];
      rotate(1, 5, m);
      vec3_t a; a[0] = 0; a[1] = 1; a[2] = 0;
      vec3_t b;
      vec_mul_matrix(a, m, b);
      VectorCopy(up,b);
      printf("%f %f %f\n", up[0], up[1], up[2]);
      break;
    case ',': // rotate cam up-vector counter-clockwise 5 degrees
      vec_t m2[4][4];
      rotate(1, -5, m2);
      vec3_t c;
      vec_mul_matrix(up, m, c);
      VectorCopy(up,c);
      printf("%f %f %f\n", up[0], up[1], up[2]);
      break;
    case 'o': // orthogonal projection
      glMatrixMode(GL_PROJECTION);
      glLoadIdentity();
      //glOrtho(-12, 12, -12, 12, 1, 100);
      glOrtho(-50, 50, -50, 50, 1, 100);
      break;
    case 'p': // perspective projectection
      glMatrixMode(GL_PROJECTION);
      glLoadIdentity();
      gluPerspective(60, 1, 1, 100);
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
