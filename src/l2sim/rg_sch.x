/********************************************************************16**

                         (c) COPYRIGHT 2011 by
                         Continuous Computing Corporation.
                         All rights reserved.

     This software is confidential and proprietary to Continuous Computing
     Corporation (CCPU).  No part of this software may be reproduced,
     stored, transmitted, disclosed or used in any form or by any means
     other than as expressly provided by the written Software License
     Agreement between CCPU and its licensee.

     CCPU warrants that for a period, as provided by the written
     Software License Agreement between CCPU and its licensee, this
     software will perform substantially to CCPU specifications as
     published at the time of shipment, exclusive of any updates or
     upgrades, and the media used for delivery of this software will be
     free from defects in materials and workmanship.  CCPU also warrants
     that has the corporate authority to enter into and perform under the
     Software License Agreement and it is the copyright owner of the software
     as originally delivered to its licensee.

     CCPU MAKES NO OTHER WARRANTIES, EXPRESS OR IMPLIED, INCLUDING
     WITHOUT LIMITATION WARRANTIES OF MERCHANTABILITY OR FITNESS FOR
     A PARTICULAR PURPOSE WITH REGARD TO THIS SOFTWARE, SERVICE OR ANY RELATED
     MATERIALS.

     IN NO EVENT SHALL CCPU BE LIABLE FOR ANY INDIRECT, SPECIAL,
     CONSEQUENTIAL DAMAGES, OR PUNITIVE DAMAGES IN CONNECTION WITH OR ARISING
     OUT OF THE USE OF, OR INABILITY TO USE, THIS SOFTWARE, WHETHER BASED
     ON BREACH OF CONTRACT, TORT (INCLUDING NEGLIGENCE), PRODUCT
     LIABILITY, OR OTHERWISE, AND WHETHER OR NOT IT HAS BEEN ADVISED
     OF THE POSSIBILITY OF SUCH DAMAGE.

                       Restricted Rights Legend

     This software and all related materials licensed hereby are
     classified as "restricted computer software" as defined in clause
     52.227-19 of the Federal Acquisition Regulation ("FAR") and were
     developed entirely at private expense for nongovernmental purposes,
     are commercial in nature and have been regularly used for
     nongovernmental purposes, and, to the extent not published and
     copyrighted, are trade secrets and confidential and are provided
     with all rights reserved under the copyright laws of the United
     States.  The government's rights to the software and related
     materials are limited and restricted as provided in clause
     52.227-19 of the FAR.

                    IMPORTANT LIMITATION(S) ON USE

     The use of this software is limited to the use set
     forth in the written Software License Agreement between CCPU and
     its Licensee. Among other things, the use of this software
     may be limited to a particular type of Designated Equipment, as
     defined in such Software License Agreement.
     Before any installation, use or transfer of this software, please
     consult the written Software License Agreement or contact CCPU at
     the location set forth below in order to confirm that you are
     engaging in a permissible use of the software.

                    Continuous Computing Corporation
                    9450, Carroll Park Drive
                    San Diego, CA-92121, USA

                    Tel: +1 (858) 882 8800
                    Fax: +1 (858) 777 3388

                    Email: support@ccpu.com
                    Web: http://www.ccpu.com

*********************************************************************17*/


/**********************************************************************

    Name:   LTE MAC layer

    Type:   C include file

    Desc:   Defines required by LTE MAC

    File:  rg_sch.x

    Sid:      rg_sch.x@@/main/2 - Sat Jul 30 02:22:12 2011

    Prg:   sd

**********************************************************************/
/** @file rg_sch.x
@brief This file contains basic data structures for the scheuler.
*/


/*
*     The defines in this file correspond to structures and variables
*     used by the following TRILLIUM software:
*
*     part no.             description
*     --------     ----------------------------------------------
*     1000372       LTE MAC
*
*/

#ifndef __SCH__
#define __SCH__

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

typedef TfuDciFormat1aInfo RgDciFmt1AInfo;
typedef TfuRaReqInfo       RgTfuRaReqInfo;
typedef TfuSubbandCqiInfo  RgSchSubbandCqiInfo;
typedef TfuHqIndInfo       RgTfuHqIndInfo;
typedef TfuHqInfo          RgTfuHqInfo;

/* Forward declarations for some structures */
#ifdef LTE_L2_MEAS
typedef struct rgSchL2MeasCb     RgSchL2MeasCb;
typedef struct rgSchQciCb        RgSchQciCb;
#endif /* LTE_L2_MEAS */
typedef struct rgSchUeCb         RgSchUeCb;
typedef struct rgSchCellCb       RgSchCellCb;
typedef struct rgSchErrInfo      RgSchErrInfo;
typedef struct rgSchUlAlloc      RgSchUlAlloc;
typedef struct rgSchUlRetxAlloc  RgSchUlRetxAlloc;
typedef struct rgSchUlHqProcCb   RgSchUlHqProcCb;
typedef struct rgSchDlHqProcCb   RgSchDlHqProcCb;
/* rg007.201 - Changes for MIMO feature addition */
/* rg008.201 - Removed dependency on MIMO compile-time flag */
typedef struct rgSchDlHqTbCb     RgSchDlHqTbCb;
typedef struct rgSchLcgCb        RgSchLcgCb;
typedef struct rgSchDlHqEnt      RgSchDlHqEnt;
typedef struct rgSchRaCb         RgSchRaCb;
typedef struct _rgSchCb          RgSchCb;
typedef struct rgSchUlLcCb       RgSchUlLcCb;
typedef struct rgSchDlLcCb       RgSchDlLcCb;
typedef struct _rgSchdApis       RgSchdApis;
#ifdef LTE_TDD
typedef struct rgSchTddPhichOffInfo    RgSchTddPhichOffInfo;
typedef U8 RgSchTddNpValTbl[RGSCH_TDD_MAX_P_VAL];
#endif
/*rg008.201 - Added support for SPS*/
#ifdef LTEMAC_SPS
typedef struct rgSchDlSfAllocInfo RgSchDlSfAllocInfo;
#endif

/**
  * @brief
  * Scheduler APIs
  */
struct _rgSchdApis
{
   S16 (*rgSCHRgrUeCfg) ARGS((RgSchCellCb *cell, RgSchUeCb *ue, RgrUeCfg *cfg,
            RgSchErrInfo *err));
   S16 (*rgSCHRgrUeRecfg) ARGS((RgSchCellCb *cell, RgSchUeCb *ue, RgrUeRecfg *recfg,
            RgSchErrInfo *err));
   Void (*rgSCHFreeUe) ARGS((RgSchCellCb *cell, RgSchUeCb *ue));
   S16 (*rgSCHRgrCellCfg) ARGS((RgSchCellCb *cell, RgrCellCfg *cfg,
            RgSchErrInfo *err));
   S16 (*rgSCHRgrCellRecfg) ARGS((RgSchCellCb *cell, RgrCellRecfg *recfg,
            RgSchErrInfo *err));
   Void (*rgSCHFreeCell) ARGS((RgSchCellCb *cell));
   S16 (*rgSCHRgrLchCfg) ARGS((RgSchCellCb *cell, RgSchUeCb *ue,
            RgSchDlLcCb *dl, RgrLchCfg *cfg, RgSchErrInfo *errInfo));
   S16 (*rgSCHRgrLcgCfg) ARGS((RgSchCellCb *cell, RgSchUeCb *ue,
            RgSchLcgCb *lcg, RgrLcgCfg *cfg, RgSchErrInfo *errInfo));
   S16 (*rgSCHRgrLchRecfg) ARGS((RgSchCellCb *cell, RgSchUeCb *ue,
            RgSchDlLcCb *dl, RgrLchRecfg *recfg,
            RgSchErrInfo *errInfo));
   S16 (*rgSCHRgrLcgRecfg) ARGS((RgSchCellCb *cell, RgSchUeCb *ue,
            RgSchLcgCb *lcg, RgrLcgRecfg *recfg, RgSchErrInfo *errInfo));
   Void (*rgSCHFreeDlLc) ARGS((RgSchCellCb *cell, RgSchUeCb *ue, RgSchDlLcCb *dlLc));
   Void (*rgSCHFreeLcg) ARGS((RgSchCellCb *cell, RgSchUeCb *ue, RgSchLcgCb *lcg));
   Void (*rgSCHActvtUlUe) ARGS((RgSchCellCb *cell, RgSchUeCb *ue));
   Void (*rgSCHActvtDlUe) ARGS((RgSchCellCb *cell, RgSchUeCb *ue));
   Void (*rgSCHHdlUlTransInd) ARGS((RgSchCellCb *cell, RgSchUeCb *ue,
            CmLteTimingInfo timingInfo));
   Void (*rgSCHUeReset) ARGS((RgSchCellCb *cell, RgSchUeCb *ue));
   S16 (*rgSCHUpdBsrShort) ARGS((RgSchCellCb *cell, RgSchUeCb *ue, RgSchLcgCb *ulLcg, U8 bsr, RgSchErrInfo *err));
   S16 (*rgSCHUpdBsrTrunc) ARGS((RgSchCellCb *cell, RgSchUeCb *ue, RgSchLcgCb *ulLcg, U8 bsr, RgSchErrInfo *err));
   S16 (*rgSCHUpdBsrLong) ARGS((RgSchCellCb *cell, RgSchUeCb *ue, U8 bsArr[], RgSchErrInfo *err));
   S16 (*rgSCHUpdPhr) ARGS((RgSchCellCb *cell, RgSchUeCb *ue, U8 phr, RgSchErrInfo *err));
#ifdef RG_UNUSED
   S16 (*rgSCHUpdUlHqProc) ARGS((RgSchCellCb *cell, RgSchUlHqProcCb *curProc,
            RgSchUlHqProcCb *oldProc));
#endif
   S16 (*rgSCHContResUlGrant) ARGS((RgSchCellCb *cell, RgSchUeCb *ue, RgSchErrInfo *err));
   S16 (*rgSCHSrRcvd) ARGS((RgSchCellCb *cell, RgSchUeCb *ue, CmLteTimingInfo, RgSchErrInfo *err));
   S16 (*rgSCHTti) ARGS((RgSchCellCb *cell, RgSchErrInfo *err));
   Void (*rgSCHUlCqiInd) ARGS(( RgSchCellCb *cell, RgSchUeCb *ue, TfuUlCqiRpt *ulCqiInfo));
   Void (*rgSCHPucchDeltaPwrInd) ARGS(( RgSchCellCb *cell,
            RgSchUeCb *ue, S8 delta));
   S16 (*rgSCHlUeReset) ARGS(( RgSchCellCb *cell, RgSchUeCb *ue));
   Void (*rgSCHDlDedBoUpd) ARGS((RgSchCellCb *cell, RgSchUeCb *ue, RgSchDlLcCb *svc));
   /* rg004.201: ccpu00105914: PHR handling for MSG3 */
   Void (*rgSCHUlRecMsg3Alloc) ARGS((RgSchCellCb *cell, RgSchUeCb *ue,
            RgSchRaCb *raCb));
   Void (*rgSCHUlHqProcForUe) ARGS((RgSchCellCb *cell, CmLteTimingInfo frm,
            RgSchUeCb *ue, RgSchUlHqProcCb  **procRef));
   RgSchUlAlloc *(*rgSCHFirstRcptnReq) ARGS((RgSchCellCb *cell));
   RgSchUlAlloc *(*rgSCHNextRcptnReq) ARGS((RgSchCellCb *cell,
            RgSchUlAlloc *alloc));
   RgSchUlAlloc *(*rgSCHFirstHqFdbkAlloc) ARGS((RgSchCellCb *cell));
   RgSchUlAlloc *(*rgSCHNextHqFdbkAlloc) ARGS((RgSchCellCb *cell,
            RgSchUlAlloc *alloc));
   Void (*rgSCHDlProcAddToRetx) ARGS((RgSchCellCb *cell,RgSchDlHqProcCb *hqP));
   Void (*rgSCHDlCqiInd) ARGS((RgSchCellCb *cell, RgSchUeCb *ue,
            TfuDlCqiRpt *dlCqiInd, CmLteTimingInfo timingInfo));
/* rg009.201. Added changes of TFU_UPGRADE */
#ifdef TFU_UPGRADE
Void (*rgSCHSrsInd) ARGS((RgSchCellCb *cell, RgSchUeCb *ue,
         TfuSrsRpt* srsInd, CmLteTimingInfo timingInfo));
#endif

   Void (*rgSCHDlTARpt) ARGS((RgSchCellCb *cell, RgSchUeCb *ue));
   /* rg007.201 - Changes for MIMO feature addition */
   /* rg008.201 - Removed dependency on MIMO compile-time flag */
   Void (*rgSCHDlRlsSubFrm) ARGS((RgSchCellCb *cell, CmLteTimingInfo subFrm));
   /*rg008.201 - Added support for SPS*/
#ifdef LTEMAC_SPS
   Void (*rgSCHDlProcDtx) ARGS((RgSchCellCb *cell, RgSchDlHqProcCb *hqP));
   Void (*rgSCHDlRelPdcchFbk) ARGS((RgSchCellCb *cell, RgSchUeCb * ue,
            Bool isAck));
   Void (*rgSCHUlSpsRelInd) ARGS((RgSchCellCb *cell, RgSchUeCb * ue,
            Bool isExplRel));
   Void (*rgSCHUlCrcFailInd) ARGS((RgSchCellCb *cell, RgSchUeCb * ue,
           CmLteTimingInfo    crcTime));
   Void (*rgSCHUlCrcInd) ARGS((RgSchCellCb *cell, RgSchUeCb * ue,
           CmLteTimingInfo    crcTime));
#endif /* LTEMAC_SPS */
#ifdef LTEMAC_DRX
   Void (*rgSCHDrxStrtInActvTmrInUl) ARGS((RgSchCellCb *cell));
#endif
};

/* rg009.201. Added changes of TFU_UPGRADE */
#ifdef TFU_UPGRADE

/**
* @brief Periodic CQI/PMI/RI configuration parameters information
*/
typedef RgrUePrdDlCqiCfg RgSchUeDlPCqiCfg;

/**
* @brief Periodic CQI Setup configuration parameters information
*/
/* Reference: 36.313: CQI-ReportPeriodic */
typedef RgrUeDlPCqiSetup RgSchUeDlPCqiSetup;

/**
* @brief SRS configuration parameters information
*/
/* Reference 36.313 SoundingRS-UL-Config */

typedef RgrUeUlSrsCfg RgSchUeUlSrsCfg;


/**
* @brief SRS configuration setup parameters information
*/
/* Reference 36.313 SoundingRS-UL-Config */

typedef RgrUeUlSrsSetupCfg RgSchUeSrsUlSetupCfg;

/**
* @brief SR configuration parameters information
*/

typedef RgrUeSrCfg RgSchUeSrCfg;

/**
* @brief SR Setup configuration parameters information
*/

typedef RgrUeSrSetupCfg RgSchUeSrSetupCfg;



/**
* @brief Store Periodic CQI, SRS and SR tranmission instances list
* */

typedef struct rgSchPerPucchCb
{
   CmLListCp         srLst;
   CmLListCp         srsLst;
   CmLListCp         cqiLst;
   CmLListCp         riLst;
}RgSchPerPucchCb;

/**
  @brief   UE Specific SRS Periodicity   and Subframe Offset Configuration FDD */

typedef struct rgSchUePCqiSrsSrCfgIdxTbl
{
   U16     min;          /*!< start ISRB Index */
   U16     max;          /*!< end ISRB Index */
   U16     peri;         /*!< SRS Periodicity in ms */
   U16     offset;        /*!< Offset with ISRB value */
}RgSchUePCqiSrsSrCfgIdxTbl;

/**
  @brief   UE Specific SRS Periodicity   and Subframe Offset Configuration TDD */

/* Ref: Table 8.2-2: UE Specific SRS Periodicity */

typedef struct rgSchUeSrsCfgIdxTbl
{
   U16     min;      /*!< start ISRB Index */
   U16     max;      /*!< end ISRB Index */
   U16     peri;    /*!< SRS Periodicity in ms */
   U16     offset1;  /*!< Offset with ISRB value */
   U16     offset2;  /*!< Offset with ISRB value */

}RgSchUeSrsCfgIdxTbl;



/**
  @brief   UE Specific SRS Control Block */

typedef struct rgSchUeSrsCb
{
    RgSchUeUlSrsCfg     srsCfg;     /*!< SRS Specific configuration */
    U16                 peri;       /*!< Periodicity */
    U16                  offset;     /*!< subframe Offset */
    U16                  nSrsTrIdx;
                                   /*!< Next Tranmission instance
                                       Referenence: 36.213 Section:8.2
                                      ((10*sfn +sf-sfOffset))/mod(peri))  */
    CmLList             srsLstEnt;      /*!< SRS List for next tranmission instance subframe */
    U8                selectedAnt;    /*!< Selected Antenna for SRS Reception*/
    U16                 srsDist;    /*!<  crnt Distance to get RI Transmission */

}RgSchUeSrsCb;

/**
@brief  Periodic CQI/PMI Reporting configuration using PUCCH */



/**
@brief Subband Size (k) and Bandwidth Parts (J) vs. DL sys BW */

typedef struct rgSchUeBwSubSzBwParts
{
   U8 rbStart;   /* RB Start */
   U8 rbEnd;     /* RB End */
   U8 subSize;   /* k */
   U8 bwParts;   /* J */

}RgSchUeBwSubSzBwParts;


/**
@brief Periodic CQI/PMI/RI Control Block */

typedef struct rgSchUePCqiCb
{
    RgrUePrdDlCqiCfg    cqiCfg;        /*!< Cqi Pmi Configuration */
    U16                  cqiPeri;       /*!< Periodicity */
    U16                  cqiOffset;     /*!< CQI offset */
    U16                  riOffset;      /*!< RI Offset */
    U16                  riPeri;        /*!< RI periodicity */
    U16                  nCqiTrIdx;
                                      /*!< Next Active Tranmission instace index ,
                                          precompute CQI/PMI
                                          Transmission Instance
                                          Referenence: 36.213 Section:8.2 */

   /* 1.  where wideband CQI/PMI reporting is configured
         (Mode: 1-0 or 1-1)
          10*sfn+floor(subframe/2)-Noffsetcqi )Mod(Nperiod)=0
      2.  where both wideband CQI/PMI and subband CQI reporting are configured
         (Mode: 2-0 or 2-1)
             10*sfn+floor(subframe/2)-Noffsetcqi )Mod(Nperiod)=0
             only the few occasations will be used for WB and
             few will be used for subbands.
   */

    U16                nRiTrIdx;      /*!< Next Active Transmission instance for RI
                                           Transmission Instance
                                           Referenence: 36.213 Section:8.2 */
    U16                riDist;        /*!<  crnt Distance to get RI Transmission */
    U16                h;            /*!< Precompute and store H */
    U8                  riNumBits;     /*Precomputed value as it doesn't change on-the-fly */

   /* 1. In the case where wideband RI reporting is configured
         (Mode 1-0 or  1-1)
          (10*sfn+floor(subframe)-Noffsetcqi-NoffsetRI )Mod(NCqiperiod *MriPeriod)=0

      2. Where Widesband and Subband RI reporting is configured
         (Mode 2-0 or 2-1 )
          (10*sfn+floor(subframe)-Noffsetcqi-NoffsetRI )
         Mod(H. NCqiperiod *MriPeriod )=0
         where H= J * K +1;  J=Number of bandwidth parts(BW/subsize). K is RGR interf input */
     Bool              isWb; /*! < Is True when a WideBand CQI is to be Transmitted in a SubBand CQI Config*/
     U8                 bpIdx;  /*Index of the Bandwidth Part*/
     U8                 label; /*Label L associated */
     U8                 J; /*Number of Bandwidth Parts*/

   CmLList           cqiLstEnt;         /*!< List CQI UEs for next transmission instance */
   CmLList           riLstEnt;          /*!< List RI UEs for next transmission instance */

}RgSchUePCqiCb;



/**
@brief SR Configuration Index */

typedef struct rgSchUeSrCfgIdx
{
   U16     sIsr;      /*!< Start ISR Index */
   U16     eIsr;      /*!< End   ISR Index */
   U16     peri;      /*!< SR Periodicity */
   U16     offset;    /*!< offset */
}RgSchUeSrCfgIdx;
#ifdef RG_UNUSED
/* Reference : 36.213 Table 10.1-5 */
/* Note: SR is same table for TDD and FDD */
RgSchUeSrCfgIdx  schUeSrCfgIdx[RG_SCH_ISR_INDX_MAX]=
{
 { 0,4,   5, 0 },
 { 5,14,   10, 5 },
 { 15,34,  20,15 },
 { 35,74, 40,35},
 { 75,154, 80, 75},
 /* RESERVED: Configuration should not allow Res values */
};
#endif
/**
@brief SR Control block */

typedef struct rgSchUeSrCb
{
    RgSchUeSrCfg         srCfg;    /*!< SR configuration */
    U16                  offset;   /*!< SR offset */
    U16                  peri;     /*!< SR periodicity */

    U16                  nSrTrIdx;    /*!< Next Active Tranmission Instance Index ,
                                     Referenence: 36.213 Section:10.1 */

   /*
      ((10*sfn+floor(sf)-srOffset)Mod(srPeriodcity))
   */

   CmLList           srLstEnt;      /*!< Next Transmission instance SR UE list */

}RgSchUeSrCb;


/**
@brief  APeriodic CQI Control Block */
typedef struct rgSchUeACqiCb
{
 RgrUeAprdDlCqiCfg    aCqiCfg;        /*!< Cqi Pmi Configuration */
 U8                              L; /*Unique Label through which positioning of Subbands
                                         will be conveyed by the UE */
 U8                              N;  /*Total Bandwidth / Num of Subbands*/
 U8                              M; /*Number of Selected Subbands*/
 U8                              riNumBits;
 U8                              k; /*k - subband size. Valid only for Modes 1-2,3-0,3-1 Ref: TS36.213 Table: 7.2.1-3*/
                                        /*k - subband size. Valid only for Modes 2-0, 2-2  Ref: TS36.213 Table: 7.2.1-5*/
 U8                              cqiPmiSzR1;  /*CQIPMI Size for Rank =1*/
 U8                              cqiPmiSzRn1; /*CQIPMI Size for Rank > 1*/
}RgSchUeACqiCb;

typedef enum
{
   RG_SCH_FDD_PCQI_TBL = 0,
   RG_SCH_TDD_PCQI_TBL,
   RG_SCH_RI_TBL,
   RG_SCH_FDD_SRS_TBL,
   RG_SCH_TDD_SRS_TBL,
   RG_SCH_SR_TBL
} RgSchPerTbl;

/*ccpu00116923 - ADD - SRS present support*/
#ifdef LTE_TDD
typedef U8 RgSchTddCellSpSrsSubfrmTbl[RGSCH_CELLSP_SRS_SF_CONFIGS][RGSCH_NUM_SUB_FRAMES];
#else
typedef U8 RgSchFddCellSpSrsSubfrmTbl[RGSCH_CELLSP_SRS_SF_CONFIGS][RGSCH_NUM_SUB_FRAMES];
#endif


#endif



#ifdef LTE_TDD
/**
  * @brief
  * TDD UE specific PUCCH recpetion information.
  */
