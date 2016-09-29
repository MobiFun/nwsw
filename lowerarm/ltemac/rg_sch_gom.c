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
  
     Desc:     C source code for Entry point fucntions
  
     File:     rg_sch_gom.c 
  
     Sid:      gk_sch_gom.c@@/main/2 - Sat Jul 30 02:21:41 2011
  
     Prg:     ns 
  
**********************************************************************/

/** @file rg_sch_gom.c 
@brief This module does processing related to handling of upper interface APIs 
invoked by RRM towards MAC.
*/

/* header include files -- defines (.h) */
#include "envopt.h"        /* environment options */
#include "envdep.h"        /* environment dependent */
#include "envind.h"        /* environment independent */
#include "gen.h"           /* general layer */
#include "ssi.h"           /* system service interface */
#include "cm_hash.h"       /* common hash list */
#include "cm_mblk.h"       /* common memory link list library */
#include "cm_llist.h"      /* common linked list library */
#include "cm_err.h"        /* common error */
#include "cm_lte.h"        /* common LTE */
#include "lrg.h"
#include "rgr.h"
#include "tfu.h"
#include "rg_env.h"
#include "rg_sch_inf.h"
#include "rg_sch.h"
#include "rg_sch_err.h"

/* header/extern include files (.x) */
#include "gen.x"           /* general layer */
#include "ssi.x"           /* system service interface */
#include "cm5.x"           /* common timers */
#include "cm_lib.x"        /* common library */
#include "cm_hash.x"       /* common hash list */
#include "cm_llist.x"      /* common linked list library */
#include "cm_mblk.x"       /* memory management */
#include "cm_tkns.x"       /* common tokens */
#include "cm_lte.x"        /* common LTE */
#include "lrg.x"
#include "rgr.x"
#include "tfu.x"
#include "rg_sch_inf.x"
#include "rg_sch.x"

/* local defines */
PRIVATE S16 rgSCHGomHndlCfgReq ARGS((RgSchCb *instCb, SpId spId,
         RgrCfg  *cfg, RgSchErrInfo *errInfo));
PRIVATE S16 rgSCHGomHndlRecfgReq ARGS((RgSchCb *instCb, SpId spId,
         RgrRecfg  *recfg, RgSchErrInfo *errInfo));
PRIVATE S16 rgSCHGomHndlResetReq ARGS((RgSchCb *instCb,SpId spId,RgrRst *reset,
         RgSchErrInfo  *errInfo));
PRIVATE S16 rgSCHGomGetCellIdFrmCfgReq ARGS((RgrCfgReqInfo *rgrCfgReq,
         CmLteCellId *cellId));
PRIVATE S16 rgSCHGomCfgReq ARGS((Region reg, Pool pool, RgSchCb *instCb,
         SpId spId, RgrCfgTransId transId, RgrCfgReqInfo *cfgReqInfo));
PRIVATE S16 rgSCHGomEnqCfgReq ARGS((Region reg, Pool pool, RgSchCellCb *cell,
         RgrCfgTransId transId, RgrCfgReqInfo *rgrCfgReq));
PRIVATE S16 rgSCHGomHndlDelReq ARGS((RgSchCb  *instCb,SpId spId, 
         RgrDel  *del,RgSchErrInfo  *errInfo));

/* local typedefs */
 
/* local externs */
 
/* forward references */



/**
 * @brief Handler for config request from RRM to Schedular.
 *
 * @details
 *
 *     Function: rgSCHGomHndlCfg
 *     
 *     This API is called from schedulers UIM and it handles config request
 *     from RRM to Scheduler.
 *     
 *     Processing Steps:
 *      - If the request is for the inactive cell, 
 *       - Handle request.Call rgSCHGomCfgReq.
 *      - Else,
 *       - Enqueue the request. Call rgSCHGomEnqCfgReq.
 *         
 *  @param[in]  Region        reg
 *  @param[in]  Poll          pool
 *  @param[in]  RgSchCb       *instCb
 *  @param[in]  SpId          spId
 *  @param[in]  RgrCfgTransId transId
 *  @param[in]  RgrCfgReqInfo *cfgReqInfo
 *  @return  S16
 *      -# ROK 
 *      -# RFAILED 
 **/
