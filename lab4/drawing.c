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
bool_t tFlatShd=true;

/* prototypes */
void draw_floor();
void PPMWriteImage(unsigned char *imgBuffer, const int nx, const int ny);
void PPMReadImage(const char *, GLubyte **, int *, int *);
GLuint loadTexture(const char *filename, int wrap);
GLuint checkerBoard(int size);

void
display()
{
  if(tFlatShd)
    glShadeModel(GL_FLAT);
  else
    glShadeModel(GL_SMOOTH);

  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
  gluLookAt(0, 10, 25, // pos
            0, 0, -25,   // coi
            0,  1, 0);

  GLfloat light_position[] = {-25.0, 10.0, 25.0, 1.0};
  glLightfv(GL_LIGHT0, GL_POSITION, light_position);

  /* draw teapot */
  glColor3f(1, 1, .2);
  glPushMatrix();
  glTranslatef(0, 3, 0);
  glutSolidTeapot(3);
  glPopMatrix();

  glEnable(GL_TEXTURE_2D);
  glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);
  draw_floor();
  glDisable(GL_TEXTURE_2D);

  
  glFlush();
  glutSwapBuffers();
}

void
draw_floor()
{
  glColor3f(1, 1, 1);
  glPushMatrix();

  GLuint texture;

  if(tGndTex)
    texture = loadTexture("test/Untitled.ppm", 1);
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

  setup_projection_matrix();

  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
  gluLookAt(0, 10, -25,
            0, 0, 25,
            0,  1, 0);
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
setup_ortho_matrix()
{
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  glOrtho(-26 / zoomFactor, 26 / zoomFactor, -26 / zoomFactor, 
          26 / zoomFactor, 1, 100);
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

GLuint buildTexture(GLubyte *data, int wrap, int width, int height)
{
  GLuint texture;

  // alloc tex name
  glGenTextures(1, &texture);

  // select texture
  glBindTexture( GL_TEXTURE_2D, texture );

  // mix texture with color for shading
  glTexEnvf( GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);

  // when texture area is small, bilinear filter the closest mipmap
  glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,
                   GL_LINEAR_MIPMAP_NEAREST );
  // when texture area is large, bilinear filter the first mipmap
  glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );

  // if wrap is true, the texture wraps over at the edges (repeat)
  //       ... false, the texture ends at the edges (clamp)
  glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S,
                   wrap ? GL_REPEAT : GL_CLAMP );
  glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T,
                   wrap ? GL_REPEAT : GL_CLAMP );

  // build texture mipmaps
  gluBuild2DMipmaps( GL_TEXTURE_2D, 3, width, height,
                    GL_RGB, GL_UNSIGNED_BYTE, data);

  return texture;
}

GLuint loadTexture(const char *filename, int wrap)
{
  GLubyte *image;
  GLuint texture;
  int width, height;

  PPMReadImage(filename, &image, &width, &height);
  texture = buildTexture(image, wrap, width, height);
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
  return buildTexture(data, 0, size, size);
}
