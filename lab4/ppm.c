/*
 *  ppm.c
 *
 *  Created by Silas Baronda on 4/4/10.
 *  Copyright 2010 Silas Baronda. All rights reserved.
 *
 */

#if defined(_WIN32) || defined(__WIN32__)
#include "windows.h"
#endif

#include <stdio.h>
#include <stdlib.h>

#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

void PPMWriteImage(unsigned char *imgBuffer, const int nx, const int ny)
{
  FILE *stream;
  int iy, ix;

  stream = fopen("frame.ppm", "wb");
  
  fprintf(stream, "P6\n%d %d\n255\n", nx, ny);
  
  for(iy = ny; iy > 0; iy-- ) {
    for(ix = 0; ix < nx; ix++ ) {
      fwrite(imgBuffer + (iy * ny + ix) * 3, 1, 3, stream);
    }
  }
  
  fclose(stream);
}

void
PPMReadImage(const char *filename, GLubyte **imgBuffer, int *width, int *height)
{
  FILE *stream;
  GLubyte *ptr;
  int i;

  // struct of PPM format
  int magic_number;
  char comment[255];
  int w, h;
  int max_color;

  stream = fopen(filename, "rb");

  fscanf(stream, "P%d\n", &magic_number);
  printf("magic number: %d\n", magic_number);

  fgets(comment, 255, stream);
  printf("comment: %s", comment);

  fscanf(stream, "%d %d\n", &w, &h);
  printf("dim: %d %d\n", w, h);

  fscanf(stream, "%d", &max_color);
  printf("max_color: %d\n", max_color);

  *width = w;
  *height = h;

  *imgBuffer = (GLubyte *)malloc(w * h * 3);
  // *imgBuffer memory location is assigned to ptr
  ptr = *imgBuffer + (h-1) * (w * 3);
  for(i = h; i > 0; i--){
    // when we are reading into ptr we are actually writing directly to the imgBuffer
    fread((void *)ptr, 1, (w * 3), stream);
    ptr -= (w * 3);
  }
  fclose(stream);
}
