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


/************************************************************************
 
     Name:     LTE-MAC layer
  
     Type:     C source file
  
     Desc:     C source code for Entry point fucntions.
  
     File:     rg_rom.c
  
     Sid:      gk_rom.c@@/main/3 - Sat Jul 30 02:21:34 2011
  
     Prg:      ps
  
**********************************************************************/

/** @file rg_rom.c
@brief APIs to handle all the primitives invoked on RGU interface.
*/

/* header include files (.h) */
#include "envopt.h"        /* environment options */
#include "envdep.h"        /* environment dependent */
#include "envind.h"        /* environment independent */
  
#include "gen.h"           /* general */
#include "ssi.h"           /* system services */

#include "cm_tkns.h"       /* Common Token Defines */
#include "cm_llist.h"      /* Common Link List Defines */
#include "cm_hash.h"       /* Common Hash List Defines */
#include "cm_mblk.h"       /* common memory link list library */
#include "cm_lte.h"        /* Common LTE */

#include "rg_env.h"        /* MAC Environment Defines */
#include "crg.h"           /* CRG Interface defines */
#include "rgu.h"           /* RGU Interface defines */
#include "tfu.h"           /* TFU Interface defines */
#include "rg_sch_inf.h"           /* RGR Interface defines */
#include "lrg.h"           /* LRG Interface defines */

#include "rg.h"            /* MAC defines */
#include "rg_err.h"        /* MAC error defines */

/* header/extern include files (.x) */
#include "gen.x"           /* general */
#include "ssi.x"           /* system services */
#include "cm5.x"           /* Timer */
#include "cm_tkns.x"       /* Common Token Definitions */
#include "cm_llist.x"      /* Common Link List Definitions */
#include "cm_lib.x"        /* Common Library Definitions */
#include "cm_hash.x"       /* Common Hash List Definitions */
#include "cm_mblk.x"       /* common memory link list library */
#include "cm_lte.x"        /* Common LTE */

#include "crg.x"           /* CRG Interface includes */
#include "rgu.x"           /* RGU Interface includes */
#include "tfu.x"           /* TFU Interface includes */
#include "rg_sch_inf.x"           /* RGR Interface includes */
#include "lrg.x"           /* LRG Interface includes */

#include "rg.x"            /* MAC includes */

/* local defines */
#define RG_NON_MIMO_IDX 0

/* local typedefs */
 
PRIVATE S16 rgROMHndlCcchDatReq     ARGS((RgCellCb *cell,
                                    RgRguCmnDatReq *datReq, RgErrInfo *err));
PRIVATE S16 rgROMHndlBcchPcchDatReq ARGS((RgCellCb *cell,
                                    RgRguCmnDatReq *datReq, RgErrInfo *err));
PRIVATE S16 rgROMHndlCcchStaRsp     ARGS((RgCellCb *cell, 
                                    RgRguCmnStaRsp *staRsp, RgErrInfo *err));
PRIVATE S16 rgROMHndlBcchPcchStaRsp ARGS((RgCellCb *cell, 
                                    RgRguCmnStaRsp *staRsp, RgErrInfo *err));

/* ADD Changes for Downlink UE Timing Optimization */
#ifdef LTEMAC_DLUE_TMGOPTMZ
PRIVATE S16 rgROMUpdDlSfRemDataCnt ARGS((RgCellCb  *cellCb,
                                    RgDlSf    *dlSf));
PUBLIC S16 rgTOMUtlProcDlSf ARGS(( RgDlSf     *dlSf,
                                   RgCellCb   *cellCb,
                                   RgErrInfo  *err));
#endif

/* forward references */

/**
 * @brief Handler for dedicated DatReq received on RGU for an UE.
 *
 * @details
 *
 *     Function : rgROMDedDatReq
 *     
 *     This function shall
 *      -# store the BO reported for the given logical channels.
 *      -# invoke DL HARQ for further processing.
 *     
 *           
 *  @param[in]  RgRguDedDatReq *datReq 
 *  @return  S16
 *      -# ROK 
 *      -# RFAILED 
 **/
