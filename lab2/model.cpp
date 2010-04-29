#include "model.h"
#include <cstdio>
#include <cstdlib>

model_t sampleModel;

/*
int mmReadCh(FILE **file)
{
  return fgetc(*file);
}

void loadModel(const char *fileName, model_t *model)
{
  printf("Opening: %s\n", fileName);

  FILE *file = fopen(fileName, "r");

  if(file != NULL)
  {
    // fscanf the number of vertices
    //fscanf(file, "%i", &model->nPolygons);
    int vertices;
    fscanf(file, "%i", &vertices);
    for(int i = 0; i < vertices; i++)
    {
      vertex_t v;
      fscanf(file, "%f %f", &v.x, &v.y);
      printf("%f %f\n", v.x, v.y);
    }
    // alloc the number polygons
    model->pList = (polygon_t *)malloc(model->nPolygons * sizeof(polygon_t));
  }
  else
  {
    fprintf(stderr, "Couldn't load file %s\n", fileName);
  }

}

void freeModel(model_t *model)
{
  if( model->pList)
  {
    free(model->pList);
    model->pList = NULL;
  }
}
*/
