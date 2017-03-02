#ifndef ATA_GENERAL_H
#define ATA_GENERAL_H
/************************************************************************/
/*                                                                      */
/* ATA_General.h                                                        */
/*                                                                      */
/*  3-11-2015                                                           */
/*                                                                      */
/*    70 lines                                                          */
/*                                                                      */
/* Automated Test Assembly                                              */
/* general constants, types and routines                                */
/*                                                                      */
/************************************************************************/

/************************************************************************/
/*** constants etc ******************************************************/
/************************************************************************/

#define EPS          1e-3     /* criterion divergence for logit         */
#define CRIT         1e-5     /* criterion convergence                  */
#define LOG_BIG      10.0     /* criterion divergence                   */
#define DISC_MAX     10.0     /* maximum discrimination                 */
#define DISC_MIN      0.1     /* minimum discrimination                 */

/************************************************************************/
/*** general functions **************************************************/
/************************************************************************/

inline double sqr(double x)
{
  return x * x;
}

/*** get mean and standard deviation ************************************/

void Mean_Sd(long double sum, long double sumsqr, long count,
             double & mean, double & sd);

/*** root mean square error *********************************************/

double RMSE(double Val[], double Est[], long First, long Last);

/************************************************************************/
/*** general psychometric functions *************************************/
/************************************************************************/

/*** item response function *********************************************/

double IRF(double Theta, double Disc, double Beta);

/*** logit function *****************************************************/

double Logit(double x);

/************************************************************************/
/*** random numbers *****************************************************/
/************************************************************************/

/*** random number normal distribution **********************************/

double NormalRandom(double Mu, double Sigma);

/*** array in random order 0 .. Count-1 *********************************/

void RandomOrder(int * Array, int Count);

/************************************************************************/

#endif
