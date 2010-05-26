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

