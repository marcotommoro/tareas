
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "ctimer.h"

int parallelized_calculation_standard_deviation(double *array, int size)
{
  double sum = 0;
#pragma omp parallel for reduction(+ \
                                   : sum)
  for (int i = 0; i < size; i++)
  {
    sum += array[i];
  }
  double mean = sum / size;
  double sum_of_squares = 0;

#pragma omp parallel for reduction(+ \
                                   : sum_of_squares)
  for (int i = 0; i < size; i++)
  {
    sum_of_squares += (array[i] - mean) * (array[i] - mean);
  }
  return sqrt(sum_of_squares / size);
}

double parallelized_calculate_average(double *array, int size)
{
  double sum = 0;
#pragma omp parallel for reduction(+ \
                                   : sum)
  for (int i = 0; i < size; i++)
  {
    sum += array[i];
  }
  return sum / size;
}

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

  // #pragma omp parallel for private(i)
#pragma omp parallel for
  for (int i = 0; i < n_vectores; i++)
  {
    // media[i] = parallelized_calculate_average(M[i], tam[i]);
    // desvt[i] = parallelized_calculation_standard_deviation(M[i], tam[i]);

    double sum = 0;
    // #pragma omp parallel for reduction(+:sum)
    for (int j = 0; j < tam[i]; j++)
    {
      sum += M[i][j];
    }
    double mean = sum / tam[i];
    double sum_of_squares = 0;

#pragma omp parallel for reduction(+: sum_of_squares)
    for (int j = 0; j < tam[i]; j++)
    {
      sum_of_squares += (M[i][j] - mean) * (M[i][j] - mean);
    }
    desvt[i] = sqrt(sum_of_squares / tam[i]);

    double sum_1 = 0;
    // #pragma omp parallel for reduction(+:sum)
    for (int j = 0; j < tam[i]; j++)
    {
      sum_1 += M[i][j];
    }
    media[i] = sum_1 / tam[i];
  }

  /* FIN DEL CODIGO A INCLUIR                              */
  /*********************************************************/
  ctimer(&elapsed, &ucpu, &scpu);
  printf("Tiempo = %f segundos\n", elapsed);

  // FILE *fp;
  // fp = fopen("media_desvt","w");
  // fwrite (media, sizeof(double), n_vectores, fp);
  // fwrite (desvt, sizeof(double), n_vectores, fp);

  // fclose(fp);

  write_array_double_to_file_utf8(media, n_vectores, "media.txt");
  // write_array_double_to_file_utf8(desvt, n_vectores, "desvt");

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