#ifdef ANSI
PUBLIC S16 rgROMDedDatReq
(
RgRguDedDatReq *datReq
)
#else
PUBLIC S16 rgROMDedDatReq(datReq)
RgRguDedDatReq *datReq;
#endif
{
   RgCellCb     *cell;
   RgUeCb       *ue;
   U8           idx1,idx2;
   RgDlHqProcCb *hqProc;
   U8           hqPId;
   RgErrInfo    err;
   Pst          schPst;
   RgInfDedBoRpt boRpt;
   CmLteTimingInfo timingInfo;
   RgDlSf       *sf;
#if (ERRCLASS & ERRCLS_DEBUG)
   RgUstaDgn   dgn;      /* Alarm diagnostics structure */
#endif
/* ADD Changes for Downlink UE Timing Optimization */
#ifdef LTEMAC_DLUE_TMGOPTMZ
   S16 ret;
#endif

   TRC2(rgROMDedDatReq)

      /* rg005.201 Avoiding memset as it is getting initialized 
         for all the fields - for optimization */ 

      RGDBGPRM((rgPBuf, "rgROMDedDatReq(): datReq->cellId = %u; datReq->rnti = %u\n",
               datReq->cellId, datReq->rnti));

   /* rg005.201 Moving the error type assignment to error cases only 
    * - for optimization */
   if ((cell = rgDBMGetCellCb(datReq->cellId)) == NULLP)
   {
#if (ERRCLASS & ERRCLS_INT_PAR)
      /* Handle Cell fetch failure */
      RGLOGERROR(ERRCLS_INT_PAR,ERG001,(ErrVal)datReq->cellId,
            "rgROMDedDatReq(): Invalid cell Id");
#endif
      err.errType = RGERR_ROM_DEDDATREQ;
      err.errCause = RGERR_ROM_INV_CELL_ID;
      /* Update stats */
      rgUpdtRguDedSts(RG_RGU_SDU_DROP, datReq);
      RETVALUE(RFAILED);
   }

   if ((ue = rgDBMGetUeCb(cell, datReq->rnti)) == NULLP)
   {
#if (ERRCLASS & ERRCLS_INT_PAR)
      /* Handle Ue fetch failure */
      RGLOGERROR(ERRCLS_INT_PAR,ERG002,(ErrVal)datReq->rnti,
            "rgROMDedDatReq(): Invalid ue Id");
#endif
      err.errType = RGERR_ROM_DEDDATREQ;
      err.errCause = RGERR_ROM_INV_UE_ID;
      /* Update stats */
      rgUpdtRguDedSts(RG_RGU_SDU_DROP, datReq);
      RETVALUE(RFAILED);
   }

   timingInfo.subframe = (U8)((datReq->transId >> 8) & 0XFF);
   timingInfo.sfn = (U16)((datReq->transId >> 16) & 0xFFFF);
   sf = &cell->subFrms[timingInfo.subframe];

   if( (sf->txDone == TRUE) ||
         (!RG_TIMEINFO_SAME(sf->schdTime,timingInfo)))
   {
#if (ERRCLASS & ERRCLS_DEBUG)
      /* Transmission is already done for this subframe. This is a delayed
       * datReq. So discard */
      rgFillDgnParams(&dgn, LRG_USTA_DGNVAL_MEM);
      rgLMMStaInd(LCM_CATEGORY_PROTOCOL, LCM_EVENT_UI_INV_EVT,
            LRG_CAUSE_DELAYED_DATREQ, &dgn);
#endif
      err.errType = RGERR_ROM_DEDDATREQ;
      err.errCause = RGERR_ROM_DELAYED_DATREQ;
      /* Update stats */
      rgUpdtRguDedSts(RG_RGU_SDU_DROP, datReq);
#ifdef MSPD
	  MSPD_DBG("%d ERROR %d %u %u %u %u\n",datReq->rnti, sf->txDone,
	  sf->schdTime.sfn, sf->schdTime.subframe, timingInfo.sfn, timingInfo.subframe);
#endif
      RETVALUE(RFAILED);
   }

   hqPId = (U8)(datReq->transId);
   hqPId = hqPId >> 2;
   /* get harq process and invoke DHM */
   rgDHMGetHqProcFrmId(ue, hqPId, &hqProc);
   /* rg007.201 - Changes for MIMO feature addition */
   /* rg008.201 - Removed dependncy on MIMO compile-time flag */
   if (rgDHMHndlDedDatReq(hqProc, datReq, &err) == RFAILED)
   {
      RGDBGERR((rgPBuf, "rgROMDedDatReq(): Handling of Data request in DHM \
               failed\n"));
      MSPD_ERR("rgROMDedDatReq(): Handling of Data request in DHM \
               failed\n");
      err.errType = RGERR_ROM_DEDDATREQ;
      /* errcause shall be filled in appropriately by DHM */
      /* Update stats */
      /* rg007.201 - Changes for MIMO feature addition */
      /* rg008.201 - Removed dependncy on MIMO compile-time flag */
      rgUpdtRguDedSts(RG_RGU_SDU_DROP, datReq);
      RETVALUE(RFAILED);
   }

   /* Merging the updation of statistics of SDUs with for loop below */ 

   rgGetPstToInst(&schPst, cell->schInstMap.schInst);
   schPst.event = 0;
   boRpt.cellSapId  = cell->schInstMap.cellSapId;
   boRpt.cellId  = datReq->cellId;
   boRpt.rnti    = datReq->rnti; 
 
   
   /* Fill the DStaRsp struct and send it to scheduler */
   for (idx1 = 0; idx1 < datReq->nmbOfTbs; idx1++)
   {
      for(idx2 = 0; idx2 < datReq->datReqTb[idx1].nmbLch; idx2++)
      {
         /* Updating dedicated SDUs received statistics without 
            additional function above for optimization */
         rgCb.rguSap.sapSts.numPduRcvd +=
            datReq->datReqTb[idx1].lchData[idx2].pdu.numPdu;

         boRpt.lcId    = datReq->datReqTb[idx1].lchData[idx2].lcId; 
         boRpt.bo      = datReq->datReqTb[idx1].lchData[idx2].boReport.bo;
         RgMacSchDedBoUpdt(&schPst, &boRpt);
      }
   }

/* ADD Changes for Downlink UE Timing Optimization */
#ifdef LTEMAC_DLUE_TMGOPTMZ
   /*Presently this function is not returning RFAILED, thus not checking
     for failure condition.*/
   ret = rgROMUpdDlSfRemDataCnt(cell, sf);
#endif

   RETVALUE(ROK);
}  /* rgROMDedDatReq */


