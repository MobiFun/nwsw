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
  
     Desc:     C source code for Entry point functions
  
     File:     rg_com.c 
  
     Sid:      gk_com.c@@/main/3 - Sat Jul 30 02:21:25 2011
  
     Prg:      sd
  
**********************************************************************/

/** @file rg_com.c
@brief This module does processing related to handling of upper interface APIs 
invoked by RRC towards MAC.
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
#include "crg.h"
#include "rgu.h"
#include "tfu.h"
#include "rg_sch_inf.h"
#include "rg_env.h"
#include "rg.h"
#include "rg_err.h"

/* header/extern include files (.x) */
#include "gen.x"           /* general layer */
#include "ssi.x"           /* system service interface */
#include "cm5.x"           /* common timers */
#include "cm_lib.x"        /* common library */
#include "cm_hash.x"       /* common hash list */
#include "cm_mblk.x"       /* common memory link list library */
#include "cm_llist.x"      /* common linked list library */
#include "cm_tkns.x"       /* common tokens */
#include "cm_lte.x"        /* common LTE */
#include "lrg.x"
#include "crg.x"
#include "rgu.x"
#include "tfu.x"
#include "rg_sch_inf.x"
#include "rg.x"

/* local defines */
PRIVATE S16 rgCOMHndlCfgReq ARGS((CrgCfg  *cfg, RgErrInfo *errInfo));
PRIVATE S16 rgCOMHndlRecfgReq ARGS((CrgRecfg *recfg, RgErrInfo *errInfo));
PRIVATE S16 rgCOMHndlDelReq ARGS((CrgDel *del, RgErrInfo *errInfo));
PRIVATE S16 rgCOMHndlResetReq ARGS((CrgRst *reset,RgErrInfo *errInfo));

/* local typedefs */
 
/* local externs */
 
/* forward references */

/**
 * @brief Handler to handle config request from RRC to MAC.
 *
 * @details
 *
 *     Function: rgCOMCfgReq
 *     
 *     This API handles processing for config request from RRC to MAC. 
 *     
 *     Processing Steps:
 *      - If configuration, process configuration request. Call rgCOMHndlCfgReq.
 *      - else If re-configuration, process re-configuration request. 
 *        Call rgCOMHndlRecfgReq.
 *      - else If reset, process reset request. Call rgCOMHndlResetReq.
 *      - else If delete, process delete request. Call rgCOMHndlDelReq.
 *      - If successful, send confirmation to RRC. Call rgUIMCrgCfgCfm.
 *      - If failed, FAIL.
 *
 *  @param[in]  CrgCfgTransId transId
 *  @param[in]  CrgCfgReqInfo *crgCfgReq
 *  @return  S16
 *      -# ROK 
 *      -# RFAILED 
 **/
#ifdef ANSI
PUBLIC S16 rgCOMCfgReq
(
CrgCfgTransId transId,
CrgCfgReqInfo *crgCfgReq
)
#else
PUBLIC S16 rgCOMCfgReq(transId, crgCfgReq)
CrgCfgTransId transId;
CrgCfgReqInfo *crgCfgReq;
#endif
{
   S16             ret;
   U8              cfmStatus = CRG_CFG_CFM_OK;
   RgErrInfo       errInfo;

   TRC2(rgCOMCfgReq);
   RGDBGPRM((rgPBuf, "Processing CRG Config request: action %d\n",
            crgCfgReq->action));

   /* Process Config/Reconfig/Delete request from RRC */
   switch (crgCfgReq->action)
   {
      case CRG_CONFIG:
         {
            ret = rgCOMHndlCfgReq(&crgCfgReq->u.cfgInfo, &errInfo);
            break;
         }
      case CRG_RECONFIG:
         {
            ret = rgCOMHndlRecfgReq(&crgCfgReq->u.recfgInfo, &errInfo);
            break;
         }
         /* Start: LTEMAC_2.1_DEV_CFG */
      case CRG_RESET:
         {
            ret = rgCOMHndlResetReq(&crgCfgReq->u.rstInfo, &errInfo);
            break;
         }
         /* End: LTEMAC_2.1_DEV_CFG */
      case CRG_DELETE:
         {
            ret = rgCOMHndlDelReq(&crgCfgReq->u.delInfo, &errInfo);
            break;
         }
      default:
         {
            RGDBGERR((rgPBuf, "Invalid configuration action %d\n",
                     crgCfgReq->action));
            MSPD_ERR("Invalid configuration action %d\n",
                     crgCfgReq->action);

            ret = RFAILED;
         }
   }

   if (ret != ROK)
   {
      cfmStatus = CRG_CFG_CFM_NOK;
   }

   /* Send back confirmation status to RRC */
   /*rg008.201 - Removed commented code*/
   rgUIMCrgCfgCfm(transId, cfmStatus); 
   RGDBGINFO((rgPBuf, "CRG Configuration request processed\n"));
   RETVALUE(ret);
}  /* rgCOMCfgReq */
/**
 * @brief Handler for processing Cell/Ue/Logical channel configuration request
 * recieved from RRC.
 *
 * @details
 *
 *     Function: rgCOMHndlCfgReq
 *     
 *     This API handles processing of configuration request from RRC to MAC. 
 *     
 *     Processing Steps:
 *        - Validate configuration request parameters at CFG module. 
 *          Call rgCFGVldtCrgCellCfg, rgCFGVldtCrgUeCfg, rgCFGVldtCrgLcCfg 
 *          for Cell, UE and Logical channel configuration respectively.
 *        - If validated successfully, Call rgCFGCrgCellCfg, rgCFGCrgUeCfg, 
 *          rgCFGCrgLcCfg for Cell, UE and Logical channel configuration 
 *          respectively, else FAIL.
 *
 *  @param[in]  CrgCfg    *cfg
 *  @param[out] RgErrInfo *errInfo
 *  @return  S16
 *      -# ROK 
 *      -# RFAILED 
 **/