typedef struct rgSchUePucchRecpInfo
{
   CmHashListEnt     hashLstEnt;     /*!< List of PUCCH for receiving
                                          ACK/NACK feedback information */
   TfuUeRecpReqInfo  *pucchRecpInfo; /*!< UE PUCCH Reception information */
} RgSchUePucchRecpInfo;

/**
  * @brief
  * TDD switch point information.
  */
typedef struct rgSchTddSubfrmInfo
{
   U8 switchPoints;     /*!< Number of DL-UL switch points */
   U8 numFrmHf1;        /*!< Number of subframes for half frame 1
                             Present for both 5ms and 10ms periodicity */
   U8 numFrmHf2;        /*!< Number of subframes for half frame 2
                             Present only for 5ms periodicity */
} RgSchTddSubfrmInfo;

/**
  * @brief
  * TDD DL Association Set information.
  */
typedef struct rgSchTddDlAscSetIdxK
{
   U8 numFdbkSubfrms;                        /*!< Number of Feedbacks for DL Subframes */
   U8 subfrmNum[RGSCH_NUM_SUB_FRAMES-1];     /*!< List of Subframe Number */
} RgSchTddDlAscSetIdxK;

/** @brief PRACH Information for a frequency resource. */
typedef struct rgrSchTddPrachInfo
{
   U8     freqIdx;          /*!< Frequency Index */
   U8     sfn;              /*!< Even/Odd/All Radio Frames */
   U8     halfFrm;          /*!< First/Second Half Frame */
   U8     ulStartSfIdx;     /*!< Uplink Start Subframe Index*/
} RgSchTddPrachInfo;

/** @brief PRACH resource Information for each of the
 *  frequency resources. */
typedef struct rgrSchTddPrachRscInfo
{
   U8                 numRsc;                             /*!< Number of frequency resources*/
   RgSchTddPrachInfo  prachInfo[RGSCH_TDD_MAX_FREQ_RSRC]; /*!< PRACH Information */
} RgSchTddPrachRscInfo;

/**
  * @brief
  * TDD Special subframe configuration information.
  */
struct rgSchTddSplSubfrmInfo
{
   U8   norDlDwPts;        /*!< DL Normal CP: DwPTS in Ts */
   U8   norDlNorUpPts;     /*!< DL Normal CP: UL Normal CP:UpPTS in Ts */
   U8   norDlExtUpPts;     /*!< DL Normal CP: UL Extended CP: UpPTS in Ts */
   U8   extDlDwPts;        /*!< DL Extended CP: DwPTS in Ts */
   U8   extDlNorUpPts;     /*!< DL Extended CP: UL Normal CP:UpPTS in Ts */
   U8   extDlExtUpPts;     /*!< DL Extended CP: UL Extended CP: UpPTS in Ts */
};

/**
  * @brief
  * RACH response awaiting scheduling from the current time is
  * identified with sfn offset and subframe.
  */
typedef struct rgSchTddRachRspInfo
{
   U8    sfnOffset;                    /*!< SFN offset with respect to
                                             expected RACH available for
                                             scheduling */
   U8    numSubfrms;                   /* Number of subframes present */
   U8    subframe[RGSCH_NUM_SUB_FRAMES];  /*!< List of Subframe numbers */
} RgSchTddRachRspInfo;

typedef RgSchTddRachRspInfo RgSchTddRachDelInfo;

/**
  * @brief
  * List of awaiting RACH responses for scheduling across radio frames.
  *
  */
typedef struct rgSchTddRachRspLst
{
   U8                   numRadiofrms;   /*!< Number of radio frames */
   RgSchTddRachRspInfo  rachRsp[2];     /*!< RACH Occasions for which response
                                          can be sent */
   RgSchTddRachDelInfo  delInfo;        /*!< Previous RACH responses for
                                          which the scheduling deadline
                                          has expired. So those responses
                                          can be deleted */
} RgSchTddRachRspLst;

/**
  * @brief
  * Uplink association index information indicates the SFN offset and
  * subframe in which DL HARQ ACK/NACK is expected.
  */
typedef struct rgSchTddUlAscInfo
{
   U8    subframe;         /*!< Subframe number */
   U8    sfnOffset;        /*!< SFN offset with respect to expected
                                UL data reception time */
} RgSchTddUlAscInfo;

/**
  * @brief
  * PUSCH information indicates the SFN offset and
  * subframe in which UL data is scheduled.
  */
typedef struct rgSchTddPuschOffInfo
{
   U8    subframe;         /*!< Subframe number */
   U8    sfnOffset;        /*!< SFN offset with respect to expected
                                UL data reception time */
} RgSchTddPuschOffInfo;

/**
  * @brief
  * PHICH information indicates the SFN offset and
  * subframe for which PHICH should be sent.
  */
struct rgSchTddPhichOffInfo
{
   U8    numSubfrms;       /*!< Number of subframes */
   U8    subframe[2];      /*!< Subframe number; Two entries are present
                                 only in case of UL-DL configuration Index 0 */
   U8    sfnOffset;        /*!< SFN offset with respect to expected
                                UL data reception time */
};

/**
  * @brief
  * DL feedback reception information indicates the SFN offset
  * and subframe at which feedback is expected.
  */
typedef struct rgSchTddDlFdbkInfo
{
   U8    subframe;      /*!< Subframe number */
   U8    sfnOffset;     /*!< SFN offset with respect to current
                             scheduled time */
   U8    m;             /*!< m factor used in Downlink Association
                             Set Index */
} RgSchTddDlFdbkInfo;


/**
  * @brief
  * Special subframe configuration index.
  */
typedef struct rgSchTddSplSubfrmCfg
{
   U16   dwPts;      /*!< DwPTS in OFDM Symbol Duration */
   U16   upPts;      /*!< UpPTS in OFDM Symbol Duration */
} RgSchTddSplSubfrmCfg;

/**
  * @brief
  * ACK/NACK information to be used for ACK/NACK bundling mode.
  */
typedef struct rgSchTddANInfo
{
   U16    sfn;           /*!< ACK/NACK is sent for PDU in this SFN */
   U8     subframe;      /*!< ACK/NACK is sent for PDU in this subframe */
   U8     dlDai;         /*!< Downlink Assignment Index for
                                                    UL-DL Configuration 1-6 */
   U8     ulDai;         /*!< DAI for uplink */
   U8     latestMIdx;    /*!< Last transmitted DL subframe 'm' index */
} RgSchTddANInfo;
#endif

/**
  * @brief
  * Information about one MCS entry.
  */
typedef struct rgSchUlIMcsInfo
{
   U8 qm;
   U8 iTbs;
} RgSchUlIMcsTbl[29];
EXTERN RgSchUlIMcsTbl rgUlIMcsTbl;

typedef struct rgSchUeCatTbl
{
   U32    maxUlBits;
   U32    maxDlBits;   /*!< Max bits per TB per TTI */
/* rg002.101: correcting DL harq softbuffer limitation logic */
   U32    maxSftChBits;
   Bool   ul64qamSup;
/* rg007.201 - Changes for MIMO feature addition */
/* rg008.201 - Removed dependency on MIMO compile-time flag */
   U32    maxDlTbBits; /*!< Max TB Bits per TTI */
   U8     maxTxLyrs;
} RgSchUeCatTbl[5];
EXTERN RgSchUeCatTbl rgUeCatTbl;

/* rg007.201 - Changes for MIMO feature addition */
/* rg008.201 - Removed dependency on MIMO compile-time flag */
typedef U32 RgSchTbSzTbl[RGSCH_MAX_NUM_LYR_PERCW][RGSCH_NUM_ITBS][RGSCH_MAX_NUM_RB];

#ifdef LTE_TDD
typedef U8 RgSchRaPrmblToRaFrmTbl[RGSCH_MAX_TDD_RA_PREAMBLE_FMT+1];
#else
/* rg001.101: Added matrix 'rgRaPrmblToRaFrmTbl' for computation of RA
   sub-frames from preamble format */
typedef U8 RgSchRaPrmblToRaFrmTbl[RGSCH_MAX_RA_PREAMBLE_FMT+1];
#endif
EXTERN RgSchRaPrmblToRaFrmTbl rgRaPrmblToRaFrmTbl;

EXTERN U8 rgRvTable[4];

typedef struct rgDciFmt
{
   U8            dciType;
   union
   {
      RgDciFmt1AInfo   dci1a;
   } dci;
} RgDciFmt;

/**
  * @brief
  * Information about one PDCCH.
  */
typedef struct rgSchPdcch {
   CmLList        lnk;     /*!< To link PDCCHs in a subframe */
   U8             nCce;    /*!< CCE index */
   CmLteAggrLvl   aggrLvl; /*!< Aggregation level */
   TfuDciInfo     dci;     /*!< PDCCH format */
   U16            rnti;    /*!< RNTI to who the PDCCH is allocated */
   /*rg008.201 - Added support for SPS*/
#ifdef LTEMAC_SPS
   CmLteTimingInfo relFbkTiming; /*!< Feebback timing information for release
                                   PDCCH */
#endif
} RgSchPdcch;

/**
  * @brief
  * PDCCH information for cell.
  */
typedef struct rgSchPdcchInfo {
   U8             *map;    /*!< Bit map of PDCCHs */
   U16            nCce;    /*!< Total CCEs */
   CmLListCp      pdcchs;  /*!< List of RgSchPdcch */
} RgSchPdcchInfo;

typedef U8        RgSchPcfichInfo;

typedef struct rgSchPhich
{
   CmLList        lnk;        /*!< To link PHICHs in a subframe */
   U8             hqFeedBack; /*!< Harq Feed Back */
   U8             rbStart;    /*!< Starting RB */
   U8             nDmrs;      /*!< 3 bits for DMRS cyclic shift */
   /*rg009.201:[ccpu00116309]-changes for passing iphich at TFU;*/
#ifdef LTE_TDD
   U8             iPhich;     /*!< For determining phich group */
#endif
} RgSchPhich;

typedef struct rgSchPhichInfo
{
   CmLListCp      phichs;  /*!< List of RgSchPhich */
} RgSchPhichInfo;

typedef struct rgSchBcchTb
{
   RgSchPdcch     *pdcch;
   Buffer         *tb;
   U16            tbSize;
} RgSchBcchTb;

typedef struct rgSchPcchTb
{
   RgSchPdcch     *pdcch;
   Buffer         *tb;
   U16            tbSize;
} RgSchPcchTb;

typedef struct rgSchRaRspAlloc
{
   U16            raRnti;
   U32            tbSz;
   TknU8          backOffInd; /*!< Backoff index value */
   CmLListCp      raRspLst;   /*!< List of RaCbs */
   CmLListCp      contFreeUeLst; /*! List of HandOver or PdcchOrder UEs */
   RgSchPdcch     *pdcch;     /*!< NULLP if no Rsp allocation done for raRnti*/
}RgSchRaRspAlloc;

typedef struct rgSchBchTb
{
   Buffer         *tb;    /*!< BCH data for this frame */
   U16            tbSize; /*!< Non-Zero if bch data is scheduled for this SF */
}RgSchBchTb;

/*rg008.201 - Added support for SPS*/
#ifdef LTEMAC_SPS
/**
  * TODO: check compilation
  @brief Downlink Resource allocation type information. */
struct rgSchDlSfAllocInfo
{
  U32 raType0Mask;       /*!< RBG allocation mask for type 0*/
  U32 raType1Mask[RG_SCH_NUM_RATYPE1_32BIT_MASK]; /*!< RA Type 1
                                                    allocation mask */
  U32 raType1UsedRbs[RG_SCH_NUM_RATYPE1_32BIT_MASK];/*!< RA Type 1 Used RBs
                                                     per subset */
  U32 nxtRbgSubset;     /*!< Next RBG subset to be used for allocation */
  U32 raType2Mask[RG_SCH_NUM_RATYPE2_32BIT_MASK];
                        /*!< Mask for resource allocation type 2 */
};
#endif /* LTEMAC_SPS */
typedef struct rgSchDlSf
{
   U8                sfNum;      /*!< Number of the subframe */
   U16               bw;         /*!< Number of RBs in the cell */
   U16               bwAlloced;  /*!< Number of RBs already allocated by DLFS */
   U16               bwAssigned; /*!< Number of RBs already allocated by scheduler */
/* rg007.201 - Changes for MIMO feature addition */
/* rg008.201 - Removed dependency on MIMO compile-time flag */
   U8                type2Start; /*!< Start RB for the next type 2 allocation */
   U8                type2End;   /*!< End position of Type2 allocation with
                                      in the bit mask */
   U8                type0End;   /*!< End position of Type0 allocation with
                                      in the bit mask */
   U8                lstRbgDfct; /*!< The last RBG deficit RBs, Ex. if DLBW = 97,
                                  * RBGsz = 4, lstRbgDfct = 3 */
   Bool              txDone;     /*!< Flag to indicate if transmission is done*/
   RgSchBchTb        bch;        /*!< BCH data for this frame */
   RgSchBcchTb       bcch;       /*!< BCCH allocation for this frame */
   RgSchPcchTb       pcch;       /*!< PCCH allocation for this frame */
   RgSchPcfichInfo   pcfichInfo; /*!< PCFICH info */
   RgSchPdcchInfo    pdcchInfo;  /*!< PDCCH info */
   RgSchPhichInfo    phichInfo;  /*!< PHICH info */
   CmLListCp         tbs;        /*!< TBs or DL HARQ processes */
   U8                remUeCnt;   /*!< Remaining number of UEs that can be scheduled */
#ifdef LTE_TDD
   U8                   nCce;       /*!< Number of CCEs */
   RgSchTddPhichOffInfo phichOffInfo;  /*!< PHICH Information */
   RgSchTddDlFdbkInfo   dlFdbkInfo; /*!< HARQ Ack/Nack feedback expected time */
   RgSchTddUlAscInfo    ulAscInfo;  /*!< UL Association set Information */
   RgSchRaRspAlloc      raRsp[RGSCH_MAX_TDD_RA_RSP_ALLOC]; /* Array of list
                                                 of Ra Rsp
                                                 Allocations for each rarnti */
#else
   RgSchRaRspAlloc   raRsp[RGSCH_MAX_RA_RSP_ALLOC]; /* Array of list of Ra Rsp
                                                 Allocations for each rarnti */
#endif
   Void              *dlfsSf;     /*!< DLFS specific information per sub-frame */
   CmLListCp         ackNakRepQ;  /*!< ACK NACK repetition queue */
   /*rg008.201 - Added support for SPS*/
#ifdef LTEMAC_SPS
   RgSchDlSfAllocInfo dlSfAllocInfo;/*!< Allocation information for DL SF */
   U32               spsAllocdBw;   /*!< Number of RBs allocated for SPS */
   RgSchPdcch        *relPdcch;     /*!< Only one release PDCCH scheduled per
                                      sub-frame */
#ifdef LTE_TDD
   struct
   {
      CmLteRnti         crnti;       /*!< UE's C-RNTI (not SPS RNTI) */
      RgrTddAckNackMode ackNackMode; /*!< Ack/nack mode */
   } relPdcchInfo;                  /*!< Info required for receiving Hq fdbk
                                     *   for release PDCCH (DL) */
#endif
#endif
} RgSchDlSf;

/* Data Structures used for DL Allocations by Schedulers */
/**
  @brief Downlink Resource allocation type 0 information. */
typedef struct rgSchDlRbAllocRaType0
{
   U8 numDlAlloc;          /*!< Number of downlink allocations */
   U32 dlAllocBitMask;     /*!< Downlink allocations done for the UE */
} RgSchDlRbAllocRaType0;

/*rg008.201 - Added support for SPS*/
#ifdef LTEMAC_SPS
/**
  @brief Downlink Resource allocation type 1 information. */
typedef struct rgSchDlRbAllocRaType1
{
   U8 numDlAlloc;          /*!< Number of downlink allocations */
   U8 shift;               /*!< Indicates if the shift is triggered in the
                                allocation */
   U8 rbgSubset;           /*!< RBG subset number selected for the allocation*/
   U32 dlAllocBitMask;     /*!< Downlink allocations done for the UE */
} RgSchDlRbAllocRaType1;
#endif /* LTEMAC_SPS */
/**
  @brief Downlink Resource allocation type 2 information. */
typedef struct rgSchDlRbAllocRaType2
{
   Bool        isLocal;       /*!< True if localised VRB */
   U8          rbStart;       /*!< Starting RB */
   U8          numRb;         /*!< Number of RBs */
} RgSchDlRbAllocRaType2;

/* rg007.201 - Changes for MIMO feature addition */
/* rg008.201 - Removed dependency on MIMO compile-time flag */
/**
  @brief Downlink allocation details per TB. */
typedef struct rgSchDlTbAllocInfo
{
   Bool          schdlngForTb;/*!< Indicates if this TB has been scheduled */
   /*ccpu00120365:-ADD-is this TB disabled. Refer to 36.213-7.1.7.2 */
   Bool          isDisabled; /*!< Indicates if this TB is disabled */
   U32           bytesReq;    /*!< Number of bytes required to be allocated
                                 for each TB: filled in by RR/MAX C/I/PFS */
   U32           bytesAlloc;  /*!< Num of bytes allocated for each TB */
   U8            iTbs;        /*!< Itbs for allocation for this allocation
                               * of TB */
   U8            imcs;        /*!< Imcs for allocation for this allocation
                               * of TB */
   U8            noLyr;       /*!< No. of SM layers for this TB transmission */
   RgSchDlHqTbCb *tbCb;       /*!< address of TB Control Block */
}RgSchDlTbAllocInfo;

/* rg007.201 - Changes for MIMO feature addition */
/* rg008.201 - Removed dependency on MIMO compile-time flag */
/**
  @brief Downlink MIMO related allocation details. */
typedef struct rgSchMimoAllocInfo
{
   Bool    hasNewTxData;/*!< Indicator from Specific Sched to Common.
                         * Set, if UE has new data available for clubbing
                         * with RETX in case of TM3 and TM4 */
   Bool    swpFlg;      /*!< Swap Flag to indicate TB to CW association
                         * incase of 2 TB transmission */
   U8      precIdxInfo; /*!< Precoding index information stored */
   U8      numTxLyrs;   /*!< Number of SM layers scheduled for Transmission */
}RgSchMimoAllocInfo;

/**
  @brief Downlink RB allocation details. */
typedef struct rgSchDlRbAlloc
{
   CmLteRnti rnti;          /*!< RNTI for allocation: used only for RARs and
                                 dedicated transmissions */
/* rg007.201 - Changes for MIMO feature addition */
/* rg008.201 - Removed dependency on MIMO compile-time flag */
   U8 rbsReq;               /*!< Number of RBs required to be allocated:
                                 filled in by RR/MAX C/I/PFS */
   RgSchDlSf *dlSf;         /*!< DL sub-frame for which allocation is to be
                                 done: filled in by RR/MAX C/I/PFS */
   TfuDciFormat dciFormat;  /*!< DCI format for the allocation */
   U8 raType;               /*!< Resource allocation Type */
   RgSchPdcch *pdcch;       /*!< Pointer to allocated PDCCH */
   union allocInfoU
   {
      RgSchDlRbAllocRaType0 raType0; /*!< Resource allocation type 0 information */
      /*rg008.201 - Added support for SPS*/
#ifdef LTEMAC_SPS
      RgSchDlRbAllocRaType1 raType1; /*!< Resource allocation type 1
                                       information */
#endif
      RgSchDlRbAllocRaType2 raType2;  /*!< Resource allocation type 2 information */
   } allocInfo;             /*!< Resource allocation information */
/* rg007.201 - Changes for MIMO feature addition */
/* rg008.201 - Removed dependency on MIMO compile-time flag */
   U8 rbsAlloc;             /*!< Number of RBs allocated */
   CmLList reqLnk;          /*!< Link for list of UEs/RACBs to be scheduled */
   CmLList schdLstLnk;      /*!< Link for list of scheduled/non-scheduled
                                   allocations: used by RB allocation module*/
   U8      numRapids;       /*!< Number of RAPIDs serviced within RAR*/
   /*rg001.201:107637 Nprb indication at PHY for common Ch */
   U8      nPrb;            /*!< NPRB column num. either 2 or 3 */
   U32     raIndex;
   Bool    schdFirst;
   /* rg002.201: Add BI sub-header size to the tbSize requirement */
   U8      biEstmt;
/* rg007.201 - Changes for MIMO feature addition */
/* rg008.201 - Removed dependency on MIMO compile-time flag */
   RgSchMimoAllocInfo mimoAllocInfo; /*!< Mimo specific allocation params */
   RgSchDlTbAllocInfo tbInfo[2]; /*!< Allocation information for each TB. */
   /*rg008.201 - Added support for SPS*/
#ifdef LTEMAC_SPS
   U8      cqiForTx;     /*!< Assumed value of CQI at which transmission is
                           scheduled */
   RgSchDlSfAllocInfo resAllocInfo; /*!< Resource allocation information for
                                      the current allocation for
                                      RA type 0, 1 and 2 */
   Bool    isSchdBySps;  /*!< Indicates if this UE is already scheduled by SPS
                           module */
   CmLteTimingInfo spsSchdTime; /*!< Timing info at which UE was scheduled by
                                  SPS module */
#endif
} RgSchDlRbAlloc;

/**
  * @brief
  * Uplink Grant Information per UE.
  */
typedef struct rgSchUlGrnt
{
   U8           iMcs;       /*!< Original MCS index */
   U8           iMcsCrnt;   /*!< Current MCS index of the grant */
   U8           hop;        /*!< Hopping flag, set to 0 in this version */
   U8           rbStart;    /*!< Start Resource block of allocation */
   U8           numRb;      /*!< Number of resource blocks allocated */
   U8           tpc;        /*!< TPC command for the uplink grant */
   U8           nDmrs;      /*!< n1DMRS for the uplink grant */
   U8           delayBit;   /*!< Delay bit, for msg3 grant, set to 1 */
   /*rg003.201: Added for Uplink Adaptive retransmission */
   Bool         isRtx;      /*!< To indicate if this grant is for a RETX */
   /* rg001.201:107387: To include the length and ModOrder in DataRecp Req. */
   U16          datSz;      /*!< Length of the Data */
   TfuModScheme modOdr;     /*!< Modulation order */
} RgSchUlGrnt;


   /*rg003.201: Added for Uplink Adaptive retransmission */
/**
  * @brief
  * Uplink retransmission allocation.
  */
struct rgSchUlRetxAlloc
{
   CmLteRnti       rnti;      /*!< RNTI */
   U8              numSb;     /*!< Number of subbands */
   U32             tbSz;      /*!< Transmission Block Size */
   U8              iMcs;      /*!< Original MCS index */
   RgSchUeCb       *ue;       /*!< UE assocated with allocation */
   Bool            forMsg3;   /*!< If allocation is for Msg3 */
   /* rg004.201: ccpu00105914: PHR handling for MSG3 */
   RgSchRaCb       *raCb;     /*!< Pointer to RaCb, if forMsg3 = TRUE */
   /* MS_FIX: syed Adaptive Msg3 Retx crash. */
   CmLListCp       *reTxLst;  /*!< Pointer to container List */
};

/**
  * @brief
  * Hole information, pertaining to uplink bandwidth
 */
typedef struct rgSchUlHole
{
   U8 start;      /*!< Beginning of hole */
   U8 num;        /*!< Number of elements making up the hole */
   struct rgSchUlHole *prv; /*!< Previous hole */
   struct rgSchUlHole *nxt; /*!< Next hole */
   struct rgSchUlAlloc *nxtAlloc; /*!< Next allocation */
   struct rgSchUlAlloc *prvAlloc; /*!< Previous allocation */
} RgSchUlHole;

