/********************************************************************16**

                         (c) COPYRIGHT 2009 by 
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

                    Email: support@trillium.com
                    Web: http://www.ccpu.com

*********************************************************************17*/


/********************************************************************20**
  
        Name:    LTE-RLC Layer - Lower Interface Functions
    
        Type:    C file
  
     Desc:     Source code for RLC Lower Interface Module.
               This file contains following functions
      
        --KwLiRguBndCfm
        --KwLiRguCDatInd
        --KwLiRguDDatInd
        --KwLiRguCStaInd
        --KwLiRguDStaInd
        --KwLiRguHqStaInd

     File:     gp_lim.c

     Sid:      gp_lim.c@@/main/2 - Fri Nov 13 14:13:38 2009

     Prg:      gk 

**********************************************************************/
/** @file gp_lim.c
@brief RLC Lower Interface Module
*/


/*
 *      This software may be combined with the following TRILLIUM
 *      software:
 *
 *      part no.                      description
 *      --------    ----------------------------------------------
 *      1000241     LTE-MAC
 *      1000369     LTE-RRC
 */


/* header (.h) include files */
#include "envopt.h"        /* environment options */
#include "envdep.h"        /* environment dependent */
#include "envind.h"        /* environment independent */

#include "gen.h"           /* general */
#include "ssi.h"           /* system services */
#include "cm5.h"           /* common timer defines */
#include "cm_tkns.h"       /* common tokens defines */
#include "cm_mblk.h"       /* common memory allocation library defines */
#include "cm_llist.h"      /* common link list  defines  */
#include "cm_hash.h"       /* common hash list  defines */
#include "cm_lte.h"        /* common LTE defines */
#include "lkw.h"           /* LKW defines */
#include "ckw.h"           /* CKW defines */
#include "kwu.h"           /* KWU defines */
#include "rgu.h"           /* RGU defines */
#include "kw_err.h"
#include "kw_env.h"        /* RLC environment options */
#ifdef KW_PDCP
#include "lpj.h"           /* LPJ defines */
#include "cpj.h"           /* CPJ defines */
#include "pju.h"           /* PJU defines */
#endif /* KW_PDCP */

#include "kw.h"            /* RLC defines */

/* extern (.x) include files */
#include "gen.x"           /* general */
#include "ssi.x"           /* system services */

#include "cm5.x"           /* common timer library */
#include "cm_tkns.x"       /* common tokens */
#include "cm_mblk.x"       /* common memory allocation */
#include "cm_llist.x"      /* common link list */
#include "cm_hash.x"       /* common hash list */
#include "cm_lte.x"        /* common LTE includes */
#include "cm_lib.x"        /* common memory allocation library */
#include "lkw.x"           /* LKW */
#include "ckw.x"           /* CKW */
#include "kwu.x"           /* KWU */
#include "rgu.x"           /* RGU */
#ifdef KW_PDCP
#include "lpj.x"           /* LPJ defines */
#include "cpj.x"           /* CPJ defines */
#include "pju.x"           /* PJU defines */
#endif /* KW_PDCP */

#include "kw.x"