/**
 * @brief Handler for DatReq received on RGU for a common logical channel.
 *
 * @details
 *
 *     Function : rgROMCmnDatReq
 *     
 *     This function shall invoke rgROMHndlCcchDatReq() if datReq is on CCCH
 *     If not, it shall invoke rgROMHndlBcchPcchDatReq().
 *     
 *           
 *  @param[in]  RgRguCmnDatReq *datReq 
 *  @return  S16
 *      -# ROK 
 *      -# RFAILED 
 **/
#ifdef ANSI
PUBLIC S16 rgROMCmnDatReq
(
RgRguCmnDatReq *datReq
)
#else
PUBLIC S16 rgROMCmnDatReq(datReq)
RgRguCmnDatReq *datReq;
#endif
{
   RgCellCb    *cell;
   RgErrInfo   err;
   S16         ret;
/* ADD Changes for Downlink UE Timing Optimization */
#ifdef LTEMAC_DLUE_TMGOPTMZ
   CmLteTimingInfo timingInfo;
   RgDlSf   *sf;
#endif

   TRC2(rgROMCmnDatReq)

   RGDBGPRM((rgPBuf, "rgROMCmnDatReq(): datReq->cellId = %u; datReq->lcId = %u\n",
                                            datReq->cellId, datReq->lcId));

   err.errType = RGERR_ROM_CMNDATREQ;
   if ((cell = rgDBMGetCellCb(datReq->cellId)) == NULLP)
   {
#if (ERRCLASS & ERRCLS_INT_PAR)
      /* Handle Cell fetch failure */
      RGLOGERROR(ERRCLS_INT_PAR,ERG003,(ErrVal)datReq->cellId,
                            "rgROMCmnDatReq(): Invalid cell Id");
#endif
      err.errCause = RGERR_ROM_INV_CELL_ID;
      /* Update stats */
      rgUpdtRguCmnSts(RG_RGU_SDU_DROP);
      RETVALUE(RFAILED);
   }

   /* Handle CCCH datReq */
   if (datReq->lcId == cell->dlCcchId)
   {
      RGDBGINFO((rgPBuf, "rgROMCmnDatReq(): Msg4 arrived on CCCH\n"));
      ret = rgROMHndlCcchDatReq(cell, datReq, &err);

      /*Get the timing Info*/
/* ADD Changes for Downlink UE Timing Optimization */
#ifdef LTEMAC_DLUE_TMGOPTMZ
      timingInfo.subframe = (U8)((datReq->transId >> 8) & 0XFF);
      timingInfo.sfn = (U16)((datReq->transId >> 16) & 0xFFFF);
#endif
   } 
   else
   {
      ret = rgROMHndlBcchPcchDatReq(cell, datReq, &err);

      /*Get the timing Info*/
/* ADD Changes for Downlink UE Timing Optimization */
#ifdef LTEMAC_DLUE_TMGOPTMZ
      timingInfo.subframe = (U8)(datReq->transId & 0XFF);
      timingInfo.sfn = (U16)((datReq->transId >> 8) & 0xFFFF);
#endif
   }

   /* Update stats */
   if (ret == RFAILED)
   {
      rgUpdtRguCmnSts(RG_RGU_SDU_DROP);
   }
   else
   {
      /* Update stats with number of SDUs received */
      rgUpdtRguCmnSts(RG_RGU_SDU_RCVD);
   }

/* ADD Changes for Downlink UE Timing Optimization */
#ifdef LTEMAC_DLUE_TMGOPTMZ
   sf = &cell->subFrms[timingInfo.subframe];

   /*Presently this function is not returning RFAILED, thus not checking
     for failure condition.*/
   ret = rgROMUpdDlSfRemDataCnt(cell, sf);
#endif

   RETVALUE(ret);
}  /* rgROMCmnDatReq */

/**
 * @brief Handler for DatReq received on RGU for CCCH.
 *
 * @details
 *
 *     Function : rgROMHndlCcchDatReq
 *     
 *     This function shall fetch the raCb with the given rnti and indicate msg4
 *     arrival to RAM.
 *     
 *           
 *  @param[in]  RgCellCb       *cell
 *  @param[in]  RgRguCmnDatReq *datReq 
 *  @param[out] RgErrInfo      *err
 *  @return  S16
 *      -# ROK 
 *      -# RFAILED 
 **/