/**
  * @brief
  * Uplink allocation
  */
struct rgSchUlAlloc
{
   RgSchUlHqProcCb *hqProc;         /*!< HARQ process */
   CmLteRnti       rnti;            /*!< RNTI */
   U8              sbStart;         /*!< Subband start idx */
   U8              numSb;           /*!< Number of subbands */
   RgSchUlGrnt     grnt;            /*!< Grant info */
   /* Not keeping grant attributes now */
   RgSchPdcch      *pdcch;          /*!< Associated PDCCH */
   RgSchUeCb       *ue;             /*!< UE assocated with allocation */
   Bool            forMsg3;         /*!< If allocation is for Msg3 */
   /* rg004.201: ccpu00105914: PHR handling for MSG3 */
   RgSchRaCb       *raCb;           /*!< RaCb, if forMsg3 = TRUE */
   Bool            mrgdNewTxAlloc;  /*!< Alloc corresponding to newTx
                                     *   that was merged in the process of
                                     * comdining txSf and reTxSf. */
   RgSchUlAlloc    *prv;            /*!< Previous alloc */
   RgSchUlAlloc    *nxt;            /*!< Next alloc */
   RgSchUlHole     *prvHole;        /*!< Next hole */
   RgSchUlHole     *nxtHole;        /*!< Prev hole */
};

/**
  * @brief
  * Allocation free pool for an allocation db
 */
typedef struct rgSchUlAllocMem
{
   U8            maxAllocs;     /*!< Maximum possible allocations per subframe */
   RgSchUlAlloc  *firstFree;    /*!< First free index */
   RgSchUlAlloc  *allocs;       /*!< Array of 'maxAllocs' elems */
} RgSchUlAllocMem;

/**
  * @brief
  * Allocation information for a subframe
 */
typedef struct rgSchUlAllocDb
{
   U8              count;      /*!< Number of allocations */
   RgSchUlAlloc    *first;     /*!< First allocation */
   RgSchUlAllocMem mem;        /*!< Alloc pool management */
} RgSchUlAllocDb;

/**
  * @brief
  * Hole free pool for a hole db
 */
typedef struct rgSchUlHoleMem
{
   U8           maxHoles;     /*!< Maximum possible holes per subframe */
   RgSchUlHole  *firstFree;    /*!< First free index */
   RgSchUlHole  *holes;        /*!< Array of 'maxHoles' elems */
} RgSchUlHoleMem;

/**
  * @brief
  * Holes information for a subframe
  */
typedef struct rgSchUlHoleDb
{
   U8             count;           /*!< Number of holes */
   RgSchUlHole    *first;       /*!< First hole */
   RgSchUlHoleMem mem;        /*!< Hole pool management */
} RgSchUlHoleDb;

/**
  * @brief
  * Uplink subframe information.
  */
typedef struct rgSchUlSf
{
   U8               idx;      /*!< Identifier for uplink subframe (range: 0-7)
                                - maps to HARQ proc ID */
   CmLListCp        reTxLst;  /*!< Retransmission List*/
#ifdef LTE_TDD
   RgSchTddPuschOffInfo puschOffset; /*!< PUSCH offset information */
#endif
#ifdef LTE_L2_MEAS
   U32              totPrb;    /*!< Total PRB used in this sub frame */
#endif
#ifdef MSPD
   /* MS_FIX:ccpu00120610 add a counter to keep track of remaining allocations */
   U8               *allocCountRef; /*!< Allocation count in this
                                *  subframe, it is a reference to the actual
                                *  counter held in allocDb (no additional
                                *  logic needed to maintain this) */
#endif
   /*rg003.201: Added for Uplink Adaptive retransmission */
   RgSchUlAllocDb   *allocDb;  /*!< Allocation info */
   RgSchUlHoleDb    *holeDb;   /*!< Holes info */
} RgSchUlSf;

/*rg001.201:107637 Nprb indication at PHY for common Ch */
/**
  * @brief
  * Bo report given for common logical channel.
  */
typedef struct rgSchClcBoRpt
{
   CmLList         boLstEnt;  /*!< Linked list entity for BO list */
   U32             bo;        /*!< Bo in bytes */
   CmLteTimingInfo timeToTx;  /*!< Time at which the BO needs to be satisfied
                                   (data to be sent to PHY) */
   CmLteTimingInfo maxTimeToTx;
   U8              retxCnt;
   U16             i;
   U8              nPrb;      /*! NPRB column. Allowed values {2,3} */
   /* rg002.201: ccpu00108811: Corrected allocation for common channels */
   U8              mcs;
} RgSchClcBoRpt;

/**
  * @brief
  * Logical channel control block for BCCH and PCCH channels.
  */
typedef struct rgSchClcDlLcCb
{
   U8              lcId;                     /*!< Logical channel ID */
   Bool            si;
   CmLListCp       boLst;                    /*!< List of BOs reported (RgSchClcBoRpt) */
} RgSchClcDlLcCb;

/**
  * @brief
  * Transmission offset configuration for SRS.
  */
typedef struct rgSchSrsTxOffst
{
   U8 count;                       /*!< Number of offsets in the array */
   U8 offst[RGSCH_MAX_SRS_TX_OFFSET]; /*!< SRS TX offset value */
} RgSchSrsTxOffst;

/**
  * @brief
  * Cell Control block per cell.
  */
typedef struct rgSchSrsCfg
{
   RgrSrsCfgPrd    srsCfgPrdEnum; /*!< SRS configuration period (in subframes) */
   RgrSrsBwCfg     srsBwEnum;     /*!< SRS Bandwidth configuration per cell.*/
   RgSchSrsTxOffst srsTxOffst;    /*!< Transmission offset for SRS */
/*ccpu00116923 - ADD - SRS present support*/
#ifdef TFU_UPGRADE
    U8           srsSubFrameCfg;/*!< SRS subframe configuration index per cell.
                                     Range - [0-15] */
#endif
} RgSchSrsCfg;

/**
  * @brief
  * MAC Configuration element for CRG interface.
  */
typedef struct rgSchCfgElem
{
   CmLList         cfgReqLstEnt;  /*!< Linked list entry for cfgReqLst */
   CmLteTimingInfo actvTime;      /*!< Activation time to apply configuration */
   struct rgrCfgS
   {
      Region reg;
      Pool   pool;
      RgrCfgReqInfo   *rgrCfgReq;    /*!< Configuration request information from
                                       RRM */
      RgrCfgTransId   transId;       /*!< Transaction ID for the config request */
   } rgrCfg;                         /*!< RGR configuration information */
} RgSchCfgElem;


/**
  * @brief
  * Configuration Control block per cell.
  */
typedef struct rgSchCfgCb
{
   CmLListCp  crntRgrCfgLst;    /*!< List of config requests recieved in the
                                     current TTI from RRM */
   CmLListCp  pndngRgrCfgLst;  /*!< List of pending config requests recieved in the
                                     previous TTIs from RRM */
} RgSchCfgCfb;

/**
  * @brief
  * Link of RNTIs managed by MAC.
  */
typedef struct rgSchRntiLnk
{
   U16 rnti;                /*!< RNTI */
   struct rgSchRntiLnk *prv;   /*!< Link to previous RNTI */
   struct rgSchRntiLnk *nxt;   /*!< Link to next RNTI */
} RgSchRntiLnk;

/**
  * @brief
  * Database of RNTIs managed by MAC.
  */
typedef struct rgSchRntiDb
{
   U16  rntiStart;         /*!< Start RNTI */
   U16  maxRntis;          /*!< Maximum number of RNTIs managed by MAC */
   RgSchRntiLnk *freeRnti;   /*!< first free RNTI in the list */
   RgSchRntiLnk *lastRnti;   /*!< last RNTI in the list */
   RgSchRntiLnk *rntiPool;   /*!< Linked list of RNTIs */
} RgSchRntiDb;

/**
  * @brief
  * Random access information per cell.
  */
typedef struct rgSchRaInfoCb
{
#ifdef LTE_TDD
   U8              maxRaSize;                /*!< Number of RA-RNTIs stored
                                                  across radio frames */
   U8              lstSize;                  /*!< list size */
   CmLListCp       *raReqLst;                 /*!< Each array elem is linked
                                                   list of RgRaReqInfo
                                                   (per ra-rnti) */
#else
   CmLListCp       raReqLst[RGSCH_MAX_RA_RNTI];  /*!< Each array elem is linked
                                                   list of RgRaReqInfo
                                                   (per ra-rnti) */
#endif
   CmLListCp       raCbLst;                   /*!< List of RaCbs */
   CmLListCp       toBeSchdLst;               /*!< List of RaCbs to be
                                                   scheduled */
} RgSchRaInfoCb;

/**
  * @brief
  * Uplink HARQ process information per UE
  */
struct rgSchUlHqProcCb
{
   U8                ndi;          /*!< NDI */
   U8                remTx;        /*!< Number of remaining transmissions */
   RgSchUlAlloc      *alloc;       /*!< Uplink allocation */
   /* rg005.201 Renamed rcvdDatInd to rcvdCrcInd */
   Bool              rcvdCrcInd;   /*!< Set to true when data rcvd, false when
                               decode failure. */
   U8                rvIdx;        /*!< Redundancy version index */
   TknU8             rvIdxPhy;     /*!< Whatever is given by PHY. To be inspected in case
                               of NACK.Always initialised to 'not present' */
   CmLList           reTxLnk;      /*!< Retransmission List */
   RgSchUlRetxAlloc  reTxAlloc;    /*!< Retransmission allocation
                                        information */
   Bool              isRetx;       /*!< Awaiting retransmission */
   U8                procId;       /*!< HARQ Process ID */
   U8                ulSfIdx;      /*!< UL Subframe Index */
#ifdef LTE_TDD
   /*rg009.201:[ccpu00116309]:Changes for passing iPhich at TFU*/
   U8                iPhich;       /*!< Needed to Calculate PHICH
                                        location. For TDD only */
#endif
};

/**
  * @brief
  * Uplink HARQ entity information per UE.
  */
typedef struct rgSchUlHqCb
{
   U8           maxHqRetx;        /*!< Maximum number of harq
                                   * re-transmissions */
   U8              numHqPrcs; /*!< Number of HARQ Processes */
#ifdef LTE_TDD
   RgSchUlHqProcCb *hqProcCb; /*!< Uplink harq process info */
#else
   RgSchUlHqProcCb hqProcCb[RGSCH_NUM_UL_HQ_PROC]; /*!< Uplink harq process info */
#endif
} RgUeUlHqCb;

/**
  * @brief
  * Logical channel group.
  */
#ifdef LTE_L2_MEAS
/**
 * @brief
 * Structure to store values for Ra Preambles received values
 * */
typedef struct rgSchRaPreambles
{
   U8               dedPream;    /*!< Dedicated RA Preamble received */
   U8               preamGrpA;   /*!< Randomly selected preambles in low range */
   U8               preamGrpB;   /*!< Randomly selected preambles in high range */
}RgSchRaPreambles;

/**
 * @brief strutcure to store Avergae PRB usage for a given Time period in
 * Downlink.
 * */
typedef struct rgSchAvgPrbDl
{
   Bool             cntActive;   /* Set to TRUE if counting activated */
   CmLteTimingInfo  startTime;   /*!< Start Time */
   U8               timePrd;     /*!< Period For which meas to be done */
   U32              prbCount;    /*!< PRB Count Updated every Per TTI */
} RgSchAvgPrbDl;

/**
 * @brief strutcure to store Avergae PRB usage for a given Time period in
 * Uplink .
 * */
typedef struct rgSchAvgPrbUl
{
   Bool             cntActive;   /*!< Set to TRUE if counting activated */
   CmLteTimingInfo  startTime;   /*!< Start Time */
   U8               timePrd;     /*!< Period For which meas to be done */
   U32              prbCount;    /*!< PRB Count Updated every Per TTI */
} RgSchAvgPrbUl;

/** @brief
 *  L2 measurement control block.
 *  */
struct rgSchL2MeasCb
{
   CmLList            measLnk;       /*!< MeasCb node */
   LrgSchMeasReqInfo  measReq;       /*!< Request received for measurement */
   CmLteTimingInfo    startTime;     /*!< Time at which measurement started */
   LrgAvgPrbQCICfm    avgPrbQciUl;   /*!< Used to store result from MAC */
   Bool               cfmRcvd;       /*!< Used to check if confirm received from MAC */
                                     /*!< For average PRB usage in UL */
};
/** @brief
 *  Control block to store measurement details per QCI.
 *  */
struct rgSchQciCb
{
   U8     qci;              /*!< QCI of the Logical Channel */
   U32    dlPrbCount;       /*!< Cumulative Prb Count for this QCI */
   U32    dlUeCount;        /*!<  Cumulative number of active UE's */
   U32    dlTotal_UeCount;  /*!< Cummulative count added for every sampling
                              Occasion*/
   U32    ulUeCount;        /*!<  Cumulative number of active UE's */
   U32    ulTotal_UeCount;  /*!< Cummulative count added for every sampling
                              Occasion*/
};
#endif /* LTE_L2_MEAS */
struct rgSchLcgCb
{
   /* Right now not keeping associated logical channels, searching for
    * associated channels needed only during config */
   U8    lcgId;  /*!< Group ID */
   Void  *sch;
#ifdef LTE_L2_MEAS
   U8               numLch;                    /*!< Number fo LC's for this LCG*/
   RgSchUlLcCb      *lcArray[RGSCH_MAX_LC_PER_UE]; /*!< Dedicated Uplink logical
                                                 channel per LCG */
#endif /* LTE_L2_MEAS */
   /*rg008.201 - Added support for SPS*/
#ifdef LTEMAC_SPS
   U8    lcCnt;
   U8    lcId[RGR_MAX_SPS_LC];
#endif
};

/**
  * @brief
  * Uplink Dedicated Logical channel control block.
  */
struct rgSchUlLcCb
{
   U8            lcId;    /*!< Logical channel ID */
   RgSchLcgCb    *lcg;    /*!< Logical channel group */
   RgrLchQosCfg  ulQos;   /*!< UL Qos parameters */
   /* After Merging from 2.1 to 2.2 */
#ifdef LTE_L2_MEAS
   RgSchQciCb   *qciCb; /* QCI Details for this LC */
#endif
};

/**
  * @brief
  * Ul Grnt Info for Ho/Po UE in RAR.
  */
typedef struct rgSchHoPoUeGrnt
{
   U8      rapId;         /*!< dedicated rapId */
   U8      hop;           /*!< Hopping flag, set to 0 in this version */
   U8      rbStart;       /*!< Start Resource block of allocation */
   U8      numRb;         /*!< Number of resource blocks allocated */
   U8      tpc;           /*!< TPC command for the uplink grant */
   U8      iMcsCrnt;      /*!< Current MCS index of the grant */
   TknU16  ta;            /*!< Timing Adjustment */
   CmLList raRspLnk;      /*!< To link UE to RaRsp cont free list */
   /*rg008.201 - Added support for SPS*/
   U16     datSz;      /*!< Length of the Data */
} RgSchHoPoUeGrnt;

/**
  * @brief
  * Uplink control block for UE specific information.
  */
typedef struct rgSchUeUlCb
{
   RgUeUlHqCb       hqEnt;      /*!< Uplink HARQ information for the UE */
   RgSchUlLcCb      *lcCb[RGSCH_MAX_LC_PER_UE]; /*!< Dedicated Uplink logical channel
                                              information */
   RgSchLcgCb       lcgArr[RGSCH_MAX_LCG_PER_UE]; /*!< Logical channel groups */
   U8               ulInactvMask; /*!< Bitmask indicating if UE is inactive for UL scheduling */
   CmLList          ulInactvLnk;  /*!< Link to the inactive UE List for UL */
   RgSchHoPoUeGrnt  rarGrnt;      /*!< UE's RAR grant Information */
/* rg007.201 - Changes for MIMO feature addition */
/* rg008.201 - Removed dependency on MIMO compile-time flag */
   RgrUeTxAntSelCfg ulTxAntSel;/*!< UL Transmit antenna selection Cfg Info */
#ifdef RGR_V1
   /* rg006.201: [ccpu00112398] Added periodic BSR timer */
   RgrUeBsrTmrCfg   bsrTmrCfg;    /*!< BSR timers configuraiton for the UE */
#endif
   /*rg008.201 - Added support for SPS*/
#ifdef LTEMAC_SPS
   RgrUeSpsUlCfg    ulSpsCfg;       /*!< UL SPS configuration information */
   U8               explicitRelCnt; /*!< Number of subframes after sending
                                         release to consider it successful */
   U16              spsPrdcty;      /*!< Shall store the SPS periodicity of the
                                         UE. Needed to round-off in case of
                                         TDD */
#endif
#ifdef LTE_L2_MEAS
   U32                  nPrb;      /*!< Number of resource block allocated */
#endif
#ifdef TFU_UPGRADE
   U8                betaHqOffst; /*!< Delta HARQ offset used in the case where HARQ
                             feedback is multiplexed with PUSCH data.  */
   U8                betaCqiOffst;/*!< Beta offset for CQI[0-15]. Given by RRC*/
   U8                betaRiOffst;/*!Beta offset for RI[0-15]. Given by RRC */
#endif
} RgSchUeUlCb;

/**
  * @brief
  * Downlink CCCH Tx/Retx schedule information.
  */
typedef struct rgSchCcchSchdInfo
{
   U32     totBytes;       /*!< Total bytes allocated */
   CmLList retxLnk;        /*!< To link hqP to retx Queues */
   U8      rvIdx;          /*!< Rv Index */
} RgSchCcchSchdInfo;

/**
  * @brief
  * Downlink Grant Information per UE.
  */
typedef struct rgSchDlGrnt
{
   U16     schdTime;       /*!< Time at which Harq proc has been scheduled */
   U8      rbStrt;         /*!< Starting RB of the allocation */
   U8      numRb;          /*!< Number of RBs allocated */
   U8      iMcs;           /*!< Index to the MCS */
   U8      rv;             /*!< RV for HARQ (re)transmission */
} RgSchDlGrnt;

/**
  * @brief
  * Logical channel data as being scheduled by the scheduler.
  */
typedef struct rgSchLchAllocInfo
{
   U8       lcId;       /*!< Logical channel ID */
   U32      schdData;   /*!< Scheduled logical channel data */
} RgSchLchAllocInfo;

#ifdef LTEMAC_DRX
/** @brief This structure is part of the downlink HARQ process, this structure
 * encapsulates all the DRX related information.
 */
typedef struct rgSchDrxDlHqProcCb
{
   CmLList harqRTTEnt;   /*!< Entry into the HARQ RTT timer list */
   CmLList harqRetxEnt;   /*!< Entry into the harqRetxQ */

   U16     rttIndx;    /*!< Current Index into HARQ RTT Q */
   U16     reTxIndx;   /*!< Current Index into Re-Tx Q */


} RgSchDrxDlHqProcCb;
#endif /*LTEMAC_DRX*/

/* rg007.201 - Changes for MIMO feature addition */
/* rg008.201 - Removed dependency on MIMO compile-time flag */
/** @enum TfuDciFormat
 * This Enum has values for Harq Proc TB state.
 */
typedef enum
{
   HQ_TB_ACKED=0,
   HQ_TB_NACKED,
   HQ_TB_WAITING
}RgSchHqTbState;
/**
  * @brief
  * Downlink TB information per DL HqProc.
  */
struct rgSchDlHqTbCb
{
   U8                tbIdx;          /*!< Index of this TB (0/1). Initialized
                                          at HqEnt allocation */
   U32               tbSz;           /*!< TB size scheduled for this TB */
   U8                txCntr;         /*!< Transmission counter */
   U8                ndi;            /*!< New Data Indicator 0 or 1*/
   TknU8             schdTa;      /*!< TA scheduled to be sent */
   /*rg009.201 - CR timer implementation changes*/
   U8                contResCe;
   U8                numLch;         /*!< No. of DL Logical Channels scheduled
                                          for this TB */
   CmLteTimingInfo   timingInfo;     /*!< This field holds the scheduled time */
   RgSchLchAllocInfo lchSchdDataArr[RGSCH_MAX_NUM_DED_LC];
   RgSchLchAllocInfo *lchSchdData;
   RgSchCcchSchdInfo ccchSchdInfo;/*!< CCCH TX/RETX scheduler Info */
   U8                fbkRepCntr;     /*!< Counter to ANRep Feedback */
   U8                fbkRecpRepCntr; /*!< Counter to ANRep Feedback */
   CmLList           anRepLnk[6];    /*!< Links for ANRep we need
                                          these many links to
                                          simultaneously hold the
                                          hqProcs in multiple
                                          subframes. */
   RgSchDlSf         *crntSubfrm[6]; /*!< Pointer to subframes */
   U8          ackCount;   /*!< Counter for ACKs */
   /* rg004.201 patch - ccpu00110457 DTX Changes Start */
   U8          isAckNackDtx; /* ACK or NACK or DTX*/
   U8          nackCount;  /* NACK Counter */
   U8          dtxCount;   /* DTX Counter */
   /* rg004.201 patch - ccpu00110457 DTX Change */
   RgSchDlGrnt       dlGrnt;      /*!< Scheduler grant */
   CmLList           sfLnk;       /*!< Used to queue it up on a subfrm */
   Bool              taSnt;       /*!< TA sent or not */
#ifdef LTE_TDD
   CmLteTimingInfo   fdbkTime;    /*!< Expected feedback time */
   U8                m;           /*!< Subframe Order Index within the subframe
                                       downlink association index K */
#endif
   /* 3.1 MIMO */
   U8                numLyrs;     /*!< Number of layers using which this TB is
                                       being transmitted */
   RgSchHqTbState    state;       /*!< State of this Trasport Block */
   RgSchDlHqProcCb   *hqP;        /*!< Reference to the TB container */
   RgSchPdcch        *pdcch;      /* Pdcch Allocated for this Hq TB */
   RgSchDlSf         *subFrm;     /* Subfrm for which this TB is assgnd */
   /* rg003.301 -Add-[ccpu00120794] - Freeing up the HARQ proc blocked for
    * indefinite time in case of Retx */
   U8                cntrRetxAllocFail; /*!< Number of times allocation failed
                                            for the retransmission of this TB */
};

/**
  * @brief
  * Downlink HARQ process information per UE.
  */
struct rgSchDlHqProcCb
{
   CmLList           lnk;         /*!< To link with other HARQ processes */
   RgSchDlHqEnt      *hqE;        /*!< HARQ entity */
   /* rg007.201  - Protected under flag*/
#ifdef LTE_TDD
   U8                txCntr;      /*!< Transmission counter */
#endif
   U8                procId;      /*!< Harq process ID */
   Void              *sch;        /*!< Cmn sched Hq control block */
   /* 3.1 MIMO */
   RgSchDlHqTbCb     tbInfo[2];   /*!< TB related information for 1+1(if any) TBs */
#ifdef LTEMAC_DRX
   RgSchDrxDlHqProcCb   drxCb;   /*!< DRX control block */
#endif
/* rg009.201. Added changes of TFU_UPGRADE */
/* rg002.301 - Del - [ccpu00120233] rcptDone flag; ADD - TB Count*/
#ifdef TFU_UPGRADE
   U8                tbCnt;     /*!< Count of the number TBs being sent with a
                                  Reception Requesti */
#endif
#ifdef LTEMAC_SPS
   TknU16       spsN1PucchRes;   /*!< N1Pucch resource for HARQ process at SPS
                                      ocassions: present only when HqP contains
                                      Data without PDCCH */
#endif
};

