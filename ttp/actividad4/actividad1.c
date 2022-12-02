
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "ctimer.h"

void write_array_double_to_file_utf8(double *array, int size, char *filename)
{
  FILE *fp;
  fp = fopen(filename, "w");
  for (int i = 0; i < size; i++)
  {
    fprintf(fp, "%f", array[i]);
  }
  fclose(fp);
}

int main(int argc, char *argv[])
{

  int v, i;
  double suma;

  if (argc < 2)
  {
    printf("Usage: %s n_vectores [tam_max] \n", argv[0]);
    return 1;
  }
  int n_vectores;
  sscanf(argv[1], "%d", &n_vectores);
  int tam_max = 10000;
  if (argc > 2)
  {
    sscanf(argv[2], "%d", &tam_max);
  }

  double **M = (double **)malloc(n_vectores * sizeof(double *));
  int *tam = (int *)malloc(n_vectores * sizeof(int));
  for (v = 0; v < n_vectores; v++)
  {
    tam[v] = rand() % tam_max;
    M[v] = (double *)malloc(tam[v] * sizeof(double));
    for (i = 0; i < tam[v]; i++)
    {
      M[v][i] = (double)rand() / RAND_MAX * 2.0 * tam[v] - 1.0 * tam[v];
    }
  }

  double elapsed, ucpu, scpu;
  ctimer(&elapsed, &ucpu, &scpu);
  double *media = (double *)malloc(n_vectores * sizeof(double));
  double *desvt = (double *)malloc(n_vectores * sizeof(double));
  /*********************************************************/
  /* PRINCIPIO DEL CODIGO A INCLUIR                        */

  // Bucle a paralelizar

#pragma omp parallel
#pragma omp single
  {
    for (int v = 0; v < n_vectores; v++)
    {
      /* Cálculo de la media */
      double sum = 0.0;

#pragma omp task depend(out:media[v])
      {

        for (int i = 0; i < tam[v]; i++)
        {
          sum += M[v][i];
        }
        media[v] = sum / tam[v];
      }

      /* Cálculo de la desviación típica */

#pragma omp task depend(in:media[v])
      {
      sum = 0.0;
        for (int i = 0; i < tam[v]; i++)
        {
          sum += (M[v][i] - media[v]) * (M[v][i] - media[v]);
        }
        desvt[v] = sqrt(sum / tam[v]);
      }
    }
  }

  /* FIN DEL CODIGO A INCLUIR                              */
  /*********************************************************/

  ctimer(&elapsed, &ucpu, &scpu);
  printf("Tiempo = %f segundos\n", elapsed);

  // FILE *fp;
  // fp = fopen("media_desvt", "wb");
  // fwrite(media, sizeof(double), n_vectores, fp);
  // fwrite(desvt, sizeof(double), n_vectores, fp);
  // fclose(fp);

  // write_array_double_to_file_utf8(media, n_vectores, "media.txt");

  free(desvt);
  free(media);
  free(tam);
  for (i = 0; i < n_vectores; i++)
  {
    free(M[i]);
  }
  free(M);

  return 0;
}
