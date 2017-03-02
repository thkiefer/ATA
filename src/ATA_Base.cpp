/************************************************************************/
/*                                                                      */
/* ATA_Base.cpp                                                         */
/*                                                                      */
/*  9-11-2015                                                           */
/*                                                                      */
/*   710 lines                                                          */
/*                                                                      */
/* Automated Test Assembly basic / calibration design                   */
/* ATA_Base class                                                       */
/*                                                                      */
/************************************************************************/

#include <cstdlib>
#include <cmath>
#include <algorithm>
#include "ATA_General.h"
#include "ATA_Base.h"
using namespace std;

/************************************************************************/
/*** ATA_Item class for basic item **************************************/
/************************************************************************/

ATA_Item::ATA_Item():
  ID (-1),
  Ctg (NULL),
  Clu_ID (-1),
  ExcCount (0),
  Exc_ID (NULL),
  MaxUse (1),
  CurUse (0),
  In_Test (NULL),
  Not_Test (NULL)
{}

/************************************************************************/

ATA_Item::~ATA_Item()
{
  delete [] Ctg;
  delete [] Exc_ID;
  delete [] In_Test;
  delete [] Not_Test;
}

/************************************************************************/

bool ATA_Item::SetCtgCount(int CtgCount)
{
  if (CtgCount < 0) return false;
  Ctg = new bool[CtgCount];
  for (int i = 0; i < CtgCount; i++) Ctg[i] = false;
  return true;
}

/************************************************************************/

bool ATA_Item::SetInTest(int tst)
{
  In_Test[tst] = true;
  return true;
}

/************************************************************************/

bool ATA_Item::SetNotInTest(int tst)
{
  Not_Test[tst] = true;
  return true;
}

/************************************************************************/

bool ATA_Item::SetTstCount(int TstCount)
{
  if (TstCount < 0) return false;
  In_Test = new bool[TstCount];
  Not_Test = new bool[TstCount];
  for (int i = 0; i < TstCount; i++)
  {
    In_Test[i] = false;
    Not_Test[i] = false;
  }
  return true;
}

/************************************************************************/
/*** ATA_Test class for basic test specifications ***********************/
/************************************************************************/

ATA_Test::ATA_Test():
  ItmCount (0),
  CtgCount (0),
  Ctg_MinI (NULL),
  Ctg_MaxI (NULL),
  Ctg_MinF (NULL),
  Ctg_MaxF (NULL),
  MaxLen (0),
  Ctg_Cur (NULL),
  CurLen (0),
  Itm (NULL)
{}

/************************************************************************/

ATA_Test::~ATA_Test()
{
  if (CtgCount > 0)
  {
    delete [] Ctg_Cur;
    delete [] Ctg_MinI;
    delete [] Ctg_MaxI;
    delete [] Ctg_MinF;
    delete [] Ctg_MaxF;
  }
  if (ItmCount > 0) delete [] Itm;
}

/************************************************************************/

bool ATA_Test::Add_Item(int itm, ATA_Item & aItm)
{
  if ((itm < 0) || (itm >= ItmCount) || Itm[itm]) return false;
  Itm[itm] = true;
  CurLen++;
  for (int i = 0; i < CtgCount; i++) if (aItm.Ctg[i]) Ctg_Cur[i]++;
  aItm.CurUse++;
  return true;
}

/************************************************************************/

double ATA_Test::Ctg_Deviation()
{
  double c, Result = 0;

  for (int i = 0; i < CtgCount; i++)
  {
    c = fmax(Ctg_MinI[i], Ctg_MinF[i] * CurLen);
    if (Ctg_Cur[i] <= c)
      Result += (c - Ctg_Cur[i]);
    else
    {
      if (Ctg_MaxF[i] >= 0)
        c = Ctg_MaxF[i] * CurLen;
      else
        c = 1e10;
      if (Ctg_MaxI[i] >= 0)
        c = fmin(c, Ctg_MaxI[i]);
      if (Ctg_Cur[i] > c) Result += (Ctg_Cur[i] - c);
    }
  }
  return Result;
}

/*** deviation from desired content if cluster is added *****************/