/**
  * @brief
  * Downlink HARQ entity information per UE.
  */
struct rgSchDlHqEnt
{
   RgSchRaCb    *raCb;     /*!< Parent RACB when attached to RACB */
   RgSchUeCb    *ue;       /*!< Parent UE */
   CmLListCp    free;      /*!< List of free HARQ processes */
   CmLListCp    inUse;     /*!< List of in-use HARQ processes */
   U8           maxHqTx;   /*!< Maximum number of harq transmissions */
   RgSchDlHqProcCb *msg4Proc; /*!< Points to MSG4 HARQ process */
#ifdef RGR_V1
   /*rg009.201 - CR timer changes*/
   RgSchDlHqProcCb *ccchSduProc; /*!< Points to CCCH SDU HARQ process
				  to identify feedback for CCCH SDU
				  transmissions done without cont res CE*/
#endif
   U8              numHqPrcs; /*!< Number of HARQ Processes */
#ifdef LTE_TDD
   RgSchDlHqProcCb *procs; /*!< Downlink harq processes */
#else
   RgSchDlHqProcCb procs[RGSCH_NUM_DL_HQ_PROC]; /*!< Downlink harq processes */
#endif
   Void         *sch;      /*!< Scheduler specific Info */
};


/**
  * @enum rgSchTaState
  * Enumeration of time alignment states.
  */
typedef enum rgSchTaState
{
   RGSCH_TA_IDLE = 0,      /*!< TA is to be scheduled */
   RGSCH_TA_TOBE_SCHEDULED,/*!< TA is to be scheduled */
   RGSCH_TA_SCHEDULED      /*!< TA is scheduled */
} RgSchTaState;

/**
  * @brief
  * Downlink Time alignment information per UE.
  */
typedef struct rgSchUeDlTaCb
{
  U16       cfgTaTmr; /*!< Configured TA timer value */
  U8        ta;       /*!< TA value for UE */
  U8        numRemSf; /*!< Number of subframes left to apply TA */
  RgSchTaState state;    /*!< Indicates HARQ state for TA */
} RgSchUeDlTaCb;

/**
  * @brief
  * Downlink Dedicated Logical channel control block.
  */
struct rgSchDlLcCb
{
   U8            lcId;    /*!< Logical channel ID */
   U32           bo;
   RgSchUeCb     *ue;     /* UE to which this LC belongs to */
#ifdef LTE_L2_MEAS
   RgSchQciCb    *qciCb;  /*!< Pointer to ACI Control Block */
#endif /* LTE_L2_MEAS */
   /*rg008.201 - Added support for SPS*/
#ifdef LTEMAC_SPS
   RgrLchSpsCfg   dlLcSpsCfg;  /*!< SPS configuration for DL logical channel */
#endif
#ifdef CCPU_OPT
   U16           estRlcHdrSz;  /*!< Estimated RLC header size */
   Bool          staPduPrsnt;  /*!< Indicate whether the BO includes
                                    a status PDU aswell */
#endif
   Void    *sch;    /*!< Holds Scheduler Info pointer */
};

/**
  * @brief
  * Downlink control block for UE specific information.
  */
typedef struct rgSchUeDlCb
{
   RgSchDlHqEnt  *hqEnt;        /*!< Downlink HARQ information for the UE */
   RgSchUeDlTaCb taCb;         /*!< Downlink timing adjustment information for the
UE */
   RgSchDlLcCb   *lcCb[RGSCH_MAX_LC_PER_UE];/*!< Dedicated Downlink logical channels in
UE */
   RgrUeDlCqiCfg  ueDlCqiCfg;    /*!< UE DL CQI config */
   RgSchDlRbAlloc dlAllocCb;     /*!< RB Allocation Info for Ded Trans/Retrans */
   U8             dlInactvMask;  /*!< Bitmask indicating if UE is inactive for DL scheduling */
   Bool           reqForCqi;     /*!< set to True if Aperiodic CQI from UE is required */
   CmLList        dlInactvLnk;   /*!< Link to the inactive UE List for DL */
#ifdef LTE_TDD
   RgrTddAckNackMode ackNackMode;   /*!< ACK/NACK Bundling/ ACK/NACK Multiplexing */
   RgSchTddANInfo    *anInfo;       /*!< ACK/NACK related Information */
   U8                nextFreeANIdx; /*!< Next Available ANInfo Index */
#endif
   /*rg008.201 - Added support for SPS*/
#ifdef LTEMAC_SPS
   RgrUeSpsDlCfg  dlSpsCfg;         /*!< DL SPS configuration information */
   Bool           isSpsHqPInUse;    /*!< Indicates if SPS HARQ procs are in use
                                         or are to be used for SPS
                                         transmissions */
   RgSchPdcch      spsOccPdcch;      /*!< PDCCH information for SPS ocassion */
#endif
} RgSchUeDlCb;

/** @brief This structure contains the Measurement gap configuration for an UE.
 */
typedef struct rgUeMeasGapCfg
{
   Bool isMesGapEnabled;    /*!< Is Measuremnet gap enabled or disabled */
   U8   gapPrd;             /*!< Gap period 40ms/80ms */
   U8   gapOffst;           /*!< Gap offset - Vaue is 0 to 1*/
} RgUeMeasGapCfg;

/**
  @brief Measurement Gap related information per UE. */
typedef struct rgSchUeMeasGapCb
{
   Bool              isMesGapEnabled;/*!< TRUE if Measurement gap is enabled for this UE */
   U8                isMeasuring;  /*!< Set to TRUE during measurement gap */
   U8                gapPrd;     /*!< Measurement gap period configuration for the UE */
   U8                gapOffst;   /*!< Measurement gap offset for the UE */
   CmLList           measQLnk;   /*!< To Link to the measurement gap list */
   CmLList           ackNakQLnk; /*!< To Link to the ACK NACK Rep list */
   CmTimer           measGapTmr;  /*!< Timer for Measurement Gap */
   CmTimer           measGapUlInactvTmr; /*!< UL Inactive timer for measurement gap */
   CmTimer           measGapDlInactvTmr; /*!< DL Inactive timer for measurement gap */
} RgSchUeMeasGapCb;

/**
  @brief ACK-NACK repetition related information per UE. */
typedef struct rgSchUeAckNakRepCb
{
   Bool        isAckNackEnabled;  /*!< Is ACK/NACK Enabled*/
   U8          isAckNakRep;  /*!< Set to TRUE during ACK-NACK repetition prd */
   U8          cfgRepCnt;         /*!< Configured value for the repetition counter */
   U8          repCntr;           /*!< Actual repetition counter */
   U16         pucchRes;          /*!< PUCCH resource for repetition */
   CmTimer     ackNakRepUlInactvTmr; /*!< UL Inactive timer for ack-nack repetition */
   CmTimer     ackNakRepDlInactvTmr; /*!< DL Inactive timer for ack-nack repetition */
   CmTimer     ackNakRepTmr;      /*!< Timer for ack-nack repetition */
   CmLList     ackNakRepLnk;      /*!< ACK NACK repetition queue link */
   CmLListCp   *prsntQ;   /*!< Pointer to the Queue that this UE is current
                            present in. */
} RgSchUeAckNakRepCb;

/* rg007.201 - Changes for MIMO feature addition */
/* rg008.201 - Removed dependency on MIMO compile-time flag */
/**
  * @brief
  * UE's MIMO specific information.
  */
typedef struct rgSchUeMimoInfo
{
   RgrTxMode          oldTMode;     /*!< UE's Previous Transmission Mode */
   RgrTxMode          txMode;       /*!< UE's Transmission Mode */
   TknU32             doa;          /*!< DOA indicator for this UE */
   Bool               puschFdbkVld; /*!< True if Precoding Info in PDCCH has to be
                                         in-accordance with the latest PUSCH report */
   TfuDlCqiPuschInfo  puschPmiInfo; /*!< PUSCH report details for explicit PMI
                                       * information to PHY during a PDSCH */
   RgrCodeBookRstCfg  cdbkSbstRstrctn; /*!< Codebook subset restriction defined as per
                                       * 36.331 section 6.3.2. As of now, this info
                                       * is not processed by MAC. SCH shall use the
                                       * PMI reported by UE unconditionally.*/
#ifdef MSPD
   /* MS_WORKAROUND: introduce ulcount to automatically switch to SM from TD */
   U8          ulcount;
#endif

}RgSchUeMimoInfo;

/* rg009.201. Added changes of TFU_UPGRADE */
#ifdef TFU_UPGRADE

/** @brief This structure that stores the length of Bits that
*     will be received over PUSCH for Aperiodic Mode 3-1.
*/
typedef struct rgSchCqiRawPuschMode31
{
   U8                            wideBCqiCw0;    /*!< Length of Wideband CQI Codeword 0 */
   U8                            totLenSbDiffCqiCw0;    /*!< Length of SubBand Differential CQI Codeword 0 */
   U8                            r1WideBCqiCw1;    /*!< Length of Wideband CQI Codeword 1 for Rank =1*/
   U8                            r1TotLenSbDiffCqiCw1;    /*!< Length of SubBand Differential CQI Codeword 1 for Rank = 1*/
   U8                            rg1WideBCqiCw1;    /*!< Length of Wideband CQI Codeword 1 for Rank > 1*/
   U8                            rg1TotLenSbDiffCqiCw1;    /*!< Length of SubBand Differential CQI Codeword 1 for Rank > 1*/
   U8                            r1PmiBitLen;   /*!< Length of PMI Bits for Rank = 1*/
   U8                            rg1PmiBitLen;   /*!< Length of PMI Bits for Rank > 1*/
} RgSchCqiRawPuschMode31;

/** @brief This structure that stores the length of Bits that
*     will be received over PUSCH for Aperiodic Mode 3-0.
*/
typedef struct rgSchCqiRawPuschMode30
{
   U8                     wideBCqiCw;    /*!< Length of Wideband CQI */
   U8                     totLenSbDiffCqi;    /*!< Length of SubBand Differential CQI */
} RgSchCqiRawPuschMode30;

/** @brief This structure that stores the length of Bits that
*     will be received over PUSCH for Aperiodic Mode 2-2.
*/
typedef struct rgSchCqiRawPuschMode22
{
   U8                            wideBCqiCw0;    /*!< Length of Wideband CQI Codeword 0 */
   U8                            sBDiffCqiCw0;    /*!< Length of SubBand Differential CQI Codeword 0 */
   U8                            r1WideBCqiCw1;    /*!< Length of Wideband CQI Codeword 1  for Rank =1 */
   U8                            r1SbDiffCqiCw1;    /*!< Length of SubBand Differential CQI Codeword 1 for Rank =1*/
   U8                            rg1WideBCqiCw1;    /*!< Length of Wideband CQI Codeword 1  for Rank > 1*/
   U8                            rg1SbDiffCqiCw1;    /*!< Length of SubBand Differential CQI Codeword 1 for Rank >1*/
   U8                           posOfM;   /*!< Position of M selected SubBands */
   U8                           r1PmiBitLen;   /*!< Length of PMI Bits for Rank =1*/
   U8                           rg1PmiBitLen;   /*!< Length of PMI Bits for Rank >1*/
} RgSchCqiRawPuschMode22;

/** @brief This structure that stores the length of Bits that
*     will be received over PUSCH for Aperiodic Mode 2-0.
*/
typedef struct rgSchCqiRawPuschMode20
{
   U8                     wideBCqiCw;    /*!< Length of Wideband CQI */
   U8                     subBandDiffCqi;    /*!< Length of SubBand Differential CQI */
   U8                     posOfM;   /*!< Position of M selected SubBands */
} RgSchCqiRawPuschMode20;

/** @brief This structure that stores the length of Bits that
*     will be received over PUSCH for Aperiodic Mode 1-2.
*/
typedef struct rgSchCqiRawPuschMode12
{
   U8                     wideBCqiCw0;    /*!< Length of Wideband CQI Codeword 0 */
   U8                     r1WideBCqiCw1;    /*!< Length of Wideband CQI Codeword 1 for Rank =1*/
   U8                     rg1WideBCqiCw1;    /*!< Length of Wideband CQI Codeword for Rank > 1 */
   U8                     r1TotalPmiBitLen;   /*!< Aggregate length of PMI Bits for Rank =1 */
   U8                     rg1TotalPmiBitLen;  /*!< Aggregate length of PMI Bits for Rank > 1 */
} RgSchCqiRawPuschMode12;


/** @brief This structure that stores the length of Bits that
*     will be received over PUSCH.
*/
typedef struct rgSchDlCqiRawPusch
{
   TfuDlCqiPuschMode      mode;          /*!< PUSCH CQI mode */
   TknU8                  ri;            /*!< Rank Indicator for TM 3,4 */
   union
   {
      RgSchCqiRawPuschMode12   mode12Info;    /*!< Mode 1-2 information */
      RgSchCqiRawPuschMode20   mode20Info;    /*!< Mode 2-0 information */
      RgSchCqiRawPuschMode22   mode22Info;    /*!< Mode 2-2 information */
      RgSchCqiRawPuschMode30   mode30Info;    /*!< Mode 3-0 information */
      RgSchCqiRawPuschMode31   mode31Info;    /*!< Mode 3-1 information */
   }u;
} RgSchDlCqiRawPusch;

typedef struct rgSchUeRawCqiBitWidthInfo
{
  TfuRecpReqType         type;          /*!< Type indicating PUCCH or PUSCH */
   union
   {
      TfuDlCqiPucch        pucchRawCqiBitWidth;  /*!< Raw CQI Bit Width for PUCCH */
      RgSchDlCqiRawPusch        puschRawCqiBitWidth;  /*!< Raw CQI Bit Width for PUSCH */
   }t;
} RgSchUeRawCqiBitWidthInfo;
#endif


#ifdef LTEMAC_HDFDD

/******************************************************************************
 *                         Start of Data declarations                         *
 ******************************************************************************/
/**
  @brief Half Duplex subframtion information stored per ue. */

typedef struct rgSchUeHdFddSfInfo
{
  U16             sfn;                   /*!< Store the sfn for updated state
                                              Default Value= 0xffff
                                          */
  U8              subFrmDir;
                                         /*!< 0x00= DFLT
                                              0x01= DL DATA +(OPT:CNTRL INFO)
                                              0x02= DL CNTRL
                                              0x04= UL DATA+(OPT: CTNRL INFO)
                                         */

}RgSchUeHdFddSfInfo;

/**
  @brief Half Duplex control related information per ue. */

typedef struct rgSchUeHdFddCb
{
   RgSchUeHdFddSfInfo    subfrm[RG_SCH_HDFDD_NUMSFINFO];

}RgSchUeHdFddCb;
#endif /* LTEMAC_HDFDD */


#ifdef LTEMAC_DRX
/** @brief This structure is part of rgSchUeCb structure and stored DRX related
 * information for a UE.
 */
typedef struct rgSchDrxUeCb
{
#ifdef LTEMAC_R9
   TknS32 cqiMask;  /*!<If pres, only value supported is SETUP i.e 0 */
#endif
   Bool     srRcvd;   /*!< TRUE if SR is received from the UE. */
   Bool     raRcvd;   /*!< TRUE if Random access request is received using a
                        dedicated preamble for this UE. */
   U8       onDurTmrLen;   /*!< Length of onDuration Timer [1 - 200]. */
   U16      drxStartOffset;   /*!< Value of the DRX Starting Offset [0 - 2559]. */
   S16      onDurExpDistance;   /*!< Keeps track of actual distance */

   U8       drxRetransTmrLen; /*!< Length of DRX Retransmission timer [1 - 33].*/

   U16      inactvtyTmrLen;   /*!< Length of drx-Inactivity Timer [1 - 2560]. */
   S16      drxInactDistance;   /*!< Keeps track of actual distance */

   Bool     isLongCycle;
   U16      longDrxCycle;   /*!< Value of the DRX long cycle [10 - 2560]. */

   Bool     isShortCycleCfgd;   /*!< TRUE if short cycle is enabled. */
   U8       shortCycleTmrLen;   /*!< Value of DRX short cycle Timer [1-16]. */
   U16      shortDrxCycle;   /*!< Value of the DRX short cycle [2 - 640]. */
   S16      drxShortCycleDistance;   /*!< Keeps track of actual distance */


   CmLList  onDurationEnt;   /*!< Entry into the OnDuration List. */
   CmLList  onDurationExpEnt;   /*!< Entry into the onDuration Expiry List. */
   CmLList  inActvTmrEnt;   /*!< Entry into the inactivity Timer list.  */
   CmLList  shortCycleEnt;   /*!< Entry into HARQ Retransmission list. */

   S16      distance;   /*!< Keeps track of actual distance */

/* The following elements track current indices into the drxQ present at the
 * cell level. These indicies help in fast deletion in case of UE Delete,
 * otherwise it might have required a linear search. */
   U16      onDurIndx;   /*!< The current index for onDuration Queue. */
   U16      onDurExpIndx;   /*!< The current index for onDuration Queue. */
   U16      drxInactvIndx;   /*!< The current index for drx-InactityTmr Queue. */
   U16      shortCycleIndx;   /*!< The current index for Short Cycle Queue. */

   U8       shortCycleTmr;   /*!< Counter to keep track of Short DRX Cycle. */
   U32      drxDlInactvMask;  /*!< Downlink Mask to track InActivity */
   U32      drxUlInactvMask;  /*!< Uplink Mask to track InActivity */
} RgSchDrxUeCb;

#endif   /* LTEMAC_DRX */


/* ccpu00117452 - MOD - Changed macro name from
   RGR_RRM_DLPWR_CNTRL to RGR_CQI_REPT */
#ifdef RGR_CQI_REPT
typedef struct RgSchCqiInfo
{
   U8        cqiCount;                  /* To keep track of CQI reports
                                           received so far */
   RgrUeCqiRept cqiRept[RGR_CQIRPTS_MAXN]; /* Array to maintain CQI reports */
}RgSchCqiInfo;


#endif
/**
  * @brief
  * UE control block for UE specific information.
  */
struct rgSchUeCb
{
   CmHashListEnt        ueLstEnt;  /*!< Hash List entity for UE List */
   /*rg008.201 - Added support for SPS*/
#ifdef LTEMAC_SPS
   CmHashListEnt        spsUeLstEnt;  /*!< Hash List entity for UE List */
#endif
   RgSchRntiLnk         *rntiLnk;  /*!< Link to RNTI for the UE */
   CmLteRnti            ueId;      /*!< UE identifier */
/* rg007.201 - Changes for MIMO feature addition */
/* rg008.201 - Removed dependency on MIMO compile-time flag */
   RgSchUeMimoInfo      mimoInfo;  /*!< MIMO related information for a UE */
   /*rg008.201 - Added support for SPS*/
#ifdef LTEMAC_SPS
   CmLteRnti            spsRnti;   /*!<  Configured value for spsRnti if SPg
                                         is enabled for the UE */
#ifdef LTE_TDD
   /* TODO antz - put all relPdcch related info in a struct */
   CmLteTimingInfo      relPdcchTxTime; /*!< Time at which release pdcch is
                                             transmitted */
   U8                   relPdcchValm;   /*!< 'm' for relPdcch */
#endif
   CmLteTimingInfo relPdcchFbkTiming;/*!< Release PDCCH feedback timing for
                                          the UE: value used by DHM */
#endif
   RgSchCellCb          *cell;     /*!< Cell to which this UE belongs */
   Void                 *sch;      /*!< UE Specific Scheduler information */
   RgSchUeUlCb          ul;        /*!< UE Specific Uplink information */
   RgSchUeDlCb          dl;        /*!< UE Specific Downlink information */
   CmLteTimingInfo      datIndTime;/*!< Timing info of the last received Dat
                                                              indication */
   CmLteTimingInfo      macCeRptTime;/*!< Timing info of the last received
                                       *  MAC CE (BSR/PHR) */
   U32                  y[RGSCH_NUM_SUB_FRAMES];
   CmLList              taLnk;     /*!< To link to the taUeLst list */
   CmLList              dlTaLnk;   /*!< To link to the Dl SCHED's taLst */
   CmTimer              taTmr;       /*!< Timer for Timing Alignment */
   RgSchUeMeasGapCb     measGapCb;     /*!< Measurement gap control block */
   RgSchUeAckNakRepCb   ackNakRepCb;   /*!< Ack-Nack Repetition control block */

#ifdef RGR_V1
   /* rg006.201: [ccpu00112398] Added periodic BSR timer */
   CmTimer              bsrTmr;        /*!< BSR timer expiry handling case */
   /*rg009.201 - CR timer implementation changes*/
   CmLList         ccchSduLnk;       /*!< To link raCb to the "to be
                                       scheduled"
                                       list
                                      */
   struct
   {
      U32          bo;            /*!< Buffer occupancy for
                                    CCCH */
   } dlCcchInfo;                  /*!< Params for DL
                                    CCCH */
#else

   CmTimer              bsrTmr;        /*!< BSR timer expiry handling case */
#endif
/* rg009.201. Added changes of TFU_UPGRADE */
#ifdef TFU_UPGRADE
   CmLteUeCategory     ueCatEnum;
   RgSchUeACqiCb     acqiCb;        /* ! < Aperiodic CQI Report Control Block*/
   /* Periodic CQI/SRS/SR Report Feature */
   RgSchUePCqiCb     cqiCb;        /*!< Periodic CQI PMI RI Control Block */
   RgSchUeSrsCb      srsCb;        /*!< SRS  Control Block*/
   RgSchUeSrCb       srCb;         /*!< SR Control Block */
#endif

#ifdef LTEMAC_HDFDD
   /* Half Duplex Feature */
   Bool              hdFddEnbld;        /*!< Half Duplex FDD is configured TRUE=1/FALSE=0 */
   RgSchUeHdFddCb    *hdFddCb;     /*!< Half Duplex Control Block */
#endif  /* LTEMAC_HDFDD */
/* rg009.201. Added changes of TFU_UPGRADE */
#ifdef LTEMAC_DRX
   Bool               isDrxEnabled; /*!< isDrx Enabled for this UE?*/
   RgSchDrxUeCb       *drxCb;       /*!< DRX control block. Allocated at time of
                                        UE configuration. */
   CmLList            ulDrxInactvLnk; /*!<List of UE's which become inactive for UL */
   CmLList            dlDrxInactvLnk; /*!<List of UE's which become inactive for DL */

