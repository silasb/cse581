#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

void display()
{
  glClear(GL_COLOR_BUFFER_BIT);
  
  glFlush();
}

