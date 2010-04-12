/*
 *  ppm.c
 *  lab1
 *
 *  Created by Silas Baronda on 4/4/10.
 *  Copyright 2010 Silas Baronda. All rights reserved.
 *
 */

#include "ppm.h"

void PPMWriteImage(int frame, unsigned char *imgBuffer, const int nx, const int ny)
{
  FILE *stream;
  char fileName[50];

  snprintf(fileName, 50, "out-%d.ppm", frame);

  stream = fopen(fileName, "wb");
  
  fprintf(stream, "P6\n%d %d\n255\n", nx, ny);
  
  for( int iy = 0; iy < ny; iy++ )
  {
    for( int ix = 0; ix < nx; ix++ )
    {
      fwrite(imgBuffer + (iy * ny + ix) * 3, 1, 3, stream);
    }
  }
  
  fclose(stream);
}

