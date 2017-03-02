/************************************************************************/
/*                                                                      */
/* ATA_General.cpp                                                      */
/*                                                                      */
/*  3-11-2015                                                           */
/*                                                                      */
/*   124 lines                                                          */
/*                                                                      */
/* Automated Test Assembly                                              */
/* general constants, types and routines                                */
/*                                                                      */
/************************************************************************/

#include <cstdlib>
#include <cmath>
#include "ATA_General.h"
using namespace std;

/************************************************************************/
/*** general functions **************************************************/
/************************************************************************/

/*** get mean and standard deviation ************************************/

void Mean_Sd(long double sum, long double sumsqr, long count,
             double & mean, double & sd)
{
  if (count > 1)
  {
    mean = sum / count;
    sd = sqrt((sumsqr - sqr(sum) / count) / (count - 1));
  }
  else
  {
    mean = sum;
    sd = -1;
  }
}

/*** root mean square error *********************************************/

double RMSE(double Val[], double Est[], long First, long Last)
{
  double x = 0.0;

  for (long i = First; i <= Last; i++) x += sqr(Val[i] - Est[i]);
  return sqrt(x / (Last - First + 1));
}

/************************************************************************/
/*** lower triangle matrices ********************************************/
/************************************************************************/

/************************************************************************/
/*** general psychometric functions *************************************/
/************************************************************************/

/*** item response function *********************************************/

double IRF(double Theta, double Disc, double Beta)
{
  double Result = exp(fmin(fmax(Disc * (Theta - Beta), -LOG_BIG), LOG_BIG));
  return Result / (1 + Result);
}

/*** logit function *****************************************************/

double Logit(double x)
{
  double Result = fmax(fmin(x, 1 - EPS), EPS);
  return log(Result / (1 - Result));
}

/************************************************************************/
/*** random numbers *****************************************************/
/************************************************************************/

/*** random number normal distribution **********************************/

double NormalRandom(double Mu, double Sigma)
{
  double x, y, r;

  do
  {
    x = rand();
    y = rand();
    x = 2 * (x / RAND_MAX) - 1.0;
    y = 2 * (y / RAND_MAX) - 1.0;
    r = sqr(x) + sqr(y);
  }
  while ((r >= 1) || (r < 1e-8));
  r = sqrt(-2.0 * log(r) / r);
  return x * r * Sigma + Mu;
}

/*** array in random order 0 .. Count-1 *********************************/

void RandomOrder(int * Array, int Count)
{
  if (Count > 0)
  {
    bool * b = new bool[Count];
    int    e, i, j, k;

    for (i = 0; i < Count; i++) b[i] = true;
    for (i = 0; i < Count; i++)
    {
      e = rand() % (Count - i);
      for (j = 0; (j < Count) && (e >= 0); j++)
        if (b[j])
        {
          if (e == 0) k = j;
          e--;
        }
      Array[k] = i;
      b[k] = false;
    }
    delete [] b;
  }
}

/************************************************************************/