double ATA_Test::Ctg_Deviation_AddClu(long CluID, ATA_Item * aItm)
{
  int    n, x, y;
  double cn, co, cx, cy, o_dev = 0, n_dev = 0;
  int *  c;

  c = new int[CtgCount];
  for (int i = 0; i < CtgCount; i++) c[i] = 0;
  n = 0;
  for (int i = 0; i < ItmCount; i++)
    if (aItm[i].Clu_ID == CluID)
    {
      for (int j = 0; j < CtgCount; j++)
        if (aItm[i].Ctg[j]) c[j]++;
      n++;
    }
  for (int i = 0; i < CtgCount; i++)
  {
    x = Ctg_Cur[i];
    y = x + c[i];
    cn = fmax(Ctg_MinI[i], Ctg_MinF[i] * (CurLen + n));
    co = fmax(Ctg_MinI[i], Ctg_MinF[i] * CurLen);
    if (Ctg_MaxF[i] >= 0)
    {
      cx = Ctg_MaxF[i] * CurLen;
      cy = Ctg_MaxF[i] * (CurLen + n);
    }
    else
    {
      cx = 1e10;
      cy = 1e10;
    }
    if (Ctg_MaxI[i] >= 0)
    {
      cx = fmin(cx, Ctg_MaxI[i]);
      cy = fmin(cy, Ctg_MaxI[i]);
    }
    if (x < co)
      o_dev += (co - x);
    else if (x > cx)
      o_dev += (x - cx);
    if (y < cn)
      n_dev += (cn - y);
    else if (y > cy)
      n_dev += (y - cy);
  }
  delete [] c;
  return o_dev + (n_dev - o_dev) / n;
}

/*** deviation from desired content if item is added ********************/

double ATA_Test::Ctg_Deviation_AddItm(ATA_Item & aItem)
{
  double x, c, Result = 0;

  for (int i = 0; i < CtgCount; i++)
  {
    x = Ctg_Cur[i];
    if (aItem.Ctg[i]) x++;
    c = fmax(Ctg_MinI[i], Ctg_MinF[i] * (CurLen + 1));
    if (x <= c)
      Result += (c - x);
    else
    {
      if (Ctg_MaxF[i] >= 0)
        c = Ctg_MaxF[i] * (CurLen + 1);
      else
        c = 1e10;
      if (Ctg_MaxI[i] >= 0)
        c = fmin(c, Ctg_MaxI[i]);
      if (x > c) Result += (x - c);
    }
  }
  return Result;
}

/************************************************************************/

double ATA_Test::Missing()
{
  if (MaxLen <= 0) return 0.0;

  double x = CurLen;

  return 1 - x / MaxLen;
}

/************************************************************************/

bool ATA_Test::Remove_Item(int itm, ATA_Item & aItm)
{
  if ((itm < 0) || (itm >= ItmCount) || !Itm[itm]) return false;
  Itm[itm] = false;
  CurLen--;
  for (int i = 0; i < CtgCount; i++) if (aItm.Ctg[i]) Ctg_Cur[i]--;
  aItm.CurUse--;
  return true;
}

/************************************************************************/

bool ATA_Test::SetCtgCount(int CtgCount)
{
  if (CtgCount < 0) return false;
  if (this->CtgCount > 0)
  {
    delete [] Ctg_Cur;
    delete [] Ctg_MinI;
    delete [] Ctg_MaxI;
    delete [] Ctg_MinF;
    delete [] Ctg_MaxF;
  }
  Ctg_Cur = new int[CtgCount];
  Ctg_MinI = new int[CtgCount];
  Ctg_MaxI = new int[CtgCount];
  Ctg_MinF = new double[CtgCount];
  Ctg_MaxF = new double[CtgCount];
  for (int i = 0; i < CtgCount; i++)
  {
    Ctg_Cur[i] = 0;
    Ctg_MinI[i] = -1;
    Ctg_MaxI[i] = -1;
    Ctg_MinF[i] = -1.0;
    Ctg_MaxF[i] = -1.0;
  }
  this->CtgCount = CtgCount;
}

