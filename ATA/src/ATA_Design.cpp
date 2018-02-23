/************************************************************************/
/*                                                                      */
/* ATA_Design.cpp                                                       */
/*                                                                      */
/*  9-11-2015                                                           */
/*                                                                      */
/*   755 lines                                                          */
/*                                                                      */
/* Automated Test Assembly basic / calibration design                   */
/* ATA_Design class                                                     */
/*                                                                      */
/************************************************************************/

#include <cstdlib>
#include <cmath>
#include <algorithm>
#include "ATA_General.h"
#include "ATA_Base.h"
#include "ATA_Design.h"
using namespace std;

/************************************************************************/
/*** Fisher information matrix ******************************************/
/************************************************************************/

Info_Matrix::Info_Matrix(int ItmCount):
  Count (ItmCount),
  Determinant (0.0),
  Mean_StdErr (0.0),
  StdErr (NULL)
{
  Mat = new double [Count * (Count + 1) / 2];
  for (int i = 0; i < Count * (Count + 1) / 2; i++) Mat[i] = 0.0;
}

/************************************************************************/

Info_Matrix::~Info_Matrix()
{
  delete [] Mat;
  delete [] StdErr;
}

/************************************************************************/

double Info_Matrix::Cell(int row, int col)
{
  if (row > col)
    return Mat[row * (row + 1) / 2 + col];
  else
    return Mat[col * (col + 1) / 2 + row];
}

/************************************************************************/
/*   Choleski factor L of a symmetric positive definite matrix          */
/*   At output, the main diagonal contains the reciproces of            */
/*     the elements.                                                    */
/*                                                                      */
/*   J.H. Wilkinson & C. Reinsch, Linear Algebra. Berlin, Springer,     */
/*   1971, p.17-18.                                                     */
/*                                                                      */
/*   Out: Determinant = DetMan * 2^DetExp                               */
/************************************************************************/

bool Info_Matrix::Choleski(long double & DetMan, int & DetExp)
{
  const double sixteen = 16.0, rsixteen = 0.0625;
  long double  x;
  int          p = 0, q, r;

  DetMan = 1.0;
  DetExp = 0;
  for (int i = 0; i < Count; i ++)
  {
    q = p;
    r = 0;
    for (int j = 0; j < i; j++)
    {
      x = Mat[p];
      for (int k = q; k < p; k++)
      {
        x -= Mat[k] * Mat[r];
        r++;
      }
      if (i == j)
      {
        DetMan *= x;
        if (x < 0.0) return false;
        while (DetMan > 1.0)
        {
          DetMan *= rsixteen;
          DetExp += 4;
        }
        while (DetMan < rsixteen)
        {
          DetMan *= sixteen;
          DetExp -= 4;
        }
        Mat[p] = 1.0 / sqrt(x);
      }
      else
        Mat[p] = x * Mat[r];
      p++;
    }
  }
  return true;
}

/************************************************************************/

bool Info_Matrix::Choleski_Invert(long double & DetMan, int & DetExp)
{
  if (Count == 1)
  {
    DetMan = Mat[0];
    DetExp = 0;
    if (DetMan <= 0) return false;
    Mat[0] = 1.0 / DetMan;
    return true;
  }
  if (Choleski(DetMan, DetExp))
  {
    int    p = 0, r = 0, t = 0, u, v;
    double x, y;

    for (int i = 2; i <= Count; i++)
    {
      r += i;
      y = Mat[r];
      for (int j = 2; j <= i; j++)
      {
        p++;
        v = t;
        u = i - 2;
        x = 0.0;
        for (int k = r - 1; k >= p; k--)
        {
          x -= Mat[k] * Mat[v];
          v -= u;
          u--;
        }
        Mat[p] = x * y;
        t++;
      }
      p++;
    }
    p = -1;
    for (int i = 1; i <= Count; i++)
    {
      r = p + i;
      for (int j = 1; j <= i; j++)
      {
        v = r;
        p++;
        t = p;
        x = 0.0;
        for (int k = i; k <= Count; k++)
        {
          x += Mat[v] * Mat[t];
          v += k;
          t += k;
        }
        Mat[p] = x;
      }
    }
    return true;
  }
  return false;
}

/************************************************************************/

