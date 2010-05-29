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

bool_t tGndTex=true;
bool_t tLinear=false;
bool_t trackballMove=false;

int nearClip = -100;
int farClip = 100;

/* prototypes */
void draw_floor();
void PPMWriteImage(unsigned char *imgBuffer, const int nx, const int ny);
void PPMReadImage(const char *, GLubyte **, int *, int *);
GLuint loadTexture(const char *filename, int wrap);
GLuint checkerBoard(int size);

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
  GLfloat light_position[] = {0.0, 10.0, 0.0, 1.0};
  glLightfv(GL_LIGHT0, GL_POSITION, light_position);
  glColor3f(1, 1, 1);
  glTranslatef(0, 9.5, 0);
  glutWireCube(0.5);
  glPopMatrix();

  /* draw teapot */
  glPushMatrix();
  glColor3f(1, 1, 1);
  glTranslatef(0, 3, 0);
  glutSolidTeapot(3);
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
