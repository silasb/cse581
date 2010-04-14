/*
 *  ppm.c
 *  lab1
 *
 *  Created by Silas Baronda on 4/4/10.
 *  Copyright 2010 Silas Baronda. All rights reserved.
 *
 */

#if defined(_WIN32) || defined(__WIN32__)
#include "windows.h"
#endif

#include "ppm.h"

void PPMWriteImage(int frame, unsigned char *imgBuffer, const int nx, const int ny)
{
  FILE *stream;
  char fileName[50];

#if defined(_WIN32) || defined(__WIN32__)
  _snprintf(fileName, 50, "out-%d.ppm", frame);
#else
  snprintf(fileName, 50, "out-%d.ppm", frame);
#endif

  stream = fopen(fileName, "wb");
  
  fprintf(stream, "P6\n%d %d\n255\n", nx, ny);
  
  for( int iy = ny; iy > 0; iy-- )
  {
    for( int ix = 0; ix < nx; ix++ )
    {
      fwrite(imgBuffer + (iy * ny + ix) * 3, 1, 3, stream);
    }
  }
  
  fclose(stream);
}