/************************************************************************/

bool ATA_Test::SetItmCount(int ItmCount)
{
  if (ItmCount < 0) return false;
  if (this->ItmCount > 0) delete [] Itm;
  Itm = new bool [ItmCount];
  for (int i = 0; i < ItmCount; i++) Itm[i] = false;
  this->ItmCount = ItmCount;
}

/************************************************************************/
/*** ATA_Base class for basic ATA models ********************************/
/************************************************************************/

ATA_Base::ATA_Base():
  CtgCount (0),
  TotCtgCount (0),
  TstCount (0),
  OvlCount (0),
  ItmCount (0),
  Ctg_Spc (NULL),
  Tst (NULL),
  MaxOvl (NULL),
  Itm (NULL),
  rnd (NULL),
  CurOvl (NULL)
{}

/************************************************************************/

ATA_Base::~ATA_Base()
{
  if (TotCtgCount > 0) delete [] Ctg_Spc;
  if (TstCount > 0)
  {
    delete [] Tst;
    delete [] MaxOvl;
    delete [] CurOvl;
  }
  if (ItmCount > 0)
  {
    delete [] Itm;
    delete [] rnd;
  }
}

/************************************************************************/

void ATA_Base::Add_Item(int itm, int tst)
{
  if (Itm[itm].Clu_ID < 0)
  {
    Tst[tst].Add_Item(itm, Itm[itm]);
    for (int t = 0; t < TstCount; t++)
      if ((t != tst) && (Tst[t].Itm[itm])) CurOvl[OvlIndex(t, tst)]++;
  }
  else
  {
    int n = 0;

    for (int i = 0; i < ItmCount; i++)
    if (Itm[i].Clu_ID == Itm[itm].Clu_ID)
    {
      Tst[tst].Add_Item(i, Itm[i]);
      n++;
    }
    for (int t = 0; t < TstCount; t++)
      if (t != tst) CurOvl[OvlIndex(t, tst)] += n;
  }
}

/************************************************************************/

int ATA_Base::Find_Item(int aTest)
{
  double x, m = 1e10;
  int    itm = -1;

  for (int i = 0; (i < ItmCount) && (m > 0); i++)
    if (!Tst[aTest].Itm[rnd[i]] && !Itm[rnd[i]].Not_Test[aTest] &&
      (Itm[rnd[i]].CurUse < Itm[rnd[i]].MaxUse))
    {
      if (Itm[rnd[i]].Clu_ID < 0)
        x = Tst[aTest].Ctg_Deviation_AddItm(Itm[rnd[i]]);
      else
        x = Tst[aTest].Ctg_Deviation_AddClu(Itm[rnd[i]].Clu_ID, Itm);
      if (x < m)
      {
        bool b = true;

        for (int t = 0; (t < TstCount) && b; t++)
          if (t != aTest)
          {
            int e = OvlIndex(t, aTest);
            b = ((MaxOvl[e] < 0) || (CurOvl[e] < MaxOvl[e]));
          }
        if (b)
        {
          m = x;
          itm = rnd[i];
        }
      }
    }
  return itm;
}

/************************************************************************/

int ATA_Base::Find_Test()
{
  double x, m = 0.0;
  int    tst = -1;
  int *  rnd = new int[TstCount];

  RandomOrder(rnd, TstCount);
  for (int t = 0; t < TstCount; t++)
  {
    x = Tst[rnd[t]].Missing();
    if (x > m)
    {
      m = x;
      tst = rnd[t];
    }
  }
  delete [] rnd;
  return tst;
}

/************************************************************************/

bool ATA_Base::Greedy()
{
  int i, t;

  while (true)
  {
    t = Find_Test();
    if (t < 0) return true;
    i = Find_Item(t);
    if (i < 0) return false;
    if (Itm[i].Clu_ID < 0)
      Add_Item(i, t);
    else
      for (int e = 0; e < ItmCount; e++)
        if (Itm[e].Clu_ID == Itm[i].Clu_ID)
          Add_Item(e, t);
  }
}

/************************************************************************/

bool ATA_Base::Item_in_Test(int itm, int tst)
{
  return Tst[tst].Itm[itm];
}