   CmLList            ulDrxInactvTmrLnk; /*!<List of UEs on which inactivity
                                             tmr has to be started for UL */
   CmLList            dlDrxInactvTmrLnk; /*!<List of UEs on which inactivity
                                             tmr has to be started for DL */
#endif /*LTEMAC_DRX*/
#ifdef TFU_UPGRADE
U8     validTxAnt;  /*! < Tx Antenna selected after computing the CQI among two Antennas*/
RgSchUeRawCqiBitWidthInfo  rawCqiBitW;
/* rg001.301-ADD- [ccpu00118643] Initial Num RBs support for UCI on PUSCH */
U8     initNumRbs; /* No. of RBs allocated for UL Data New Transmission */
#endif

/* ccpu00117452 - MOD - Changed macro name from
   RGR_RRM_DLPWR_CNTRL to RGR_CQI_REPT */
#ifdef RGR_CQI_REPT
   RgrUeCqiReptCfg cqiReptCfgInfo; /* Holds static information such
                                                 as configured values for DL
                                                 Power Control*/
#endif
#ifdef TFU_UPGRADE
   TknU8      pA;                     /* PA value configured by RRM
                                         ref: RRC 36.331, 6.3.2, PDSCH-Config*/
#endif
/* ccpu00117452 - MOD - Changed macro name from
   RGR_RRM_DLPWR_CNTRL to RGR_CQI_REPT */
#ifdef RGR_CQI_REPT
   RgSchCqiInfo schCqiInfo; /* This structure is place holder for
                               keeping all CQI reporting related information */
#endif
   Bool         isReEstablishing; /* TRUE is UE is in process of reestablishing.
                                     i.e from start of UE RESET to completion of 
                                     UE RECFG with newCrnti */
};


/**
  * @brief
  * Configuration Information for Upper SAPs at RGU, CRG and RGR interfaces.
  */
typedef struct rgSchUpSapCfgInfo
{
   Pst  sapPst;              /*!< Post info associated with SAP */
   SpId spId;                /*!< SpId associated with SAP */
   SuId suId;                /*!< SuId associated with SAP */
}RgSchUpSapCfgInfo;

/**
  * @brief
  * Configuration Information for Lower SAP at TFU interface.
  */
typedef struct rgSchLowSapCfgInfo
{
   Pst    sapPst;            /*!< Post info associated with SAP */
   SpId   spId;              /*!< SpId associated with SAP */
   SuId   suId;              /*!< SuId associated with SAP */
   TmrCfg bndTmr;            /*!< Bind Timer Value */
}RgSchLowSapCfgInfo;

/**
  * @brief
  * Control Block structure for Upper SAPs at RGU, CRG and RGR interfaces.
  */
typedef struct rgSchUpSapCb
{
   RgSchUpSapCfgInfo sapCfg;    /*!< Configuration info */
   RgSapSta     sapSta;      /*!< SAP Status */
   RgSchCellCb  *cell;       /*!< Cell associated with this sap */
}RgSchUpSapCb;

/**
  * @brief
  * Control Block structure for Lower SAP at TFU interface.
  */
typedef struct rgSchLowSapCb
{
   RgSchLowSapCfgInfo sapCfg;          /*!< SAP configuration info */
   RgSapSta           sapSta;          /*!< SAP Status */
   U8                 numBndRetries;   /*!< Number of Bind Retries */
   RgSchCellCb        *cell;           /*!< Cell associated with this SAP */
   CmTimer            tmrBlk;       /*!< Timer Block associated with this SAP */
}RgSchLowSapCb;

/**
  * @brief
  * structure holding Layer Manager Response information cached from control
  * request.
  */
typedef struct rgSchLmResponse
{
   TranId transId;           /*!< Transaction ID */
   Resp   response;          /*!< Response */
}RgSchLmResponse;

/* XXX: Below structures added for PDCCH Order req for RACH Module */
typedef enum sfnEnum
{
   RG_SCH_SFN_ANY,
   RG_SCH_SFN_ODD,
   RG_SCH_SFN_EVEN
} SfnEnum;

typedef struct rgSchConfigIdx
{
   U8         configurationIndex; /* New RGR Cell cfg */
   SfnEnum    sfn;
   U8         subframes[10];
} RgSchConfigIdx;

typedef struct rgSchRapId
{
   U8                rapId;
   CmLteTimingInfo   lastAllocPRACHMaskIdx;
} RgSchRapId;

typedef struct pdcchOrderCfg
{
   RgSchRapId       rapIds[RGSCH_MAX_RAPID];
   RgSchConfigIdx   configIdx;
} PdcchOrderCfg;
/* XXX: End */

/**
  @brief Measurement Gap related information per cell. */
typedef struct rgSchMeasGapCb
{
   CmLListCp  gapPrd40Q[RG_SCH_CMN_MEAS_GAPPRD40]; /*!< Measurement Gap queue
                                               for UEs with 40 ms gap period */
   CmLListCp  gapPrd80Q[RG_SCH_CMN_MEAS_GAPPRD80]; /*!< Measurement Gap queue
                                               for UEs with 80 ms gap period */
} RgSchMeasGapCb;

/**
  @brief ACK-NACK repetition related information per cell. */
typedef struct rgSchAckNakRepCb
{
#ifdef LTE_TDD
   CmLListCp  ackNakRepQ[2*RGSCH_NUM_SUB_FRAMES];  /*!< ACK NACK repetition queue */
#else
   CmLListCp  ackNakRepQ[RGSCH_NUM_SUB_FRAMES];  /*!< ACK NACK repetition queue */
#endif
} RgSchAckNakRepCb;
/**
  * @brief
  * Structure holding RBG information for the BW
  */
typedef struct rgSchBwRbgInfo
{
   U8     numRbs;        /*!< Total number of RBs for which information is
                              stored */
   U8     numRbgs;       /*!< Number of RBGs for the BW (rounded off to the
                              closest RBG number */
   U8     rbgSize;        /*!< RBG size */
   U8     lastRbgSize;    /*!< Last RBG size : in number of RBs */
   U8     rbgSubsetSize[RG_SCH_NUM_RATYPE1_SUBSETS]; /*!< RBG Subset 0,1,2,3
                                                          sizes: number of RBs
                                                      */
}RgSchBwRbgInfo;
#ifdef LTEMAC_DRX
/** @brief This structure is one element of the DRX Queue mainted per cell.
 * @details It is composed of a list of linked lists, each tracking a specific
 * timer as defined in the DRX functionality.
 */
typedef struct rgSchDrxQ
{
   CmLListCp   onDurationQ;   /*!< Tracks the start of onDuration Timer. */
   CmLListCp   onDurationExpQ;   /*!< Tracks the Expiry of onDuration Timer. */
   CmLListCp   inActvTmrQ;   /*!< Tracks the Expiry of drx-InactivityTimer. */
   CmLListCp   harqRTTQ;   /*!< Tracks the Expiry of HARQ RTT timer. */
   CmLListCp   harqRetxQ;   /*!< Tracks the Expiry of Re-Transmission timer. */
   CmLListCp   shortCycleQ;   /*!< Tracks the Expiry of DRX Short Cycle. */
#if 0
   CmLListCp   ulRetxQ;   /*!< Tracks the UL retransmissions. */
#endif
} RgSchDrxQ;

/** @brief This structure is part of the RgSchCellCb structure and stores the
 * DRX related information for a cell.dfort
 * The cell contains a single Array of queues, wherein UEs shall be enqueued
 * based on configured offset and periodicity and timer values. Please note the
 * same queue is utilized for Long and Short DRX cycle.
 */
typedef struct rgSchDRXCellCb
{
   RgSchDrxQ   drxQ[RG_SCH_MAX_DRXQ_SIZE ]; /*!< Maintains all
                                              the timers for DRX. */
   Bool        delInUlScan;                 /*!< Maintains whether elements
                                                 from timer queue should be
                                                 removed while scanning in
                                                 UL or DL.*/
} RgSchDRXCellCb;

#endif /* LTEMAC_DRX */

/* rg004.201-ADD-Added for SI Enhancement*/
#ifdef RGR_SI_SCH
/**
  @brief SI Configuration information per cell. */
typedef struct rgSchSiCfgInfo
{
   Buffer   *mib;   /*!< MIB PDU */
   Buffer   *sib1;   /*!< SIB1 PDU */
   Buffer       *si[RGR_MAX_NUM_SI]; /*!< SIs Info */
}RgSchSiCfgInfo;

/**
  @brief SI Context information per SI. */
typedef struct rgSchSiCtx
{
   CmLList      lnk;   /*!< Link of linked list */
   U8   siId;             /*!< SI Id */
   CmLteTimingInfo timeToTx;   /*!< Time at which the SI for this SI
                                    context need to be scheduled.*/
   CmLteTimingInfo maxTimeToTx;/*!< Max Time to TX for this SI */
   U8              retxCntRem; /*!< SI retransmit count remaining */
   U16             i;          /*!< i */
} RgSchSiCtx;

/**
  @brief SI Control BLock per Cell. */
typedef struct rgSchSiCb
{
   CmLListCp   siCtxLst;      /*!< SI Context list CP */
   Bool      siBitMask;     /*!< Bitmask to indicate which of the SI
                                    components have been updated */
   RgSchSiCtx   siCtx[RGR_MAX_NUM_SI]; /*!< SIs  Context */
   RgrSiCfg   newSiCfg;      /*!< New SI Configuration, valid if the
                                    respective bit is set in bit mask
                                    siBitMask  */
   RgSchSiCfgInfo   crntSiInfo;/*!< PDUs for current modification period */
   RgSchSiCfgInfo   newSiInfo; /*!< PDUs for next modification period */
} RgSchSiCb;
#endif /*RGR_SI_SCH */
/* R8 Upgrade */
typedef struct rgSchBiInfo
{
   U16             prevBiTime;   /*!< Previous BI Value in ms Calculated and
                                   Sent in Previous Response */
   CmLteTimingInfo biTime;       /*!< Time at which previous BI sent */
} RgSchBiInfo;

/**
  * @brief
  * Cell Control block per cell.
  */
struct rgSchCellCb
{
   CmLteCellId   cellId;           /*!< Cell ID */
   Inst          instIdx;          /*!< Index of the scheduler instance */
   Inst          macInst;          /*!< Index of the MAC instance */
   U8            ulSchdType;       /*!< To indicate the type of UL scheduling for the
                                   cell */
   U8            dlSchdType;       /*!< To indicate the type of DL scheduling for the
                                   cell */
   RgrDlHqCfg    dlHqCfg;          /*!< HARQ related configuration */
   RgSchRntiDb   rntiDb;           /*!< RNTIs DB: range of rntis to be managed by MAC */
   struct schdInfoS
   {
      Void         *sch;    /*!< Common Scheduler specific information */
      RgSchdApis   *apis;   /*!< Common Scheduler APIs */
   } sc;

   Bool            isCpUlExtend;       /*!< Cyclic prefix : TRUE-extended/FALSE-normal */
   Bool            isCpDlExtend;       /*!< Cyclic prefix : TRUE-extended/FALSE-normal */
   U8              numTxAntPorts;    /*!< Number of Tx antenna ports */
   RgrBwCfg        bwCfg;            /*!< Bandwidth Configuration */
   /* rg004.201 patch - ccpu00109921 fix  start */
   U8              pbchRbStart;      /*!< Indicates the Start RB of the center 6 RBs of DL BW */
   U8              pbchRbEnd;        /*!< Indicates the Start RB of the center 6 RBs of DL BW */
   U8              numCellRSPerSf;   /*!< Indicates the number of cell specific Reference symbols                                           in a Subframe */
   /* rg004.201 patch - ccpu00109921 fix  end */
   RgrPhichCfg     phichCfg;         /*!< PHICH Config Information */
   RgrPucchCfg     pucchCfg;         /*!< PUCCH Config Information */
   RgSchSrsCfg     srsCfg;           /*!< SRS Config Information */
   RgrRachCfg      rachCfg;          /*!< RACH Configuration */
   /* R8 Upgrade */
   RgSchBiInfo     biInfo;           /* CELL wide BI Info */
   RgrSiCfg        siCfg;            /*!< SI Configuration */
   /*rg008.201 - Added support for SPS*/
#ifdef LTEMAC_SPS
   RgrPuschCfg         puschCfg;     /*!< Cell-specific hopping configuration */
#endif
   RgrPreambleSetCfg macPreambleSet; /*!< Range of PDCCH Order Preamble Id's
                                         to be managed by MAC */
   CmLteTimingInfo crntTime;         /*!< Current frame and subframe number for
                                          the cell */
   RgSchCfgCfb     rgCfgInfo;        /*!< Control block for configuration related
                                          information  */
   U8              ulCcchId;         /*!< LcId for uplink CCCH */
   U8              dlCcchId;         /*!< LcId for downlink CCCH */
   RgSchClcDlLcCb  cmnLcCb[RGSCH_MAX_CMN_LC_CB]; /*!< BCCH/PCCH logical channel control block */
   CmHashListCp    ueLst;                     /*!< Hash list of UE control
                                                   blocks: RgSchUeCb */
   /*rg008.201 - Added support for SPS*/
#ifdef LTEMAC_SPS
   CmHashListCp    spsUeLst;            /*!< Hash list of UE control blocks
                                          for spsRnti: RgSchUeCb */
#endif /* LTEMAC_SPS */
   CmLListCp       taUeLst;          /*!< List of ueCbs having ta */
#ifdef RGR_V1
   /*rg009.201 - CR timer changes*/
   CmLListCp       ccchSduUeLst;     /*!< List of ueCbs scheduled for CCCH
					   SDU */
   CmLListCp       contResGrdTmrLst; /*!< List of raCbs whose Guard timer is
				       running  */
   CmLListCp       contResTmrLst;    /*!< List of raCbs whose contention
				       resolution timer is running  */
#endif
   RgSchRaInfoCb   raInfo;           /*!< Random access related information for
                                          cell */
   CmLListCp       pdcchLst;         /*!< List of free RgSchPdcch structures */
   CmTqCp          tqCp;
   CmTqType        tq[RGSCH_UE_TQ_SIZE];
   U8              crntSfIdx;       /*!< Current index for allocation */
#ifdef LTE_TDD
   RgInfSfAlloc    sfAllocArr[2*RGSCH_NUM_SUB_FRAMES]; /*!< Subframe Allocation
                                            info to be sent to MAC */
   RgInfRlsHqInfo  rlsHqArr[2*RGSCH_NUM_SUB_FRAMES]; /*!< Harq Release
                                            info to be sent to MAC */
#else
   RgInfSfAlloc    sfAllocArr[RGSCH_NUM_SUB_FRAMES]; /*!< Subframe Allocation
                                            info to be sent to MAC */
   RgInfRlsHqInfo  rlsHqArr[RGSCH_NUM_SUB_FRAMES]; /*!< Harq Release
                                            info to be sent to MAC */
#endif
   U8              crntHqIdx;       /*!< Current index for Harq release info */
   RgSchLowSapCb   *tfuSap;
/* rg001.201: Added for sending TTI tick to RRM */
#if (defined(RGR_RRM_TICK) || defined(RGR_CQI_REPT))
   RgSchUpSapCb    *rgrSap;         /*!< Pointer to the cell's RGR SAP
                                      Control Block */
#endif
#ifdef RGR_RRM_TICK
   U8               rrmTtiIndPrd;   /*!< Periodicity of TTI indication from
                                         MAC towards RGR user. Range [1-255]. A
                                         value of 1 means one tick per System
                                         Frame and 2 means one tick per 2 System
                                         Frame, and so on. */
#endif
   PdcchOrderCfg   pdcchOrdCfg;
   RgSchMeasGapCb    measGapCb;    /*!< Measurement gap control block */
   RgSchAckNakRepCb  ackNakRepCb;  /*!< Ack-Nack Repetition control block */
#ifdef LTE_TDD
   RgSchTddRachRspLst   *rachRspLst;     /*!< List of awaiting RACH responses */
   U8                   numDlSubfrms;    /*!< Number of DL subframes */
   U8                   ulDlCfgIdx;      /*!< UL-DL Configuration Index */
   RgSchTddSplSubfrmCfg splSubfrmCfg;    /*!< Special subframe configuration */
   RgSchTddNpValTbl     rgSchTddNpValTbl;/*!< Mapping between 'Np' and 'p' */
   RgSchDlSf            **subFrms;       /*!< DL subframes list */
   RgSchTddPrachRscInfo prachCfg;        /*!< PRACH configuration */
   CmHashListCp         ueTfuPendLst;    /*!< Used for HARQ Ack/Nack
                                              Multiplexing */
#else
/*rg009.201 [ccpu00115187]: Using RGSCH_NUM_DL_SUBFRAMES as size of subFrms*/
   RgSchDlSf          *subFrms[RGSCH_NUM_DL_SUBFRAMES];
   U16             nCce;
#endif
/* rg007.201 - Changes for MIMO feature addition */
/* rg008.201 - Removed dependency on MIMO compile-time flag */
   U8                   noOfRbgs;       /*!< Number of RBGs for this bw */
   U8                   rbgSize;        /*!< RBG Size */
   /*rg008.201 - Added support for SPS*/
#ifdef LTEMAC_SPS
   RgSchBwRbgInfo     dlBwRbgInfo; /*!< RBG information for the configured
                                         DL BW */
   RgrSpsDlCellCfg    dlSpsCfg;    /*!< SPS specific configuration for
                                        the cell */
   RgSchBwRbgInfo  spsBwRbgInfo;    /*!< RBG information for configured maximum
                                        SPS bandwidth */
#endif
#ifdef LTE_L2_MEAS
   RgSchRaPreambles  raPrmbs;              /*!< Different received RA preambles */
   RgSchAvgPrbDl     avgPrbDl;             /*!< PRB usage in percentage per QCI for DL */
   RgSchAvgPrbUl     avgPrbUl;             /*!< PRB usage in percentage per QCI for UL */
   CmLListCp         l2mList;              /*!< List of all L2M requests
                                           rgSchL2MeasCb */
   RgSchQciCb        qciArray[LRG_MAX_QCI]; /*!< Array of all qci's configured
                                            rgSchQciCb */
   U8                ulAvailBw;            /*!< Total Uplink band width available
                                              for this sub frame */
#endif /* LTE_L2_MEAS */
  /* rg009.201. Added changes of TFU_UPGRADE */
#ifdef TFU_UPGRADE
 RgSchPerPucchCb   pCqiSrsSrLst[RG_SCH_PCQI_SRS_SR_TRINS_SIZE];
                                /*!< To store next active Periodic CQI/PMI, RI,
                                     SRS and SR Transmission instance */
#endif /* TFU_UPGRADE */

#ifdef LTEMAC_HDFDD
 CmLListCp         hdFddLst;   /*!< Half Duplex FDD UE list */
#endif /* LTEMAC_HDFDD */
#ifdef LTEMAC_DRX
   RgSchDRXCellCb    *drxCb;   /*!< Pointer to the DRX control block shall be
                                 allocated at the time of cell configuration. */
#endif
   /* rg004.201-ADD-Added for SI Enhancement*/
#ifdef RGR_SI_SCH
   RgSchSiCb       siCb;            /*!< SI Control Block */
#endif /*RGR_SI_SCH */
   Bool isDummyTti;                 /*< Flag to indicate dummy/real TTI */
};

/**
  * @brief
  * Structure holding LTE MAC's General Configuration information.
  */
typedef struct rgSchGenCb
{
   U8              tmrRes;           /*!< Timer resolution */
   RgSchLmResponse *bndCfmResp;       /*!< Respones value for Bind Confirm */
   RgSchLmResponse ustaResp;         /*!< Respones value for Alarms */
}RgSchGenCb;

/**
  * @brief
  * Global Control block for LTE-MAC.
  */
struct _rgSchCb
{
   TskInit       rgSchInit;                    /*!< Task Init info */
   RgSchGenCb    genCfg;                    /*!< General Config info */
   U8            numSaps;                   /*!< Num RGR Saps = Num TFU Saps */
   RgSchUpSapCb  *rgrSap;                    /*!< RGR SAP Control Block */
   RgSchLowSapCb *tfuSap;                    /*!< TFU SAP Control Block */
   CmTqCp        tmrTqCp;                   /*!< Timer Task Queue Cntrl Point */
   CmTqType      tmrTq[RGSCH_TQ_SIZE];         /*!< Timer Task Queue */
};

/* Declaration for scheduler control blocks */
EXTERN RgSchCb rgSchCb[RGSCH_MAX_INST];

/*
 * Data structures for RAM
 */

/**
  * @brief
  * Random Access Req Info to be stored in cellCb.
  */
typedef struct rgSchRaReqInfo
{
   CmLList         raReqLstEnt;  /*!< Linked list entity for RaReq List */
   CmLteTimingInfo timingInfo;   /*!< RACHO: Time of RaReq Reception */
   RgTfuRaReqInfo  raReq;        /*!< Random Access Request Information */
} RgSchRaReqInfo;

/**
  * @enum rgSchRaState
  * Enumeration of random access states.
  */
typedef enum rgSchRaState
{
   RGSCH_RA_MSG3_PENDING,   /*!< Msg3 reception pending */
   RGSCH_RA_MSG4_PENDING,   /*!< Msg4 transmission pending */
   RGSCH_RA_MSG4_DONE      /*!< Msg4 transmission successful */
} RgSchRaState;

/**
  * @brief
  * Control block for Random Access.
  */
struct rgSchRaCb
{
   CmLList         raCbLnk;       /*!< To link to the raCb list */
   CmLList         schdLnk;       /*!< To link raCb to the "to be scheduled"
                                       list */
   CmLteRnti       tmpCrnti;      /*!< Temporary C-RNTI */
   CmLteTimingInfo timingInfo;    /*!< Timing information */
   RgSchRntiLnk    *rntiLnk;      /*!< Link to RNTI for raCb */
   RgSchRaState    raState;       /*!< Random access state */
   struct
   {
      U32          bo;            /*!< Buffer occupancy for CCCH */
   } dlCcchInfo;                  /*!< Params for DL CCCH */
   U8              msg3HqProcId;  /*!< Msg3 Harq Process ID */
   RgSchUlHqProcCb *msg3HqProcRef;/*!< Reference to msg3 harq process */
   RgSchUlHqProcCb msg3HqProc;    /*!< msg3HqProcRef points to this initially */
   RgSchUeCb       *ue;           /*!< NULL initially */
   Bool            toDel;         /*!< To delete this RaCb after msg4 reject */
   TknU8           phr;           /*!< To store the PHR, if received along with
                                       Msg3 */
   CmLList         rspLnk;        /*!< Used to link RACB to a frame for resp */
   U8              rapId;         /*!< RAP ID */
   TknU16          ta;            /*!< Timing Adjustment */
   RgSchUlGrnt     msg3Grnt; /*!< Msg3 grant as given by the UL Sched */
   U32             y[RGSCH_NUM_SUB_FRAMES]; /*!< y values using tmpCrnti by DLSCHED */
   RgSchDlHqEnt    *dlHqE;     /*!< DL HARQ module */
   U8              ccchCqi;     /*!< DL Cqi obtained from RaReq and Used for CCCH */
   RgSchDlRbAlloc  rbAllocInfo; /*!< RB Allocation Info for MSG4 Trans/Retrans */
   /* rg004.201: ccpu00105914: PHR handling for MSG3 */
   CmLteTimingInfo msg3AllocTime; /*!< Allocation time for msg3 grant */
#ifdef RGR_V1
   /*rg009.201 - CR timer  changes*/
   CmLList         contResTmrLnk;       /*!< To link raCb to the
				       Guard Timer/Contention Resolution timer list*/
   CmLteTimingInfo expiryTime; /*!< Expiry time for Guard/Contention
				 Resolution timers */

   U32             ccchSduBo;   /*!<To store CCCH SDU BO if it arrives while
				  ContRes CE is transmitting or retransmitting*/
#endif
};

/**
  * @brief
  * Carries the Error information.
  */
struct rgSchErrInfo
{
   U8    errType;    /*!< Error Type */
   U16   errCause;   /*!< Cause of Error */
};
/* Global Variables */
#ifdef LTE_TDD