void Info_Matrix::Insert_FirstRow()
{
  Count++;

  double * p = new double[Count * (Count + 1) / 2];
  int      k = 0, l;

  for (int i = 0; i < Count * (Count + 1) / 2; i++) p[i] = 0.0;
  for (int i = 1; i < Count; i++)
  {
    l = k;
    k = i * (i + 1) / 2;
    for (int j = 0; j < i; j++) p[k + j] = Mat[l + j];
  }
  delete [] Mat;
  Mat = p;
}

/************************************************************************/

bool Info_Matrix::OffDiagonal_Information()
{
  int    i = 0, j, y;
  double x;

  TotObs = 0;
  for (int r = 0; r < Count; r++)
  {
    j = i;
    x = 0.0;
    for (int c = 0; c < r; c++)
    {
      x += Mat[i];
      i++;
    }
    y = Mat[i];
    TotObs += y;
    for (int c = r; c < Count; c++)
    {
      i += c;
      x += Mat[i];
    }
    if (x == 0) return false;
    x = -x / y;
    i = j;
    for (int c = 0; c < r; c++)
    {
      Mat[i] /= x;
      i++;
    }
    i++;
  }
  return true;
}

/************************************************************************/
/*   In  : A a LT kxk matrix;                                           */
/*   Out : PAP, with                                                    */
/*   P   : the kxk barycentric matrix I-[1/k]                           */
/*     [pa_ij] = -1/k Sum_m a_mj + a_ij                                 */
/*     [pap_ij]= -1/k Sum_n pa_in + pa_ij                               */
/*        = 1/k^2 Sum_mn a_mn -1/k(Sum_n a_in + Sum_m a_mj) + a_ij      */
/************************************************************************/

void Info_Matrix::Pre_Post_BaryCenter()
{
  double * ColSum = new double [Count];
  double   Sum = 0.0;
  int      k = 0;

  for (int i = 0; i < Count; i++)
  {
    ColSum[i] = 0.0;
    for (int j = 0; j <= i; j++)
    {
      ColSum[i] += Mat[k];
      if (i > j)
      {
        ColSum[j] += Mat[k];
        Sum += 2 * Mat[k];
      }
      else
        Sum += Mat[k];
      k++;
    }
  }
  for (int i = 0; i < Count; i++) ColSum[i] /= Count;
  Sum /= sqr(Count);
  k = 0;
  for (int i = 0; i < Count; i++)
    for (int j = 0; j <= i; j++)
    {
      Mat[k] += Sum - ColSum[i] - ColSum[j];
      k++;
    }
  delete [] ColSum;
}

/************************************************************************/

void Info_Matrix::Remove_FirstRow_Divide()
{
  double   AvgObs = TotObs;
  double * p = new double[Count * (Count - 1) / 2];
  int      k = 0, l;

  AvgObs /= Count;
  for (int i = 1; i < Count; i++)
  {
    l = k;
    k = i * (i + 1) / 2;
    for (int j = 0; j < i; j++) p[l + j] = Mat[k + j] / AvgObs;
  }
  delete [] Mat;
  Mat = p;
  Count--;
}

/************************************************************************/

void Info_Matrix::Remove_Row(int row)
{
  if ((row >= 0) && (row < Count))
  {
    double * p = new double [Count * (Count - 1) / 2];
    int      k = row * (row + 1) / 2, l;

    if (row > 0)
      for (int i = 0; i < k; i++)
        p[i] = Mat[i];
    l = k;
    k += (row + 1);
    for (int i = row + 1; i < Count; i++)
    {
      for (int j = 0; j < row; j++)
      {
        p[l] = Mat[k];
        k++;
        l++;
      }
      k++;
    }
    delete [] Mat;
    Mat = p;
  }
}

/************************************************************************/

void Info_Matrix::Set_Value(int row, int col, double value)
{
  if (row > col)
    Mat[row * (row + 1) / 2 + col] = value;
  else
    Mat[col * (col + 1) / 2 + row] = value;
}

/************************************************************************/

