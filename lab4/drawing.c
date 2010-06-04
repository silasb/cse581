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

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

bool_t tGndTex=true;
bool_t tLinear=false;
bool_t trackballMove=false;

int nearClip = -100;
int farClip = 100;

int frame=0;
int movie[300];

/* prototypes */
void draw_floor();
void PPMWriteImage(unsigned char *imgBuffer, const int nx, const int ny);
void PPMReadImage(const char *, GLubyte **, int *, int *);
GLuint loadTexture(const char *filename, int wrap);
GLuint checkerBoard(int size);

void glutSolidCube2(GLdouble size);

void drawTV();
void legs();
void drawChair();

void
display()
{
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  if(ortho)
    setup_ortho_matrix();
  else
    setup_projection_matrix();

  if(trackballMove) {
    glRotatef(angle, axis[0], axis[1], axis[2]);
  }

  glPushMatrix();
  GLfloat light_position[] = {0.0, 20.0, 10.0, 1.0};
  glLightfv(GL_LIGHT0, GL_POSITION, light_position);
  glColor3f(1, 1, 1);
  glTranslatef(light_position[0], light_position[1], light_position[2]);
  glutWireCube(0.5);
  glPopMatrix();

  /* draw teapot */
  /*
  glPushMatrix();
  glColor3f(1, 1, 1);
  glTranslatef(0, 3, 0);
  glutSolidTeapot(3);
  glPopMatrix();
  */

  glPushMatrix();
  glTranslatef(-10, 0, -10);
  glRotatef(30, 0, 1, 0);
  drawTV();
  glPopMatrix();

  glPushMatrix();
  glTranslatef(10, 0, 15);
  glRotatef(40, 0, 1, 0);
  drawChair();
  glPopMatrix();

  glEnable(GL_TEXTURE_2D);
  glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);
  draw_floor();
  glDisable(GL_TEXTURE_2D);

  glPopMatrix();

  glutSwapBuffers();
}

void
draw_floor()
{
  glPushMatrix();
  glColor3f(1, 1, 1);

  GLuint texture;

  if(tGndTex)
    texture = gndTexture;
  else
    texture = checkerBoard(64);

  glBindTexture(GL_TEXTURE_2D, texture);
  glBegin(GL_QUADS);
  //glColor3f(0,0,1);
  glTexCoord2d(0, 0);
  glVertex3f(-32, 0, 32);
  //glColor3f(1,0,1);
  glTexCoord2d(1, 0);
  glVertex3f(32, 0, 32);
  //glColor3f(0,1,0);
  glTexCoord2d(1, 1);
  glVertex3f(32, 0, -32);
  //glColor3f(1,1,0);
  glTexCoord2d(0, 1);
  glVertex3f(-32, 0, -32);
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

  winWidth = width;
  winHeight = height;
}

void
setup_projection_matrix()
{
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();

  gluPerspective(60/zoomFactor, 1, 1.0f, 100.0f);
}

void
setup_ortho_matrix()
{
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  glOrtho(-26 / zoomFactor, 26 / zoomFactor, -26 / zoomFactor, 
          26 / zoomFactor, nearClip, farClip);
  glMatrixMode(GL_MODELVIEW);
}

/* export PPM */
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

GLuint buildTexture(GLubyte *data, int width, int height, int wrap)
{
  GLuint texture;

  // alloc tex name
  glGenTextures(1, &texture);

  // select texture
  glBindTexture( GL_TEXTURE_2D, texture );


  // mix texture with color for shading
  //glTexEnvf( GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
  glTexEnvf( GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);

  if(tLinear) {
    glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
    glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
  } else {
    glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST );
    glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST );
  }

  // if wrap is true, the texture wraps over at the edges (repeat)
  //       ... false, the texture ends at the edges (clamp)
  glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S,
                   wrap ? GL_REPEAT : GL_CLAMP );
  glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T,
                   wrap ? GL_REPEAT : GL_CLAMP );

  glTexImage2D(GL_TEXTURE_2D, 0, 3, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);

  return texture;
}

GLuint loadTexture(const char *filename, int wrap)
{
  GLubyte *image;
  GLuint texture;
  int width, height;

  PPMReadImage(filename, &image, &width, &height);
  texture = buildTexture(image, width, height, wrap);
  free(image); // opengl has the image now

  return texture;
}

void unloadTexture(GLuint texture) {
  free(texture);
}

