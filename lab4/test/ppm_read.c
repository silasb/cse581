
#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

#include <stdio.h>
#include <stdlib.h>

void initGL();
void display();
void PPMReadImage(const char *, unsigned char *, int *, int *);

int main(int argc, char** argv)
{
  glutInit(&argc, argv);
  glutInitWindowSize(512, 512); // initially 512 x 512
  glutCreateWindow("lab4");
  glutInitDisplayMode(GLUT_DEPTH | GLUT_RGB | GLUT_DOUBLE);

  // callbacks
  glutDisplayFunc(display);

  initGL();

  glutMainLoop();

  return 0;
}

/* opengl defaults */
void
initGL() {
  glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
}

void
display()
{
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  GLubyte *image;
  const char *filename = "Untitled.ppm";
  int w, h;

  PPMReadImage(filename, &image, &w, &h);
  glDrawPixels(w, h, GL_RGB, GL_UNSIGNED_BYTE, image);
  free(image);

  glFlush();
}