#ifdef ANSI
PUBLIC S16 rgSCHGomHndlCfg
(
Region        reg,
Pool          pool,
RgSchCb       *instCb,
SpId          spId,
RgrCfgTransId transId,
RgrCfgReqInfo *cfgReqInfo
)
#else
PUBLIC S16 rgSCHGomHndlCfg(reg, pool, instCb, spId, transId, cfgReqInfo)
Region        reg;
Pool          pool;
RgSchCb       *instCb;
SpId          spId;
RgrCfgTransId transId;
RgrCfgReqInfo *cfgReqInfo;
#endif
{
   S16           ret;
   CmLteCellId   cellId;
   RgSchCellCb   *cell = NULLP;    
   U8            cfmStatus = RGR_CFG_CFM_NOK;
#ifdef DEBUGP
   /* rg001.201 fixed the double decrement */
   Inst          inst = (instCb->rgSchInit.inst );
#endif

   TRC2(rgSCHGomHndlCfg);
   RGSCHDBGPRM(inst,(rgSchPBuf(inst), "Handling RGR Config request: "
            "action %d cfgType %d\n",
            cfgReqInfo->action, cfgReqInfo->u.cfgInfo.cfgType));
   /* Apply the configuration for Cell Configuration or Delete */
   if (cfgReqInfo->action != RGR_RECONFIG)
   {
      RGSCHDBGINFO(inst, (rgSchPBuf(inst), "Applying RGR cell"
                " Config/Delete \n"));
      ret = rgSCHGomCfgReq (reg, pool, instCb, spId, transId, cfgReqInfo);
      RETVALUE(ret);
   }

   /* Fetch the cell Id for the recieved request */
   if((rgSCHGomGetCellIdFrmCfgReq(cfgReqInfo, &cellId)) != ROK)
   {
      RGSCHDBGERR(inst,(rgSchPBuf(inst), "Action.Cofgig Type Error\n"));

      SPutSBuf(reg, pool, (Data *)cfgReqInfo, (Size)sizeof(*cfgReqInfo));
      rgSCHUtlRgrCfgCfm(instCb->rgSchInit.inst, spId, transId, cfmStatus); 
      RETVALUE(RFAILED);
   }
   /* Extract the cell and Enquee Config Request */
   if(NULLP != instCb->rgrSap[spId].cell)
   {
      if(cellId != instCb->rgrSap[spId].cell->cellId) 
      {
         RGSCHDBGERR(inst,(rgSchPBuf(inst), "Cell with Id %d already exists "
           "on sap %d\n", instCb->rgrSap[spId].cell->cellId, spId));  

         SPutSBuf(reg, pool, (Data *)cfgReqInfo, (Size)sizeof(*cfgReqInfo));
         rgSCHUtlRgrCfgCfm(instCb->rgSchInit.inst, spId, transId, cfmStatus); 
         RETVALUE(RFAILED);
      }
      cell = instCb->rgrSap[spId].cell;

      /* Enqueue the configuration */
      ret = rgSCHGomEnqCfgReq(reg, pool, cell, transId, cfgReqInfo);
      if (ret != ROK)
      {
         RGSCHDBGERR(inst, (rgSchPBuf(inst),"rgSCHGomHndlCfg: Enqueuing CfgReq "
              "Failed \n"));

         SPutSBuf(reg, pool, (Data *)cfgReqInfo, (Size)sizeof(*cfgReqInfo));
         rgSCHUtlRgrCfgCfm(instCb->rgSchInit.inst, spId, transId, cfmStatus); 
         RETVALUE(RFAILED);
      }

      RGSCHDBGINFO(inst, (rgSchPBuf(inst), "RGR Config Request handling done\n"));
      RETVALUE(ROK);
   }
   RGSCHDBGINFO(inst, (rgSchPBuf(inst), "NO CELL!! RGR Config Request Failed\n"));
   SPutSBuf(reg, pool, (Data *)cfgReqInfo, (Size)sizeof(*cfgReqInfo));
   rgSCHUtlRgrCfgCfm(instCb->rgSchInit.inst, spId, transId, cfmStatus); 
   RETVALUE(RFAILED);

}/* rgSCHGomHndlCfg */


/**
 * @brief Handler to handle config request from RRM to Scheduler.
 *
 * @details
 *
 *     Function: rgSCHGomCfgReq
 *     
 *     This API handles processing for config request from RRM to Scheduler.
 *     
 *     Processing Steps: 
 *      - If Configuration request, call rgSCHGomHndlCfgReq.
 *      - Else if Reconfiguration request, call rgSCHGomHndlRecfgReq.
 *      - If successful, send configuration confirm to RRM.
 *        Call rgSCHUtlRgrCfgCfm else FAIL.
 *
 *  @param[in]  Region        reg
 *  @param[in]  Poll          pool
 *  @param[in]  RgSchCb       *instCb
 *  @param[in]  SpId          spId
 *  @param[in]  RgrCfgTransId transId
 *  @param[in]  RgrCfgReqInfo *cfgReqInfo
 *  @return  S16
 *      -# ROK 
 *      -# RFAILED 
 **/
#ifdef ANSI
PRIVATE S16 rgSCHGomCfgReq
(
Region        reg,
Pool          pool,
RgSchCb       *instCb,
SpId          spId,
RgrCfgTransId transId,
RgrCfgReqInfo *cfgReqInfo
)
#else
PRIVATE S16 rgSCHGomCfgReq(reg, pool, instCb, spId, transId, cfgReqInfo)
Region        reg;
Pool          pool;
RgSchCb       *instCb;
SpId          spId;
RgrCfgTransId transId;
RgrCfgReqInfo *cfgReqInfo;
#endif
{
   U8              cfmStatus = RGR_CFG_CFM_OK;
   S16             ret;
   RgSchErrInfo    errInfo;
#ifdef DEBUGP
   /* rg001.201 fixed the double decrement of inst */
   Inst            inst = (instCb->rgSchInit.inst );
#endif
   TRC2(rgSCHGomCfgReq);
   RGSCHDBGPRM(inst,(rgSchPBuf(inst), "Processing RGR Config request: "
          "action %d\n", cfgReqInfo->action));

   /* Process Config/Reconfig/Delete request from RRM */
   switch (cfgReqInfo->action)
   {
      case RGR_CONFIG:
         {
            ret = rgSCHGomHndlCfgReq(rgSchCb, spId, 
                             &cfgReqInfo->u.cfgInfo, &errInfo);
            break;
         }
      case RGR_RECONFIG:
         {
            ret = rgSCHGomHndlRecfgReq(rgSchCb, spId, 
                             &cfgReqInfo->u.recfgInfo, &errInfo);
            break;
         }
      case RGR_RESET:
         {
            ret = rgSCHGomHndlResetReq(rgSchCb, spId, 
                             &cfgReqInfo->u.rstInfo, &errInfo);
            break;
         }
      case RGR_DELETE:
         {

				 if (cfgReqInfo->u.delInfo.delType == RGR_UE_CFG && 
				     cfgReqInfo->u.delInfo.u.ueDel.relType == 1)
				 {
					 stop_printf("RLF OCCURED, PHY IS BEING SHUT DOWN TO STOP OVERWRITING OF LOGS");
				 }
					 ret = rgSCHGomHndlDelReq(rgSchCb, spId,
                             &cfgReqInfo->u.delInfo, &errInfo);
            break;
         } 
      default:
         {
            RGSCHDBGERR(inst,(rgSchPBuf(inst), "Invalid configuration "
               "action %d\n", cfgReqInfo->action));
            ret = RFAILED;
         }
   } /* End of switch */

   if (ret != ROK)
   {
      cfmStatus = RGR_CFG_CFM_NOK;
   }

   SPutSBuf(reg, pool, (Data *)cfgReqInfo, (Size)sizeof(*cfgReqInfo));
   /* Send back confirmation status to RRM */   
   rgSCHUtlRgrCfgCfm(instCb->rgSchInit.inst, spId, transId, cfmStatus); 
   RGSCHDBGINFO(inst,(rgSchPBuf(inst), "RGR Configuration request processed\n"));
   RETVALUE(ret);
}  /* rgSCHGomCfgReq */


