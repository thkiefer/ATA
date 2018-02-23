#ifndef ATA_BASE_H
#define ATA_BASE_H
/************************************************************************/
/*                                                                      */
/* ATA_Base.h                                                           */
/*                                                                      */
/*  9-11-2015                                                           */
/*                                                                      */
/*   126 lines                                                          */
/*                                                                      */
/* Automated Test Assembly basic / calibration design                   */
/* ATA_Base class                                                       */
/*                                                                      */
/************************************************************************/

/************************************************************************/
/*** ATA_Item class for basic item **************************************/
/************************************************************************/

class ATA_Item
{
public:
  long         ID;            /* item ID                                */
  bool *       Ctg;           /* content categories                     */
  long         Clu_ID;        /* cluster ID                             */
  int          ExcCount;      /* number of exclusions                   */
  long *       Exc_ID;        /* exclusion IDs                          */
  int          MaxUse;        /* maximum incidence rate                 */
  bool *       In_Test;       /* locked in test                         */
  bool *       Not_Test;      /* locked not in test                     */
  /* run-time info ******************************************************/
  int          CurUse;        /* current incidence rate                 */
  ATA_Item();
  ~ATA_Item();
  bool         SetCtgCount(int CtgCount);
  bool         SetInTest(int tst);
  bool         SetNotInTest(int tst);
  bool         SetTstCount(int TstCount);
};

/************************************************************************/
/*** ATA_Test class for basic test specifications ***********************/
/************************************************************************/

class ATA_Test
{
public:
  int          ItmCount;      /* item count                             */
  int          CtgCount;      /* number of content categories           */
  int *        Ctg_MinI;      /* minimum items from category            */
  int *        Ctg_MaxI;      /* maximum items from category            */
  double *     Ctg_MinF;      /* minimum fraction from category         */
  double *     Ctg_MaxF;      /* maximum fraction from category         */
  int          MaxLen;        /* maximum number of items                */
  /* run-time info ******************************************************/
  int *        Ctg_Cur;       /* current categories (greedy etc.)       */
  int          CurLen;        /* current length (greedy etc.)           */
  bool *       Itm;           /* item in test?                          */
  ATA_Test();
  ~ATA_Test();
  bool         Add_Item(int itm, ATA_Item & aItm);
  double       Ctg_Deviation();
  double       Ctg_Deviation_AddItm(ATA_Item & aItm);
  double       Ctg_Deviation_AddClu(long CluID, ATA_Item * aItm);
  double       Missing();
  bool         Remove_Item(int itm, ATA_Item & aItm);
  /* property setters ***************************************************/
  bool         SetCtgCount(int CtgCount);
  bool         SetItmCount(int ItmCount);
};

/************************************************************************/
/*** ATA_Base class for basic ATA models ********************************/
/************************************************************************/

class ATA_Base
{
protected:
  
  int          CtgCount;      /* number of content categories in specs  */
  int          TotCtgCount;   /* total number of content categories     */
  int          TstCount;      /* number of test specs                   */
  int          OvlCount;      /* number of overlaps                     */
  int          ItmCount;      /* number of items                        */
  int *        Ctg_Spc;       /* category to spec translation table     */
  ATA_Test *   Tst;           /* test specifications                    */
  int *        MaxOvl;        /* maximum overlap test forms             */
  ATA_Item *   Itm;           /* items                                  */
  /* run-time info ******************************************************/
  int *        rnd;           /* array in random order                  */
  int *        CurOvl;        /* current overlaps                       */
  virtual void Add_Item(int itm, int tst);
  virtual int  Find_Item(int aTest);
  virtual int  Find_Test();
  inline int   OvlIndex(int tst1, int tst2);
  virtual void Remove_Item(int itm, int tst);
public:
  ATA_Base();
  ~ATA_Base();
  virtual bool Greedy();
  bool         Item_in_Test(int itm, int tst);
  int          Overlap(int tst1, int tst2);
  /* property setters ***************************************************/
  bool         SetCtgCount(int CtgCount);
  bool         SetCtg(int ctg, int spc);
  bool         SetTotCtgCount(int TotCtgCount);
  virtual bool SetItmCount(int ItmCount);
  bool         SetItmCategory(int itm, int ctg);
  bool         SetItmCluster(int itm, long CluID);
  bool         SetItmExclusion(int itm, int exc, long ExcID);
  bool         SetItmExclusionCount(int itm, int ExcCount);
  bool         SetItmID(int itm, long ID);
  bool         SetItmInTest(int itm, int tst);
  bool         SetItmNotInTest(int itm, int tst);
  bool         SetItmUse(int itm, int use);
  virtual bool SetTstCount(int TstCount);
  bool         SetTstCtgMaxFrac(int tst, int ctg, double frac);
  bool         SetTstCtgMinFrac(int tst, int ctg, double frac);
  bool         SetTstCtgMaxItem(int tst, int ctg, int itm);
  bool         SetTstCtgMinItem(int tst, int ctg, int itm);
  bool         SetTstMaxLen(int tst, int len);
  bool         SetTstMaxOverlap(int tst1, int tst2, int ovl);
};

/************************************************************************/

/************************************************************************/
inline int ATA_Base::OvlIndex(int tst1, int tst2)
{
  if (tst1 > tst2)
    return tst1 * (tst1 - 1) / 2 + tst2;
  else
    return tst2 * (tst2 - 1) / 2 + tst1;
}
/************************************************************************/

#endif
