#ifndef FFT_H
#define FFT_H

#include <assert.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>


#define q	3		/* for 2^3 points */
#define N	(1<<q)		/* N-point FFT, iFFT */

typedef float real;
typedef struct{real Re; real Im;} complex;

#ifndef PI
# define PI	3.14159265358979323846264338327950288
#endif


static void print_vector( const char *title, complex *x, int n );


void fft( complex *v, int n, complex *tmp );

void ifft( complex *v, int n, complex *tmp );

int main(void);

#endif