void Info_Matrix::Var_CoVar()
{
  long double D_Man;
  int         D_Exp;

  for (int i = 0; i < Count; i++)
    if (Cell(i, i) == 0.0) Remove_Row(i);
  if (!OffDiagonal_Information() || (TotObs == 0)) return;
  Remove_FirstRow_Divide();
  if (Choleski_Invert(D_Man, D_Exp))
  {
    StdErr = new double [Count];
    Mean_StdErr = 0.0;
    Insert_FirstRow();
    Pre_Post_BaryCenter();
    Determinant = exp((log(D_Man) + D_Exp * log(2)) / Count);
    for (int i = 0; i < Count; i++)
    {
      StdErr[i] = sqrt(Cell(i, i));
      Mean_StdErr += StdErr[i];
    }
    Mean_StdErr /= Count;
  }
}

/************************************************************************/
/*** ATA_Design class for optimal design models *************************/
/************************************************************************/

void ATA_Design::Exchange_Items()
{
  int min, max;

  Min_Max_Overlap(min, max);
  while (min < max - 1)
  {
    int  l1 = 0, l2 = 1, y;
    bool found = false;

    while (!found && Find_NextTestPair(min, l1, l2))
    {
      y = max;
      while (!found && (min < y - 1))
      {
        int h1 = -1, h2 = 0;

        while (!found && (Find_NextTestPair(y, h1, h2)))
          found = Find_Swap(l1, l2, h1, h2);
        if (!found) y--;
      }
    }
    if (found)
      Min_Max_Overlap(min, max);
    else
      min++;
  }
}

/************************************************************************/

bool ATA_Design::Find_NextTestPair(int ovl, int & tst1, int & tst2)
{
  do
  {
    tst1++;
    if (tst1 == tst2)
    {
      tst2++;
      tst1 = 0;
    }
  }
  while ((tst2 < TstCount) && (CurOvl[OvlIndex(tst1, tst2)] != ovl));
  return (tst2 < TstCount);
}

/************************************************************************/

bool ATA_Design::Find_Swap(int l1, int l2, int h1, int h2)
{
  if (l1 == h1) return Swap3(l1, l2, h2);
  if (l1 == h2) return Swap3(l1, l2, h1);
  if (l2 == h1) return Swap3(l2, l1, h2);
  if (l2 == h2) return Swap3(l2, l1, h1);
  return Swap4(l1, l2, h1, h2);
}

/************************************************************************/

void ATA_Design::Min_Max_Overlap(int & min, int & max)
{
  min = CurOvl[0];
  max = CurOvl[0];

  for (int i = 1; i < OvlCount; i++)
  {
    if (CurOvl[i] < min)
      min = CurOvl[i];
    else if (CurOvl[i] > max)
      max = CurOvl[i];
  }
}

/************************************************************************/

double ATA_Design::Sqrt_Ovl(int tst1, int tst2)
{
  if (tst1 == tst2) return 0.0;
  return sqrt(CurOvl[OvlIndex(tst1, tst2)]);
}

/************************************************************************/

bool ATA_Design::Swap3(int t, int tl, int th)
{
  double dev = Tst[tl].Ctg_Deviation() + Tst[th].Ctg_Deviation();
  double ovl = 0.0;
  int    p = -1, q = -1;

  for (int i = 0; i < TstCount; i++)
  {
    ovl += Sqrt_Ovl(i, t);
    if (i != t)
    {
      ovl += Sqrt_Ovl(i, tl);
      if (i != tl) ovl += Sqrt_Ovl(i, th);
    }
  }
  for (int i = 0; i < ItmCount; i++)
  {
    if (Tst[t].Itm[i] && !Tst[tl].Itm[i] && Tst[th].Itm[i] &&
      !Itm[i].In_Test[tl] && !Itm[i].Not_Test[th])
    {
      for (int j = 0; j < ItmCount; j++)
      {
        if (!Tst[t].Itm[j] && Tst[tl].Itm[j] && !Tst[th].Itm[j] &&
          !Itm[j].In_Test[th] && !Itm[j].Not_Test[tl] &&
          Try_Swap3(i, j, t, tl, th, ovl, dev))
        {
          p = i;
          q = j;
        }
      }
    }
  }
  if (p >= 0)
  {
    Swap_Items(p, q, th, tl);
    return true;
  }
  return false;
}

/************************************************************************/