/**
 * @brief Handler to enqueuing config request from RRM to Scheduler.
 *
 * @details
 *
 *     Function: rgSCHGomEnqCfgReq
 *     
 *     This API enqueues config request from RRM to MAC. 
 *     
 *     Processing Steps:
 *      - Allocate the configuration request element.
 *      - Copy the contents of the recieved configuration to config request
 *        element and free the recieved configuration pointer.
 *      - If the configuration is without activation time,
 *         - Enqueue the request in crntRgrCfgLst of the cell at the end of 
 *           the list.
 *      - Else
 *         - Enqueue the request in pndngRgrCfgLst of the cell.
 *         
 *  @param[in]  Region        reg,
 *  @param[in]  Pool          pool
 *  @param[in]  RgSchCellCb   *cell
 *  @param[in]  RgrCfgTransId transId
 *  @param[in]  RgrCfgReqInfo *rgrCfgReq
 *  @return  S16
 *      -# ROK 
 *      -# RFAILED 
 **/
#ifdef ANSI
PRIVATE S16 rgSCHGomEnqCfgReq
(
Region        reg,
Pool          pool,
RgSchCellCb   *cell,
RgrCfgTransId transId,
RgrCfgReqInfo *rgrCfgReq
)
#else
PRIVATE S16 rgSCHGomEnqCfgReq(reg, pool, cell, transId, rgrCfgReq)
Region        reg;
Pool          pool;
RgSchCellCb   *cell;
RgrCfgTransId transId;
RgrCfgReqInfo *rgrCfgReq;
#endif
{
   S16                ret;
   U32                sfDiff;
   RgSchCfgElem       *rgrCfgElem  = NULLP;
   CmLteTimingInfo    actvTime; 
   Inst               inst = cell->instIdx;

   TRC2(rgSCHGomEnqCfgReq);
   RGSCHDBGPRM(inst,(rgSchPBuf(inst), "Enqueuing RGR config request\n"));

   /* Allocate memory for config Element */
   ret = rgSCHUtlAllocSBuf(inst, (Data **)&rgrCfgElem, sizeof(RgSchCfgElem));
   if ((ret != ROK) || ((U8 *)rgrCfgElem == NULLP))
   {
      RETVALUE(RFAILED);
   }

   /* Initialize the configuration element */
   cmMemcpy((U8*)rgrCfgElem->rgrCfg.transId.trans,(U8*)transId.trans,
         sizeof(transId.trans));
   rgrCfgElem->rgrCfg.reg = reg;
   rgrCfgElem->rgrCfg.pool = pool;
   rgrCfgElem->rgrCfg.rgrCfgReq = rgrCfgReq;
   rgrCfgElem->cfgReqLstEnt.prev = NULLP;
   rgrCfgElem->cfgReqLstEnt.next = NULLP;
   rgrCfgElem->cfgReqLstEnt.node = (PTR )rgrCfgElem;

   /* Add configuration element to current/pending cfgLst */
   if (((rgrCfgReq->action == RGR_RECONFIG) &&
            (rgrCfgReq->u.recfgInfo.recfgType == RGR_CELL_CFG) &&
            (rgrCfgReq->u.recfgInfo.u.cellRecfg.recfgActvTime.pres == TRUE)))

   {
      actvTime = 
         rgrCfgReq->u.recfgInfo.u.cellRecfg.recfgActvTime.actvTime;

      /* Check if the activation time is valid */
      if (actvTime.sfn >= RGSCH_MAX_SFN 
            || actvTime.subframe >= RGSCH_NUM_SUB_FRAMES)
      {
         RGSCHDBGERR(inst, (rgSchPBuf(inst), "Invalid activation time for RGR "
           "config request: activation sfn %d activation subframe %d current "
           "sfn %d current subframe %d", actvTime.sfn, actvTime.subframe, 
           cell->crntTime.sfn, cell->crntTime.subframe));
         /* ccpu00117052 - MOD - Passing double pointer
         for proper NULLP assignment*/
         rgSCHUtlFreeSBuf(inst, (Data **)&rgrCfgElem, sizeof(*rgrCfgElem));
         RETVALUE(RFAILED);
      }

      sfDiff = RGSCH_CALC_SF_DIFF(actvTime, cell->crntTime);

      if (sfDiff > (RGR_ACTV_WIN_SIZE * RGSCH_NUM_SUB_FRAMES))
      {
         RGSCHDBGERR(inst, (rgSchPBuf(inst), "Invalid activation time for RGR"
              " config request: activation sfn %d activation subframe %d "
              "current sfn %d current subframe %d", actvTime.sfn,
              actvTime.subframe, cell->crntTime.sfn, cell->crntTime.subframe));
         /* ccpu00117052 - MOD - Passing double pointer
         for proper NULLP assignment*/
         rgSCHUtlFreeSBuf(inst, (Data **)&rgrCfgElem, sizeof(*rgrCfgElem));
         RETVALUE(RFAILED);
      }

      if (sfDiff)
      {
         /* Add to pending cfgReqLst */
         rgrCfgElem->actvTime = actvTime; 
         rgSCHDbmInsPndngRgrCfgElem(cell, rgrCfgElem);
         RGSCHDBGINFO(inst,(rgSchPBuf(inst), "Pending RGR Config Request"
                " Enqueued\n"));
         /* Cfm to be sent only after applying request */
         RETVALUE(ROK);
      }
   }

   /* Add to current cfgReq list */
   rgSCHDbmInsCrntRgrCfgElem(cell, rgrCfgElem);
   RGSCHDBGINFO(inst, (rgSchPBuf(inst), "Current RGR Config Request "
               "Enqueued\n"));
   /* Cfm to be sent only after applying request */
   RETVALUE(ROK);
}  /* rgSCHGomEnqCfgReq */