/************************************************************************/

int ATA_Base::Overlap(int tst1, int tst2)
{
  return CurOvl[OvlIndex(tst1, tst2)];
}

// /************************************************************************/
// 
// inline int ATA_Base::OvlIndex(int tst1, int tst2)
// {
//   if (tst1 > tst2)
//     return tst1 * (tst1 - 1) / 2 + tst2;
//   else
//     return tst2 * (tst2 - 1) / 2 + tst1;
// }

/************************************************************************/

void ATA_Base::Remove_Item(int itm, int tst)
{
  if (Itm[itm].Clu_ID < 0)
  {
    Tst[tst].Remove_Item(itm, Itm[itm]);
    for (int t = 0; t < TstCount; t++)
      if ((t != tst) && (Tst[t].Itm[itm])) CurOvl[OvlIndex(t, tst)]--;
  }
  else
  {
    int n = 0;

    for (int i = 0; i < ItmCount; i++)
      if (Itm[i].Clu_ID == Itm[itm].Clu_ID)
      {
        Tst[tst].Remove_Item(i, Itm[i]);
        n++;
      }
    for (int t = 0; t < TstCount; t++)
      if (t != tst) CurOvl[OvlIndex(t, tst)] -= n;
  }
}

/************************************************************************/

bool ATA_Base::SetCtgCount(int CtgCount)
{
  if ((CtgCount < 0) || (CtgCount > TotCtgCount)) return false;
  this->CtgCount = CtgCount;
  return true;
}

/************************************************************************/

bool ATA_Base::SetCtg(int ctg, int spc)
{
  if ((ctg < 0) || (ctg >= TotCtgCount) || (spc < 0) || (spc >= CtgCount))
    return false;
  Ctg_Spc[spc] = ctg;
  return true;
}

/************************************************************************/

bool ATA_Base::SetTotCtgCount(int TotCtgCount)
{
  if (TotCtgCount < CtgCount) return false;
  this->TotCtgCount = TotCtgCount;
  if (TotCtgCount > 0)
  {
    Ctg_Spc = new int[TotCtgCount];
    for (int e = 0; e < TotCtgCount; e++) Ctg_Spc[e] = -1;
  }
  return true;
}

/************************************************************************/

bool ATA_Base::SetItmCount(int ItmCount)
{
  if (ItmCount < 0) return false;
  if (ItmCount > 0)
  {
    Itm = new ATA_Item[ItmCount];
    for (int i = 0; i < ItmCount; i++)
    {
      Itm[i].SetCtgCount(CtgCount);
      Itm[i].SetTstCount(TstCount);
    }
    for (int t = 0; t < TstCount; t++) Tst[t].SetItmCount(ItmCount);
    rnd = new int[ItmCount];
    RandomOrder(rnd, ItmCount);
    this->ItmCount = ItmCount;
  }
  return true;
}

/************************************************************************/

bool ATA_Base::SetItmCategory(int itm, int ctg)
{
  if ((itm < 0) || (itm >= ItmCount) || (ctg < 0) || (ctg >= TotCtgCount))
    return false;
  Itm[itm].Ctg[Ctg_Spc[ctg]] = true;
  return true;
}

/************************************************************************/

bool ATA_Base::SetItmCluster(int itm, long CluID)
{
  if ((itm < 0) || (itm >= ItmCount) || (CluID < 0)) return false;
  Itm[itm].Clu_ID = CluID;
  return true;
}

/************************************************************************/

bool ATA_Base::SetItmExclusion(int itm, int exc, long ExcID)
{
  if ((itm < 0) || (itm >= ItmCount) ||
    (exc < 0) || (exc >= Itm[itm].ExcCount) || (ExcID < 0))
    return false;
  Itm[itm].Exc_ID[exc] = ExcID;
  return true;
}

/************************************************************************/

bool ATA_Base::SetItmExclusionCount(int itm, int ExcCount)
{
  if ((itm < 0) || (itm >= ItmCount) || (ExcCount < 0)) return false;
  if (ExcCount > 0)
  {
    Itm[itm].Exc_ID = new long[ExcCount];
    for (int i = 0; i < ExcCount; i++) Itm[itm].Exc_ID[i] = -1;
    Itm[itm].ExcCount = ExcCount;
  }
  return true;
}

