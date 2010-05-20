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

#include "common.h"
#include "drawing.h"
#include "events.h"

#include <stdlib.h>
#include <math.h>

void draw_octa();
void draw_tetra();

void draw_floor();
void draw_pointer();
void draw_bounding_box();

/* tetrahedron data */
GLfloat tdata[4][3] =
{
  {1, 1, 1},
  {1, -1, -1},
  {-1, 1, -1},
  {-1, -1, 1}
};
int tndex[4][3] =
{
    {0, 1, 3},
    {2, 1, 0},
    {3, 2, 0},
    {1, 2, 3}
};

/* octahedron data */
GLfloat odata[6][3] =
{
  {1.0, 0.0, 0.0},
  {-1.0, 0.0, 0.0},
  {0.0, 1.0, 0.0},
  {0.0, -1.0, 0.0},
  {0.0, 0.0, 1.0},
  {0.0, 0.0, -1.0}
};
int ondex[8][3] =
{
  {0, 4, 2},
  {1, 2, 4},
  {0, 3, 4},
  {1, 4, 3},
  {0, 2, 5},
  {1, 5, 2},
  {0, 5, 3},
  {1, 3, 5}
};


void PPMWriteImage(unsigned char *imgBuffer, const int nx, const int ny);

void
display()
{
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
  gluLookAt(eye[0], eye[1], eye[2],
            coi[0], coi[1], coi[2],
             up[0],  up[1],  up[2]);

  if(bounding_box)
    draw_bounding_box();

  if(pointer)
    draw_pointer();

  glPushMatrix();
  glTranslatef(20, 5, 20);
  glutSolidTeapot(3);
  glPopMatrix();

  glColor3f(1, 0, 1);
  glPushMatrix();
  glTranslatef(0, 5, 0);
  glutSolidSphere(5, 10, 10);
  glPopMatrix();

  glColor3f(0, 0.5, 1);
  glPushMatrix();
  glTranslatef(-10, 10, 0);
  glRotatef(90, 0, 1, 1);
  GLUquadric *obj1 = gluNewQuadric();
  gluQuadricDrawStyle(obj1, GLU_FILL);
  gluPartialDisk(obj1, 5, 11, 10, 10, 0, 240);
  glPopMatrix();

  glColor3f(0, 0.5, 1);
  glPushMatrix();
  glTranslatef(10, 10, 0);
  glRotatef(30, 1, 0, 0);
  GLUquadric *obj2 = gluNewQuadric();
  gluQuadricDrawStyle(obj2, GLU_FILL);
  gluCylinder(obj1, 2, 3, 10, 10, 10);
  glPopMatrix();

  glPushMatrix();
  glTranslatef(5, 15, 0);
  draw_octa();
  glPopMatrix();

  glPushMatrix();
  glTranslatef(5, 15, -5);
  draw_tetra();
  glPopMatrix();


  draw_floor();
  
  glFlush();
  glutSwapBuffers();
}

void
draw_pointer()
{
  glPushMatrix();
  glTranslatef(coi[0], coi[1], coi[2]);
  glutWireCube(1.0f);
  glPopMatrix();
}

void
draw_bounding_box()
{
  glPushMatrix();
  glTranslatef(0, 25, 0);
  glutWireCube(50.0f);
  glPopMatrix();
}

void
draw_floor()
{
  glPushMatrix();
  glBegin(GL_QUADS);
  glColor3f(0,0,1);
  glVertex3f(-25, 0, 25);
  glColor3f(1,0,1);
  glVertex3f(25, 0, 25);
  glColor3f(0,1,0);
  glVertex3f(25, 0, -25);
  glColor3f(1,1,0);
  glVertex3f(-25, 0, -25);
  glEnd();
  glPopMatrix();
}

void
resize(int width, int height)
{

  if(height > width)
    glViewport(0, height-width, width, width);
  else
    glViewport(0, 0, height, height);

  setup_projection_matrix();

  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
  gluLookAt(eye[0], eye[1], eye[2],
            coi[0], coi[1], coi[2],
             up[0],  up[1],  up[2]);

  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
}

void
setup_projection_matrix()
{
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();

  gluPerspective(60/zoomFactor, 1, 1.0f, 100.0f);
}

void
recorditem(GLfloat * n1, GLfloat * n2, GLfloat * n3)
{
  glBegin(GL_LINE_LOOP);
  glVertex3fv(n1);
  glVertex3fv(n2);
  glVertex3fv(n3);
  glEnd();
}

void
subdivide(GLfloat * v0, GLfloat * v1, GLfloat * v2)
{
  int depth;
  GLfloat w0[3], w1[3], w2[3];
  GLfloat l;
  int i, j, k, n;

  depth = 1;
  for (i = 0; i < depth; i++) {
    for (j = 0; i + j < depth; j++) {
      k = depth - i - j;
      for (n = 0; n < 3; n++) {
        w0[n] = (i * v0[n] + j * v1[n] + k * v2[n]) / depth;
        w1[n] = ((i + 1) * v0[n] + j * v1[n] + (k - 1) * v2[n]) / depth;
        w2[n] = (i * v0[n] + (j + 1) * v1[n] + (k - 1) * v2[n]) / depth;
      }
      l = sqrt(w0[0] * w0[0] + w0[1] * w0[1] + w0[2] * w0[2]);
      w0[0] /= l;
      w0[1] /= l;
      w0[2] /= l;
      l = sqrt(w1[0] * w1[0] + w1[1] * w1[1] + w1[2] * w1[2]);
      w1[0] /= l;
      w1[1] /= l;
      w1[2] /= l;
      l = sqrt(w2[0] * w2[0] + w2[1] * w2[1] + w2[2] * w2[2]);
      w2[0] /= l;
      w2[1] /= l;
      w2[2] /= l;
      recorditem(w1, w0, w2);
    }
  }
}

void
drawtriangle(int i, GLfloat data[][3], int ndx[][3])
{
  GLfloat *x0, *x1, *x2;

  x0 = data[ndx[i][0]];
  x1 = data[ndx[i][1]];
  x2 = data[ndx[i][2]];
  subdivide(x0, x1, x2);
}

void
draw_octa()
{
  int i;
  for (i = 7; i >= 0; i--)
    drawtriangle(i, odata, ondex);
}

void
draw_tetra()
{
  int i;
  for (i = 3; i >= 0; i--)
    drawtriangle(i, tdata, tndex);
}

void
exportPPM(void)
{
  int width = glutGet(GLUT_WINDOW_WIDTH);
  int height = glutGet(GLUT_WINDOW_HEIGHT);
  unsigned char *image = (unsigned char *)malloc(width * height * 3 * sizeof(unsigned char *));
  glReadPixels(0, 0, width, height, GL_RGB, GL_UNSIGNED_BYTE, image);
  PPMWriteImage(image, width, height);
  free(image);
}