/**
 * @brief Handler for TTI processing for configurations recieved from RRM.
 *
 * @details
 *
 *     Function: rgSCHGomTtiHndlr
 *     
 *     This API does TTI processing for configurations recieved from RRM.
 *     
 *     Processing Steps:
 *      - It dequeues config request from the current configuration list.
 *        For each config request in the list: 
 *        - Processes the request. Call rgSCHGomCfgReq.
 *      - It dequeues config request for the current tti from the pending 
 *        configuration list. For each config request in the list:
 *        - Processes the request. Call rgSCHGomCfgReq.
 *
 *  @param[in]  RgSchCellCb *cell
 *  @return  S16
 *      -# ROK 
 *      -# RFAILED 
 **/
#ifdef ANSI
PUBLIC S16 rgSCHGomTtiHndlr
(
RgSchCellCb      *cell,
SpId             spId
)
#else
PUBLIC S16 rgSCHGomTtiHndlr(cell, spId)
RgSchCellCb      *cell;
SpId             spId;
#endif
{
   RgSchCfgElem       *cfgElem;
   Inst               inst= cell->instIdx;
   /* rg003.201: Trace Added */
   TRC2(rgSCHGomTtiHndlr);

   /* Dequeue from current config list */
   while ((cfgElem = rgSCHDbmGetNextCrntRgrCfgElem(cell, NULLP)) != NULLP)
   {
      rgSCHDbmDelCrntRgrCfgElem(cell, cfgElem);
      RGSCHDBGINFO(inst,(rgSchPBuf(inst), "Applying enqueued current RGR "
         "Configuration\n"));
      rgSCHGomCfgReq(cfgElem->rgrCfg.reg,cfgElem->rgrCfg.pool,
            &rgSchCb[inst], spId, cfgElem->rgrCfg.transId, 
                                          cfgElem->rgrCfg.rgrCfgReq);
      /* ccpu00117052 - MOD - Passing double pointer
      for proper NULLP assignment*/
      rgSCHUtlFreeSBuf(inst, (Data **)&cfgElem, sizeof(*cfgElem));
   }

   /* Handle config requests from pending config list */
   while((cfgElem = rgSCHDbmGetPndngRgrCfgElemByKey(cell, cell->crntTime)) != NULLP)
   {
      rgSCHDbmDelPndngRgrCfgElem(cell, cfgElem);
      RGSCHDBGINFO(inst,(rgSchPBuf(inst), "Applying enqueued pending RGR "
                "Configuration\n"));
      rgSCHGomCfgReq(cfgElem->rgrCfg.reg, cfgElem->rgrCfg.pool, 
            &rgSchCb[inst], spId, cfgElem->rgrCfg.transId, 
                                          cfgElem->rgrCfg.rgrCfgReq);
      /* ccpu00117052 - MOD - Passing double pointer
      for proper NULLP assignment*/
      rgSCHUtlFreeSBuf(inst, (Data **)&cfgElem, sizeof(*cfgElem));
   } 

   RGSCHDBGINFO(inst,(rgSchPBuf(inst), "RGR Configuration applied\n"));
   RETVALUE(ROK);
}


/**
 * @brief Handler to handle configuration request from RRM to MAC.
 *
 * @details
 *
 *     Function: rgSCHGomHndlCfgReq
 *     
 *     This API handles processing for configuration request from RRM to MAC.
 *     
 *     - Processing Steps: 
 *        - Validate configuration request parameters at CFG module. 
 *          Call rgSCHCfgVldtRgrCellCfg for cell configuration.
 *        - If validated successfully, send configuration request to CFG.
 *          Call rgSCHCfgRgrCellCfg else FAIL.
 *          
 *  @param[in]  RgSchCb      *instCb
 *  @param[in]  SpId         spId
 *  @param[in]  RgrCfg       *cfg
 *  @param[out] RgSchErrInfo *errInfo
 *  @return  S16
 *      -# ROK 
 *      -# RFAILED 
 **/
#ifdef ANSI
PRIVATE S16 rgSCHGomHndlCfgReq
(
RgSchCb        *instCb,
SpId           spId,
RgrCfg         *cfg,
RgSchErrInfo   *errInfo
)
#else
PRIVATE S16 rgSCHGomHndlCfgReq(instCb, spId, cfg, errInfo)
RgSchCb        *instCb;
SpId           spId;
RgrCfg         *cfg;
RgSchErrInfo   *errInfo;
#endif
{
   S16          ret;
   RgSchCellCb  *cell = instCb->rgrSap[spId].cell;
   /* rg001.201 fixed the double decrement of inst */
   Inst         inst = (instCb->rgSchInit.inst );
   RgSchUeCb    *ue;

   TRC2(rgSCHGomHndlCfgReq);
   RGSCHDBGPRM(inst,(rgSchPBuf(inst), "Processing RGR Configuration request:"
           " configType %d\n", cfg->cfgType));

   errInfo->errType = RGSCHERR_GOM_CFG_REQ;
   
   /* Validate and process the configuration request */ 
   switch (cfg->cfgType)
   {
      case RGR_CELL_CFG:
      {
#ifdef PWAV_LARM_LOGS 
         printSchCellCfg(&cfg->u.cellCfg);
#endif
         ret = rgSCHCfgVldtRgrCellCfg(inst, &cfg->u.cellCfg, cell, errInfo);
            if (ret != ROK)
            {
               RGSCHDBGERR(inst,(rgSchPBuf(inst), "Rgr Cell configuration "
                  "validation FAILED: Cell %d\n", cfg->u.cellCfg.cellId));
               RETVALUE(RFAILED);
            }
         ret = rgSCHCfgRgrCellCfg(instCb, spId, &cfg->u.cellCfg, errInfo);
         break;
      }
      case RGR_UE_CFG:
      {
#ifdef PWAV_LARM_LOGS 
         printSchUeCfg(&cfg->u.ueCfg);
#endif
         ret = rgSCHCfgVldtRgrUeCfg(inst, &cfg->u.ueCfg, &cell, errInfo);
         if (ret != ROK)
         {
            RGSCHDBGERR(inst,(rgSchPBuf(inst), "Ue configuration validation"
               " FAILED: Ue %d\n", cfg->u.ueCfg.crnti));
            RETVALUE(RFAILED);
         }
         ret = rgSCHCfgRgrUeCfg(cell, &cfg->u.ueCfg, errInfo);
         break;
      }
      case RGR_LCH_CFG:
      {
#ifdef PWAV_LARM_LOGS 
         printSchLchCfg(&cfg->u.lchCfg);
#endif
         ret = rgSCHCfgVldtRgrLcCfg(inst, &cfg->u.lchCfg, &cell, &ue, errInfo);
         if (ret != ROK)
         {
            RGSCHDBGERR(inst,(rgSchPBuf(inst), "LC configuration validation "
              "FAILED: LC %d\n", cfg->u.lchCfg.lcId));
            RETVALUE(RFAILED);
         }
         ret = rgSCHCfgRgrLchCfg(cell, ue, &cfg->u.lchCfg, errInfo); 
         break;
      }
      case RGR_LCG_CFG:
      {
#ifdef PWAV_LARM_LOGS 
         printSchLcgCfg(&cfg->u.lcgCfg);
#endif
         ret = rgSCHCfgVldtRgrLcgCfg(inst, &cfg->u.lcgCfg, &cell, &ue, errInfo);
         if (ret != ROK)
         {
            RGSCHDBGERR(inst,(rgSchPBuf(inst), "LCG configuration validation "
              "FAILED: Lcg %d\n", cfg->u.lcgCfg.ulInfo.lcgId));
            RETVALUE(RFAILED);
         }
         ret = rgSCHCfgRgrLcgCfg(cell, ue, &cfg->u.lcgCfg, errInfo); 
         break;
      }
      default:
      {
#if(ERRCLASS & ERRCLS_INT_PAR)
            RGSCHDBGERR(inst,(rgSchPBuf(inst), "Should never come here: "
                "cfgType %d\n", cfg->cfgType));
#endif
            RETVALUE(RFAILED);
      }
   }
   
   RGSCHDBGINFO(inst,(rgSchPBuf(inst), "RGR Configuration processed\n"));
   RETVALUE(ret);
}  /* rgSCHGomHndlCfgReq */


