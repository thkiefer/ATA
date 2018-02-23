/************************************************************************/
/*                                                                      */
/* mainRCPP.cpp                                                         */
/*                                                                      */
/*  TK: 2016-04-29                                                      */
/*  adaption of main.cpp by AV from 2015-10-02                          */
/*  in order to run from R                                              */
/*                                                                      */
/* Automated Test Assembly                                              */
/* main                                                                 */
/*                                                                      */
/************************************************************************/

/* Rcpp Header **********************************************************/
#include <Rcpp.h>


#ifndef BEGIN_RCPP
#define BEGIN_RCPP
#endif

#ifndef END_RCPP
#define END_RCPP
#endif

using namespace Rcpp;

/************************************************************************/
#include "ATA_General.h"
#include "ATA_Base.h"
#include "ATA_Design.h"
using namespace std;

/* ata_random ***********************************************************/

// declarations
extern "C" {
  SEXP ata_random(SEXP icnt_, SEXP tcnt_, SEXP len_) ;
}

// definition
SEXP ata_random(SEXP icnt_, SEXP tcnt_, SEXP len_){
  BEGIN_RCPP
  
  //* Initialization ********************************      
  int icnt = as<int>(icnt_), tcnt = as<int>(tcnt_), len = as<int>(len_);
  //Rcpp::Rcout << "tcnt=" <<  tcnt << ", len=" << len << ", icnt=" << icnt;
  
  double x = tcnt * len;
  x /= icnt;
  
  ATA_Design bas;
  Rcpp::IntegerMatrix outDesign_(icnt, tcnt);
  
  //* Algorithm *************************************      
  bas.SetItmCount(icnt);
  for (int i = 0; i < icnt; i++){
    bas.SetItmID(i, i+1);
    bas.SetItmUse(i, ceil(x));
  }
  bas.SetTstCount(tcnt);
  for (int i = 0; i < tcnt; i++) bas.SetTstMaxLen(i, len);
  bas.Greedy();
  
  //* Output ****************************************
  for(int i = 0; i < icnt; i++){
    for(int t = 0; t < tcnt; t++)
      if(bas.Item_in_Test(i, t))
        outDesign_(i, t) = 1;
      else
        outDesign_(i, t) = 0;
  }
  
  return outDesign_ ;    
  END_RCPP
}