#ifdef ANSI
PRIVATE S16 rgCOMHndlCfgReq
(
CrgCfg   *cfg,
RgErrInfo *errInfo
)
#else
PRIVATE S16 rgCOMHndlCfgReq(cfg, errInfo)
CrgCfg   *cfg;
RgErrInfo *errInfo;
#endif
{
   S16       ret;
   RgCellCb  *cell = NULLP;
   RgUeCb    *ue   = NULLP;

   TRC2(rgCOMHndlCfgReq);
   RGDBGPRM((rgPBuf, "Processing CRG Configuration request: configType %d\n", 
         cfg->cfgType));

   errInfo->errType = RGERR_COM_CFG_REQ;

   /* Validate and process the configuration request */ 
   switch (cfg->cfgType)
   {
      case CRG_CELL_CFG:
      {
#ifdef PWAV_LARM_LOGS 
         printMacCellCfg(&cfg->u.cellCfg);
#endif
         ret = rgCFGVldtCrgCellCfg(&cfg->u.cellCfg,errInfo);
            if (ret != ROK)
            {
               RGDBGERR((rgPBuf, "Cell configuration validation FAILED: Cell %d\n",
                    cfg->u.cellCfg.cellId));
               MSPD_ERR("Cell configuration validation FAILED: Cell %d\n",
                    cfg->u.cellCfg.cellId);
               RETVALUE(RFAILED);
            }
         ret = rgCFGCrgCellCfg(&cfg->u.cellCfg, errInfo);
         break;
      }
      case CRG_UE_CFG:
      {
#ifdef PWAV_LARM_LOGS
         printMacUeCfg(&cfg->u.ueCfg);
#endif
         ret = rgCFGVldtCrgUeCfg(&cfg->u.ueCfg, &cell, errInfo);
         if (ret != ROK)
         {
            RGDBGERR((rgPBuf, "Ue configuration validation FAILED: Ue %d\n",
                  cfg->u.ueCfg.crnti));
            MSPD_ERR("Ue configuration validation FAILED: Ue %d\n",
                  cfg->u.ueCfg.crnti);
            RETVALUE(RFAILED);
         }
         ret = rgCFGCrgUeCfg(cell, &cfg->u.ueCfg, errInfo);
         break;
      }
      case CRG_LCH_CFG:
      {
#ifdef PWAV_LARM_LOGS 
         printMacLchCfg(&cfg->u.lchCfg);
#endif
         ret = rgCFGVldtCrgLcCfg(&cfg->u.lchCfg, &cell, &ue,errInfo);
         if (ret != ROK)
         {
            RGDBGERR((rgPBuf, "LC configuration validation FAILED: Lc %d\n",
                  cfg->u.lchCfg.lcId));
            MSPD_ERR("LC configuration validation FAILED: Lc %d\n",
                  cfg->u.lchCfg.lcId);
            RETVALUE(RFAILED);
         }
         ret = rgCFGCrgLcCfg(cell, ue, &cfg->u.lchCfg, errInfo);
         break;
      }
      default:
      {
#if(ERRCLASS & ERRCLS_INT_PAR)
         RGDBGERR((rgPBuf, "Should never come here: cfgType %d\n", cfg->cfgType));
#endif
         MSPD_ERR("Should never come here: cfgType %d\n", cfg->cfgType);
         RETVALUE(RFAILED);
      }
   }

   RGDBGINFO((rgPBuf, "CRG configuration processed \n"));
   RETVALUE(ret);
}  /* rgCOMHndlCfgReq */