/**
 * @brief Handler to handle re-configuration request from RRM to MAC.
 *
 * @details
 *
 *     Function: rgSCHGomHndlRecfgReq
 *     
 *     This API handles processing for re-configuration request from RRM to MAC.
 *     
 *     - Processing Steps: 
 *        - Validate re-configuration request parameters at CFG module. 
 *          Call rgSCHCfgVldtRgrCellRecfg for cell re-configuration.
 *        - If validated successfully, send configuration request to CFG.
 *          Call rgSCHCfgRgrCellRecfg else FAIL.
 *
 *  @param[in]  RgSchCb      *instCb
 *  @param[in]  SpId         spId
 *  @param[in]  RgrRecfg     *recfg
 *  @param[out] RgSchErrInfo *errInfo
 *  @return  S16
 *      -# ROK 
 *      -# RFAILED 
 **/
#ifdef ANSI
PRIVATE S16 rgSCHGomHndlRecfgReq
(
RgSchCb       *instCb,
SpId          spId,
RgrRecfg      *recfg,
RgSchErrInfo  *errInfo
)
#else
PRIVATE S16 rgSCHGomHndlRecfgReq(instCb, spId, recfg, errInfo)
RgSchCb       *instCb;
SpId          spId;
RgrRecfg      *recfg;
RgSchErrInfo  *errInfo;
#endif
{
   /* rg009.201 Added Insure fixes of UMR. ue Now a NULL pointer */
   RgSchUeCb    *ue = NULLP;
   RgSchDlLcCb  *dlLc;
   S16          ret; 
   RgSchCellCb  *cell = instCb->rgrSap[spId].cell;
   /* rg001.201 fixed the double decrement of inst */
   Inst         inst = (instCb->rgSchInit.inst );

   TRC2(rgSCHGomHndlRecfgReq);
   RGSCHDBGPRM(inst,(rgSchPBuf(inst), "Processing RGR Reconfiguration request:"
            "reconfigType %d\n", recfg->recfgType));

   errInfo->errType = RGSCHERR_GOM_RECFG_REQ;
   
   /* Validate and process the re-configuration request */
   switch (recfg->recfgType)
   {
      case RGR_CELL_CFG:
      {
         ret = rgSCHCfgVldtRgrCellRecfg(inst, &recfg->u.cellRecfg, &cell,
               errInfo);
         if (ret != ROK) 
         {
            RGSCHDBGERR(inst,(rgSchPBuf(inst), "Rgr Cell Recfg Validation "
                     "FAILED\n"));
            RETVALUE(RFAILED);
         }
         ret = rgSCHCfgRgrCellRecfg(cell, &recfg->u.cellRecfg, errInfo);
         break;
      }
      case RGR_UE_CFG:
      {
         ret = rgSCHCfgVldtRgrUeRecfg(inst, &recfg->u.ueRecfg, &cell, &ue, errInfo);
         if ( ret != ROK)
         {
            RGSCHDBGERR(inst,(rgSchPBuf(inst), "Ue Recfg Validation FAILED\n"));
            RETVALUE(RFAILED);
         }
         ret = rgSCHCfgRgrUeRecfg(cell, ue, &recfg->u.ueRecfg, errInfo);
         break;
      }
      case RGR_LCH_CFG:
      {
         ret = rgSCHCfgVldtRgrLchRecfg(inst, &recfg->u.lchRecfg, &cell, &ue, 
               &dlLc, errInfo);
         if (ret != ROK)
         {
            RGSCHDBGERR(inst,(rgSchPBuf(inst), "Lc Recfg Validation FAILED\n"));
            RETVALUE(RFAILED);
         }
         ret = rgSCHCfgRgrLchRecfg(cell, ue, dlLc, &recfg->u.lchRecfg, errInfo);
         break;
      }
      case RGR_LCG_CFG:
      {
         ret = rgSCHCfgVldtRgrLcgRecfg(inst, &recfg->u.lcgRecfg, cell, &ue,
               errInfo);
         if (ret != ROK)
         {
            RGSCHDBGERR(inst,(rgSchPBuf(inst), "Lcg Recfg Validation FAILED\n"));
            RETVALUE(RFAILED);
         }
         ret = rgSCHCfgRgrLcgRecfg(cell, ue, &recfg->u.lcgRecfg, errInfo);
         break;
      }
      default:
      {
#if(ERRCLASS & ERRCLS_INT_PAR)
         RGSCHDBGERR(inst,(rgSchPBuf(inst), "Should never come here: "
                       "recfgType %d\n", recfg->recfgType));
#endif
         RETVALUE(RFAILED);
      }
   }

   RGSCHDBGINFO(inst,(rgSchPBuf(inst), "RGR Reconfiguration processed\n"));
   RETVALUE(ret);
}  /* rgSCHGomHndlRecfgReq */