#ifdef ANSI
PRIVATE S16 rgROMHndlCcchDatReq
(
RgCellCb       *cell,
RgRguCmnDatReq *datReq,
RgErrInfo      *err
)
#else
PRIVATE S16 rgROMHndlCcchDatReq(cell, datReq, err)
RgCellCb       *cell;
RgRguCmnDatReq *datReq;
RgErrInfo      *err;
#endif
{
   RgUeCb   *ue;
   U8       hqPId;
   RgDlHqProcCb *hqProc;
   CmLteTimingInfo timingInfo;
   RgDlSf   *sf;
#if (ERRCLASS & ERRCLS_DEBUG)
   RgUstaDgn   dgn;      /* Alarm diagnostics structure */
#endif

   TRC2(rgROMHndlCcchDatReq);

   RGDBGPRM((rgPBuf, "rgROMHndlCcchDatReq():cell->cellId = %u; "
            "datReq->lcId = %u\n", cell->cellId, datReq->lcId));

   err->errType = RGERR_ROM_CMNDATREQ;

   if ((ue = rgDBMGetUeCb(cell, datReq->u.rnti)) == NULLP)
   {
      if ((ue = rgDBMGetUeCbFromRachLst(cell, datReq->u.rnti)) == NULLP)
      {
   #if (ERRCLASS & ERRCLS_INT_PAR)
         /* Handle Ue fetch failure */
         RGLOGERROR(ERRCLS_INT_PAR,ERG004,(ErrVal)datReq->u.rnti,
                              "rgROMHndlCcchDatReq(): Invalid ue Id");
   #endif
         err->errCause = RGERR_ROM_INV_UE_ID;
         RETVALUE(RFAILED);
      }
   }

   timingInfo.subframe = (U8)((datReq->transId >> 8) & 0XFF);
   timingInfo.sfn = (U16)((datReq->transId >> 16) & 0xFFFF);
   sf = &cell->subFrms[timingInfo.subframe];

   if( (sf->txDone == TRUE) ||
       (!RG_TIMEINFO_SAME(sf->schdTime,timingInfo)))
   {
#if (ERRCLASS & ERRCLS_DEBUG)
      /* Transmission is already done for this subframe. This is a delayed
       * datReq. So discard */
      rgFillDgnParams(&dgn, LRG_USTA_DGNVAL_MEM);
      rgLMMStaInd(LCM_CATEGORY_PROTOCOL, LCM_EVENT_UI_INV_EVT,
                              LRG_CAUSE_DELAYED_DATREQ, &dgn);
#endif
      err->errCause = RGERR_ROM_DELAYED_DATREQ;
      RETVALUE(RFAILED);
   }

   hqPId = (U8)(datReq->transId);
   hqPId = hqPId >> 2;

   /* get harq process and invoke DHM */
   rgDHMGetHqProcFrmId(ue, hqPId, &hqProc);
 
   /*rg009.201 - changed for CR timer implementation*/
   /* invoke DHM to process CCCH data */
   if (rgDHMHndlCmnDatReq(hqProc, datReq, err) == RFAILED)
   {
      RGDBGERR((rgPBuf, "rgROMHndlCcchDatReq(): Handling of Data request in DHM \
               failed\n"));
      MSPD_ERR("rgROMHndlCcchDatReq(): Handling of Data request in DHM \
               failed\n");
      /* rg007.201 - Changes for MIMO feature addition */
      /* rg008.201 - Removed dependncy on MIMO compile-time flag */
      /* Release First TB */
	  #ifdef PWAV
	  MSPD_DBG("Common Data Request failed for UE:%d @ (%d, %d)\n", ue->ueId, cell->crntTime.sfn, cell->crntTime.subframe);
	  #endif
      rgDHMRlsHqProcTB(hqProc, 1);
      /* err shall be filled in appropriately by DHM */
      RETVALUE(RFAILED);
   }
 
   RETVALUE(ROK); 
} /* rgROMHndlCcchDatReq */


/**
 * @brief Handler for DatReq received on RGU for BCCH or PCCH.
 *
 * @details
 *
 *     Function : rgROMHndlBcchPcchDatReq
 *     
 *     This function shall store the buffer and time to transmit in lcCb.
 *     
 *           
 *  @param[in]  RgCellCb       *cell
 *  @param[in]  RgRguCmnDatReq *datReq 
 *  @param[out] RgErrInfo      *err
 *  @return  S16
 *      -# ROK 
 *      -# RFAILED 
 **/