#ifdef __cplusplus
EXTERN "C" {
#endif /* __cplusplus */


/*****************************************************************************
 *                          RGU INTERFACE
 ****************************************************************************/
/**
 *
 * @brief 
 *
 *        Handler for bind confirmation from MAC.
 *
 * @b Description:
 *
 *        This function handles the bind confirmation received
 *        from MAC. 
 *
 *  @param[in] pst     Post structure  
 *  @param[in] suId    Service User ID
 *  @param[in] status  Status of confirmation 
 *  @return  S16
 *      -# ROK 
 *      -# RFAILED
 *
 */

#ifdef ANSI
PUBLIC S16 KwLiRguBndCfm
(
Pst *pst,   /* Post structure */
SuId suId,   /* service user SAP ID */
U8 status   /* status of confirmation */
)
#else
PUBLIC S16 KwLiRguBndCfm (pst, suId, status)
Pst *pst;    /* Post structure */
SuId suId;    /* service user SAP ID */
U8 status;    /* status of confirmation */
#endif
{
   U16             event;              /* Event */
   U16             cause;              /* Cause */
   KwRguSapCb      *rguSap;            /* RGU SAP Control Block */

   TRC3(KwLiRguBndCfm)

#ifdef DEBUGP
   KWDBGP((KW_DBGMASK_INF | KW_DBGMASK_BRIEF), (_kwPBuf,
           "KwLiRguBndCfm(post, suId(%d), status(%d)\n", suId, status));
#endif

   UNUSED(pst);

#if (ERRCLASS & ERRCLS_INT_PAR)
   if (kwCb.init.cfgDone != TRUE)
   {
      /* INNC: ERROR CLASS */
      KWLOGERROR(ERRCLS_INT_PAR, EKW040, (ErrVal) suId,
                 "KwLiRguBndCfm: General configuration not done\n");
      KW_SEND_SAPID_ALARM(suId, LKW_EVENT_LI_BND_CFM, LCM_CAUSE_INV_STATE);

      RETVALUE(RFAILED);
   }

   if (suId < 0)
   {
      /* INNC: ERROR CLASS */
      KWLOGERROR(ERRCLS_INT_PAR, EKW041, (ErrVal) suId,
                 "KwLiRguBndCfm: Invalid suId\n");
      KW_SEND_SAPID_ALARM(suId, LKW_EVENT_LI_BND_CFM, LCM_CAUSE_INV_SUID);

      RETVALUE(RFAILED);
   }
#endif /* ERRCLASS & ERRCLS_INT_PAR */

   rguSap = &(kwCb.rguSap);

#ifdef DEBUGP
   KWDBGP((KW_DBGMASK_INF | KW_DBGMASK_DETAIL),
       (_kwPBuf, "KwLiRguBndCfm: For RGU SAP state=%d\n",
       rguSap->state));
#endif

   /* Check rguSap state */
   switch (rguSap->state)
   {
      case KW_SAP_BINDING:
      {
         kwStopTmr ((PTR)rguSap, KW_EVT_WAIT_BNDCFM);

         rguSap->retryCnt = 0;
          
         if (status == CM_BND_OK)
         {
            rguSap->state = KW_SAP_BND;
            event = LCM_EVENT_BND_OK;
            cause = LKW_CAUSE_SAP_BNDENB;
         }
         else
         {
            rguSap->state = KW_SAP_CFG;
            event = LCM_EVENT_BND_FAIL;
            cause = LKW_CAUSE_UNKNOWN;
         }
      }
      break;

     default:
        event = LKW_EVENT_RGU_BND_CFM;
        cause = LCM_CAUSE_INV_STATE;
        break;
   }

   /* Send an alarm with proper event and cause */
   KW_SEND_SAPID_ALARM(suId, event, cause);

   RETVALUE(ROK);
} /* KwLiRguBndCfm */
#if 0

/**
 *
 * @brief 
 *
 *        Handler for getting the PDU from MAC for common logical channels. 
 *
 * @b Description:
 *
 *        This function receives the PDU from MAC for common logical channels
 *        and invokes the handler for processing the PDU.
 *
 *  @param[in] pst     -   Post structure  
 *  @param[in] suId    -   Service User ID
 *  @param[in] datInd  -   Data Indication Information 
 *
 *  @return  S16
 *      -# ROK 
 *      -# RFAILED
 *
 */
#ifdef ANSI
PUBLIC S16 KwLiRguCDatInd
(
Pst            *pst,                   /* Post Structure */
SuId           suId,                   /* Service User SAP ID */
RguCDatIndInfo *datInd                 /* Data Indication Information */
)
#else
PUBLIC S16 KwLiRguCDatInd(pst,suId,datInd)
Pst            *pst;                   /* Post Structure */
SuId           suId;                   /* Service User SAP ID */
RguCDatIndInfo *datInd;                /* Data Indication Information */
#endif
{
   S16         ret;                    /* Return Value */
   KwRbCb      *rbCb;                  /* RB Control Block */

   TRC3(KwLiRguCDatInd)

   KWDBGP((KW_DBGMASK_INF | KW_DBGMASK_BRIEF), (_kwPBuf,
           "KwLiRguCDatInd(post, suId(%d)\n", suId));

   ret = ROK;
   rbCb = NULLP;
   /* kw006.201 ccpu00120058, Added array boundary condition check */
#if (ERRCLASS & ERRCLS_DEBUG)
   if(KW_MAX_LCH_PER_CELL <= datInd->lcId)
   {
      KWDBGP((KW_DBGMASK_INF | KW_DBGMASK_ERROR), (_kwPBuf, 
             "KwLiRguCDatInd: datInd->lcId(%d) is invalid\n", datInd->lcId));
      KW_PST_FREE(pst->region, pst->pool, datInd, sizeof(RguCDatIndInfo));
      RETVALUE(RFAILED);
   }
#endif /* (ERRCLASS & ERRCLS_DEBUG) */
   /* Fertch RbCb from lcId */
   ret = kwDbmFetchRbCbForLi(0, datInd->cellId, datInd->lcId, KW_DIR_UL, &rbCb);
   if (ret != ROK)
   {
      KWDBGP((KW_DBGMASK_INF | KW_DBGMASK_ERROR), (_kwPBuf,
               "KwLiRguCDatInd: kwDbmFetchRbCbForLi --\
               ueId(%d), cellId(%d), lcId(%d)\n",
               0, datInd->cellId, datInd->lcId));
      /* kw002.201 :Freeing from proper region */
      KW_PST_FREE(pst->region, pst->pool, datInd, sizeof(RguCDatIndInfo));
      RETVALUE(ret);
   }

   /* Dispatch to TM Module */
#ifdef CCPU_OPT
   ret = kwTmmRcvFrmLi(rbCb, datInd->rnti, datInd->pdu);
#else /* CCPU_OPT */
   ret = kwTmmRcvFrmLi(rbCb, datInd->pdu);
#endif /* CCPU_OPT */
#if (ERRCLASS & ERRCLS_DEBUG)
   if (ret != ROK)
   {
      KWDBGP((KW_DBGMASK_INF | KW_DBGMASK_ERROR), (_kwPBuf,
               "KwLiRguCDatInd: kwTmmRcvFrmLi Failed\n"));
      /* kw002.201 :Freeing from proper region */
      KW_PST_FREE(pst->region, pst->pool, datInd, sizeof(RguCDatIndInfo));
      RETVALUE(ret);
   }
#endif /* (ERRCLASS & ERRCLS_DEBUG) */

      /* kw002.201 :Freeing from proper region */
   KW_PST_FREE(pst->region, pst->pool, datInd, sizeof(RguCDatIndInfo));

   RETVALUE(ROK);
} /* KwLiRguCDatInd */


/**
 *
 * @brief 
 *
 *        Handler for getting PDU(s) from MAC for one or more 
 *        dedicated logical channels. 
 *
 * @b Description:
 *
 *        This function receives the PDU(s) from MAC for one or 
 *        more dedicated logical channels and invokes the 
 *        respective handlers for processing the PDU(s).
 *
 *  @param[in] pst     -   Post structure  
 *  @param[in] suId    -   Service User ID
 *  @param[in] datInd  -   Data Indication Information 
 *
 *  @return  S16
 *      -# ROK 
 *      -# RFAILED
 *
 */
#ifdef ANSI
PUBLIC S16 KwLiRguDDatInd
(
Pst            *pst,                   /* Post Structure */
SuId           suId,                   /* Service User SAP ID */
RguDDatIndInfo *datInd                 /* Data Indication Information */
)
#else
PUBLIC S16 KwLiRguDDatInd(pst,suId,datInd)
Pst            *pst;                   /* Post Structure */
SuId           suId;                   /* Service User SAP ID */
RguDDatIndInfo *datInd;                /* Data Indication Information */
#endif
{
   TRC3(KwLiRguDDatInd)

   KWDBGP((KW_DBGMASK_INF | KW_DBGMASK_BRIEF), (_kwPBuf,
          "KwLiRguDDatInd(pst, suId(%d))\n", suId));
   /* Dispatch to UM Module */
   if (kwUtlRcvFrmLi(datInd) != ROK)
   {
      KWDBGP((KW_DBGMASK_INF | KW_DBGMASK_ERROR), (_kwPBuf,
               "KwLiRguDDatInd: kwUtlRcvFrmLi Failed\n"));
   }

   /* kw002.201 :Freeing from proper region */
   KW_PST_FREE(pst->region, pst->pool, datInd, sizeof(RguDDatIndInfo));
   RETVALUE(ROK);
} /* KwLiRguDDatInd */


/**
 *
 * @brief 
 *
 *        Handler for trigerring the data transfer from RLC to MAC
 *        for common logical channels.
 *
 * @b Description:
 *
 *        This function receives the size of the PDU to be transmitted
 *        and acts as a trigger for forming PDU and sending it to MAC. 
 *
 *  @param[in] pst     -   Post structure  
 *  @param[in] suId    -   Service User ID
 *  @param[in] staInd  -   Status Indication Information.
 *
 *  @return  S16
 *      -# ROK 
 *      -# RFAILED
 *
 */
#ifdef ANSI
PUBLIC S16 KwLiRguCStaInd
(
Pst       *pst,                         /* Post Structure */
SuId       suId,                         /* Service User ID */
RguCStaIndInfo *staInd   /* Status Indication Information for Common Logical Channels */
)
#else
PUBLIC S16 KwLiRguCStaInd(pst,suId,staInd)
Pst       *pst;                         /* Post Structure */
SuId       suId;                         /* Service User ID */
RguCStaIndInfo *staInd;   /* Status Indication Information for Common Logical Channels */
#endif
{
   S16         ret;                    /* Return Value */
   KwRbCb      *rbCb;                  /* RB Control Block */

   TRC3(KwLiRguCStaInd)

   KWDBGP((KW_DBGMASK_INF | KW_DBGMASK_BRIEF), (_kwPBuf,
           "KwLiRguCStaInd(post, suId(%d)\n", suId));

   rbCb = NULLP;

   /* kw006.201 ccpu00120058, added boundary condition check */
#if (ERRCLASS & ERRCLS_DEBUG)
   if(KW_MAX_LCH_PER_CELL <= staInd->lcId)
   {
      KWDBGP((KW_DBGMASK_INF | KW_DBGMASK_ERROR), (_kwPBuf, 
             "KwLiRguCStaInd: staInd->lcId(%d) is invalid\n", staInd->lcId));
      KW_PST_FREE(pst->region, pst->pool, staInd, sizeof(RguCStaIndInfo));
      RETVALUE(RFAILED);
   }
#endif /* (ERRCLASS & ERRCLS_DEBUG) */
   /* Fertch RbCb from lcId */
   ret = kwDbmFetchRbCbForLi(0, staInd->cellId, staInd->lcId, KW_DIR_DL, &rbCb);
   if (ret != ROK)
   {
      /* INCNC: DEBUG_PRINT_ERROR
       * RB CB not found
       */
      KWDBGP((KW_DBGMASK_INF | KW_DBGMASK_ERROR), (_kwPBuf,
               "KwLiRguCStaInd: kwDbmFetchRbCbForLi --\
               ueId(%d), cellId(%d), lcId(%d)\n",
               0, staInd->cellId, staInd->lcId));
      /* kw002.201 :Freeing from proper region */
      KW_PST_FREE(pst->region, pst->pool, staInd, sizeof(RguCStaIndInfo));
      RETVALUE(ret);
   }

   /* Dispatch to TM Module */
   rbCb->transId = staInd->transId;
   ret = kwTmmSndToLi(rbCb, staInd);
   if (ret != ROK)
   {
      KWDBGP((KW_DBGMASK_INF | KW_DBGMASK_ERROR), (_kwPBuf,
               "KwLiRguCStaInd: kwTmmSndToLi Failed\n"));
      /* kw002.201 :Freeing from proper region */
      KW_PST_FREE(pst->region, pst->pool, staInd, sizeof(RguCStaIndInfo));
      RETVALUE(ret);
   }
   /* kw002.201 :Freeing from proper region */
   KW_PST_FREE(pst->region, pst->pool, staInd, sizeof(RguCStaIndInfo));
   RETVALUE(ROK);
} /* KwLiRguCStaInd */


/**
 *
 * @brief  
 *
 *        Handler for trigerring the data transfer from RLC to MAC
 *        for dedicated logical channels.
 *
 * @b Description:
 *
 *        This function receives the size of the PDUs to be transmitted to
 *        MAC via one or more dedicated logical channels and acts as a trigger
 *        for forming PDUs and sending them to MAC. 
 *
 *  @param[in] pst     -   Post structure  
 *  @param[in] suId    -   Service User ID
 *  @param[in] staInd  -   Status Indication Information. 
 *
 *  @return  S16
 *      -# ROK 
 *      -# RFAILED
 *
 */
#ifdef ANSI
PUBLIC S16 KwLiRguDStaInd
(
Pst       *pst,          /* Post Structure */
SuId       suId,          /* Service User ID */
RguDStaIndInfo *staInd   /* Status Indication Information for Dedicated Logical Channels */
)
#else
PUBLIC S16 KwLiRguDStaInd(pst,suId,staInd)
Pst       *pst;          /* Post Structure */
SuId       suId;          /* Service User ID */
RguDStaIndInfo *staInd;   /* Status Indication Information for Dedicated Logical Channels */
#endif
{
   volatile int t;
   U32 t2;

   TRC3(KwLiRguDStaInd)
#ifdef MSPD_MLOG_NEW
   t = MacGetTick();
#endif
   KWDBGP((KW_DBGMASK_INF | KW_DBGMASK_BRIEF), (_kwPBuf,
           "KwLiRguDStaInd(post, suId(%d)\n", suId));

   /* Dispatch to UM/AM Module */
   if (kwUtlSndToLi(staInd) != ROK)
   {
      KWDBGP((KW_DBGMASK_INF | KW_DBGMASK_ERROR), (_kwPBuf,
               "KwLiRguDStaInd: kwUtlSndToLi Failed\n"));
      /* kw002.201 :Freeing from proper region */
      KW_PST_FREE(pst->region, pst->pool, staInd, sizeof(RguDStaIndInfo));
      RETVALUE(RFAILED);
   }

   /* kw002.201 :Freeing from proper region */
   KW_PST_FREE(pst->region, pst->pool, staInd, sizeof(RguDStaIndInfo));
   #ifdef MSPD_MLOG_NEW
   MLogTask(PID_RLC_STA_IND, RESOURCE_LARM, t, t2 = MacGetTick());
   #endif

   RETVALUE(ROK);
} /* KwLiRguDStaInd */
/* kw005.201 added support for L2 Measurement */
#ifdef LTE_L2_MEAS

/**
 *
 * @brief  
 *
 *        Handler for indicating the Harq Status of the data sent.
 *
 * @b Description:
 *
 *        This function receives the harq status of the data sent to MAC.
 *        This information is used for two things.
 *        1. Computing the UuLoss of UM
 *        2. Computing the DL Delay for UM and AM.
 *
 *  @param[in] pst     -   Post structure  
 *  @param[in] suId    -   Service User ID
 *  @param[in] staInd  -   Harq Status Indication Information. 
 *
 *  @return  S16
 *      -# ROK 
 *      -# RFAILED
 *
 */
#ifdef ANSI
PUBLIC S16 KwLiRguHqStaInd
(
Pst              *pst,     /* Post Structure */
SuId             suId,     /* Service User ID */
RguHarqStatusInd *staInd   /* Status Indication Information for Dedicated Logical Channels */
)
#else
PUBLIC S16 KwLiRguHqStaInd(pst,suId,staInd)
Pst              *pst;      /* Post Structure */
SuId             suId;      /* Service User ID */
RguHarqStatusInd *staInd;   /* Status Indication Information for Dedicated Logical Channels */
#endif
{

   KwUeKey    ueKey;
   S16        ret; 
   KwUeCb     *ueCb;
   U8         tbIdx;

   TRC3(KwLiRguHqStaInd)

   KWDBGP((KW_DBGMASK_INF | KW_DBGMASK_BRIEF), (_kwPBuf,
           "KwLiRguHqStaInd(post, suId(%d)\n", suId));

   ueKey.cellId = staInd->cellId;
   ueKey.ueId   = staInd->ueId;

   ret = kwDbmFetchUeCb(ueKey.ueId, ueKey.cellId, &ueCb);
   if (ret != ROK )
   {
      /* Fetch UeCb failed */
      KWLOGERROR(ERRCLS_ADD_RES, EKW217, (ErrVal) RFAILED,
            "kwDbmFetchUeCb : Fetch UeCb Failed.");
      RETVALUE(RFAILED);
   }

   /*Call kwUtlProcHarqInd as many times as number of Tbs present*/
   for ( tbIdx = 0; tbIdx < staInd->numTbs; tbIdx++)
   {
      kwUtlProcHarqInd(staInd, ueCb, tbIdx);
   }
   KW_FREE(staInd,sizeof(RguHarqStatusInd));  

   RETVALUE(ROK);
} /* KwLiRguHqStaInd */
#endif /* LTE_L2_MEAS */
#endif
#ifdef __cplusplus
}
#endif /* __cplusplus */

/********************************************************************30**
         End of file:     gp_lim.c@@/main/2 - Fri Nov 13 14:13:38 2009
*********************************************************************31*/


/********************************************************************40**
  
        Notes:
  
*********************************************************************41*/

/********************************************************************50**

*********************************************************************51*/

   
/********************************************************************60**
  
        Revision history:
  
*********************************************************************61*/

/********************************************************************90**
     ver       pat    init                  description
------------ -------- ---- ----------------------------------------------
/main/1      ---       gk   1. initial release.
/main/2      ---       nm   1. LTERLC Release 2.1
/main/3      kw002.201 vp   1. Patch for optimization,fixes for multi-region
                               and retransmission.
/main/4      kw005.201 ap   1. Changed the for loop condition in KwLiRguHqStaInd
                               for calling kwUtlProcHarqInd as many times as
                               number of TBs present.
                            2. Added support for L2 Measurement.
                       rd   3. Updated with important Avtecs comments.
/main/5      kw006.201 rd   1. Added an lcId check for CdatInd and CstaInd in 
                               KwLiRguCDatInd and KwLiRguCStaInd.
*********************************************************************91*/