typedef U8 RgSchTddUlDlSubfrmTbl[RGSCH_MAX_TDD_UL_DL_CFG][RGSCH_NUM_SUB_FRAMES];
EXTERN RgSchTddUlDlSubfrmTbl rgSchTddUlDlSubfrmTbl;

typedef struct rgSchTddSplSubfrmInfo RgSchTddSplSubfrmInfoTbl[RGSCH_MAX_TDD_SPL_SUBFRM_CFG];
EXTERN RgSchTddSplSubfrmInfoTbl rgSchTddSplSubfrmInfoTbl;

typedef struct rgSchTddDlAscSetIdxK RgSchTddDlAscSetIdxKTbl[RGSCH_MAX_TDD_UL_DL_CFG][RGSCH_NUM_SUB_FRAMES];
EXTERN RgSchTddDlAscSetIdxKTbl rgSchTddDlAscSetIdxKTbl;

typedef U8 RgSchTddPhichMValTbl[RGSCH_MAX_TDD_UL_DL_CFG][RGSCH_NUM_SUB_FRAMES];
EXTERN RgSchTddPhichMValTbl rgSchTddPhichMValTbl;

typedef U8 RgSchTddKPhichTbl[RGSCH_MAX_TDD_UL_DL_CFG][RGSCH_NUM_SUB_FRAMES];
EXTERN RgSchTddKPhichTbl rgSchTddKPhichTbl;

typedef RgSchTddPhichOffInfo RgSchTddPhichOffInfoTbl[RGSCH_MAX_TDD_UL_DL_CFG][RGSCH_NUM_SUB_FRAMES];

typedef U8 RgSchTddUlAscIdxKDashTbl[RGSCH_MAX_TDD_UL_DL_CFG-1][RGSCH_NUM_SUB_FRAMES];
EXTERN RgSchTddUlAscIdxKDashTbl rgSchTddUlAscIdxKDashTbl;

typedef U8 RgSchTddPuschTxKTbl[RGSCH_MAX_TDD_UL_DL_CFG][RGSCH_NUM_SUB_FRAMES];
EXTERN RgSchTddPuschTxKTbl rgSchTddPuschTxKTbl;

typedef U8 RgSchTddUlNumHarqProcTbl[RGSCH_MAX_TDD_UL_DL_CFG];
EXTERN RgSchTddUlNumHarqProcTbl rgSchTddUlNumHarqProcTbl;

typedef U8 RgSchTddDlNumHarqProcTbl[RGSCH_MAX_TDD_UL_DL_CFG];
EXTERN RgSchTddDlNumHarqProcTbl rgSchTddDlNumHarqProcTbl;

/* Number of ACK/NACK Feedback to be stored based on UL-DL Configuration Index */
typedef U8 RgSchTddANFdbkMapTbl[RGSCH_MAX_TDD_UL_DL_CFG];
EXTERN RgSchTddANFdbkMapTbl rgSchTddANFdbkMapTbl;

/* Number of UL subframes */
typedef RgSchTddSubfrmInfo RgSchTddMaxUlSubfrmTbl[RGSCH_MAX_TDD_UL_DL_CFG];
EXTERN RgSchTddMaxUlSubfrmTbl rgSchTddMaxUlSubfrmTbl;

/* Number of UL subframes */
typedef U8 RgSchTddNumUlSubfrmTbl[RGSCH_MAX_TDD_UL_DL_CFG][RGSCH_NUM_SUB_FRAMES];
EXTERN RgSchTddNumUlSubfrmTbl rgSchTddNumUlSubfrmTbl;

/* Number of low UL subframes Indices*/
typedef U8 RgSchTddLowUlSubfrmIdxTbl[RGSCH_MAX_TDD_UL_DL_CFG][RGSCH_NUM_SUB_FRAMES];
EXTERN RgSchTddLowUlSubfrmIdxTbl rgSchTddLowUlSubfrmIdxTbl;

/* Number of high UL subframes Indices*/
typedef U8 RgSchTddHighUlSubfrmIdxTbl[RGSCH_MAX_TDD_UL_DL_CFG][RGSCH_NUM_SUB_FRAMES];
EXTERN RgSchTddHighUlSubfrmIdxTbl rgSchTddHighUlSubfrmIdxTbl;

/* Number of low DL subframes Indices*/
typedef U8 RgSchTddLowDlSubfrmIdxTbl[RGSCH_MAX_TDD_UL_DL_CFG][RGSCH_NUM_SUB_FRAMES];
EXTERN RgSchTddLowDlSubfrmIdxTbl rgSchTddLowDlSubfrmIdxTbl;

/* Number of high DL subframes Indices*/
typedef U8 RgSchTddHighDlSubfrmIdxTbl[RGSCH_MAX_TDD_UL_DL_CFG][RGSCH_NUM_SUB_FRAMES];
EXTERN RgSchTddHighDlSubfrmIdxTbl rgSchTddHighDlSubfrmIdxTbl;

/* Number of DL subframes and Special subframes with DwPTS */
typedef U8 RgSchTddNumDlSubfrmTbl[RGSCH_MAX_TDD_UL_DL_CFG][RGSCH_NUM_SUB_FRAMES];
EXTERN RgSchTddNumDlSubfrmTbl rgSchTddNumDlSubfrmTbl;

/* Number of DL subframes and Special subframes with DwPTS */
typedef RgSchTddSubfrmInfo RgSchTddMaxDlSubfrmTbl[RGSCH_MAX_TDD_UL_DL_CFG];
EXTERN RgSchTddMaxDlSubfrmTbl rgSchTddMaxDlSubfrmTbl;

typedef U8 RgSchTddMsg3SubfrmTbl[RGSCH_MAX_TDD_UL_DL_CFG][RGSCH_NUM_SUB_FRAMES];
EXTERN RgSchTddMsg3SubfrmTbl rgSchTddMsg3SubfrmTbl;
#ifdef LTEMAC_SPS
typedef RgSchTddMsg3SubfrmTbl RgSchTddSpsUlRsrvTbl;
EXTERN RgSchTddMsg3SubfrmTbl rgSchTddSpsUlRsrvTbl;
#endif

typedef U8 RgSchTddRlsDlSubfrmTbl[RGSCH_MAX_TDD_UL_DL_CFG][RGSCH_NUM_SUB_FRAMES];
EXTERN RgSchTddRlsDlSubfrmTbl rgSchTddRlsDlSubfrmTbl;

#endif

/* APIs exposed by TMR module */
EXTERN Void rgSCHTmrStartTmr ARGS((
         RgSchCellCb *cellCb,
         Ptr         cb,
         S16         tmrEvnt,
         U32         tmrVal));

EXTERN Void rgSCHTmrStopTmr ARGS((
         RgSchCellCb *cellCb,
         S16         tmrEvnt,
         Ptr         cb));

EXTERN Void rgSCHTmrProcTmr ARGS((
         Ptr         cb,
         S16         tmrEvnt));

/* APIs exposed by TOM */

EXTERN S16 rgSCHTomRaReqInd ARGS((
         RgSchCellCb     *cell,
         TfuRaReqIndInfo *raReqInd));

EXTERN S16 rgSCHTomPucchDeltaPwrInd ARGS((
         RgSchCellCb           *cell,
         TfuPucchDeltaPwrIndInfo *pucchDeltaPwr));

EXTERN S16 rgSCHTomUlCqiInd ARGS((
         RgSchCellCb     *cell,
         TfuUlCqiIndInfo *ulCqiInd));

EXTERN S16 rgSCHTomSrInd ARGS((
         RgSchCellCb     *cell,
         TfuSrIndInfo    *srInd));

EXTERN S16 rgSCHTomDlCqiInd ARGS((
         RgSchCellCb     *cell,
         TfuDlCqiIndInfo *dlCqiInd));
/* rg009.201. Added changes of TFU_UPGRADE */
#ifdef TFU_UPGRADE
EXTERN S16 rgSCHTomRawCqiInd ARGS
((
RgSchCellCb     *cell,
TfuRawCqiIndInfo* rawCqiInd
));

EXTERN S16 rgSCHTomSrsInd ARGS
((
RgSchCellCb     *cell,
TfuSrsIndInfo* srsInd
));

#endif

EXTERN S16 rgSCHTomDoaInd ARGS((
         RgSchCellCb     *cell,
         TfuDoaIndInfo   *doaInd));
EXTERN S16 rgSCHTomCrcInd ARGS((
         RgSchCellCb       *cell,
         TfuCrcIndInfo *crcInd));

EXTERN S16 rgSCHTomTtiInd ARGS((
         RgSchCellCb   *cell,
         SuId          suId,
         TfuTtiIndInfo *ttiInd));

EXTERN S16 rgSCHTomHarqAckInd ARGS((
         RgSchCellCb       *cell,
         TfuHqIndInfo      *harqAckInd));

EXTERN S16 rgSCHTomTimingAdvInd ARGS((
         RgSchCellCb         *cell,
         TfuTimingAdvIndInfo *timingAdvInd));


/*
 * APIs exposed by LMM
 */
EXTERN S16 schActvInit ARGS((Ent entity, Inst inst, Region
       region, Reason reason));
EXTERN S16 rgSCHLmmStartTmr ARGS ((Inst instId, S16 tmrEvnt,
         U32 tmrVal, PTR cb));
EXTERN S16 rgSCHLmmStopTmr ARGS((Inst instId, S16 tmrEvnt, PTR cb));
EXTERN S16 rgSCHLmmTmrExpiry ARGS((PTR cb, S16 tmrEvnt));
/* This function invokes a Control Confirmation to the LM from scheduler. */
EXTERN S16 rgSCHLmmBndCfm ARGS((Pst *pst, SuId suId, U8 status));
EXTERN S16 schActvTmr ARGS((Ent entity, Inst inst));
/* To send a Unsolicited Status Indication to Layer Manager */
EXTERN S16 rgSCHLmmStaInd ARGS((Inst instId, U16 category, U16 event,
                          U16 cause, RgUstaDgn *dgn));
EXTERN S16 schActvTsk ARGS((Pst *pst, Buffer *mBuf));
EXTERN Void rgSCHLmmFillCfmPst ARGS((Pst    *reqPst,Pst *cfmPst,RgMngmt  *cfm));
EXTERN U16 rgSCHLmmInstCfg ARGS((RgCfg    *cfg, Inst inst));
EXTERN Void rgSCHLmmGenCntrl ARGS((RgMngmt *cntrl,RgMngmt  *cfm,Pst *cfmPst));
EXTERN Void rgSCHLmmSapCntrl ARGS((RgMngmt *cntrl,RgMngmt *cfm,Pst *cfmPst));

/*
 * APIs exposed by CFG module
 */
EXTERN S16 rgSCHCfgVldtRgrCellCfg ARGS((Inst inst, RgrCellCfg  *cellCfg,
      RgSchCellCb *cell, RgSchErrInfo *errInfo));
EXTERN S16 rgSCHCfgRgrCellCfg ARGS((RgSchCb *instCb, SpId spId,
      RgrCellCfg *cellCfg, RgSchErrInfo *errInfo));
EXTERN S16 rgSCHCfgVldtRgrCellRecfg ARGS((Inst inst, RgrCellRecfg *cellRecfg,
      RgSchCellCb  **cell, RgSchErrInfo *errInfo));
EXTERN S16 rgSCHCfgRgrCellRecfg ARGS((RgSchCellCb *cell, RgrCellRecfg *cellRecfg,
      RgSchErrInfo *errInfo));

EXTERN S16 rgSCHCfgVldtRgrUeCfg ARGS((Inst inst, RgrUeCfg  *ueCfg,
      RgSchCellCb  **cell, RgSchErrInfo *errInfo));
EXTERN S16 rgSCHCfgRgrUeCfg ARGS((RgSchCellCb  *cell, RgrUeCfg  *ueCfg,
      RgSchErrInfo  *errInfo));
EXTERN S16 rgSCHCfgVldtRgrUeRecfg ARGS((Inst inst, RgrUeRecfg *ueRecfg,
      RgSchCellCb **cell, RgSchUeCb **ue, RgSchErrInfo *errInfo));
EXTERN S16 rgSCHCfgRgrUeRecfg ARGS((RgSchCellCb *cell, RgSchUeCb *ue, RgrUeRecfg
      *ueRecfg, RgSchErrInfo *errInfo));

EXTERN S16 rgSCHCfgVldtRgrLcCfg ARGS((Inst inst, RgrLchCfg *lcCfg,
      RgSchCellCb **cell, RgSchUeCb **ue, RgSchErrInfo  *errInfo));
EXTERN S16 rgSCHCfgRgrLchCfg ARGS((RgSchCellCb *cell, RgSchUeCb *ue,
      RgrLchCfg *lcCfg, RgSchErrInfo *errInfo));
EXTERN S16 rgSCHCfgVldtRgrLchRecfg ARGS((Inst inst, RgrLchRecfg *lcRecfg,
      RgSchCellCb **cell, RgSchUeCb **ue, RgSchDlLcCb **dlLc,
      RgSchErrInfo *errInfo));
EXTERN S16 rgSCHCfgRgrLchRecfg ARGS((RgSchCellCb *cell, RgSchUeCb *ue,
         RgSchDlLcCb *dlLc, RgrLchRecfg *lcRecfg, RgSchErrInfo *errInfo));

EXTERN S16 rgSCHCfgVldtRgrLcgRecfg ARGS ((Inst inst,RgrLcgRecfg *lcgRecfg,
         RgSchCellCb *cell,RgSchUeCb **ue,RgSchErrInfo *errInfo ));
EXTERN S16 rgSCHCfgVldtRgrLcgCfg ARGS ((Inst inst,RgrLcgCfg *lcgCfg,
         RgSchCellCb **cell,RgSchUeCb **ue,RgSchErrInfo *errInfo ));
EXTERN S16 rgSCHCfgRgrLcgCfg ARGS ((RgSchCellCb *cell,RgSchUeCb *ue,
         RgrLcgCfg *lcgCfg,RgSchErrInfo *errInfo ));
EXTERN S16 rgSCHCfgRgrLcgRecfg ARGS ((RgSchCellCb *cell,RgSchUeCb *ue,
         RgrLcgRecfg *lcgRecfg,RgSchErrInfo *errInfo));

EXTERN S16 rgSCHCfgVldtRgrUeReset ARGS((Inst inst, RgrRst *reset, RgSchCellCb *cell,
         RgSchUeCb **ue,RgSchErrInfo     *errInfo));
EXTERN S16 rgSCHCfgRgrUeReset ARGS((RgSchCellCb *cell,RgSchUeCb *ue,RgrRst *reset,
         RgSchErrInfo   *errInfo));

EXTERN S16 rgSCHCfgRgrCellDel ARGS((RgSchCellCb *cell, RgrDel *cellDelInfo,
      RgSchErrInfo *errInfo));
EXTERN S16 rgSCHCfgRgrUeDel ARGS((RgSchCellCb *cell, RgrDel *ueDelInfo,
      RgSchErrInfo *errInfo));
EXTERN S16 rgSCHCfgRgrLcDel ARGS((RgSchCellCb *cell, RgrDel *lcDelInfo,
      RgSchErrInfo *errInfo));
EXTERN S16 rgSCHCfgRgrLcgDel ARGS ((RgSchCellCb *cell,RgrDel *lcDelInfo,
         RgSchErrInfo *errInfo));
EXTERN Void rgSCHCfgFreeCellCb ARGS((RgSchCellCb *cell));
/* rg004.201-ADD-Added for SI Enhancement*/
#ifdef RGR_SI_SCH
EXTERN S16 rgSCHCfgVldtRgrSiCfg ARGS(( Inst inst, RgrSiCfgReqInfo *siCfg,
                    RgSchCellCb      *cell, RgSchErrInfo *errInfo));
EXTERN S16 rgSCHGomHndlSiCfg ARGS(( Region   reg, Pool   pool,
                  RgSchCb       *instCb, SpId    spId,
                  RgrCfgTransId transId, RgrSiCfgReqInfo *cfgReqInfo));
EXTERN S16 rgSCHUtlRgrSiCfgCfm ARGS ((Inst inst, SpId spId,
                                  RgrCfgTransId transId,U8 status));
#endif /* RGR_SI_SCH */

/*
 * APIs exposed by GOM module
 */
EXTERN S16 rgSCHGomHndlCfg ARGS((Region reg, Pool pool, RgSchCb *instCb, SpId spId,
                            RgrCfgTransId transId, RgrCfgReqInfo *cfgReqInfo));
EXTERN S16 rgSCHGomTtiHndlr ARGS((RgSchCellCb *cell, SpId spId));


/*
 * APIs exposed by RAM module
 */
EXTERN S16 rgSCHRamVldtUeCfg ARGS((
RgSchCellCb  *cell,
RgrUeCfg     *ueCfg
));
EXTERN S16 rgSCHRamProcRaReq     ARGS((RgSchCellCb *cell, CmLteRnti raRnti,
                                    TfuRachInfo  *raReqInd,
                                    CmLteTimingInfo timingInfo,
                                    RgSchErrInfo *err));
EXTERN S16 rgSCHRamCreateRaCb ARGS((RgSchCellCb *cell, RgSchRaCb **raCb,
                                    RgSchErrInfo *err));
EXTERN S16 rgSCHRamRgrUeCfg      ARGS((RgSchCellCb *cell, RgSchUeCb *ue,
                                    RgSchRaCb *raCb, RgSchErrInfo *err));
EXTERN S16 rgSCHRamProcMsg3      ARGS((RgSchCellCb *cell, RgSchUeCb *ue,
                                    RgSchRaCb *raCb, RgInfUeDatInd  *pdu,
                                    RgSchErrInfo *err));
EXTERN S16 rgSCHRamUpdtBo        ARGS((RgSchCellCb *cell, RgSchRaCb *raCb,
                                    RgInfCmnBoRpt *staRsp));
EXTERN S16 rgSCHRamMsg3DatInd   ARGS((RgSchRaCb *raCb));
EXTERN S16 rgSCHRamMsg3FailureInd   ARGS((RgSchRaCb *raCb));
EXTERN S16 rgSCHRamMsg4FdbkInd   ARGS((RgSchRaCb *raCb));
EXTERN S16 rgSCHRamMsg4Done      ARGS((RgSchCellCb *cell, RgSchRaCb *raCb));
EXTERN S16 rgSCHRamDelRaCb       ARGS((RgSchCellCb *cell, RgSchRaCb *raCb,
                                    Bool rlsRnti));
EXTERN S16 rgSCHRamFreeCell      ARGS((RgSchCellCb *cell));
EXTERN S16 rgSCHRamTtiHndlr      ARGS((RgSchCellCb *cell));
/* rg006.201: [ccpu000111706, ccpu00112394]: Separated TTI processing for
 * UL and DL */
EXTERN S16 rgSCHCmnUlTti ARGS((RgSchCellCb *cell, RgSchErrInfo *err));
#ifdef RGR_V1
/* rg006.201: [ccpu00112398] Added periodic BSR timer */
EXTERN S16 rgSCHCmnBsrTmrExpry ARGS(( RgSchUeCb  *ueCb));
#endif
#ifdef LTE_TDD
EXTERN S16 rgSCHRamDelRaReq      ARGS((RgSchCellCb *cell,
                                       CmLteTimingInfo timingInfo,
                                       U8 raIdx));
#endif
EXTERN S16 rgSCHRamAddToRaInfoSchdLst(RgSchCellCb   *cell, RgSchRaCb *raCb);

EXTERN S16 rgSCHRamRmvFrmRaInfoSchdLst(RgSchCellCb   *cell, RgSchRaCb   *raCb);

/* APIs exposed by UHM */
   /*rg003.201: Added for Uplink Adaptive retransmission */
EXTERN Void rgSCHUhmNonadapRetx ARGS((RgSchUlHqProcCb *hqProc));
EXTERN S16 rgSCHUhmHqEntInit ARGS((RgSchUeCb *ueCb));
EXTERN Void rgSCHUhmProcDatInd ARGS((RgSchCellCb *cell, RgSchUeCb *ue,
         CmLteTimingInfo frm));
EXTERN Void rgSCHUhmProcMsg3DatInd ARGS((RgSchUlHqProcCb *hqProc));
EXTERN Void rgSCHUhmProcMsg3Failure ARGS((RgSchUlHqProcCb *hqProc));
EXTERN Void rgSCHUhmProcHqFailure ARGS((RgSchCellCb *cell, RgSchUeCb *ue,
         CmLteTimingInfo frm, TknU8 rv));
EXTERN RgSchUlHqProcCb* rgSCHUhmGetUlHqProc ARGS(( RgSchUeCb *ue,
         U8 idx));
EXTERN Void rgSCHUhmNewTx ARGS((RgSchUlHqProcCb *hqProc, U8 maxHqRetx,
         RgSchUlAlloc *alloc));
EXTERN Void rgSCHUhmFreeProc ARGS((RgSchUlHqProcCb *hqProc));
EXTERN Void rgSCHUhmRetx ARGS((RgSchUlHqProcCb *hqProc, RgSchUlAlloc *alloc));
EXTERN Void rgSCHUhmUpdMsg3Hq ARGS((RgSchUlHqProcCb** msg3HpRefRef,
         U8 msg3HqProcId, RgSchUeCb *ue));
EXTERN Void rgSCHUhmRgrUeCfg ARGS(( RgSchCellCb *cellCb, RgSchUeCb *ueCb,
          RgrUeCfg *ueCfg));
EXTERN Void rgSCHUhmRgrUeRecfg ARGS(( RgSchCellCb *cellCb, RgSchUeCb *ueCb,
          RgrUeRecfg *ueRecfg));
EXTERN Void rgSCHUhmFreeUe ARGS(( RgSchUeCb *ueCb));
EXTERN S16 rgSCHUhmAppendPhich ARGS(( RgSchCellCb *cellCb,
          CmLteTimingInfo frm ));


/* APIs exposed by DBM */
EXTERN S16 rgSCHDbmInitCell ARGS((RgSchCellCb *cellCb));
EXTERN S16 rgSCHDbmDeInitUeCbLst ARGS(( RgSchCellCb *cellCb));
#ifdef LTE_TDD
EXTERN S16 rgSCHDbmDeInitUeTfuPendLst ARGS(( RgSchCellCb *cellCb));
#endif
EXTERN S16 rgSCHDbmInsUeCb ARGS(( RgSchCellCb *cellCb, RgSchUeCb *ueCb));
EXTERN RgSchUeCb* rgSCHDbmGetUeCb ARGS(( RgSchCellCb *cellCb, CmLteRnti ueId));
EXTERN RgSchUeCb* rgSCHDbmGetNextUeCb ( RgSchCellCb *cellCb, RgSchUeCb *ueCb);
EXTERN S16 rgSCHDbmDelUeCb ARGS(( RgSchCellCb *cellCb, RgSchUeCb *ueCb));
EXTERN S16 rgSCHDbmInitUe ARGS((RgSchUeCb *ueCb));
EXTERN Void rgSCHDbmInsDlDedLcCb ARGS((RgSchUeCb *ueCb, RgSchDlLcCb *dlLcCb));
EXTERN Void rgSCHDbmDelDlDedLcCb ARGS((RgSchUeCb *ueCb, RgSchDlLcCb *dlLcCb));
EXTERN RgSchDlLcCb* rgSCHDbmGetDlDedLcCb ARGS(( RgSchUeCb *ueCb, CmLteLcId idx));
EXTERN RgSchDlLcCb* rgSCHDbmGetFirstDlDedLcCb ARGS((RgSchUeCb *ueCbb));
EXTERN RgSchDlLcCb* rgSCHDbmGetNextDlDedLcCb ARGS((RgSchUeCb *ueCb, RgSchDlLcCb *lcCb));
EXTERN RgSchClcDlLcCb* rgSCHDbmGetCmnLcCb ARGS(( RgSchCellCb *cellCb, CmLteLcId lcId));
EXTERN RgSchClcDlLcCb* rgSCHDbmGetBcchOnBch ARGS(( RgSchCellCb *cellCb ));
EXTERN RgSchClcDlLcCb* rgSCHDbmGetFirstBcchOnDlsch ARGS(( RgSchCellCb *cellCb));
EXTERN RgSchClcDlLcCb* rgSCHDbmGetSecondBcchOnDlsch ARGS(( RgSchCellCb *cellCb));
EXTERN RgSchClcDlLcCb* rgSCHDbmGetPcch ARGS(( RgSchCellCb *cellCb));
EXTERN Void rgSCHDbmInsBcchOnBch ARGS(( RgSchCellCb *cellCb, RgSchClcDlLcCb *cmnDlLcCb));
EXTERN Void rgSCHDbmInsBcchOnDlsch ARGS(( RgSchCellCb *cellCb, RgSchClcDlLcCb *cmnDlLcCb));
EXTERN Void rgSCHDbmInsPcch ARGS(( RgSchCellCb *cellCb, RgSchClcDlLcCb *cmnDlLcCb));

