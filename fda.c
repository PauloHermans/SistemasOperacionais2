#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <error.h>
#include <errno.h>
#include "fda_lib/pgmfiles.h"
#include "fda_lib/diff2d.h"

void main (int argc, char **argv) {
  char   row[80];
  float  **matrix;
  int i, j;
  FILE   *inimage, *outimage;
  long   imax;
  float  lambda;
  int result;
  eightBitPGMImage *PGMImage;
  
  /* ---- read image name  ---- */
  
  PGMImage = (eightBitPGMImage *) malloc(sizeof(eightBitPGMImage));

  if (argc < 4) error_at_line(EXIT_FAILURE, EINVAL, __FILE__, __LINE__, "");

  strcpy(PGMImage->fileName, argv[1]);

  result = read8bitPGM(PGMImage);

  if(result < 0) 
    {
      printPGMFileError(result);
      exit(result);
    }

  /* ---- allocate storage for matrix ---- */
  
  matrix = (float **) malloc (PGMImage->x * sizeof(float *));
  if (matrix == NULL)
    { 
      printf("not enough storage available\n");
      exit(1);
    } 
  for (i=0; i<PGMImage->x; i++)
    {
      matrix[i] = (float *) malloc (PGMImage->y * sizeof(float));
      if (matrix[i] == NULL)
        { 
	  printf("not enough storage available\n");
	  exit(1);
        }
    }
  
  /* ---- read image data into matrix ---- */
  
 for (i=0; i<PGMImage->x; i++)
    for (j=0; j<PGMImage->y; j++)
      matrix[i][j] = (float) *(PGMImage->imageData + (i*PGMImage->y) + j); 
  
  /* ---- process image ---- */
 
  // NOTE: O lambda é irrelevante para o uso de memória
  lambda = 0.25f;
  imax   = strtol(argv[3], NULL, 10);

  for (i=1; i<=imax; i++)
    {
      printf("iteration number: %3ld \n", i);
      diff2d (0.5, lambda, PGMImage->x, PGMImage->y, matrix); 
    }
  
  /* copy the Result Image to PGM Image/File structure */

  for (i=0; i<PGMImage->x; i++)
    for (j=0; j<PGMImage->y; j++)
      *(PGMImage->imageData + i*PGMImage->y + j) = (char) matrix[i][j];

  /* ---- write image ---- */

  strcpy(PGMImage->fileName, argv[2]);
  write8bitPGM(PGMImage);

  /* ---- disallocate storage ---- */
  
  for (i=0; i<PGMImage->x; i++)
    free(matrix[i]);
  free(matrix);

  free(PGMImage->imageData);
  free(PGMImage);

#ifdef WINDOWS
#else
  system("ps -o min_flt,maj_flt,cmd");
#endif
}