/************************************************************************/

bool ATA_Base::SetItmID(int itm, long ID)
{
  if ((itm < 0) || (itm >= ItmCount) || (ID < 0)) return false;
  Itm[itm].ID = ID;
  return true;
}

/************************************************************************/

bool ATA_Base::SetItmInTest(int itm, int tst)
{
  if ((tst < 0) || (tst >= TstCount) || (itm < 0) || (itm >= ItmCount))
    return false;
  Itm[itm].SetInTest(tst);
  Tst[tst].Add_Item(itm, Itm[itm]);
  return true;
}

/************************************************************************/

bool ATA_Base::SetItmNotInTest(int itm, int tst)
{
  if ((tst < 0) || (tst >= TstCount) || (itm < 0) || (itm >= ItmCount))
    return false;
  Itm[itm].SetNotInTest(tst);
  return true;
}

/************************************************************************/

bool ATA_Base::SetItmUse(int itm, int use)
{
  if ((itm < 0) || (itm >= ItmCount) || (use < 0)) return false;
  Itm[itm].MaxUse = use;
  return true;
}

/************************************************************************/

bool ATA_Base::SetTstCount(int TstCount)
{
  if (TstCount < 0) return false;
  if (TstCount > 0)
  {
    Tst = new ATA_Test[TstCount];
    for (long i = 0; i < TstCount; i++)
    {
      Tst[i].SetCtgCount(CtgCount);
      Tst[i].SetItmCount(ItmCount);
    }
    for (int i = 0; i < ItmCount; i++) Itm[i].SetTstCount(TstCount);
    OvlCount = TstCount * (TstCount - 1) / 2;
    MaxOvl = new int[OvlCount];
    CurOvl = new int[OvlCount];
    for (int i = 0; i < OvlCount; i++)
    {
      MaxOvl[i] = -1;
      CurOvl[i] = 0;
    }
    this->TstCount = TstCount;
  }
  return true;
}

/************************************************************************/

bool ATA_Base::SetTstCtgMaxFrac(int tst, int ctg, double frac)
{
  if ((tst < 0) || (tst >= TstCount) || (ctg < 0) || (ctg >= CtgCount))
    return false;
  if (frac >= 0) Tst[tst].Ctg_MaxF[ctg] = frac;
  return true;
}

/************************************************************************/

bool ATA_Base::SetTstCtgMinFrac(int tst, int ctg, double frac)
{
  if ((tst < 0) || (tst >= TstCount) || (ctg < 0) || (ctg >= CtgCount))
    return false;
  Tst[tst].Ctg_MinF[ctg] = frac;
  return true;
}

/************************************************************************/

bool ATA_Base::SetTstCtgMaxItem(int tst, int ctg, int itm)
{
  if ((tst < 0) || (tst >= TstCount) || (ctg < 0) || (ctg >= CtgCount))
    return false;
  if (itm >= 0) Tst[tst].Ctg_MaxI[ctg] = itm;
  return true;
}

/************************************************************************/

bool ATA_Base::SetTstCtgMinItem(int tst, int ctg, int itm)
{
  if ((tst < 0) || (tst >= TstCount) || (ctg < 0) || (ctg >= CtgCount))
    return false;
  Tst[tst].Ctg_MinI[ctg] = itm;
  return true;
}

/************************************************************************/

bool ATA_Base::SetTstMaxLen(int tst, int len)
{
  if ((tst < 0) || (tst >= TstCount) || (len <= 0)) return false;
  Tst[tst].MaxLen = len;
  return true;
}

/************************************************************************/

bool ATA_Base::SetTstMaxOverlap(int tst1, int tst2, int ovl)
{
  if ((tst1 < 0) || (tst2 < 0) || (tst1 == tst2) || (ovl < 0)) return false;
  MaxOvl[OvlIndex(tst1, tst2)] = ovl;
  return true;
}

/************************************************************************/