EXTERN Void rgSCHDbmInitCmnLcBoLst ARGS(( RgSchClcDlLcCb *cmnDlLcCb));
EXTERN Void rgSCHDbmInsCmnLcBoRpt ARGS(( RgSchClcDlLcCb *cmnDlLcCb,
                                    RgSchClcBoRpt *cmnBoRpt));
EXTERN RgSchRaCb* rgSCHDbmGetRaCb ARGS(( RgSchCellCb *cellCb, CmLteRnti key));
EXTERN Void rgSCHDbmInsCrntRgrCfgElem ARGS(( RgSchCellCb *cellCb,
                                          RgSchCfgElem *cfgElem));
EXTERN Void rgSCHDbmInsPndngRgrCfgElem ARGS(( RgSchCellCb *cellCb,
                                          RgSchCfgElem *cfgElem));
EXTERN RgSchCfgElem* rgSCHDbmGetNextCrntRgrCfgElem ARGS(( RgSchCellCb *cellCb,
                                                   RgSchCfgElem *cfgElem));
EXTERN RgSchCfgElem* rgSCHDbmGetNextPndngRgrCfgElem ARGS(( RgSchCellCb *cellCb,
                                                   RgSchCfgElem *cfgElem));
EXTERN RgSchCfgElem* rgSCHDbmGetPndngRgrCfgElemByKey ARGS(( RgSchCellCb *cellCb,
                                                   CmLteTimingInfo key));
EXTERN RgSchCfgElem* rgSCHDbmDelCrntRgrCfgElem ARGS(( RgSchCellCb *cellCb,
                                                RgSchCfgElem *cfgElem));
EXTERN RgSchCfgElem* rgSCHDbmDelPndngRgrCfgElem ARGS(( RgSchCellCb *cellCb,
                                                RgSchCfgElem *cfgElem));

EXTERN S16 rgSCHDbmRntiDbInit ARGS(( RgSchCellCb *cellCb, U16 rntiStart, U16 maxRntis));
EXTERN Void rgSCHDbmRntiDbDeInit ARGS(( RgSchCellCb *cellCb));
EXTERN RgSchRntiLnk* rgSCHDbmGetRnti ARGS(( RgSchCellCb *cellCb));
EXTERN Void rgSCHDbmRlsRnti ARGS(( RgSchCellCb *cellCb, RgSchRntiLnk *rntiLnk));
/*rg008.201 - Added support for SPS*/
#ifdef LTEMAC_SPS
EXTERN S16 rgSCHDbmDeInitSpsUeCbLst ARGS((RgSchCellCb *cellCb));
EXTERN S16 rgSCHDbmInsSpsUeCb ARGS((RgSchCellCb *cellCb, RgSchUeCb *ueCb));
EXTERN RgSchUeCb* rgSCHDbmGetSpsUeCb ARGS((RgSchCellCb *cellCb, CmLteRnti ueId));
EXTERN RgSchUeCb* rgSCHDbmGetNextSpsUeCb ARGS((RgSchCellCb *cellCb, RgSchUeCb *ueCb));
EXTERN S16 rgSCHDbmDelSpsUeCb ARGS((RgSchCellCb *cellCb,RgSchUeCb *ueCb));
#endif /* LTEMAC_SPS */

#ifdef LTE_L2_MEAS
/*
 * L2M APIs
 */
EXTERN S16 rgSchL2mMeasReq ARGS ((
         RgSchCellCb       *cell,
         LrgSchMeasReqInfo *measInfo,
         RgSchErrInfo      err));
#endif /* LTE_L2_MEAS */
/*
 * DHM APIs
 */
EXTERN RgSchDlHqEnt *rgSCHDhmHqEntInit ARGS((RgSchCellCb *cell));
EXTERN S16 rgSCHDhmGetAvlHqProc ARGS((RgSchUeCb *ue, CmLteTimingInfo timingInfo,
                                   RgSchDlHqProcCb **hqP));
EXTERN Void rgSCHDhmHqRetx ARGS((RgSchDlHqEnt *hqE, CmLteTimingInfo timeInfo,
                                   RgSchDlHqProcCb *hqP));
EXTERN RgSchDlHqProcCb * rgSCHDhmLastSchedHqProc ARGS((RgSchDlHqEnt *hqE));
/*rg009.201 - CR timer changes*/
EXTERN S16 rgSCHDhmGetCcchSduHqProc ARGS((RgSchUeCb *ueCb, CmLteTimingInfo timeInfo));
EXTERN S16 rgSCHDhmGetMsg4HqProc ARGS((RgSchRaCb *raCb, CmLteTimingInfo timeInfo));
EXTERN Void rgSCHDhmRlsHqProc ARGS((RgSchDlHqProcCb *hqP));
/* ccpu00118350 : Correcting NDI manipulation of Harq */
EXTERN Void rgSCHDhmRlsHqpTb ARGS((RgSchDlHqProcCb *hqP, U8 tbIdx, Bool togNdi));
EXTERN Void rgSCHUtlDlHqPTbAddToTx ARGS((RgSchDlSf *subFrm,
RgSchDlHqProcCb *hqP, U8 tbIdx ));
EXTERN Void rgSCHDhmHqTbRetx ARGS(( RgSchDlHqEnt *hqE,
CmLteTimingInfo timingInfo, RgSchDlHqProcCb *hqP, U8 tbIdx));
EXTERN Void rgSCHUtlDlHqPTbAddToTx ARGS((RgSchDlSf *subFrm,
RgSchDlHqProcCb *hqP, U8 tbIdx ));
EXTERN Void rgSCHDhmHqTbRetx ARGS(( RgSchDlHqEnt *hqE,
CmLteTimingInfo timingInfo, RgSchDlHqProcCb *hqP, U8 tbIdx));
#ifdef RG_UNUSED
EXTERN S16 rgSCHDhmGetHqProcFrmId ARGS((RgSchUeCb *ue, U8 idx,
                                    RgSchDlHqProcCb **hqP));
#endif
/* rg007.201 - Changes for MIMO feature addition */
/* rg008.201 - Removed dependency on MIMO compile-time flag */
EXTERN Void rgSCHDhmSchdTa ARGS((RgSchUeCb *ueCb, RgSchDlHqTbCb *tbInfo));
EXTERN S16 rgSCHDhmHqFdbkInd ARGS((Void *cb, U8 cbType, RgSchCellCb *cellCb,
                     CmLteTimingInfo timingInfo, RgTfuHqInfo *fdbk, RgInfRlsHqInfo
                                    *rlsHqBufs,RgSchErrInfo *err));
EXTERN Void rgSCHDhmRgrUeCfg ARGS((RgSchCellCb *cellCb, RgSchUeCb *ueCb,
                                    RgrUeCfg *ueCfg, RgSchErrInfo *err));
EXTERN Void rgSCHDhmRgrUeRecfg ARGS((RgSchCellCb *cellCb, RgSchUeCb *ueCb,
                                    RgrUeRecfg *ueCfg, RgSchErrInfo *err));
EXTERN Void rgSCHDhmRgrCellCfg ARGS((RgSchCellCb *cellCb, RgrCellCfg *cellCfg,
                                    RgSchErrInfo *err));
EXTERN Void rgSCHDhmRgrCellRecfg ARGS((RgSchCellCb *cellCb, RgrCellRecfg
                                    *cellRecfg, RgSchErrInfo *err));
EXTERN Void rgSCHDhmFreeUe ARGS((RgSchUeCb *ueCb));
EXTERN Void rgSCHDhmUpdTa ARGS((RgSchCellCb *cellCb, RgSchUeCb *ueCb, U8 ta));
EXTERN Void rgSCHDhmProcTAExp ARGS((RgSchUeCb *ue));
/* rg007.201 - Changes for MIMO feature addition */
/* rg008.201 - Removed dependency on MIMO compile-time flag */
EXTERN S16 rgSCHDhmAddLcData ARGS((Inst inst, RgSchLchAllocInfo *lchData,
                                    RgSchDlHqTbCb *tbInfo));
EXTERN S16 rgSCHDhmRlsDlsfHqProc ARGS((RgSchCellCb *cellCb, CmLteTimingInfo
timingInfo));

#ifdef LTE_TDD
EXTERN S16 rgSCHDhmTddRlsSubFrm ARGS((RgSchCellCb *cell));
#endif
/*rg008.201 - Added support for SPS*/
#ifdef LTEMAC_SPS
EXTERN S16 rgSCHDhmGetHqProcFrmId ARGS((
RgSchUeCb               *ue,
U8                      idx,
RgSchDlHqProcCb         **hqP
));
#endif /* LTEMAC_SPS */
/* rg003.301 -Add-[ccpu00120794] - Freeing up the HARQ proc blocked for
 * indefinite time in case of Retx */
EXTERN S16 rgSCHDhmDlRetxAllocFail ARGS((
RgSchUeCb        *ue,
RgSchDlHqProcCb  *proc
));
/* MS_WORKAROUND for ccpu00122893 temp fix Incorrect HqProc release was done instead of
 * a Harq Entity reset. Fixing the same */
EXTERN Void rgSCHDhmHqEntReset ARGS((
   RgSchDlHqEnt      *hqE
));
/* Measurement GAP and ACK NACK */

EXTERN S16 rgSCHMeasGapANRepUeCfg ARGS((
RgSchCellCb         *cell,
RgSchUeCb           *ue,
RgrUeCfg            *ueCfg
));
EXTERN S16 rgSCHMeasGapANRepUeRecfg ARGS((
RgSchCellCb         *cell,
RgSchUeCb           *ue,
RgrUeRecfg          *ueRecfg
));
EXTERN Void rgSCHMeasGapANRepUeDel ARGS((
RgSchCellCb         *cell,
RgSchUeCb           *ue
));
EXTERN S16 rgSCHMeasGapANRepTtiHndl ARGS((
RgSchCellCb         *cell
));
EXTERN S16 rgSCHMeasGapANRepGetDlInactvUe ARGS((
RgSchCellCb         *cell,
CmLListCp           *dlInactvUeLst
));
EXTERN S16 rgSCHMeasGapANRepGetUlInactvUe ARGS((
RgSchCellCb         *cell,
CmLListCp           *ulInactvUeLst
));
EXTERN Void rgSCHMeasGapANRepDlInactvTmrExpry ARGS((
RgSchUeCb           *ue,
U8                   tmrEvnt
));
EXTERN Void rgSCHMeasGapANRepUlInactvTmrExpry ARGS((
RgSchUeCb           *ue,
U8                   tmrEvnt
));
EXTERN Void rgSCHMeasGapANRepTmrExpry ARGS((
RgSchUeCb           *ue
));
EXTERN Void rgSCHAckNakRepTmrExpry ARGS((
RgSchUeCb           *ue
));
EXTERN Void rgSCHAckNakRepSndHqFbkRcpReq ARGS((
RgSchCellCb         *cell,
RgSchDlSf           *dlSf,
CmLteTimingInfo     timingInfo));

EXTERN Void rgSCHAckNakRepAddToQ ARGS((
RgSchCellCb       *cell,
RgSchDlSf         *crntDlSf));

/*
 * SCH Util APIs
 */
#ifdef LTEMAC_SPS
EXTERN Void rgSCHUtlHdlCrcInd ARGS((
RgSchCellCb     *cell,
RgSchUeCb       *ue,
CmLteTimingInfo timingInfo
));
#endif

#ifdef LTE_L2_MEAS
EXTERN  S16 rgSCHUtlValidateMeasReq ARGS ((RgSchCellCb *cellCb,
         LrgSchMeasReqInfo *schL2MeasInfo,
         RgSchErrInfo      *err
         ));
EXTERN S16 rgSchL2mSndCfm ARGS((Pst   *pst,
         RgSchL2MeasCb   *measCb,
         LrgSchMeasReqInfo *measInfo,
         Bool              isErr
));
EXTERN  S16 rgSchFillL2MeasCfm ARGS((
         RgSchCellCb       *cell,
         RgSchL2MeasCb     *measCb,
         LrgSchMeasCfmInfo *cfm
));
EXTERN Void rgSchL2mFillCfmPst ARGS((
         Pst    *pst,
         Pst    *cfmPst,
         LrgSchMeasReqInfo *measInfo
));
EXTERN S16 rgSCHL2Meas ARGS((
         RgSchCellCb  *cell
));
#endif /* LTE_L2_MEAS */
/* rg009.201. Added changes of TFU_UPGRADE */
#ifdef TFU_UPGRADE
EXTERN F64 rgSCHUtlPower ARGS
((
F64 x,
F64 n
));

 EXTERN U32 rgSCHUtlParse ARGS
  ((
 U8 *buff,
 U8 startPos,
 U8 endPos,
 U8 buffSize
 ));

 EXTERN  U8 rgSCHUtlFindDist ARGS
((
U16    crntTime,
U16    tempIdx
));
#endif
EXTERN Bool rgSCHUtlPdcchAvail ARGS((RgSchCellCb *cell, RgSchPdcchInfo
        *pdcchInfo, U32 loc, CmLteAggrLvl aggrLvl, RgSchPdcch **pdcch));
EXTERN Void rgSCHUtlPdcchPut ARGS((RgSchCellCb *cell, RgSchPdcchInfo *pdcchInfo,
        RgSchPdcch *pdcch));
#ifdef LTE_TDD
/*rg009.201:[ccpu00116309]:Changes for passing iPhich at TFU interface*/
EXTERN S16 rgSCHUtlAddPhich  ARGS((RgSchCellCb *cellCb, CmLteTimingInfo frm,
         U8 hqFeedBack, U8 nDmrs, U8 rbStart, U8 iPhich));
#else
EXTERN S16 rgSCHUtlAddPhich  ARGS((RgSchCellCb *cellCb, CmLteTimingInfo frm,
         U8 hqFeedBack, U8 nDmrs, U8 rbStart));
#endif
EXTERN RgSchDlSf* rgSCHUtlSubFrmGet ARGS((RgSchCellCb *cell,
         CmLteTimingInfo frm));
EXTERN Void rgSCHUtlSubFrmPut ARGS((RgSchCellCb *cell, RgSchDlSf *sf));
EXTERN U8 rgSCHUtlLog32bitNbase2 ARGS((U32 n));
EXTERN Void rgSCHUtlUpdY ARGS((U16 rnti, U32 y[]));
EXTERN U32 rgSCHUtlGetY ARGS((RgSchUeCb *ue, U8 subFrm));
/*rg008.201 - Added support for SPS*/


#ifdef LTEMAC_SPS
EXTERN RgSchDlHqProcCb * rgSCHDhmSpsDlGetHqProc ARGS(( RgSchUeCb *ue,
CmLteTimingInfo  timingInfo));
#endif
#ifdef LTE_TDD
EXTERN U8 rgSCHUtlCalcNCce ARGS((U8 bw, RgrPhichNg ng, U8 cfi, U8 mPhich,
                                 U8 numAntna, Bool isEcp));
#else
EXTERN U8 rgSCHUtlCalcNCce ARGS((U8 bw, RgrPhichNg ng, U8 cfi, U8 numAntna, Bool
isEcp));
#endif
#ifdef LTE_TDD
/*rg009.201:[ccpu00116309]:Changes for passing iPhich at TFU interface*/
EXTERN S16 rgSCHUtlGetPhichInfo ARGS((RgSchUlHqProcCb *hqProc, U8 *rbStartRef,
         U8 *nDmrsRef, U8 *iPhich));
#else
EXTERN S16 rgSCHUtlGetPhichInfo ARGS((RgSchUlHqProcCb *hqProc, U8 *rbStartRef,
         U8 *nDmrsRef));
#endif
/* rg009.201. Added changes of TFU_UPGRADE */
#ifndef TFU_UPGRADE
/* rg001.201:107387: To include the length and ModOrder in DataRecp Req. */
/* rg006.201: [ccpu00111718] Updating NDI and HARQ proc Id */
EXTERN S16 rgSCHUtlAllocRcptInfo ARGS((RgSchUlAlloc *alloc, CmLteRnti *rnti,
           U8 *iMcsRef, U8 *rbStartRef, U8 *numRbRef, U8 *rvRef, U16 *size,
           TfuModScheme *modType,Bool *isRtx,
U8 *nDmrs,
Bool *ndi,
U8 *hqPId));
#else
EXTERN S16 rgSCHUtlAllocRcptInfo ARGS((
         RgSchCellCb             *cell,
         RgSchUlAlloc            *alloc,
         CmLteTimingInfo         *timeInfo,
         TfuUeUlSchRecpInfo      *recpReq
         ));
#endif /* TFU_UPGRADE */

EXTERN S16 rgSCHUtlRgrCellCfg ARGS((RgSchCellCb *cell, RgrCellCfg *cellCfg,
         RgSchErrInfo *errInfo));
EXTERN S16 rgSCHUtlRgrCellRecfg ARGS((RgSchCellCb *cell, RgrCellRecfg *recfg,
         RgSchErrInfo *errInfo));
EXTERN S16 rgSCHUtlFreeCell ARGS((RgSchCellCb *cell));
EXTERN S16 rgSCHUtlRgrUeCfg ARGS((RgSchCellCb *cell, RgSchUeCb *ue,
         RgrUeCfg *cfg, RgSchErrInfo *err));
EXTERN S16 rgSCHUtlRgrLcCfg ARGS((RgSchCellCb *cell, RgSchUeCb *ue,
         RgSchDlLcCb *dl, RgrLchCfg *cfg,RgSchErrInfo *errInfo));
EXTERN S16 rgSCHUtlRgrLcRecfg ARGS ((RgSchCellCb *cell,RgSchUeCb *ue,
      RgSchDlLcCb *dlLc,RgrLchRecfg *recfg,RgSchErrInfo *err));
EXTERN S16 rgSCHUtlRgrLcgCfg ARGS ((RgSchCellCb *cell,RgSchUeCb *ue,
         RgrLcgCfg *cfg,RgSchErrInfo *errInfo));
EXTERN S16 rgSCHUtlRgrLcgRecfg ARGS ((RgSchCellCb *cell,RgSchUeCb *ue,
         RgrLcgRecfg *recfg,RgSchErrInfo *err));
EXTERN Void rgSCHUtlRgrLcgDel ARGS ((RgSchCellCb *cell,RgSchUeCb *ue,
         U8 lcgId));
EXTERN Void rgSCHUtlDlCqiInd ARGS((RgSchCellCb *cell, RgSchUeCb *ue,
         TfuDlCqiRpt *dlCqiInd, CmLteTimingInfo timingInfo));

/* rg009.201. Added changes of TFU_UPGRADE */
#ifdef TFU_UPGRADE
EXTERN Void rgSCHUtlRawCqiInd ARGS
((
RgSchCellCb        *cell,
RgSchUeCb          *ue,
TfuRawCqiRpt*     rawCqiRpt,
CmLteTimingInfo    timingInfo
));

EXTERN Void rgSCHUtlSrsInd ARGS
((
RgSchCellCb        *cell,
RgSchUeCb          *ue,
TfuSrsRpt*     srsRpt,
CmLteTimingInfo    timingInfo
));
#endif

EXTERN Void rgSCHUtlDoaInd ARGS((RgSchCellCb *cell, RgSchUeCb *ue,
         TfuDoaRpt *doaInd));
EXTERN Void rgSCHUtlDlTARpt ARGS((RgSchCellCb *cell, RgSchUeCb *ue));
/* rg007.201 - Changes for MIMO feature addition */
/* rg008.201 - Removed dependency on MIMO compile-time flag */
EXTERN Void rgSCHUtlDlRlsSubFrm ARGS((RgSchCellCb *cell, CmLteTimingInfo subFrm));
EXTERN Void rgSCHUtlDlProcAddToRetx ARGS((RgSchCellCb *cell,
         RgSchDlHqProcCb *hqP));
EXTERN S16 rgSCHUtlRegSch ARGS((U8 schIdx, RgSchdApis *apis));
EXTERN Void rgSCHUtlDlHqProcAddToTx ARGS((RgSchDlSf *subFrm, RgSchDlHqProcCb *hqP));
/* rg007.201 - Changes for MIMO feature addition */
/* rg008.201 - Removed dependency on MIMO compile-time flag */
/* rg007.201 - Changes for MIMO feature addition */
/* rg008.201 - Removed dependency on MIMO compile-time flag */
EXTERN Void rgSCHUtlDlHqPTbRmvFrmTx ARGS((RgSchDlSf *subFrm,
         RgSchDlHqProcCb *hqP, U8 tbIdx, Bool isRepeating));
EXTERN S16 rgSCHUtlRgrUeRecfg ARGS((RgSchCellCb *cell, RgSchUeCb *ue,
         RgrUeRecfg *recfg, RgSchErrInfo *err));
EXTERN Void rgSCHUtlFreeDlLc ARGS((RgSchCellCb *cell, RgSchUeCb *ue,
         RgSchDlLcCb *dlLc));
EXTERN Void rgSCHUtlFreeUlLc ARGS((RgSchCellCb *cell, RgSchUeCb *ue,
         RgSchUlLcCb *ulLc));
EXTERN Void rgSCHUtlFreeUe ARGS((RgSchCellCb *cell, RgSchUeCb *ue));
EXTERN S16 rgSCHUtlTti ARGS((RgSchCellCb *cell, RgSchErrInfo *err));
EXTERN Void rgSCHUtlDlDedBoUpd ARGS((RgSchCellCb *cell, RgSchUeCb *ue,
         RgSchDlLcCb *svc));
#ifdef RG_UNUSED
EXTERN S16 rgSCHUtlUpdUlHqProc ARGS((RgSchCellCb *cell, RgSchUlHqProcCb *curProc,
         RgSchUlHqProcCb *oldProc));
#endif
/* rg004.201: ccpu00105914: PHR handling for MSG3 */
EXTERN Void rgSCHUtlRecMsg3Alloc ARGS((RgSchCellCb *cell, RgSchUeCb *ue,
         RgSchRaCb *raCb));