/**
 * @brief Handler for processing Cell/Ue/Logical channel re-configuration request
 * recieved from RRC.
 *
 * @details
 *
 *     Function: rgCOMHndlRecfgReq
 *     
 *     This API handles processing of reconfiguration request from RRC to MAC. 
 *     
 *     Processing Steps:
 *      - Validate reconfiguration request parameters at CFG module. Call 
 *        rgCFGVldtCrgCellRecfg, rgCFGVldtCrgUeRecfg, rgCFGVldtCrgLchRecfg for 
 *        Cell, UE and logical channel reconfiguration respectively.
 *      - If validated, Call rgCFGCrgCellRecfg, rgCFGCrgUeRecfg, 
 *        rgCFGCrgLchRecfg for Cell, UE and Logical channel re-configuration 
 *        respectively else FAIL.
 *
 *  @param[in]  CrgRecfg   *recfg
 *  @param[out] RgErrInfo  *errInfo
 *  @return  S16
 *      -# ROK 
 *      -# RFAILED 
 **/
#ifdef ANSI
PRIVATE S16 rgCOMHndlRecfgReq
(
CrgRecfg   *recfg,
RgErrInfo  *errInfo
)
#else
PRIVATE S16 rgCOMHndlRecfgReq(recfg, errInfo)
CrgRecfg  *recfg;
RgErrInfo *errInfo;
#endif
{
   S16       ret;
   RgCellCb  *cell = NULLP;
   RgUeCb    *ue   = NULLP;
   RgUlLcCb  *ulLc = NULLP;

   TRC2(rgCOMHndlRecfgReq);
   RGDBGPRM((rgPBuf, "Processing CRG Reconfiguration request:\
            reconfigType %d\n", recfg->recfgType));

   errInfo->errType = RGERR_COM_RECFG_REQ;
   
   /* Validate and process the re-configuration request */ 
   switch (recfg->recfgType)
   {
      case CRG_CELL_CFG:
      {
         ret = rgCFGVldtCrgCellRecfg(&recfg->u.cellRecfg, &cell, errInfo);
            if (ret != ROK) 
            {
               RGDBGERR((rgPBuf, "Cell Recfg Validation FAILED\n"));
               MSPD_ERR("Cell Recfg Validation FAILED\n");
               RETVALUE(RFAILED);
            }
         ret = rgCFGCrgCellRecfg(cell, &recfg->u.cellRecfg, errInfo);
         break;
      }
      case CRG_UE_CFG:
      {
         ret = rgCFGVldtCrgUeRecfg(&recfg->u.ueRecfg, &cell, &ue, errInfo);
         if ( ret != ROK)
         {
            RGDBGERR((rgPBuf, "Ue Recfg Validation FAILED\n"));
	     MSPD_ERR("Ue Recfg Validation FAILED\n");
            RETVALUE(RFAILED);
         }
         ret = rgCFGCrgUeRecfg(cell, ue, &recfg->u.ueRecfg, errInfo);
         break;
      }
      case CRG_LCH_CFG:
      {
         ret = rgCFGVldtCrgLcRecfg(&recfg->u.lchRecfg, &cell, &ue, 
               &ulLc, errInfo);
         if (ret != ROK)
         {
            RGDBGERR((rgPBuf, "Lc Recfg Validation FAILED\n"));
            MSPD_ERR("Lc Recfg Validation FAILED\n");
            RETVALUE(RFAILED);
         }
         ret = rgCFGCrgLcRecfg(cell, ue, ulLc,
               &recfg->u.lchRecfg, errInfo);
         break;
      }
      default:
      {
#if(ERRCLASS & ERRCLS_INT_PAR)
         RGDBGERR((rgPBuf, "Should never come here: recfgType %d\n",
                  recfg->recfgType));
#endif
         MSPD_ERR("Should never come here: recfgType %d\n",
                  recfg->recfgType);
         RETVALUE(RFAILED);
      }
   }

   RGDBGINFO((rgPBuf, "CRG reconfiguration processed \n"));
   RETVALUE(ret);
}  /* rgCOMHndlRecfgReq */

/*Start: LTEMAC_2.1_DEV_CFG */
/**
 * @brief Handler for processing UE Reset request recieved from RRC.
 *
 * @details
 *
 *     Function: rgCOMHndlResetReq
 *     
 *     This API handles processing of Reset request from RRC to MAC. 
 *     
 *     Processing Steps:
 *      - Validate reset request parameters at CFG module. Call 
 *        rgCFGVldtCrgUeReset for UE reset.
 *      - If validated, Call rgCFGCrgUeReset for UE reset, else FAIL.
 *
 *  @param[in]  CrgRst     *reset
 *  @param[out] RgErrInfo  *errInfo
 *  @return  S16
 *      -# ROK 
 *      -# RFAILED 
 **/
