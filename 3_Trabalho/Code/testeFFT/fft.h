#pragma once

#include <stdbool.h>
#include <stddef.h>


#ifdef __cplusplus
extern "C" {
#endif


/* 
 * Computes the discrete Fourier transform (DFT) of the given complex vector, storing the result back into the vector.
 * The vector can have any length. This is a wrapper function. Returns true if successful, false otherwise (out of memory).
 */
bool Fft_transform(double real[], double imag[], size_t n);


/* 
 * Computes the inverse discrete Fourier transform (IDFT) of the given complex vector, storing the result back into the vector.
 * The vector can have any length. This is a wrapper function. This transform does not perform scaling, so the inverse is not a true inverse.
 * Returns true if successful, false otherwise (out of memory).
 */
bool Fft_inverseTransform(double real[], double imag[], size_t n);


/* 
 * Computes the discrete Fourier transform (DFT) of the given complex vector, storing the result back into the vector.
 * The vector's length must be a power of 2. Uses the Cooley-Tukey decimation-in-time radix-2 algorithm.
 * Returns true if successful, false otherwise (n is not a power of 2, or out of memory).
 */
bool Fft_transformRadix2(double real[], double imag[], size_t n);


/* 
 * Computes the discrete Fourier transform (DFT) of the given complex vector, storing the result back into the vector.
 * The vector can have any length. This requires the convolution function, which in turn requires the radix-2 FFT function.
 * Uses Bluestein's chirp z-transform algorithm. Returns true if successful, false otherwise (out of memory).
 */
bool Fft_transformBluestein(double real[], double imag[], size_t n);


/* 
 * Computes the circular convolution of the given real vectors. Each vector's length must be the same.
 * Returns true if successful, false otherwise (out of memory).
 */
bool Fft_convolveReal(const double x[], const double y[], double out[], size_t n);


/* 
 * Computes the circular convolution of the given complex vectors. Each vector's length must be the same.
 * Returns true if successful, false otherwise (out of memory).
 */
bool Fft_convolveComplex(const double xreal[], const double ximag[], const double yreal[], const double yimag[], double outreal[], double outimag[], size_t n);


#ifdef __cplusplus
}
#endif