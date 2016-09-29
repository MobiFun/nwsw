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
  
     File:     rg_ram.c
  
     Sid:      gk_ram.c@@/main/3 - Sat Jul 30 02:21:33 2011
  
     Prg:     ps 
  
**********************************************************************/

/** @file rg_ram.c
@brief This file has APIs to handle the random access procedure functionality.
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

/* local typedefs */
 
/* forward references */

/***********************************************************
 *
 *     Func : rgRAMFreeUeCb
 *
 *
 *     Desc :
 *     - Processing Steps:
 *        - Frees UE control block.
 *
 *     Ret  : Void
 *
 *     Notes:
 *
 *     File :
 *
 **********************************************************/
#ifdef ANSI
PUBLIC Void rgRAMFreeUeCb
(
RgUeCb      *ue
)
#else
PUBLIC Void rgRAMFreeUeCb(ue)
RgUeCb      *ue;
#endif
{
   TRC2(rgRAMFreeUeCb);

   rgDHMFreeUe(&ue->dl.hqEnt);

   /*ccpu00117052 - MOD - Passing double pointer for proper NULLP 
                          assignment */
   /* De-allocate the Ue */
   rgFreeSBuf((Data **)&ue, sizeof(*ue));

/*rg008.201 - Fixed Tracing Issue*/
   /* Stack Crash problem for TRACE5 changes. Added the return below */
   RETVOID;

}  /* rgRAMFreeUeCb */

/**
 * @brief Handler for Random Access control block creation.
 *
 * @details
 *
 *     Function : rgRAMCreateUeCb
 *                Creates a raCb and gives the same to scheduler for its updation.
 *     
 *
 *  @param[in]       RgCellCb       *cell 
 *  @param[in]       CmLteRnti      tmpCrnti 
 *  @param[out]      RgErrInfo      *err
 *  @return  RgUeCb*
 **/
#ifdef ANSI
PUBLIC RgUeCb* rgRAMCreateUeCb
(
RgCellCb       *cell,
CmLteRnti      tmpCrnti,
Bool           insert,
RgErrInfo      *err
)
#else
PUBLIC RgUeCb* rgRAMCreateUeCb(cell, tmpCrnti, insert, err)
RgCellCb       *cell;
CmLteRnti      tmpCrnti;
Bool           insert;
RgErrInfo      *err;
#endif
{
   RgUeCb    *ueCb = NULLP;

   TRC2(rgRAMCreateUeCb)

   /* Allocate the Ue control block */
   if (rgAllocSBuf((Data **)&ueCb, sizeof(*ueCb)) != ROK)
   {
      RGDBGERR((rgPBuf, "Memory allocation FAILED for ue %d",
               tmpCrnti));
      MSPD_ERR("Memory allocation FAILED for ue %d",
               tmpCrnti);
      err->errCause = RGERR_RAM_MEM_EXHAUST;
      RETVALUE(NULLP);
   }

   /* Inititialize Ue control block */
   ueCb->ueId = tmpCrnti;

   /* Initialize the lists of the UE */
   rgDBMInitUe(ueCb); 

   if(insert == TRUE)
   {
      /* Insert the created raCb into raCb list of cell */
      ueCb->rachLstEnt.next = NULLP;
      ueCb->rachLstEnt.prev = NULLP;
      ueCb->rachLstEnt.node = (PTR)(ueCb);
      rgDBMInsUeCbInRachLst(cell, ueCb);
   }

   RETVALUE(ueCb);
}  /* rgRAMCreateUeCb */

/**
 * @brief Function for handling cell delete.
 *
 * @details
 *
 *     Function : rgRAMFreeCell
 *     
 *     This function shall be invoked whenever a cell needs to be deleted.
 *     This shall remove raCbs and raReqs stored in cell.
 *     
 *           
 *  @param[in,out] RgCellCb  *cell
 *  @return  S16
 *      -# ROK 
 **/
#ifdef ANSI
PUBLIC S16 rgRAMFreeCell
(
RgCellCb    *cell
)
#else
PUBLIC S16 rgRAMFreeCell(cell)
RgCellCb    *cell;
#endif
{
   RgUeCb  *ueCb;

   TRC2(rgRAMFreeCell);

   /* Free CURRENT CRG cfg list */
   while ((ueCb = rgDBMGetNextUeCbFromRachLst(cell, NULLP)) != NULLP)
   {
      rgDBMDelUeCbFromRachLst(cell, ueCb);
      rgRAMFreeUeCb(ueCb);
   }

   RETVALUE(ROK); 
} /* rgRAMFreeCell */
/* rg002.201: Changed the function description */
/**
 * @brief Function for handling RA response scheduled for a subframe.
 *
 * @details
 *
 *     Function : rgHndlRaResp
 *     
 *     This function shall be invoked whenever scheduler is done with the
 *     allocations of random access responses for a subframe RgSchMacSfAllocReq.
 *
 *     Processing steps :
 *
 *     This shall invoke RAM to create ueCbs for all the rapIds allocated and 
 *     shall invoke MUX to create RAR PDUs for raRntis allocated.
 *     
 *           
 *  @param[in] RgCellCb          *cell,
 *  @param[in] CmLteTimingInfo   timingInfo,
 *  @param[in] RgInfRarInfo      *rarInfo
 *  @param[in/out] RgErrInfo     *err
 *  @return  S16
 *      -# ROK 
 **/