#ifdef ANSI
PRIVATE S16 rgCOMHndlResetReq
(
CrgRst     *reset,
RgErrInfo  *errInfo
)
#else
PRIVATE S16 rgCOMHndlResetReq(reset, errInfo)
CrgRst     *reset;
RgErrInfo  *errInfo;
#endif
{
   S16       ret;
   RgCellCb  *cell = NULLP;
   RgUeCb    *ue   = NULLP;

   TRC2(rgCOMHndlResetReq);
   RGDBGPRM((rgPBuf, "Processing CRG UE Reset request:\n"));

   errInfo->errType = RGERR_COM_RESET_REQ;
   
   /* Validate and process the ue reset request */ 
   ret = rgCFGVldtCrgUeReset(reset, &cell, &ue, errInfo);
   if (ret != ROK) 
   {
      RGDBGERR((rgPBuf, "UE Reset Validation FAILED\n"));
      MSPD_ERR("UE Reset Validation FAILED\n");
      RETVALUE(RFAILED);
   }
   ret = rgCFGCrgUeReset(cell, ue, reset, errInfo);
   if (ret != ROK)
   {
      RGDBGERR((rgPBuf, "UE Reset FAILED\n"));
      MSPD_ERR("UE Reset FAILED\n");
      RETVALUE(RFAILED);
   }
   
   RGDBGINFO((rgPBuf, "CRG UE Reset processed \n"));
   RETVALUE(ret);
}  /* rgCOMHndlResetReq */
/*End: LTEMAC_2.1_DEV_CFG */

/**
 * @brief Handler for processing Cell/UE/Logical channel delete request
 * recieved from RRC.
 *
 * @details
 *
 *     Function: rgCOMHndlDelReq
 *     
 *     This API handles processing of delete request from RRC to MAC. 
 *     
 *     Processing Steps:
 *        - Fetch corresponding control block and pass it to CFG module.
 *        - If control block does not exist, FAIL.
 *
 *  @param[in]  CrgDel    *del
 *  @param[out] RgErrInfo *errInfo
 *  @return  S16
 *      -# ROK 
 *      -# RFAILED 
 **/
#ifdef ANSI
PRIVATE S16 rgCOMHndlDelReq
(
CrgDel    *del,
RgErrInfo *errInfo
)
#else
PRIVATE S16 rgCOMHndlDelReq(del, errInfo)
CrgDel    *del;
RgErrInfo *errInfo;
#endif
{

   S16       ret;
      
   TRC2(rgCOMHndlDelReq);
   RGDBGPRM((rgPBuf, "Processing CRG Delete request: delType %d\n", 
         del->delType));
   
   errInfo->errType = RGERR_COM_DEL_REQ;
   
   /* Process the delete request */ 
   switch (del->delType)
   {
      case CRG_CELL_CFG:
      {
         ret = rgCFGCrgCellDel(del, errInfo);
         break;
      }
      case CRG_UE_CFG:
      {

#ifdef MSPD_MLOG_NEW
         VOLATILE U32 t=0, t2=0;
         t = GetTIMETICK();
#endif	  	
#ifdef MSPD
         MSPD_DBG("[%d] Deleting UE\n", del->u.ueDel.crnti);
#endif	  	

         ret = rgCFGCrgUeDel(del, errInfo);
#ifdef MSPD
         MSPD_DBG("[%d] Delete Done\n", del->u.ueDel.crnti);
#endif
#ifdef MSPD_MLOG_NEW
         MLogTask(PID_MAC_UE_DEL, RESOURCE_LARM, t, t2=GetTIMETICK());
#endif

         break;
      }
      case CRG_LCH_CFG:
      {
         ret = rgCFGCrgLcDel(del, errInfo);
         break;
      }
      default:
      {
#if(ERRCLASS & ERRCLS_INT_PAR)
         RGDBGERR((rgPBuf, "Should never come here: delType %d\n",
                  del->delType));
#endif
         MSPD_ERR("Should never come here: delType %d\n",
                  del->delType);
         RETVALUE(RFAILED);
      }
   }

   RGDBGINFO((rgPBuf, "CRG deletion processed \n"));
   RETVALUE(ret);
}  /* rgCOMHndlDelReq */

/**********************************************************************
 
         End of file:     gk_com.c@@/main/3 - Sat Jul 30 02:21:25 2011
 
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
/main/1      ---    sd    1. Initial Release.
/main/2      ---    sd    1. LTE MAC 2.1 release
/main/3    rg008.201 rsharon 1. Removed commented code.
/main/3      ---        gvj  1. Updated for LTE MAC Rel 3.1         
*********************************************************************91*/