EXTERN S16 rgSCHUtlContResUlGrant ARGS((RgSchCellCb *cell, RgSchUeCb *ue,
         RgSchErrInfo *err));
EXTERN S16 rgSCHUtlSrRcvd ARGS((RgSchCellCb *cell, RgSchUeCb *ue,
         CmLteTimingInfo, RgSchErrInfo *err));
EXTERN S16 rgSCHUtlUpdBsrShort ARGS((RgSchCellCb *cell, RgSchUeCb *ue, U8 lcgId,
         U8 bsr, RgSchErrInfo *err));
EXTERN S16 rgSCHUtlUpdBsrTrunc ARGS((RgSchCellCb *cell, RgSchUeCb *ue, U8 lcgId,
         U8 bsr, RgSchErrInfo *err));
EXTERN S16 rgSCHUtlUpdBsrLong ARGS((RgSchCellCb *cell, RgSchUeCb *ue,
         U8 bsr1,U8 bsr2,U8 bsr3,U8 bsr4, RgSchErrInfo *err));
EXTERN S16 rgSCHUtlUpdPhr ARGS((RgSchCellCb *cell, RgSchUeCb *ue,
         U8 phr, RgSchErrInfo *err));
EXTERN S16 rgSCHUtlDataRcvd ARGS((RgSchCellCb *cell, RgSchUeCb *ue, U8 numLc,
         RgSchUlLcCb *lcArr[], U16 bytesArr[], RgSchErrInfo *err));
EXTERN Void rgSCHUtlUlCqiInd ARGS(( RgSchCellCb *cell, RgSchUeCb *ue,
                                    TfuUlCqiRpt *ulCqiInfo));
EXTERN Void rgSCHUtlPucchDeltaPwrInd ARGS(( RgSchCellCb *cell, RgSchUeCb *ue,
         S8 delta));
EXTERN Void rgSCHUtlUeReset ARGS(( RgSchCellCb *cell, RgSchUeCb *ue));
EXTERN Void rgSCHUtlUlHqProcForUe ARGS((RgSchCellCb *cell, CmLteTimingInfo frm,
         RgSchUeCb *ue, RgSchUlHqProcCb **procRef));
EXTERN RgSchUlAlloc *rgSCHUtlFirstRcptnReq ARGS((RgSchCellCb *cell));
EXTERN RgSchUlAlloc *rgSCHUtlNextRcptnReq ARGS((RgSchCellCb *cell,
         RgSchUlAlloc *alloc));
EXTERN RgSchUlAlloc *rgSCHUtlFirstHqFdbkAlloc ARGS((RgSchCellCb *cell));
EXTERN RgSchUlAlloc *rgSCHUtlNextHqFdbkAlloc ARGS((RgSchCellCb *cell,
         RgSchUlAlloc *alloc));
EXTERN S16 rgSCHUtlTfuBndReq ARGS((Inst inst, SuId suId, SpId spId));
EXTERN S16 rgSCHUtlTfuUBndReq ARGS((Inst inst, RgSchLowSapCfgInfo sapCfg, Reason reason));
EXTERN S16 rgSCHUtlResetSfAlloc ARGS((RgInfSfAlloc *sfAlloc,
            Bool resetCmnLcInfo, Bool restAlloc));
EXTERN S16 rgSCHUtlGetSfAlloc ARGS((RgSchCellCb *cell));
EXTERN S16 rgSCHUtlPutSfAlloc ARGS((RgSchCellCb *cell));
EXTERN S16  rgSCHUtlAllocSBuf  ARGS((Inst inst, Data **pData, Size size));
/* ccpu00117052 - MOD - Passing double pointer
for proper NULLP assignment*/
EXTERN Void rgSCHUtlFreeSBuf   ARGS((Inst inst, Data **data, Size size));
EXTERN Void rgSCHUtlFillDgnParams ARGS((Inst inst, RgUstaDgn *dgn,U8 dgnType));
EXTERN Void rgSCHUtlGetPstToLyr ARGS((Pst *pst,RgSchCb *schCb,Inst macInst));
EXTERN S16 rgSCHUtlFillRgInfCmnLcInfo ARGS((RgSchDlSf *sf,RgInfSfAlloc *sfAlloc,
                                 CmLteLcId lcId, Bool sendInd));
EXTERN S16 rgSCHUtlFillRgInfRarInfo ARGS((RgSchDlSf *sf,RgInfSfAlloc *sfAlloc));
EXTERN S16 rgSCHUtlFillPdschDciInfo ARGS((TfuPdschDciInfo *pdschDci,TfuDciInfo
        *pdcchDci));
EXTERN Void rgSCHUtlFillRgInfUeInfo ARGS((RgSchDlSf *sf,RgInfSfAlloc *sfAlloc));
EXTERN S16 rgSCHUtlUpdSch ARGS((RgInfSfDatInd *subfrmInfo, RgSchCellCb *cellCb,
               RgSchUeCb *ueCb, RgInfUeDatInd *pdu,RgSchErrInfo *err));
EXTERN S16 rgSCHUtlHndlCcchBoUpdt ARGS((RgSchCellCb *cell,RgInfCmnBoRpt *boRpt));
EXTERN S16 rgSCHUtlHndlBcchPcchBoUpdt ARGS((RgSchCellCb *cell,RgInfCmnBoRpt
        *boUpdt));
EXTERN S16 rgSCHUtlRgrBndCfm ARGS ((Inst inst, SuId suId,U8 status));
/* rg001.201: Added for sending TTI tick to RRM */
#ifdef RGR_RRM_TICK
EXTERN S16 rgSCHUtlRgrTtiInd ARGS ((RgSchCellCb *cell, RgrTtiIndInfo *ttiInd));
#endif
EXTERN S16 rgSCHUtlRgrCfgCfm ARGS ((Inst inst, SpId spId,
                                  RgrCfgTransId transId,U8 status));
EXTERN S16 rgSCHUtlProcMsg3 ARGS((RgInfSfDatInd *subfrmInfo, RgSchCellCb  *cellCb,
            RgSchUeCb   *ueCb, CmLteRnti  rnti,RgInfUeDatInd  *pdu,
            RgSchErrInfo *err ));
#ifdef RG_PHASE_2
EXTERN S16 rgSCHUtlTfuGrpPwrCntrlReq ARGS((Inst inst,S16  sapId,
         TfuGrpPwrCntrlReqInfo *grpPwrCntrlReq));
#endif
EXTERN S16 rgSCHUtlTfuCntrlReq ARGS((Inst  inst, S16 sapId,
         TfuCntrlReqInfo *cntrlReq));
EXTERN S16 rgSCHUtlTfuRecpReq ARGS((Inst inst, S16  sapId,
         TfuRecpReqInfo *recpReq));
EXTERN S16 rgSCHUtlValidateTfuSap ARGS((Inst  inst,SuId  suId));
EXTERN S16 rgSCHUtlAllocEventMem ARGS((Inst  inst,Ptr  *memPtr,Size  memSize));
EXTERN S16 rgSCHUtlGetEventMem ARGS((Ptr *ptr,Size  len,Ptr   memCpa));
EXTERN S16 rgSCHUtlGetRlsHqAlloc ARGS((RgSchCellCb *cell));
EXTERN S16 rgSCHUtlPutRlsHqAlloc ARGS((RgSchCellCb *cell));

EXTERN S16 rgSCHUtlDlActvtUe ARGS((RgSchCellCb *cell, RgSchUeCb *ue));
EXTERN S16 rgSCHUtlUlActvtUe ARGS((RgSchCellCb *cell, RgSchUeCb *ue));
EXTERN Void rgSCHUtlHdlUlTransInd ARGS((RgSchCellCb *cell, RgSchUeCb *ue,
                                   CmLteTimingInfo timingInfo));
/*rg001.201:107637 Nprb indication at PHY for common Ch */
/* rg002.201: ccpu00108811: Corrected allocation for common channels */
EXTERN PUBLIC S32 rgSCHUtlGetAllwdCchTbSz ARGS((U32 bo, U8 *nPrb, U8 *mcs
));
/*rg009.201 - CR timer changes*/
EXTERN PUBLIC S16 rgSCHUtlUpdtBo ARGS((RgSchCellCb  *cell,
                               RgInfCmnBoRpt     *staRsp));
EXTERN PUBLIC S16 rgSCHUtlAddUeToCcchSduLst ARGS(
                          (RgSchCellCb       *cell,
                            RgSchUeCb         *ueCb));
/* rg004.201-ADD-Added for SI Enhancement*/
#ifdef RGR_SI_SCH
EXTERN Void rgSCHUtlPutSiInfo ARGS((RgSchCellCb *cell));
#endif/*RGR_SI_SCH*/

#ifdef LTE_TDD
EXTERN S16 rgSCHUtlAllocUeANFdbkInfo ARGS((RgSchUeCb *ue));
EXTERN Void rgSCHUtlDelUeANFdbkInfo ARGS((RgSchUeCb *ue));
EXTERN S16 rgSCHUtlInitUeANFdbkInfo ARGS((RgSchTddANInfo *anInfo));
EXTERN RgSchTddANInfo* rgSCHUtlGetUeANFdbkInfo ARGS((RgSchUeCb *ueCb, CmLteTimingInfo *timeInfo));
EXTERN Void rgSCHUtlRlsUlSf ARGS((RgSchCellCb *cell));
EXTERN U8  rgSCHUtlGetDlSfIdx ARGS((RgSchCellCb *cell, CmLteTimingInfo *timeInfo));
EXTERN Void rgSCHUtlPrachCfgInit ARGS((RgSchCellCb *cell, RgrCellCfg *cellCfg ));
EXTERN Void rgSCHUtlGetNxtDlSfInfo ARGS((CmLteTimingInfo curDlTime, RgSchCellCb  *cell, RgSchDlSf *dlSf, RgSchDlSf **nxtDlsf, CmLteTimingInfo *nxtDlTime));
EXTERN Void rgSCHUtlGetPrevDlSfInfo ARGS((RgSchCellCb * cell, CmLteTimingInfo curDlTime, CmLteTimingInfo *prevDlTime, U8 *numSubfrm));
#endif
/*rg008.201 - Added support for SPS*/
#ifdef LTEMAC_SPS
EXTERN Void rgSCHUtlDlRelPdcchFbk ARGS((
RgSchCellCb        *cell,
RgSchUeCb          *ue,
Bool               isAck
));
EXTERN Void rgSCHUtlDlProcDtx ARGS((
RgSchCellCb        *cell,
RgSchDlHqProcCb    *hqP
));
EXTERN S16 rgSCHUtlSpsRelInd ARGS((
RgSchCellCb        *cellCb,
RgSchUeCb          *ueCb,
Bool               isExplRel
));
EXTERN Void rgSCHUtlHdlCrcFailInd ARGS((
RgSchCellCb     *cell,
RgSchUeCb       *ue,
CmLteTimingInfo timingInfo
));
#endif /* LTEMAC_SPS*/

/******* </AllocHolesMemMgmnt>: START *****/
EXTERN S16 rgSCHUtlUlSfInit ARGS((
         RgSchCellCb  *cell,
         RgSchUlSf    *sf,
         U8           idx,
         U8           maxUePerSf,
         U8           sbStartIdx,
         U8           numSb
         ));
EXTERN Void rgSCHUtlUlSfDeinit ARGS((
         RgSchCellCb *cell,
         RgSchUlSf   *sf
         ));
EXTERN RgSchUlAlloc *rgSCHUtlUlAllocGetHole ARGS((
         RgSchUlSf *sf,
         U8 numRb,
         RgSchUlHole *hole
         ));
EXTERN RgSchUlAlloc *rgSCHUtlUlAllocGetCompHole ARGS((
         RgSchUlSf *sf,
         RgSchUlHole *hole
         ));
EXTERN RgSchUlAlloc *rgSCHUtlUlAllocGetPartHole ARGS((
         RgSchUlSf *sf,
         U8 numRb,
         RgSchUlHole *hole
         ));
EXTERN Void rgSCHUtlUlAllocRls ARGS((
         RgSchUlSf *sf,
         RgSchUlAlloc *alloc
         ));
EXTERN RgSchUlAlloc *rgSCHUtlUlAllocFirst ARGS((
         RgSchUlSf *sf
         ));
EXTERN RgSchUlAlloc *rgSCHUtlUlAllocNxt ARGS((
         RgSchUlSf *sf,
         RgSchUlAlloc *alloc
         ));
EXTERN RgSchUlHole *rgSCHUtlUlHoleFirst ARGS((
         RgSchUlSf *sf
         ));
EXTERN RgSchUlHole *rgSCHUtlUlHoleNxt ARGS((
         RgSchUlSf *sf,
         RgSchUlHole *hole
         ));
EXTERN RgSchUlAlloc *rgSCHUtlUlAllocGetAdjNxt ARGS((
         RgSchUlAllocDb *db,
         RgSchUlAlloc *prv
         ));
EXTERN RgSchUlAlloc *rgSCHUtlUlAllocGetFirst ARGS((
         RgSchUlAllocDb *db
         ));
EXTERN Void rgSCHUtlUlHoleAddAlloc ARGS((
         RgSchUlSf *sf,
         RgSchUlAlloc *alloc
         ));
EXTERN Void rgSCHUtlUlHoleJoin ARGS((
         RgSchUlHoleDb *db,
         RgSchUlHole *prv,
         RgSchUlHole *nxt,
         RgSchUlAlloc *alloc
         ));
EXTERN Void rgSCHUtlUlHoleExtndRight ARGS((
         RgSchUlHoleDb *db,
         RgSchUlHole *prv,
         RgSchUlAlloc *alloc
         ));
EXTERN Void rgSCHUtlUlHoleExtndLeft ARGS((
         RgSchUlHoleDb *db,
         RgSchUlHole *nxt,
         RgSchUlAlloc *alloc
         ));
EXTERN Void rgSCHUtlUlHoleNew ARGS((
         RgSchUlHoleDb *db,
         RgSchUlAlloc *alloc
         ));
EXTERN Void rgSCHUtlUlHoleUpdAllocLnks ARGS((
         RgSchUlHole *hole,
         RgSchUlAlloc *prvAlloc,
         RgSchUlAlloc *nxtAlloc
         ));
EXTERN Void rgSCHUtlUlHoleIns ARGS((
         RgSchUlHoleDb *db,
         RgSchUlHole *hole
         ));
EXTERN Void rgSCHUtlUlHoleIncr ARGS((
         RgSchUlHoleDb *db,
         RgSchUlHole *hole
         ));
EXTERN Void rgSCHUtlUlHoleDecr ARGS((
         RgSchUlHoleDb *db,
         RgSchUlHole *hole
         ));
EXTERN Void rgSCHUtlUlHoleRls ARGS((
         RgSchUlHoleDb *db,
         RgSchUlHole *hole
         ));
EXTERN S16 rgSCHUtlUlAllocMemInit ARGS((
         RgSchCellCb     *cell,
         RgSchUlAllocMem *mem,
         U8 maxAllocs
         ));
EXTERN Void rgSCHUtlUlAllocMemDeinit ARGS((
         RgSchCellCb     *cell,
         RgSchUlAllocMem *mem
         ));
EXTERN S16 rgSCHUtlUlHoleMemInit ARGS((
         RgSchCellCb    *cell,
         RgSchUlHoleMem *mem,
         U8 maxHoles,
         RgSchUlHole **holeRef
         ));
EXTERN Void rgSCHUtlUlHoleMemDeinit ARGS((
         RgSchCellCb    *cell,
         RgSchUlHoleMem *mem
         ));
EXTERN RgSchUlAlloc *rgSCHUtlUlAllocMemGet ARGS((
         RgSchUlAllocMem *mem
         ));
EXTERN Void rgSCHUtlUlAllocMemRls ARGS((
         RgSchUlAllocMem *mem,
         RgSchUlAlloc *alloc
         ));
EXTERN RgSchUlHole *rgSCHUtlUlHoleMemGet ARGS((
         RgSchUlHoleMem *mem
         ));
EXTERN Void rgSCHUtlUlHoleMemRls ARGS((
         RgSchUlHoleMem *mem,
         RgSchUlHole *hole
         ));
EXTERN RgSchUlAlloc *rgSCHUtlUlGetSpfcAlloc ARGS((
         RgSchUlSf       *sf,
         U8              startSb,
         U8              numSb
));
/******* </AllocHolesMemMgmnt>: END *****/

/* DRX function declarations */
#ifdef LTEMAC_DRX
EXTERN S16 rgSCHDrxCellCfg ARGS((RgSchCellCb *cell, RgrCellCfg *cellCfg));
EXTERN Void rgSCHDrxCellDel ARGS((RgSchCellCb *cell));
EXTERN S16 rgSCHDrxUeCfg ARGS((RgSchCellCb *cell, RgSchUeCb *ue,
                              RgrUeCfg *ueCfg));
#ifdef RGR_V2
EXTERN S16 rgSCHDrxUeReCfg ARGS((RgSchCellCb *cell, RgSchUeCb *ue,
                              RgrUeRecfg *ueCfg));
#endif
EXTERN S16 rgSCHDrxUeDel ARGS((RgSchCellCb *cell,RgSchUeCb *ue));
EXTERN Void rgSCHDrxTtiInd ARGS ((RgSchCellCb *cell));

EXTERN S16 rgSCHDrxSfAlloc ARGS ((RgSchCellCb *cellCb, RgSchDlSf
                           *dlSf));
EXTERN S16 rgSCHDrxDlTrnsFail ARGS((RgSchCellCb *cell, RgSchDlHqProcCb
                             *dlHq));
EXTERN Void rgSCHDrxDedRa ARGS((RgSchCellCb *cellCb, RgSchUeCb* ueCb));
EXTERN S16 rgSCHDrxSrInd ARGS((RgSchCellCb *cell,RgSchUeCb *ue));

EXTERN Void rgSCHDrxStrtInActvTmr ARGS((RgSchCellCb *cell,
                                       CmLListCp *ueLst,
                                       U8 direction));
EXTERN S16 rgSCHUtlGetDrxSchdUesInDl ARGS((RgSchCellCb *cellCb,
                                        RgInfSfAlloc *sfAlloc,
                                        CmLListCp *dlDrxInactvTmrLst));
EXTERN Void rgSCHDrxStartHarqRTTTmr ARGS((RgSchCellCb     *cell,
                                          RgSchDlHqProcCb *hqP,
                                          U8              tbCnt));
#endif /* LTEMAC_DRX */








#ifdef LTEMAC_HDFDD
EXTERN S16 rgSCHHdFddUeCfg ARGS((
         RgSchCellCb  *cellCb,
         RgSchUeCb *ueCb,
         Bool hdFdd));
EXTERN S16 rgSCHHdFddUeDel  ARGS((
         RgSchCellCb *cellCb,
         RgSchUeCb *ueCb));
EXTERN Void rgSCHCmnHdFddPtUlMrk ARGS((
         RgSchCellCb *cellCb));
EXTERN Void rgSCHCmnHdFddChkUlAllow ARGS((
         RgSchCellCb *cellCb,
         RgSchUeCb *ueCb,
         U8 *flag));
EXTERN Void rgSCHCmnHdFddChkDlAllow ARGS((
         RgSchCellCb *cellCb,
         RgSchUeCb *ueCb,
         Bool *flag));
EXTERN Void rgSCHCmnHdFddChkNackAllow ARGS((
         RgSchCellCb *cellCb,
         RgSchUeCb *ueCb,
         CmLteTimingInfo timInfo,
         Bool *flag));
EXTERN Void rgSCHCmnHdFddUpdULMark ARGS((
         RgSchCellCb *cellCb,
         RgSchUeCb *ueCb));
EXTERN Void rgSCHCmnHdFddUpdDLMark ARGS((
         RgSchCellCb *cellCb,
         RgSchUeCb *ueCb));
EXTERN Void rgSCHHdFddGetSfn ARGS((
         U16 *sfn,
         CmLteTimingInfo timeInfo,
         S16 offset));
#endif /* ifdef LTEMAC_HDFDD */

/* ccpu00117452 - MOD - Changed macro name from
   RGR_RRM_DLPWR_CNTRL to RGR_CQI_REPT */
#ifdef RGR_CQI_REPT
PUBLIC S16 rgSCHUtlRgrStaInd ARGS((
RgSchCellCb          *cell,
RgrStaIndInfo        *rgrSta
));

PUBLIC S16 rgSCHUtlFillSndStaInd ARGS((
RgSchCellCb        *cell,
RgSchUeCb          *ue,
RgrStaIndInfo      *staInfo,
U8                 numCqiRept
));
#endif /* End of RGR_CQI_REPT */

/* MS_FIX */
PUBLIC Void rgSCHUtlRlsRnti  ARGS((
RgSchCellCb    *cellCb,
RgSchUeCb         *ue
));
#ifdef __cplusplus
}
#endif /* __cplusplus */
#endif /* __SCH__ */


/**********************************************************************
         End of file:     rg_sch.x@@/main/2 - Sat Jul 30 02:22:12 2011
**********************************************************************/
/**********************************************************************

        Revision history:

**********************************************************************/
/********************************************************************90**

     ver       pat    init                  description
------------ -------- ---- ----------------------------------------------
/main/1      ---       sd   1. LTE MAC 2.1 release
           rg001.201   sm   1. ccpu00107387: To include the length
                               and ModOrder in DataRecp Req.
                            2. ccpu00107637: Nprb indication at
                               PHY for common Channel.
                       sd   3. ccpu00107640: Corrected RACH handling for
                               multiple RAPIDs per RARNTI
           rg002.201   sd   1. ccpu00108811: Corrected allocation for common
                               channels.
                            2. Correcting DL harq softbuffer limitation logic.
                            3. Add BI sub-header size to the tbSize requirement.
           rg003.201   sm   1. ccpu00107638: Added support for non-adaptive
                               retransmission in uplink
           rg004.201   ms   1. DL DTX Handling
                       sd   2. ccpu00105914: PHR handling for MSG3
                       pk   3. ccpu00109921: PBCH allocation

           rg004.201   rnt  1. Modified for SI Enhancement
           rg005.201   sm   1. Renamed rcvdDatInd to rcvdCrcInd
           rg006.201   sd   1. ccpu00111718: Updating NDI and HARQ proc Id
                            2. Changed prototype for function
                               'rgSCHUtlAllocRcptInfo'.
                            3. ccpu00112398: Added periodic BSR timer
                            4. [ccpu000111706, ccpu00112394]: Separated TTI
                               processing for UL and DL.
            rg007.201   ap  1. Added support for MIMO
            rg008.201   sd  1. Removed dependency on MIMO compile-time flag
                 rsharon 2. Added support for SPS.
            rg009.201   rsharon  1.[ccpu00112372] Changes for CR timer.
                        gj       2. Changed the subFrms array size in rgSchCellCb
                                    to RGSCH_NUM_DL_SUBFRAMES from
                                     RGSCH_NUM_SUB_FRAMES
 rg009.201.  dv  1.Added changes of TFU_UPGRADE
                 2.Changes for passing iPhich at TFU.
/main/2      ---        gvj  1. Updated for LTE MAC Rel 3.1
            rg001.301  nudupi  1.Changes for the CR ccpu00118643.
                               2. ccpu00118350:Correcting NDI manipulation of Harq.
            rg002.301  dvasisht 1. ccpu00120233 related changes
            rg003.301  hsingh  1. ccpu00120794-Freeing up the HARQ proc blocked
                                  for indefinite time in case of Retx
*********************************************************************91*/