#ifdef ANSI
PRIVATE S16 rgROMHndlBcchPcchDatReq
(
RgCellCb       *cell,
RgRguCmnDatReq *datReq,
RgErrInfo      *err
)
#else
PRIVATE S16 rgROMHndlBcchPcchDatReq(cell, datReq, err)
RgCellCb       *cell;
RgRguCmnDatReq *datReq;
RgErrInfo      *err;
#endif
{
   RgPcchLcCb      *pcch;
   /* rg004.201-MOD-Modified for SI Enhancement*/
#ifndef RGR_SI_SCH
   RgBcchBchLcCb   *bch;
   RgBcchDlschLcCb *bcch;
#endif/*RGR_SI_SCH*/
   RgDlSf          *sf;
   CmLteTimingInfo timingInfo;
#if (ERRCLASS & ERRCLS_DEBUG)
   RgUstaDgn   dgn;      /* Alarm diagnostics structure */
#endif

   TRC2(rgROMHndlBcchPcchDatReq);

   RGDBGPRM((rgPBuf, "rgROMHndlBcchPcchDatReq(): cell->cellId = %u; \
                      datReq->lcId = %u\n", cell->cellId, datReq->lcId));

   timingInfo.subframe = (U8)(datReq->transId & 0XFF);
   timingInfo.sfn = (U16)((datReq->transId >> 8) & 0xFFFF);
   sf = &cell->subFrms[timingInfo.subframe];

   if( (sf->txDone == TRUE) ||
       (!RG_TIMEINFO_SAME(sf->schdTime,timingInfo)))
   {
#if (ERRCLASS & ERRCLS_DEBUG)
      /* Transmission is already done for this subframe. This is a delayed
       * datReq. So discard */
      rgFillDgnParams(&dgn, LRG_USTA_DGNVAL_MEM);
      rgLMMStaInd(LCM_CATEGORY_PROTOCOL, LCM_EVENT_UI_INV_EVT,
                              LRG_CAUSE_DELAYED_DATREQ, &dgn);
#endif
      err->errCause = RGERR_ROM_DELAYED_DATREQ;
      RETVALUE(RFAILED);
   }

#ifndef RGR_SI_SCH
   bcch = rgDBMGetBcchOnDlsch(cell,datReq->lcId);
   if (bcch )
   {
      /* Store BCCH-DLSCH data received in Scheduled subframe */
      sf->bcch.tb = datReq->pdu;

      SCpyMsgMsg(datReq->pdu, RG_GET_MEM_REGION(rgCb),
               RG_GET_MEM_POOL(rgCb), &bcch->tb);

      RETVALUE(ROK);
   }

   bch = rgDBMGetBcchOnBch(cell);
   if ((bch) && (bch->lcId == datReq->lcId))
   {
      /* Store BCH data received in Scheduled subframe */
      sf->bch.tb = datReq->pdu;
      RETVALUE(ROK);
   }
#endif/*RGR_SI_SCH*/

   pcch = rgDBMGetPcch(cell);
   if ((pcch) && (pcch->lcId == datReq->lcId))
   {
      /* Store PCCH-DLSCH data received in Scheduled subframe */
      sf->pcch.tb = datReq->pdu;
      RETVALUE(ROK);
   }

   /* Handle lcCb fetch failure */
   RGLOGERROR(ERRCLS_INT_PAR,ERG005,(ErrVal)datReq->lcId,
                  "rgROMHndlBcchPcchDatReq(): Invalid Lc Id");
   err->errCause = RGERR_ROM_INV_LC_ID;

   RETVALUE(RFAILED);
} /* rgROMHndlBcchPcchDatReq */

/**
 * @brief Handler for StaRsp received on RGU for a dedicated logical channel.
 *
 * @details
 *
 *     Function : rgROMDedStaRsp
 *     
 *     This fucntion shall store the BO reported for the given logical
 *     channel.
 *     
 *           
 *  @param[in]  RgRguDedStaRsp *staRsp 
 *  @return  S16
 *      -# ROK 
 *      -# RFAILED 
 **/
#ifdef ANSI
PUBLIC S16 rgROMDedStaRsp
(
RgRguDedStaRsp *staRsp
)
#else
PUBLIC S16 rgROMDedStaRsp(staRsp)
RgRguDedStaRsp *staRsp;
#endif
{
   RgCellCb   *cell;

   /* Moving the error variables and assignments to available scope */

   TRC2(rgROMDedStaRsp)

   /* Avoiding memset, as the variables of this are getting 
      initialized */

   RGDBGPRM((rgPBuf, "rgROMDedStaRsp(): staRsp->cellId = %u; staRsp->rnti = %u, \
             staRsp->lcId = %u\n",staRsp->cellId, staRsp->rnti, staRsp->lcId));

   if ((cell = rgDBMGetCellCb(staRsp->cellId)) != NULLP)
   {
            RgInfDedBoRpt boRpt;
            Pst        schPst;
            boRpt.cellSapId  = cell->schInstMap.cellSapId;
            boRpt.cellId  = staRsp->cellId;
            boRpt.rnti    = staRsp->rnti; 
            boRpt.lcId    = staRsp->lcId; 
            boRpt.bo      = staRsp->boReport.bo;
            rgGetPstToInst(&schPst, cell->schInstMap.schInst);
            schPst.event = 0;
            RgMacSchDedBoUpdt(&schPst, &boRpt);
            RETVALUE(ROK);
   }
   RGDBGERR((rgPBuf,
             "\nrgROMDedStaRsp(): Invalid cell(%d) or UE(%d) or LC Id(%d)",
              staRsp->cellId, staRsp->rnti, staRsp->lcId));
   MSPD_ERR(
             "\nrgROMDedStaRsp(): Invalid cell(%d) or UE(%d) or LC Id(%d)",
              staRsp->cellId, staRsp->rnti, staRsp->lcId);

   RETVALUE(RFAILED);
}  /* rgROMDedStaRsp */

