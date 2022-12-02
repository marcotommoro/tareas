
/*******************************************************/
/*                                                     */
/*           Multiplicacion de matrices                */
/*                                                     */
/*******************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include "ctimer.h"

#define A(i,j) A[(i)+(j)*n]
#define B(i,j) B[(i)+(j)*n]
#define Cref(i,j) Cref[(i)+(j)*n]
#define C(i,j) C[(i)+(j)*n]

int main( int argc, char *argv[] ) {

  if( argc<2 ) {
    printf("Uso: %s n \n",argv[0]);
    return 1;
  }
  int n; 
  sscanf(argv[1],"%d",&n);
  int tam_max = 10000;
  if( argc>2 ) {
    sscanf(argv[2],"%d",&tam_max);
  }

  double *A = (double *) malloc (n*n*sizeof(double));
  double *B = (double *) malloc (n*n*sizeof(double));
  double *Cref = (double *) malloc (n*n*sizeof(double));
  for( int i=0; i<n; i++ ) {
    for( int j = 0; j<n; j++ ) {
      A[i+n*j] = (double) rand()/RAND_MAX * 2.0 - 1.0;
      B[i+n*j] = (double) rand()/RAND_MAX * 2.0 - 1.0;
      Cref[i+n*j] = 0.0;
    }
  }

  printf("Calculando la matriz referencia...\n");
  for( int i=0; i<n; i++ ) {
    for( int j=0; j<n; j++ ) {
      for( int k=0; k<n; k++ ) {
        Cref(i,j) += A(i,k) * B(k,j);
      }
    }
  }

  double *C = (double *) malloc (n*n*sizeof(double));
  memset( C, 0, n*n*sizeof(double) );
  printf("Calculando el producto...\n");
  double elapsed, ucpu, scpu;
  ctimer(&elapsed,&ucpu,&scpu);
  /****************************************************************/
  /* CODIGO DE MULTIPLICACION DE MATRICES A PARALELIZAR/SUSTITUIR */

  for( int i=0; i<n; i++ ) {
    for( int j=0; j<n; j++ ) {
      for( int k=0; k<n; k++ ) {
        C(i,j) += A(i,k) * B(k,j);
      }
    }
  }

  /* FIN DEL CODIGO A INCLUIR                                     */
  /****************************************************************/
  ctimer(&elapsed,&ucpu,&scpu);
  printf("Tiempo = %.2f segundos\n",elapsed);

  double error = 0.0;
  for( int i=0; i<n; i++ ) {
    for( int j=0; j<n; j++ ) {
      error += fabs(C(i,j) - Cref(i,j));
    }
  }
  printf("Error = %e\n",error);
  free(A);
  free(B);
  free(C);
  free(Cref);

  return 0;
}

