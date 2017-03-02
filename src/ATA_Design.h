#ifndef ATA_DESIGN_H
#define ATA_DESIGN_H
/************************************************************************/
/*                                                                      */
/* ATA_Design.h                                                         */
/*                                                                      */
/*  9-11-2015                                                           */
/*                                                                      */
/*   107 lines                                                          */
/*                                                                      */
/* Automated Test Assembly basic / calibration design                   */
/* ATA_Design class                                                     */
/*                                                                      */
/************************************************************************/

#include "ATA_Base.h"

/************************************************************************/
/*** Fisher information matrix ******************************************/
/************************************************************************/

class Info_Matrix
{
protected:
  int          Count;         /* row/column count                       */
  double *     Mat;           /* matrix elements                        */
  int          TotObs;        /* total number of observations           */
  bool         Choleski(long double & DetMan, int & DetExp);
  bool         Choleski_Invert(long double & DetMan, int & DetExp);
  void         Insert_FirstRow();
  bool         OffDiagonal_Information();
  void         Pre_Post_BaryCenter();
  void         Remove_FirstRow_Divide();
public:
  double       Determinant;
  double       Mean_StdErr;
  double *     StdErr;
  Info_Matrix(int ItmCount);
  ~Info_Matrix();
  double       Cell(int row, int col);
  void         Remove_Row(int row);
  void         Set_Value(int row, int col, double value);
  void         Var_CoVar();
};

/************************************************************************/
/*** ATA_Design class for optimal design models *************************/
/************************************************************************/

class ATA_Design: public ATA_Base
{
protected:
  void         Exchange_Items();
  bool         Find_NextTestPair(int ovl, int & tst1, int & tst2);
  bool         Find_Swap(int l1, int l2, int h1, int h2);
  void         Min_Max_Overlap(int & min, int & max);
  double       Sqrt_Ovl(int tst1, int tst2);
  bool         Swap3(int t, int tl, int th);
  bool         Swap4(int tl1, int tl2, int th1, int th2);
  void         Swap_Items(int itm1, int itm2, int tst1, int tst2);
  bool         Try_Swap3(int itm1, int itm2, int t, int tl, int th,
                 double & ovl, double dev);
  bool         Try_Swap4(int itm1, int itm2, int tl1, int tl2, int th1, int th2,
                 double & ovl, double dev);
public:
  double       Det_InfoMatrix();
  bool         Greedy();
};

/************************************************************************/
/*** ATA_Irt_Item class for 1PL/2PL dichotomous item ********************/
/************************************************************************/

class ATA_Irt_Item
{
public:
  double       aPar;          /* discrimination                         */
  double       bPar;          /* difficulty                             */
  ATA_Irt_Item();
  double       Expected_Score(double aTheta);
  double       Fisher_Information(double aTheta);
};

/************************************************************************/
/*** ATA_Irt class for IRT models ***************************************/
/************************************************************************/

class ATA_Irt: public ATA_Base
{
protected:
  ATA_Irt_Item * Irt_Itm;
  double       Fisher_Information_Clu(long CluID, double aTheta);
  double       Fisher_Information_Itm(int itm, double aTheta);
  double       Fisher_Information_Tst(int tst, double aTheta);
public:
  ATA_Irt();
  ~ATA_Irt();
  bool         Greedy();
  /* property setters ***************************************************/
  bool         SetItmCount(int ItmCount);
  bool         SetItmDifficulty(int itm, double Difficulty);
  bool         SetItmDiscrimination(int itm, double Discrmination);
};

/************************************************************************/

#endif