/**
 * @brief Handler for StaRsp received on RGU for a common logical channel.
 *
 * @details
 *
 *     Function : rgROMCmnStaRsp
 *     
 *     This fucntion shall invoke rgROMHndlCcchStaRsp() for status response on
 *     CCCH and shall invoke rgROMHndlBcchPcchStaRsp() for status response on
 *     BCCH or PCCH.
 *     
 *           
 *  @param[in]  RgRguCmnStaRsp *staRsp 
 *  @return  S16
 *      -# ROK 
 *      -# RFAILED 
 **/
#ifdef ANSI
PUBLIC S16 rgROMCmnStaRsp
(
RgRguCmnStaRsp *staRsp
)
#else
PUBLIC S16 rgROMCmnStaRsp(staRsp)
RgRguCmnStaRsp *staRsp;
#endif
{
   RgCellCb   *cell;
   RgErrInfo  err;

   TRC2(rgROMCmnStaRsp)

   /* rg005.201 Moving the error assignments to available scope 
    * - for optimization */

   RGDBGPRM((rgPBuf, "rgROMCmnStaRsp(): cellId = %u; rnti = %u; lcId = %u\n",
                               staRsp->cellId, staRsp->u.rnti, staRsp->lcId));

   if ((cell = rgDBMGetCellCb(staRsp->cellId)) == NULLP)
   {
      /* Handle Cell fetch failure */
      RGLOGERROR(ERRCLS_INT_PAR,ERG006,(ErrVal)staRsp->cellId,
                           "rgROMCmnStaRsp(): Invalid cell Id");
      err.errType = RGERR_ROM_CMNSTARSP;
      err.errCause = RGERR_ROM_INV_CELL_ID;
      RETVALUE(RFAILED);
   }

   /* handle status response on CCCH */
   if(staRsp->lcId == cell->dlCcchId)
   {
      RGDBGINFO((rgPBuf, "rgROMCmnStaRsp(): Starsp arrived on CCCH\n"));
      rgROMHndlCcchStaRsp(cell, staRsp, &err); 
   }
   else
   {
      rgROMHndlBcchPcchStaRsp(cell, staRsp, &err); 
   }
   
   RETVALUE(ROK);
}  /* rgROMCmnStaRsp */

#ifdef LTE_L2_MEAS
#ifdef LTEMAC_R9
/**
 * @brief Handler for Request received on RGU for a UL Throughput measurement
 * enabled logical channel.
 *
 * @details
 *
 *     Function :rgROML2MUlThrpMeasReq 
 *     
 *     This function shall store the L2M UL Throughput Measurement status  information 
 *     for the given logical channel.
 *     
 *           
 *  @param[in]  RgRguL2MUlThrpMeasReq *measReq 
 *  @return  S16
 *      -# ROK 
 *      -# RFAILED 
 **/
#ifdef ANSI
PUBLIC S16 rgROML2MUlThrpMeasReq 
(
RgRguL2MUlThrpMeasReq *measReq
)
#else
PUBLIC S16 rgROML2MUlThrpMeasReq(measReq)
RgRguL2MUlThrpMeasReq *measReq;
#endif
{
   RgCellCb   *cell;
   RgUeCb     *ue;
   U8         lcgId;
   U8         loop;
   TRC2(rgROML2MUlThrpMeasReq)


   RGDBGPRM((rgPBuf, "rgROML2MUlThrpMeasReq(): measReq->cellId = %u; \
               measReq->rnti = %u, measReq->numlcId = %u\n",measReq->cellId, 
                                     measReq->rnti, measReq->numLcId));

   if ((cell = rgDBMGetCellCb(measReq->cellId)) != NULLP)
   {
      if ((ue = rgDBMGetUeCb(cell, measReq->rnti)) != NULLP)
      {
         for(loop=0; loop<measReq->numLcId;loop++)
         {
            RgUlLcCb   *ulLc;
            if ((ulLc = rgDBMGetUlDedLcCb(ue, measReq->lcId[loop])) != NULLP)
            {
               ue->ul.lcCb[measReq->lcId[loop]].measOn = measReq->enbMeas;
               if(ue->ul.lcCb[measReq->lcId[loop]].measOn == FALSE)
               {
                  lcgId=ue->ul.lcCb[measReq->lcId[loop]].lcgId;
                  ue->ul.lcgArr[lcgId].lcgBsInfo.outStndngBs = 0;
                  ue->ul.lcgArr[lcgId].lcgBsInfo.firstDatSegRcvd = FALSE;
               }
            }
         }
         RETVALUE(ROK);
      }
   }
   RGDBGERR((rgPBuf,
             "\nrgROML2MUlThrpMeasReq(): Invalid cell(%d) or UE(%d) or LC ID",
              measReq->cellId, measReq->rnti));
   MSPD_ERR(
             "\nrgROML2MUlThrpMeasReq(): Invalid cell(%d) or UE(%d) or LC ID",
              measReq->cellId, measReq->rnti);
   RETVALUE(RFAILED);
}  /* rgROML2MUlThrpMeasReq */
#endif
#endif