/**
 * @brief Handler to handle UE reset request from RRM to Scheduler.
 *
 * @details
 *
 *     Function: rgSCHGomHndlResetReq
 *     
 *     This API handles processing for UE reset request from RRM to Scheduler.
 *     
 *     - Processing Steps: 
 *        - Validate UE reset request parameters at CFG module. 
 *          Call rgSCHCfgVldtRgrUeReset for UE reset.
 *        - If validated successfully, send UE reset request to CFG.
 *          Call rgSCHCfgRgrUeReset else FAIL.
 *
 *  @param[in]  RgrRst   *rstInfo
 *  @param[out] RgSchErrInfo *errInfo
 *  @return  S16
 *      -# ROK 
 *      -# RFAILED 
 **/
#ifdef ANSI
PRIVATE S16 rgSCHGomHndlResetReq
(
RgSchCb       *instCb,
SpId          spId,
RgrRst        *reset,
RgSchErrInfo  *errInfo
)
#else
PRIVATE S16 rgSCHGomHndlResetReq(instCb, spId, reset, errInfo)
RgSchCb       *instCb;
SpId          spId;
RgrRst        *reset;
RgSchErrInfo  *errInfo;
#endif
{
   S16          ret; 
   RgSchCellCb  *cell= instCb->rgrSap[spId].cell;
   /* rg001.201 fixed the double decrement of inst */
   Inst         inst = (instCb->rgSchInit.inst );
   RgSchUeCb    *ue = NULLP;

   TRC2(rgSCHGomHndlResetReq);

   RGSCHDBGPRM(inst,(rgSchPBuf(inst), "Processing RGR UE Reset request:"
            "CellId %d UE Id %d \n", reset->cellId, reset->crnti));

   errInfo->errType = RGSCHERR_GOM_RESET_REQ;
   
   /* Validate and process the UE reset request */
   ret = rgSCHCfgVldtRgrUeReset(inst, reset, cell, &ue, errInfo);
   if (ret != ROK) 
   {
      RGSCHDBGERR(inst,(rgSchPBuf(inst), "Rgr UE Reset Validation FAILED\n"));
      RETVALUE(RFAILED);
   }
   
   ret = rgSCHCfgRgrUeReset(cell, ue, reset, errInfo);
   if (ret != ROK) 
   {
      RGSCHDBGERR(inst,(rgSchPBuf(inst), "Rgr UE Reset FAILED\n"));
      RETVALUE(RFAILED);
   }

   RGSCHDBGINFO(inst,(rgSchPBuf(inst), "RGR Reset Request processed\n"));
   RETVALUE(ret);
}  /* rgSCHGomHndlResetReq */


/**
 * @brief Handler for processing Cell/Ue/Logical channel delete request
 * recieved from RRM.
 *
 * @details
 *
 *     Function: rgSCHGomHndlDelReq
 *     
 *     This API handles processing of delete request from RRM to MAC. 
 *     
 *     Processing Steps:
 *        - Fetch corresponding control block and pass it to CFG module.
 *        - If control block does not exist, FAIL.
 *
 *  @param[in]  RgSchCb      *instCb
 *  @param[in]  SpId         spId
 *  @param[in]  RgrDel       *del
 *  @param[out] RgSchErrInfo *errInfo
 *  @return  S16
 *      -# ROK 
 *      -# RFAILED 
 **/
#ifdef ANSI
PRIVATE S16 rgSCHGomHndlDelReq
(
RgSchCb       *instCb,
SpId          spId,
RgrDel        *del,
RgSchErrInfo  *errInfo
)
#else
PRIVATE S16 rgSCHGomHndlDelReq(instCb, spId, del, errInfo)
RgSchCb       *instCb;
SpId          spId;
RgrDel        *del;
RgSchErrInfo  *errInfo;
#endif
{

   S16       ret;
#ifdef DEBUGP
   /* rg001.201 fixed the double decrement of inst */
   Inst      inst = (instCb->rgSchInit.inst);
#endif
   TRC2(rgSCHGomHndlDelReq);
   RGSCHDBGPRM(inst,(rgSchPBuf(inst), "Processing RGR Delete request:"
           "delType %d\n", del->delType));
   
   errInfo->errType = RGSCHERR_GOM_DEL_REQ;

   if(instCb->rgrSap[spId].cell == NULLP)
   {
      RGSCHDBGERR(inst,(rgSchPBuf(inst), "Cell doesnt exist"));
      RETVALUE(RFAILED); 
   }
   
   /* Process the delete request */ 
   switch (del->delType)
   {
      case RGR_CELL_CFG:
      {
         ret = rgSCHCfgRgrCellDel(instCb->rgrSap[spId].cell, del, errInfo);
         if(ret == ROK)
         {
            instCb->rgrSap[spId].cell = NULLP;
            instCb->tfuSap[spId].cell = NULLP;
         }
         break;
      }
      case RGR_UE_CFG:
      {
	  	
#ifdef MSPD_MLOG_NEW
         VOLATILE U32 t=0, t2=0;
         t = GetTIMETICK();
#endif
#ifdef MSPD
         MSPD_DBG("[%d] Deleting UE\n", del->u.ueDel.crnti);
#endif
         ret = rgSCHCfgRgrUeDel(instCb->rgrSap[spId].cell, del, errInfo);
#ifdef MSPD
         MSPD_DBG("[%d] Delete Done\n", del->u.ueDel.crnti);
#endif

#ifdef MSPD_MLOG_NEW
         MLogTask(PID_SCH_UE_DEL, RESOURCE_LARM, t, t2=GetTIMETICK());
#endif

         break;
      }
      case RGR_LCH_CFG:
      {
         ret = rgSCHCfgRgrLcDel(instCb->rgrSap[spId].cell, del, errInfo);
         break;
      }
      case RGR_LCG_CFG:
      {
         ret = rgSCHCfgRgrLcgDel(instCb->rgrSap[spId].cell, del, errInfo);
         break;
      }
      default:
      {
#if(ERRCLASS & ERRCLS_INT_PAR)
         RGSCHDBGERR(inst,(rgSchPBuf(inst), "Should never come here: "
               "delType %d\n", del->delType));
#endif
         RETVALUE(RFAILED);
      }
   }

   RGSCHDBGINFO(inst,(rgSchPBuf(inst), "RGR deletion processed \n"));
   RETVALUE(ret);
}  /* rgSCHGomHndlDelReq */