GLuint 
checkerBoard(int size)
{
  //GLubyte *data;
  int i, j, c;

  //data = (GLubyte *) malloc(size * size * 3);

  size = 64;
  GLubyte data[64][64][3];

  for(i = 0; i < size; i++) {
    for(j = 0; j < size; j++) {
      c = ((((i&0x8)==0)^((j&0x8))==0))*255;
      //*(data+i + j + 0) = (GLubyte)c;
      //*(data + i + j + 1) = (GLubyte)c;
      //*(data+i+j+2) = (GLubyte)c;
      data[i][j][0] = (GLubyte) c;
      data[i][j][1] = (GLubyte) c;
      data[i][j][2] = (GLubyte) c;
    }
  }

  return buildTexture(data, size, size, 1);
}

void drawTV() {
  glEnable(GL_TEXTURE_2D);
  glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);

  glBindTexture(GL_TEXTURE_2D, movie[frame]);

  glPushMatrix();
  glColor3f(1, 1, 1);
  glTranslatef(0, 8, 0);
  glutSolidCube2(10);
  glPopMatrix();

  glDisable(GL_TEXTURE_2D);

  legs();
}

void drawChair()
{
  glPushMatrix();
  glColor3f(1, 1, 1);
  glTranslatef(0, 3.5, 0);
  glScalef(1, .10, 1);
  glutSolidCube(10);
  glPopMatrix();

  glPushMatrix();
  glTranslatef(0, 8, 5);
  glScalef(1, 1, .1);
  glutSolidCube(10);
  glPopMatrix();

  legs();
}

void legs()
{
  glPushMatrix();
  glTranslatef(-4, 1, -2);
  glRotatef(60, 0, 0, 1);
  glScalef(4, 1, 1);
  glutSolidCube(1);
  glPopMatrix();

  glPushMatrix();
  glTranslatef(-4, 1, 2);
  glRotatef(60, 0, 0, 1);
  glScalef(4, 1, 1);
  glutSolidCube(1);
  glPopMatrix();

  glPushMatrix();
  glTranslatef(4, 1, -2);
  glRotatef(-60, 0, 0, 1);
  glScalef(4, 1, 1);
  glutSolidCube(1);
  glPopMatrix();

  glPushMatrix();
  glTranslatef(4, 1, 2);
  glRotatef(-60, 0, 0, 1);
  glScalef(4, 1, 1);
  glutSolidCube(1);
  glPopMatrix();
}

static void
drawBox(GLfloat size, GLenum type)
{
  static GLfloat n[6][3] =
  {
    {-1.0, 0.0, 0.0},
    {0.0, 1.0, 0.0},
    {1.0, 0.0, 0.0},
    {0.0, -1.0, 0.0},
    {0.0, 0.0, 1.0},
    {0.0, 0.0, -1.0}
  };
  static GLint faces[6][4] =
  {
    {0, 1, 2, 3},
    {3, 2, 6, 7},
    {7, 6, 5, 4},
    {4, 5, 1, 0},
    {5, 6, 2, 1},
    {7, 4, 0, 3}
  };
  GLfloat v[8][3];
  GLint i;

  v[0][0] = v[1][0] = v[2][0] = v[3][0] = -size / 2;
  v[4][0] = v[5][0] = v[6][0] = v[7][0] = size / 2;
  v[0][1] = v[1][1] = v[4][1] = v[5][1] = -size / 2;
  v[2][1] = v[3][1] = v[6][1] = v[7][1] = size / 2;
  v[0][2] = v[3][2] = v[4][2] = v[7][2] = -size / 2;
  v[1][2] = v[2][2] = v[5][2] = v[6][2] = size / 2;

  for (i = 5; i >= 0; i--) {
    glBegin(type);
    glNormal3fv(&n[i][0]);
    glTexCoord2d(0, 0);
    glVertex3fv(&v[faces[i][0]][0]);
    glTexCoord2d(0, 1);
    glVertex3fv(&v[faces[i][1]][0]);
    glTexCoord2d(1, 1);
    glVertex3fv(&v[faces[i][2]][0]);
    glTexCoord2d(1, 0);
    glVertex3fv(&v[faces[i][3]][0]);
    glEnd();
  }
}

void 
glutSolidCube2(GLdouble size)
{
  drawBox(size, GL_QUADS);
}

void loadMovie()
{
  int i = 0;
  char filename[80];
  for(; i < 300; i++) {
    sprintf(filename, "frames/image%i.ppm", i);
    movie[frame++] = loadTexture(filename, 1);
  }
}

void idle()
{
}