bool ATA_Design::Swap4(int tl1, int tl2, int th1, int th2)
{
  double dev = Tst[tl2].Ctg_Deviation() + Tst[th1].Ctg_Deviation();
  double ovl = 0.0;
  int    p = -1, q = -1, pt = -1, qt = -1;

  for (int i = 0; i < TstCount; i++)
  {
    ovl += Sqrt_Ovl(i, tl1);
    if (i != tl1)
    {
      ovl += Sqrt_Ovl(i, tl2);
      if (i != tl2)
      {
        ovl += Sqrt_Ovl(i, th1);
        if (i != th1) ovl += Sqrt_Ovl(i, th2);
      }
    }
  }
  for (int i = 0; i < ItmCount; i++)
  {
    if (Tst[tl1].Itm[i] && !Tst[tl2].Itm[i] &&
      Tst[th1].Itm[i] && Tst[th2].Itm[i] &&
      !Itm[i].Not_Test[tl2] && !Itm[i].In_Test[th1] && !Itm[i].In_Test[th2])
    {
      for (int j = 0; j < ItmCount; j++)
      {
        if (!Tst[tl1].Itm[j] && Tst[tl2].Itm[j] &&
          !Tst[th1].Itm[j] && !Tst[th2].Itm[j] &&
          !Itm[j].In_Test[tl2] && !(Itm[j].Not_Test[th1] && Itm[j].Not_Test[th2]))
        {
          if (Try_Swap4(i, j, tl1, tl2, th1, th2, ovl, dev))
          {
            p = i; pt = th1;
            q = j; qt = tl2;
          }
          else if (Try_Swap4(i, j, tl1, tl2, th2, th1, ovl, dev))
          {
            p = i; pt = th2;
            q = j; qt = tl2;
          }
      }
    }
  }
  else if  (!Tst[tl1].Itm[i] && Tst[tl2].Itm[i] &&
      Tst[th1].Itm[i] && Tst[th2].Itm[i] &&
      !Itm[i].Not_Test[tl1] && !Itm[i].In_Test[th1] && !Itm[i].In_Test[th2])
    {
      for (int j = 0; j < ItmCount; j++)
      {
        if (Tst[tl1].Itm[j] && !Tst[tl2].Itm[j] &&
          !Tst[th1].Itm[j] && !Tst[th2].Itm[j] &&
          !Itm[j].In_Test[tl1] && !(Itm[j].Not_Test[th1] && Itm[j].Not_Test[th2]))
        {
          if (Try_Swap4(i, j, tl2, tl1, th1, th2, ovl, dev))
          {
            p = i; pt = th1;
            q = j; qt = tl1;
          }
          else if (Try_Swap4(i, j, tl2, tl1, th2, th1, ovl, dev))
          {
            p = i; pt = th2;
            q = j; qt = tl1;
          }
        }
      }
    }
  }
  if (p >= 0)
  {
    Swap_Items(p, q, pt, qt);
    return true;
  }
  return false;
}

/************************************************************************/

void ATA_Design::Swap_Items(int itm1, int itm2, int tst1, int tst2)
{
  Remove_Item(itm1, tst1);
  Remove_Item(itm2, tst2);
  Add_Item(itm1, tst2);
  Add_Item(itm2, tst1);
}

/************************************************************************/

bool ATA_Design::Try_Swap3(int itm1, int itm2, int t, int tl, int th,
    double & ovl, double dev)
{
  double n_o = 0.0;
  bool   Result = false;

  Swap_Items(itm1, itm2, th, tl);
  if ((Tst[tl].CurLen <= Tst[tl].MaxLen) &&
    (Tst[th].CurLen <= Tst[th].MaxLen))
  {
    for (int i = 0; i < TstCount; i++)
    {
      n_o += Sqrt_Ovl(i, t);
      if (i != t)
      {
        n_o += Sqrt_Ovl(i, tl);
        if (i != tl) n_o += Sqrt_Ovl(i, th);
      }
    }
    if (n_o > ovl + CRIT)
    {
      double n_d = Tst[tl].Ctg_Deviation() + Tst[th].Ctg_Deviation();

      if (n_d <= dev)
      {
        ovl = n_o;
        Result = true;
      }
    }
  }
  Swap_Items(itm1, itm2, tl, th);
  return Result;
}

/************************************************************************/