#ifdef ANSI
PUBLIC S16 rgHndlRaResp
(
RgCellCb            *cell,
CmLteTimingInfo     timingInfo,
RgInfRarInfo        *rarInfo,
RgErrInfo           *err
)
#else
PUBLIC S16 rgHndlRaResp(cell, timingInfo, rarInfo, err)
RgCellCb            *cell;
CmLteTimingInfo     timingInfo;
RgInfRarInfo        *rarInfo;
RgErrInfo           *err;
#endif
{
   U8       idx1,idx2;
   Buffer   *rarPdu;
   RgDlSf   *dlSf;
   U8       idx;

   TRC2(rgHndlRaResp)

   if(NULLP == rarInfo->raRntiInfo)
   {
      RETVALUE(RFAILED);
   }

   idx = timingInfo.subframe;
   dlSf = &cell->subFrms[idx];
   dlSf->numRaRsp=0;

   /* Create RAR PDUs for all the allocated RA-RNTIs */
   for(idx1 = 0; idx1 < rarInfo->numRaRntis; idx1++)
   {
      if(ROK == (rgMUXBldRarPdu(cell, 
                     &rarInfo->raRntiInfo[idx1], &rarPdu, err)))
      {
         /* Create RaCbs for all the rapIds allocated */
         for(idx2 = 0; idx2 < rarInfo->raRntiInfo[idx1].numCrnti; idx2++)
         {
            if(FALSE == rarInfo->raRntiInfo[idx1].crntiInfo[idx2].isContFree)
            {
               if(rgRAMCreateUeCb(cell,
                  rarInfo->raRntiInfo[idx1].crntiInfo[idx2].tmpCrnti, 
                  TRUE, err) == NULLP)
               {
                  RETVALUE(RFAILED);
               }
            }
         }
         /* Store the created RAR PDU */
         dlSf->raRsp[dlSf->numRaRsp].pdcch.rnti = 
            rarInfo->raRntiInfo[idx1].raRnti;

         dlSf->raRsp[dlSf->numRaRsp].pdcch.dci = 
            rarInfo->raRntiInfo[idx1].dciInfo;

         dlSf->raRsp[dlSf->numRaRsp].rar = rarPdu;

         dlSf->numRaRsp++;
      }
      else
      {
         RGDBGERR((rgPBuf, "rgHndlRaResp(): Creation of RAR PDU for\
                  raRnti=%d failed \n", rarInfo->raRntiInfo[idx1].raRnti));
         MSPD_ERR("rgHndlRaResp(): Creation of RAR PDU for\
                  raRnti=%d failed \n", rarInfo->raRntiInfo[idx1].raRnti);
         continue;
      }
   } /* end of raRntis loop */
   RETVALUE(ROK);
} /* end of rgHndlRaResp */

/**
 * @brief Function for handling RaResp request received from scheduler to MAC.
 *
 * @details
 *
 *     Function : RgSchMacRlsRntiReq
 *     
 *     This function shall be invoked whenever scheduler is done with the
 *     allocations of random access responses for a subframe.
 *     This shall invoke RAM to create ueCbs for all the rapIds allocated and 
 *     shall invoke MUX to create RAR PDUs for raRntis allocated.
 *     
 *           
 *  @param[in] CmLteCellId         cellId,
 *  @param[in] CmLteTimingInfo     timingInfo,
 *  @param[in] RaRespInfo          *rarInfo
 *  @return  S16
 *      -# ROK 
 **/
#ifdef ANSI
PUBLIC S16 RgSchMacRlsRntiReq
(
Pst                 *pst,
RgInfRlsRnti        *rlsRnti
)
#else
PUBLIC S16 RgSchMacRlsRntiReq(pst, rlsRnti)
Pst                 *pst;
RgInfRlsRnti        *rlsRnti;
#endif
{
   RgCellCb       *cell;
   RgUeCb         *ue;

   /*rg003.201:Changed from TRC2 to TRC3 */
   TRC3(RgSchMacRlsRntiReq)

   if(NULLP == rlsRnti)
   {
      RETVALUE(RFAILED);
   }

   if((cell = rgDBMGetCellCb(rlsRnti->cellId)) == NULLP)
   {
      RGDBGERR((rgPBuf, "rgHndlSchdUe(): " 
               "No cellCb found with cellId = (%d)\n", rlsRnti->cellId));
      MSPD_ERR("rgHndlSchdUe(): " 
               "No cellCb found with cellId = (%d)\n", rlsRnti->cellId);
      RETVALUE(RFAILED);
   }
   /* Check in RachLst */
   if((ue=rgDBMGetUeCbFromRachLst (cell, rlsRnti->rnti)) == NULLP)
   {
      RGDBGINFO((rgPBuf,"rgHndlRlsUe(): No ueCb found in Rachlst"
         ": cellId (%d) rnti(%d) \n", rlsRnti->cellId, rlsRnti->rnti));
      MSPD_DBG("No ueCb found in Rachlst"
         ": cellId (%d) rnti(%d) \n", rlsRnti->cellId, rlsRnti->rnti);
      RETVALUE(RFAILED);
   }
   /* Delete Ue from the UE list */
   rgDBMDelUeCbFromRachLst(cell, ue);

   /* Free Ue */
   rgRAMFreeUeCb(ue);

   RETVALUE(ROK);
} /* end of RgSchMacRlsRntiReq */

/**********************************************************************
 
         End of file:     gk_ram.c@@/main/3 - Sat Jul 30 02:21:33 2011
 
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
/main/1      ---       ps   1.Initial Release.
/main/2      ---       ps   1.Initial LTE MAC 2.1 release
             rg002.201 sm   1. Changed the function description rgHndlRaResp.
             rg003.201 ns   1. Trace added for functions.
             rg008.201 rsharon   1.Fixed TRACE5 issue.
/main/3      ---        gvj  1. Updated for LTE MAC Rel 3.1           
*********************************************************************91*/