/***********************************************************
 *
 *     Func : rgSCHGomGetCellIdFrmCfgReq
 *        
 *
 *     Desc : 
 *     - Processing Steps: 
 *        - Retrieves the cell Id for a config request.
 *
 *  @param[in]  RgrCfgReqInfo *rgrCfgReq
 *  @param[out] CmLteCellId   *cellId
 *     Ret  : ROK on fetching cellId
 *            RFAILED on failure
 *
 *     Notes: 
 *
 *     File : rg_sch_gom.c 
 *
 **********************************************************/
#ifdef ANSI
PRIVATE S16 rgSCHGomGetCellIdFrmCfgReq
(
RgrCfgReqInfo *rgrCfgReq,
CmLteCellId   *cellId
)
#else
PRIVATE S16 rgSCHGomGetCellIdFrmCfgReq(rgrCfgReq, cellId)
RgrCfgReqInfo *rgrCfgReq;
CmLteCellId   *cellId;
#endif
{
   /*MUST rg001.201 Handle UE and LC Reconfiguration and Deletion */

   TRC2(rgSCHGomGetCellIdFrmCfgReq);


   /* Extract CellId depending on the action and Config Type in the Request 
    * As of now this function is called for only re configuration so removed
    * othe CASES below if needed we can add them*/
   switch (rgrCfgReq->action)
   {
      case RGR_RECONFIG:
         {
            if (rgrCfgReq->u.recfgInfo.recfgType ==RGR_CELL_CFG)
            {
               *cellId = rgrCfgReq->u.recfgInfo.u.cellRecfg.cellId;
            }
            else if (rgrCfgReq->u.recfgInfo.recfgType == RGR_UE_CFG)
            {
               *cellId = rgrCfgReq->u.recfgInfo.u.ueRecfg.cellId;
            }
            else if (rgrCfgReq->u.recfgInfo.recfgType == RGR_LCH_CFG)
            {
               *cellId = rgrCfgReq->u.recfgInfo.u.lchRecfg.cellId;
            }
            else if (rgrCfgReq->u.recfgInfo.recfgType == RGR_LCG_CFG) 
            {
               *cellId = rgrCfgReq->u.recfgInfo.u.lcgRecfg.cellId;
            }
            else
            {
               RETVALUE(RFAILED);
            }
            break;
         }
      default:
      {
         RETVALUE(RFAILED);
      }
   }  /* End of Switch */

   RETVALUE(ROK);
}  /* rgSCHGomGetCellIdFrmCfgReq */

/* rg004.201-ADD-Added for SI Enhancement*/
#ifdef RGR_SI_SCH
/**
 * @brief Handler to handle SI configuration request from RRM to MAC.
 *
 * @details
 *
 *     Function: rgSCHGomHndlSiCfg
 *     
 *     This API handles processing for SI configuration request from RRM to MAC.
 *     
 *     - Processing Steps: 
 *        - Validate SI configuration request parameters at CFG module. 
 *          Call rgSCHCfgVldtSiCfg for SI configuration.
 *        - If validated successfully, send configuration request to CFG.
 *          Call rgSCHCfgRgrCellCfg else FAIL.
 *          
 *  @param[in]  Region        reg
 *  @param[in]  Pool          pool
 *  @param[in]  RgSchCb       *instCb
 *  @param[in]  SpId          spId
 *  @param[in]  RgrCfgTransId transId
 *  @param[in]  RgrSiCfgReqInfo *cfgReqInfo
 *  @return  S16
 *      -# ROK 
 *      -# RFAILED 
 **/