bool ATA_Design::Try_Swap4(int itm1, int itm2, int tl1, int tl2, int th1, int th2,
    double & ovl, double dev)
{
  double n_o = 0.0;
  bool   Result = false;

  Swap_Items(itm1, itm2, th1, tl2);
  if ((Tst[tl2].CurLen <= Tst[tl2].MaxLen) &&
    (Tst[th1].CurLen <= Tst[th1].MaxLen))
  {
    for (int i = 0; i < TstCount; i++)
    {
      n_o += Sqrt_Ovl(i, tl1);
      if (i != tl1)
      {
        n_o += Sqrt_Ovl(i, tl2);
        if (i != tl2)
        {
          n_o += Sqrt_Ovl(i, th1);
          if (i != th1) n_o += Sqrt_Ovl(i, th2);
        }
      }
    }
    if (n_o > ovl + CRIT)
    {
      double n_d = Tst[tl2].Ctg_Deviation() + Tst[th1].Ctg_Deviation();

      if (n_d <= dev)
      {
        ovl = n_o;
        Result = true;
      }
    }
  }
  Swap_Items(itm1, itm2, tl2, th1);
  return Result;
}

/************************************************************************/

double ATA_Design::Det_InfoMatrix()
{
  Info_Matrix Info(ItmCount);

  for (int t = 0; t < TstCount; t++)
    for (int i = 0; i < ItmCount; i++)
      if (Tst[t].Itm[i])
      {
        Info.Set_Value(i, i, Info.Cell(i, i) + 1.0);
        for (int j = 0; j < i; j++)
          if (Tst[t].Itm[j])
            Info.Set_Value(i, j, Info.Cell(i, j) + 1.0);
      }
  Info.Var_CoVar();
}

/************************************************************************/

bool ATA_Design::Greedy()
{
  int i, t;

  while (true)
  {
    t = Find_Test();
    if (t < 0) break;
    i = Find_Item(t);
    if (i < 0) break;
    if (Itm[i].Clu_ID < 0)
      Add_Item(i, t);
    else
      for (int e = 0; e < ItmCount; e++)
        if (Itm[e].Clu_ID == Itm[i].Clu_ID)
          Add_Item(e, t);
  }
  Exchange_Items();
}

/************************************************************************/
/*** ATA_Irt_Item class for 1PL/2PL dichotomous item ********************/
/************************************************************************/

ATA_Irt_Item::ATA_Irt_Item():
  aPar (0.0),
  bPar (0.0)
{}

/************************************************************************/

double ATA_Irt_Item::Expected_Score(double aTheta)
{
  return IRF(aTheta, aPar, bPar);
}

/*** Fisher information for item ***************************************/

double ATA_Irt_Item::Fisher_Information(double aTheta)
{
  if ((aPar < DISC_MIN) || (aPar > DISC_MAX)) return 0.0;

  double Result = exp(fmin(fmax(aPar * (aTheta - bPar) / aPar, -LOG_BIG), LOG_BIG));

  return sqr(aPar) * Result / sqr(1 + Result);
}

/*** Fisher information per item for cluster ***************************/

double ATA_Irt::Fisher_Information_Clu(long CluID, double aTheta)
{
  int    n = 0;
  double Result = 0.0;

  for (int i = 0; i < ItmCount; i++)
    if (Itm[i].Clu_ID == CluID)
    {
      Result += Irt_Itm[i].Fisher_Information(aTheta);
      n++;
    }
  if (n > 0) return Result / n;
  return 0.0;
}

/*** Fisher information for item ***************************************/

double ATA_Irt::Fisher_Information_Itm(int itm, double aTheta)
{
  if ((itm < 0) || (itm >= ItmCount)) return 0.0;
  return Irt_Itm[itm].Fisher_Information(aTheta);
}

/*** Fisher information for test ***************************************/

double ATA_Irt::Fisher_Information_Tst(int tst, double aTheta)
{
  if ((tst < 0) || (tst >= TstCount)) return 0.0;

  double Result = 0.0;

  for (int i = 0; i < ItmCount; i++)
    if (Tst[tst].Itm[i])
      Result += Irt_Itm[i].Fisher_Information(aTheta);
  return Result;
}

/************************************************************************/