/**
 * @brief Handler for StaRsp received on RGU for CCCH.
 *
 * @details
 *
 *     Function : rgROMHndlCcchStaRsp
 *     
 *     This function shall fetch the raCb with the given RNTI and ask RAM to
 *     update BO. 
 *     
 *           
 *  @param[in]  RgCellCb       *cell
 *  @param[in]  RgRguCmnStaRsp *staRsp
 *  @param[out] RgErrInfo      *err
 *  @return  S16
 *      -# ROK 
 *      -# RFAILED 
 **/
#ifdef ANSI
PRIVATE S16 rgROMHndlCcchStaRsp
(
RgCellCb       *cell,
RgRguCmnStaRsp *staRsp,
RgErrInfo      *err
)
#else
PRIVATE S16 rgROMHndlCcchStaRsp(cell, staRsp, err)
RgCellCb       *cell;
RgRguCmnStaRsp *staRsp;
RgErrInfo      *err;
#endif
{
   Pst      schPst;
   RgInfCmnBoRpt boRpt;

   TRC2(rgROMHndlCcchStaRsp);

   /* rg005.201 Removing memset for optimization */

   RGDBGPRM((rgPBuf, "rgROMHndlCcchStaRsp(): staRsp->u.rnti = %u\n",
                                             staRsp->u.rnti));
   boRpt.cellSapId  = cell->schInstMap.cellSapId;
   boRpt.cellId  = staRsp->cellId;
   boRpt.u.rnti    = staRsp->u.rnti; 
   boRpt.lcId    = staRsp->lcId; 
   boRpt.lcType  = staRsp->lcType; 
   boRpt.bo      = staRsp->bo;
   rgGetPstToInst(&schPst, cell->schInstMap.schInst);
   RgMacSchCmnBoUpdt(&schPst, &boRpt);

   RETVALUE(ROK);
} /* rgROMHndlCcchStaRsp */


/**
 * @brief Handler for StaRsp received on RGU for BCCH or PCCH.
 *
 * @details
 *
 *     Function : rgROMHndlBcchPcchStaRsp
 *     
 *     This function shall store the buffer and time to transmit in lcCb.
 *     
 *           
 *  @param[in]  RgCellCb       *cell
 *  @param[in]  RgRguCmnStaRsp *staRsp
 *  @param[out] RgErrInfo      *err
 *  @return  S16
 *      -# ROK 
 *      -# RFAILED 
 **/
#ifdef ANSI
PRIVATE S16 rgROMHndlBcchPcchStaRsp
(
RgCellCb       *cell,
RgRguCmnStaRsp *staRsp,
RgErrInfo      *err
)
#else
PRIVATE S16 rgROMHndlBcchPcchStaRsp(cell, staRsp, err)
RgCellCb       *cell;
RgRguCmnStaRsp *staRsp;
RgErrInfo      *err;
#endif
{
   Pst      schPst;
   RgInfCmnBoRpt boRpt;

   TRC2(rgROMHndlBcchPcchStaRsp);
   cmMemset((U8*)&schPst, (U8)0, sizeof(Pst));

#ifndef ALIGN_64BIT
   RGDBGPRM((rgPBuf, "rgROMHndlBcchPcchStaRsp(): lcId = %u, bo = %ld,\
                  timeToTx.sfn = %u, timeToTx.subframe = %u\n", 
                  staRsp->lcId, staRsp->bo, 
                  staRsp->u.timeToTx.sfn, staRsp->u.timeToTx.subframe));
#else
   RGDBGPRM((rgPBuf, "rgROMHndlBcchPcchStaRsp(): lcId = %u, bo = %d,\
                  timeToTx.sfn = %u, timeToTx.subframe = %u\n", 
                  staRsp->lcId, staRsp->bo, 
                  staRsp->u.timeToTx.sfn, staRsp->u.timeToTx.subframe));
#endif
   if (rgDBMChkCmnLcCb(cell, staRsp->lcId) != ROK)
   {
      /* Handle lcCb fetch failure */
      RGLOGERROR(ERRCLS_INT_PAR,ERG007,(ErrVal)staRsp->lcId,
                        "rgROMHndlBcchPcchStaRsp(): Invalid Lc Id");
      err->errCause = RGERR_ROM_INV_LC_ID;
      RETVALUE(RFAILED);
   }
   /* MS_WORKAROUND : This is to ensure that the queue for BCH is not filled with old BO requests :
         This assumes that BO is not received more than 4 frames in advance from the enodeb application */
	if (cell->bcchBchInfo.lcId == staRsp->lcId)
	{
	   U32 nextBchSfn;

	   nextBchSfn = (cell->crntTime.sfn + 4 - (cell->crntTime.sfn%4)) % RG_MAX_SFN;
	   if ((staRsp->u.timeToTx.sfn != nextBchSfn) ||
	   	((staRsp->u.timeToTx.sfn == cell->crntTime.sfn) && (cell->crntTime.subframe >= 7)))
	   {
#ifdef MSPD
	      MSPD_DBG("Dropping delayed BCH BO(%u) for (%u,%u) @ (%u,%u) nextBchSfn(%u)\n", 
		   	staRsp->lcId, staRsp->u.timeToTx.sfn, staRsp->u.timeToTx.subframe,
		   	cell->crntTime.sfn, cell->crntTime.subframe, nextBchSfn); 
#endif
		  RETVALUE(ROK);
	   }
	}

   boRpt.cellSapId     = cell->schInstMap.cellSapId;
   boRpt.cellId     = staRsp->cellId;
   boRpt.u.timeToTx = staRsp->u.timeToTx; 
   boRpt.lcId       = staRsp->lcId; 
   boRpt.lcType     = staRsp->lcType; 
   boRpt.bo         = staRsp->bo;
   rgGetPstToInst(&schPst, cell->schInstMap.schInst);
   RgMacSchCmnBoUpdt(&schPst, &boRpt);

   RETVALUE(ROK);
} /* rgROMHndlBcchPcchStaRsp */