#ifdef ANSI
PUBLIC S16 rgSCHGomHndlSiCfg
(
Region        reg,
Pool          pool,
RgSchCb       *instCb,
SpId          spId,
RgrCfgTransId transId,
RgrSiCfgReqInfo *cfgReqInfo
)
#else
PUBLIC S16 rgSCHGomHndlSiCfg(reg, pool, instCb, spId, transId, cfgReqInfo)
Region        reg;
Pool          pool;
RgSchCb       *instCb;
SpId          spId;
RgrCfgTransId transId;
RgrSiCfgReqInfo *cfgReqInfo;
#endif
{
   S16          ret;
   RgSchCellCb  *cell = instCb->rgrSap[spId].cell;
   Inst         inst = (instCb->rgSchInit.inst );
   RgSchErrInfo    errInfo;
   U8           cfmStatus = RGR_CFG_CFM_NOK;
   MsgLen       msgLen = 0;
   S32          tbSz   = 0;
   U8           nPrb   = 0; 
   U8           mcs    = 0;

   TRC2(rgSCHGomHndlSiCfg);

   RGSCHDBGPRM(inst,(rgSchPBuf(inst), "Processing RGR SI Configuration"
            " request:"));

   /* check if cell does not exists */
   if (((U8 *)cell == NULLP) || (cell->cellId != cfgReqInfo->cellId))
   {
      RGSCHDBGERR(inst, (rgSchPBuf(inst), "Cell Control block does not exist\n"
               ));
      SPutMsg(cfgReqInfo->pdu);
      SPutSBuf(reg, pool, (Data *)cfgReqInfo, (Size)sizeof(*cfgReqInfo)); 
      rgSCHUtlRgrSiCfgCfm(instCb->rgSchInit.inst, spId, transId, cfmStatus); 
      RETVALUE(RFAILED);
   }

   /*Validate the received SI configuration */
   ret = rgSCHCfgVldtRgrSiCfg(inst, cfgReqInfo, cell, &errInfo);
   if (ret != ROK)
   {
      RGSCHDBGERR(inst,(rgSchPBuf(inst), "Rgr SI configuration "
               "validation FAILED\n"));
      SPutMsg(cfgReqInfo->pdu);
      SPutSBuf(reg, pool, (Data *)cfgReqInfo, (Size)sizeof(*cfgReqInfo)); 
      rgSCHUtlRgrSiCfgCfm(instCb->rgSchInit.inst, spId, transId, cfmStatus); 
      RETVALUE(RFAILED);
   }

   /*rg009.201:check if the pdu sent from application
    * matches a transport block size. if not,
    * add padding bytes. This is usually done
    * by RRC but since we are bypassing RRC,
    * MAC is taking over that responsibility
    */

   if ( RGR_SI_CFG_TYPE_MIB != cfgReqInfo->cfgType)
   {
      SFndLenMsg(cfgReqInfo->pdu, &msgLen);

      /* check if the application pdu matches a tb size */
      tbSz = rgSCHUtlGetAllwdCchTbSz(msgLen*8, &nPrb, &mcs);

      if ( tbSz != (msgLen*8) )
      {
         MsgLen  nmPadBytes = 0;
         Data*   padding    = NULLP;

         /* need to add padding bytes */
         nmPadBytes = (tbSz - (msgLen*8))/8;

         if ( SGetSBuf(reg,pool,&padding,nmPadBytes) != ROK)
         {
            RGSCHDBGERR(inst,(rgSchPBuf(inst), "Rgr SI configuration "
                     "SGetSBuf failed for padding failed\n"));
            SPutSBuf(reg, pool, (Data *)cfgReqInfo, (Size)sizeof(*cfgReqInfo)); 
            rgSCHUtlRgrSiCfgCfm(instCb->rgSchInit.inst, spId, transId, cfmStatus); 
            RETVALUE(RFAILED);
         }

         cmMemset((U8*)padding,(U8)0,nmPadBytes);

         if ( SAddPstMsgMult((Data*)padding,nmPadBytes,cfgReqInfo->pdu) != ROK )
         {
            RGSCHDBGERR(inst,(rgSchPBuf(inst), "Rgr SI configuration "
                     "Failed to add padding bytes\n"));
            SPutSBuf(reg, pool, (Data *)cfgReqInfo, (Size)sizeof(*cfgReqInfo)); 
            SPutSBuf(reg, pool, (Data* )padding,(Size)nmPadBytes);
            rgSCHUtlRgrSiCfgCfm(instCb->rgSchInit.inst, spId, transId, cfmStatus); 
            RETVALUE(RFAILED);
         }
         SPutSBuf(reg, pool, (Data* )padding,(Size)nmPadBytes);
      }/* if (tbSz != ...*/
   }/* if (RGR_SI_CFG_TYPE_SI...*/

   /*Set the received pdu at the appropriate place */
   switch(cfgReqInfo->cfgType)
   {
      case RGR_SI_CFG_TYPE_MIB:   /* SI CFG Type MIB */
         RGSCHCHKNUPDSIPDU(cell->siCb.crntSiInfo.mib, 
               cell->siCb.newSiInfo.mib, 
               cfgReqInfo->pdu, cell->siCb.siBitMask, 
               RGSCH_SI_MIB_UPD);
         break;

      case RGR_SI_CFG_TYPE_SIB1:
         RGSCHCHKNUPDSIPDU(cell->siCb.crntSiInfo.sib1, 
               cell->siCb.newSiInfo.sib1, 
               cfgReqInfo->pdu, cell->siCb.siBitMask, 
               RGSCH_SI_SIB1_UPD);
         break;

      case RGR_SI_CFG_TYPE_SI:    /* SI CFG TYPE SI */
         RGSCHCHKNUPDSIPDU(cell->siCb.crntSiInfo.si[cfgReqInfo->siId-1], 
               cell->siCb.newSiInfo.si[cfgReqInfo->siId-1], 
               cfgReqInfo->pdu, 
               cell->siCb.siBitMask, RGSCH_SI_SI_UPD);
         break;

      default:
         RGSCHDBGERR(inst, (rgSchPBuf(inst), "Invalid cfgType "
                  "parameter value\n"));
         SPutMsg(cfgReqInfo->pdu);
         SPutSBuf(reg, pool, (Data *)cfgReqInfo, 
               (Size)sizeof(*cfgReqInfo)); 
         rgSCHUtlRgrSiCfgCfm(instCb->rgSchInit.inst, spId, transId, 
               cfmStatus); 
         RETVALUE(RFAILED);
   }

   SPutSBuf(reg, pool, (Data *)cfgReqInfo, (Size)sizeof(*cfgReqInfo)); 
   cfmStatus = RGR_CFG_CFM_OK;
   rgSCHUtlRgrSiCfgCfm(instCb->rgSchInit.inst, spId, transId, cfmStatus); 

   RGSCHDBGINFO(inst,(rgSchPBuf(inst), "RGR SI Configuration processed\n"));

   RETVALUE(ROK);
}  /* rgSCHGomHndlSiCfg */
#endif/*RGR_SI_SCH */

/**********************************************************************
 
         End of file:     gk_sch_gom.c@@/main/2 - Sat Jul 30 02:21:41 2011
 
**********************************************************************/
 
/**********************************************************************
 
        Notes:
 
**********************************************************************/
 
/**********************************************************************
 
        Revision history:
 
**********************************************************************/
/********************************************************************90**
 
     ver       pat    init                  description
------------ -------- ---- ----------------------------------------------
/main/1      ---     sd   1. LTE MAC 2.1 release
             rg001.201 sm 1. Removed extra comments. 
             rg003.201 ns 1. Trace added to functions & Removed extra comments.
             rg004.201 rnt 1. Modified for SI Enhancement
             rg009.201 adi 1. ccpu00114920:add paddying bytes if application
                              provided PDU does not match a pre-defined TB sz
                    nudupi 1.Added the code for Insure fixes.
/main/2      ---        gvj  1. Updated for LTE MAC Rel 3.1
*********************************************************************91*/