/* ADD Changes for Downlink UE Timing Optimization */
#ifdef LTEMAC_DLUE_TMGOPTMZ
/**
 * @brief Handler for updating DL SF information on receiving
 *  DL dedicated data, CCCH, BCCH/PCCH data request.
 *
 * @details
 *
 *     Function : rgROMUpdDLSfRemDataCnt
 *     
 *           
 *  @param[in]  RgCellCb       *cell
 *  @param[in]  RgDlSf         *dlSf;
 *  @return  S16
 *      -# ROK 
 *      -# RFAILED 
 **/
#ifdef ANSI
PRIVATE S16 rgROMUpdDlSfRemDataCnt
(
RgCellCb       *cellCb,
RgDlSf         *dlSf
)
#else
PRIVATE S16 rgROMUpdDlSfRemDataCnt(cellCb, dlSf)
RgCellCb       *cellCb;
RgDlSf         *dlSf;
#endif
{
   RgErrInfo            err;
   S16      ret;

   TRC2(rgROMUpdDlSfRemDataCnt);

   RGDBGPRM((rgPBuf, "rgROMUpdDLSfRemDataCnt():cell Id(%d) "
                  "timeToTx.sfn = %u, timeToTx.subframe = %u\n", 
                  cellCb->cellId, dlSf->schdTime.sfn, 
                  dlSf->schdTime.subframe));

   if(!dlSf->remDatReqCnt)
   {
       /*This is an error scenario of RLC generating more data          
        * request than the allocation. Do nothing for this. */
      RGDBGERR ((rgPBuf,"rgROMUpdDlSfRemDataCnt():RX new data while "
                  "remDatReqCnt is 0 for cellId (%d))\n",
                  cellCb->cellId));
      MSPD_ERR ("rgROMUpdDlSfRemDataCnt():RX new data while "
                  "remDatReqCnt is 0 for cellId (%d))\n",
                  cellCb->cellId);
      RETVALUE(RFAILED);
   }

   /*Decrement the remaining data request to be received countter
     for this SF.
     Check if this was the last pending data request for this DL SF.*/
    /* MS_FIX[ccpu00126310]: Tracks Data Requests from RLC for both loosely and tight coupled 
     RLC-MAC */
   if((0 == --dlSf->remDatReqCnt) && !(dlSf->txDone) &&
      (RG_TIMEINFO_SAME(cellCb->crntTime, dlSf->schdTime)) && (dlSf->statIndDone))
   {
      /*Check if we have already received a TTI for this Data,
        if that is the case then we need to send TFU Data request 
        to PHY */

      if ((ret = rgTOMUtlProcDlSf (dlSf, cellCb, &err)) != ROK)
      {
         RGDBGERR ((rgPBuf,"rgROMUpdDlSfRemDataCnt() Unable to process "
                "downlink subframe for cellId (%d))\n", cellCb->cellId));
         MSPD_ERR ("rgROMUpdDlSfRemDataCnt() Unable to process "
                "downlink subframe for cellId (%d))\n", cellCb->cellId);
         err.errType = RGERR_ROM_DEDDATREQ;
      }

      /* Mark this frame as sent */
      dlSf->txDone = TRUE;
   }

   RETVALUE(ROK);
} /* rgROMUpdDlSfRemDataCnt*/
#endif

/**********************************************************************
 
         End of file:     gk_rom.c@@/main/3 - Sat Jul 30 02:21:34 2011
 
**********************************************************************/
 
/**********************************************************************
 
        Notes:
 
**********************************************************************/
 
/**********************************************************************
 
        Revision history:
 
**********************************************************************/
/********************************************************************90**
 
     ver       pat       init                  description
------------ --------    ---- ----------------------------------------------
/main/1      ---          ps                 1. Initial Release.
/main/2      ---          ps                 1. LTE MAC 2.1 release
             rg004.201    rnt                1. Modified for SI Enhancement
             rg005.201    sm                 1. Changes for optimization
             rg007.201    ap                 1. Added support for MIMO
             rg008.201    sd                 1. Removed dependncy on 
                                                MIMO compile-time flag
             rg009.201  rsharon              1.[ccpu00112372]Changes for CR
                                               timer.
/main/3      ---        gvj  1. Updated for LTE MAC Rel 3.1           
*********************************************************************91*/
