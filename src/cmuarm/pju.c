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
  
        Name:    LTE PDCP Interface  
    
        Type:    C file
  
        Desc:    This file contains the pack/unpack functions for the
                 data structures at PJU Interface.
 
        File:    pju.c

        Sid:      pju.c@@/main/5 - Fri Sep  2 19:27:53 2011
  
        Prg:     Dinesh 
  
*********************************************************************21*/

/* header include files (.h) */
  
#include "envopt.h"             /* Environment options */  
#include "envdep.h"             /* Environment dependent */
#include "envind.h"             /* Environment independent */
#include "gen.h"                /* General */
#include "ssi.h"                /* System services interface */
#include "cm_lte.h"             /* Common LTE header file */
#include "cm_llist.h"           /* Linked list header file */
#include "cm_tkns.h"            /* Common tokens header file */
#include "cm_hash.h"            /* Hash List header file */
#include "pju.h"                /* PJU Interface header file */



/* header/extern include files (.x) */

#include "gen.x"                /* General */
#include "ssi.x"                /* System services interface */
#include "cm_lib.x"             /* Common lib header file */
#include "cm_hash.x"            /* Hash List header file */
#include "cm_llist.x"           /* Linked list header file */
#include "cm_tkns.x"            /* Common tokens header file */
#include "cm_lte.x"             /* Common LTE header file */
#include "pju.x"                /* PJU Interface header file */

#if (defined(LCPJU) || defined(LWLCPJU))

#ifdef __cplusplus
EXTERN "C" {
#endif /* __cplusplus */

/* pju_c_001.main_5 ccpu00120534, fixed the problem of passing suId and spId as argument */
#ifdef ANSI
PUBLIC S16 cmPkPjuBndReq
(
Pst* pst,
SuId suId,
SpId spId
)
#else
PUBLIC S16 cmPkPjuBndReq(pst, suId, spId)
Pst* pst;
SuId suId;
SpId spId;
#endif
{
   Buffer *mBuf = NULLP;
   TRC3(cmPkPjuBndReq)

   if (SGetMsg(pst->region, pst->pool, &mBuf) != ROK) {
#if (ERRCLASS & ERRCLS_ADD_RES)
      SLogError(pst->srcEnt, pst->srcInst, pst->srcProcId,
         __FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
         (ErrVal)EPJU001, (ErrVal)0, "Packing failed");
#endif
      RETVALUE(RFAILED);
   }
   if (SPkS16(suId, mBuf) != ROK) {
#if (ERRCLASS & ERRCLS_ADD_RES)
      SLogError(pst->srcEnt, pst->srcInst, pst->srcProcId,
         __FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
         (ErrVal)EPJU002, (ErrVal)0, "Packing failed");
#endif
      SPutMsg(mBuf);
      RETVALUE(RFAILED);
   }
   if (SPkS16(spId, mBuf) != ROK) {
#if (ERRCLASS & ERRCLS_ADD_RES)
      SLogError(pst->srcEnt, pst->srcInst, pst->srcProcId,
         __FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
         (ErrVal)EPJU003, (ErrVal)0, "Packing failed");
#endif
      SPutMsg(mBuf);
      RETVALUE(RFAILED);
   }
   pst->event = (Event) EVTPJUBNDREQ;
   RETVALUE(SPstTsk(pst,mBuf));
}

#ifdef ANSI
PUBLIC S16 cmUnpkPjuBndReq
(
PjuBndReq func,
Pst *pst,
Buffer *mBuf
)
#else
PUBLIC S16 cmUnpkPjuBndReq(func, pst, mBuf)
PjuBndReq func;
Pst *pst;
Buffer *mBuf;
#endif
{
   SpId spId;
   SuId suId;
   
   TRC3(cmUnpkPjuBndReq)

   if (SUnpkS16(&spId, mBuf) != ROK) {
      SPutMsg(mBuf);
#if (ERRCLASS & ERRCLS_ADD_RES)
      SLogError(pst->srcEnt, pst->srcInst, pst->srcProcId,
         __FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
         (ErrVal)EPJU004, (ErrVal)0, "Unpacking failed");
#endif
      RETVALUE(RFAILED);
   }
   if (SUnpkS16(&suId, mBuf) != ROK) {
      SPutMsg(mBuf);
#if (ERRCLASS & ERRCLS_ADD_RES)
      SLogError(pst->srcEnt, pst->srcInst, pst->srcProcId,
         __FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
         (ErrVal)EPJU005, (ErrVal)0, "Unpacking failed");
#endif
      RETVALUE(RFAILED);
   }
   SPutMsg(mBuf);
   /* pju_c_001.main_5 ccpu00120534, fixed the problem of passing suId and spId as argument */
   RETVALUE((*func)(pst, suId, spId));
}

#ifdef ANSI
PUBLIC S16 cmPkPjuBndCfm
(
Pst* pst,
SuId suId,
U8 status
)
#else
PUBLIC S16 cmPkPjuBndCfm(pst, suId, status)
Pst* pst;
SuId suId;
U8 status;
#endif
{
   Buffer *mBuf = NULLP;
   TRC3(cmPkPjuBndCfm)

   if (SGetMsg(pst->region, pst->pool, &mBuf) != ROK) {
#if (ERRCLASS & ERRCLS_ADD_RES)
      SLogError(pst->srcEnt, pst->srcInst, pst->srcProcId,
         __FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
         (ErrVal)EPJU006, (ErrVal)0, "Packing failed");
#endif
      RETVALUE(RFAILED);
   }
   if (SPkU8(status, mBuf) != ROK) {
#if (ERRCLASS & ERRCLS_ADD_RES)
      SLogError(pst->srcEnt, pst->srcInst, pst->srcProcId,
         __FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
         (ErrVal)EPJU007, (ErrVal)0, "Packing failed");
#endif
      SPutMsg(mBuf);
      RETVALUE(RFAILED);
   }
   if (SPkS16(suId, mBuf) != ROK) {
#if (ERRCLASS & ERRCLS_ADD_RES)
      SLogError(pst->srcEnt, pst->srcInst, pst->srcProcId,
         __FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
         (ErrVal)EPJU008, (ErrVal)0, "Packing failed");
#endif
      SPutMsg(mBuf);
      RETVALUE(RFAILED);
   }
   pst->event = (Event) EVTPJUBNDCFM;
   RETVALUE(SPstTsk(pst,mBuf));
}

#ifdef ANSI
PUBLIC S16 cmUnpkPjuBndCfm
(
PjuBndCfm func,
Pst *pst,
Buffer *mBuf
)
#else
PUBLIC S16 cmUnpkPjuBndCfm(func, pst, mBuf)
PjuBndCfm func;
Pst *pst;
Buffer *mBuf;
#endif
{
   SuId suId;
   U8 status;
   
   TRC3(cmUnpkPjuBndCfm)

   if (SUnpkS16(&suId, mBuf) != ROK) {
      SPutMsg(mBuf);
#if (ERRCLASS & ERRCLS_ADD_RES)
      SLogError(pst->srcEnt, pst->srcInst, pst->srcProcId,
         __FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
         (ErrVal)EPJU009, (ErrVal)0, "Unpacking failed");
#endif
      RETVALUE(RFAILED);
   }
   if (SUnpkU8(&status, mBuf) != ROK) {
      SPutMsg(mBuf);
#if (ERRCLASS & ERRCLS_ADD_RES)
      SLogError(pst->srcEnt, pst->srcInst, pst->srcProcId,
         __FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
         (ErrVal)EPJU010, (ErrVal)0, "Unpacking failed");
#endif
      RETVALUE(RFAILED);
   }
   SPutMsg(mBuf);
   RETVALUE((*func)(pst, suId, status));
}

#ifdef ANSI
PUBLIC S16 cmPkPjuUbndReq
(
Pst* pst,
SpId spId,
Reason reason
)
#else
PUBLIC S16 cmPkPjuUbndReq(pst, spId, reason)
Pst* pst;
SpId spId;
Reason reason;
#endif
{
   Buffer *mBuf = NULLP;
   TRC3(cmPkPjuUbndReq)

   if (SGetMsg(pst->region, pst->pool, &mBuf) != ROK) {
#if (ERRCLASS & ERRCLS_ADD_RES)
      SLogError(pst->srcEnt, pst->srcInst, pst->srcProcId,
         __FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
         (ErrVal)EPJU011, (ErrVal)0, "Packing failed");
#endif
      RETVALUE(RFAILED);
   }
   if (SPkS16(reason, mBuf) != ROK) {
#if (ERRCLASS & ERRCLS_ADD_RES)
      SLogError(pst->srcEnt, pst->srcInst, pst->srcProcId,
         __FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
         (ErrVal)EPJU012, (ErrVal)0, "Packing failed");
#endif
      SPutMsg(mBuf);
      RETVALUE(RFAILED);
   }
   if (SPkS16(spId, mBuf) != ROK) {
#if (ERRCLASS & ERRCLS_ADD_RES)
      SLogError(pst->srcEnt, pst->srcInst, pst->srcProcId,
         __FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
         (ErrVal)EPJU013, (ErrVal)0, "Packing failed");
#endif
      SPutMsg(mBuf);
      RETVALUE(RFAILED);
   }
   pst->event = (Event) EVTPJUUBNDREQ;
   RETVALUE(SPstTsk(pst,mBuf));
}

#ifdef ANSI
PUBLIC S16 cmUnpkPjuUbndReq
(
PjuUbndReq func,
Pst *pst,
Buffer *mBuf
)
#else
PUBLIC S16 cmUnpkPjuUbndReq(func, pst, mBuf)
PjuUbndReq func;
Pst *pst;
Buffer *mBuf;
#endif
{
   SpId spId;
   Reason reason;
   
   TRC3(cmUnpkPjuUbndReq)

   if (SUnpkS16(&spId, mBuf) != ROK) {
      SPutMsg(mBuf);
#if (ERRCLASS & ERRCLS_ADD_RES)
      SLogError(pst->srcEnt, pst->srcInst, pst->srcProcId,
         __FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
         (ErrVal)EPJU014, (ErrVal)0, "Unpacking failed");
#endif
      RETVALUE(RFAILED);
   }
   if (SUnpkS16(&reason, mBuf) != ROK) {
      SPutMsg(mBuf);
#if (ERRCLASS & ERRCLS_ADD_RES)
      SLogError(pst->srcEnt, pst->srcInst, pst->srcProcId,
         __FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
         (ErrVal)EPJU015, (ErrVal)0, "Unpacking failed");
#endif
      RETVALUE(RFAILED);
   }
   SPutMsg(mBuf);
   RETVALUE((*func)(pst, spId, reason));
}

#ifdef ANSI
PUBLIC S16 cmPkPjuDatReq
(
Pst* pst,
SpId spId,
CmLtePdcpId * pdcpId,
PjuSduId sduId,
Buffer * buff
)
#else
PUBLIC S16 cmPkPjuDatReq(pst, spId, pdcpId, sduId, buff)
Pst* pst;
SpId spId;
CmLtePdcpId * pdcpId;
PjuSduId sduId;
Buffer * buff;
#endif
{
   Buffer *mBuf = NULLP;
   MsgLen msgLen = 0;
   TRC3(cmPkPjuDatReq)

   if (SGetMsg(pst->region, pst->pool, &mBuf) != ROK) {
#if (ERRCLASS & ERRCLS_ADD_RES)
      SLogError(pst->srcEnt, pst->srcInst, pst->srcProcId,
         __FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
         (ErrVal)EPJU016, (ErrVal)0, "Packing failed");
#endif
      SPutSBuf(pst->region, pst->pool, (Data *)pdcpId, sizeof(CmLtePdcpId));
      /* pju_c_001.main_3 - Free the existing buffer */
      SPutMsg(buff); 
      RETVALUE(RFAILED);
   }
   if (pst->selector == PJU_SEL_LC) {
      if (SFndLenMsg(buff, &msgLen) != ROK) {
#if (ERRCLASS & ERRCLS_ADD_RES)
         SLogError(pst->srcEnt, pst->srcInst, pst->srcProcId,
            __FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
            (ErrVal)EPJU017, (ErrVal)0, "Packing failed");
#endif
         SPutSBuf(pst->region, pst->pool, (Data *)pdcpId, sizeof(CmLtePdcpId));
         SPutMsg(buff);
      /* pju_c_001.main_3 - Memory leak fix */
         SPutMsg(mBuf);
         RETVALUE(RFAILED);
      }
      if (SCatMsg(mBuf, buff, M1M2) != ROK) {
#if (ERRCLASS & ERRCLS_ADD_RES)
         SLogError(pst->srcEnt, pst->srcInst, pst->srcProcId,
            __FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
            (ErrVal)EPJU018, (ErrVal)0, "Packing failed");
#endif
         SPutSBuf(pst->region, pst->pool, (Data *)pdcpId, sizeof(CmLtePdcpId));
         SPutMsg(buff);
      /* pju_c_001.main_3 - Memory leak fix */
         SPutMsg(mBuf);
         RETVALUE(RFAILED);
      }
      SPutMsg(buff);
      CMCHKPK(SPkS16, msgLen, mBuf);

   }
   if (SPkU32(sduId, mBuf) != ROK) {
#if (ERRCLASS & ERRCLS_ADD_RES)
      SLogError(pst->srcEnt, pst->srcInst, pst->srcProcId,
         __FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
         (ErrVal)EPJU020, (ErrVal)0, "Packing failed");
#endif
      SPutSBuf(pst->region, pst->pool, (Data *)pdcpId, sizeof(CmLtePdcpId));
      SPutMsg(mBuf);
      RETVALUE(RFAILED);
   }
   if (pst->selector == PJU_SEL_LC) {
      if (cmPkLtePdcpId(pdcpId, mBuf) != ROK) {
#if (ERRCLASS & ERRCLS_ADD_RES)
         SLogError(pst->srcEnt, pst->srcInst, pst->srcProcId,
            __FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
            (ErrVal)EPJU021, (ErrVal)0, "Packing failed");
#endif
         SPutSBuf(pst->region, pst->pool, (Data *)pdcpId, sizeof(CmLtePdcpId));
         SPutMsg(mBuf);
         RETVALUE(RFAILED);
      }
   }

   if (SPkS16(spId, mBuf) != ROK) {
#if (ERRCLASS & ERRCLS_ADD_RES)
      SLogError(pst->srcEnt, pst->srcInst, pst->srcProcId,
         __FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
         (ErrVal)EPJU023, (ErrVal)0, "Packing failed");
#endif
      SPutSBuf(pst->region, pst->pool, (Data *)pdcpId, sizeof(CmLtePdcpId));
      SPutMsg(mBuf);
      RETVALUE(RFAILED);
   }

   /* Free Request Structure after packing */
   SPutSBuf(pst->region, pst->pool, (Data *)pdcpId, sizeof(CmLtePdcpId));

   pst->event = (Event) EVTPJUDATREQ;
   RETVALUE(SPstTsk(pst,mBuf));
}

#ifdef ANSI
PUBLIC S16 cmUnpkPjuDatReq
(
PjuDatReq func,
Pst *pst,
Buffer *mBuf
)
#else
PUBLIC S16 cmUnpkPjuDatReq(func, pst, mBuf)
PjuDatReq func;
Pst *pst;
Buffer *mBuf;
#endif
{
   S16   ret1;
   SpId spId;
   CmLtePdcpId *pdcpId = NULLP;
   PjuSduId sduId;
   Buffer *buff = NULLP;
   
   TRC3(cmUnpkPjuDatReq)

   if (SUnpkS16(&spId, mBuf) != ROK) {
      SPutMsg(mBuf);
#if (ERRCLASS & ERRCLS_ADD_RES)
      SLogError(pst->srcEnt, pst->srcInst, pst->srcProcId,
         __FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
         (ErrVal)EPJU025, (ErrVal)0, "Unpacking failed");
#endif
      RETVALUE(RFAILED);
   }

   if (pst->selector == PJU_SEL_LC) 
   {
      if((ret1 = SGetSBuf(pst->region, pst->pool, (Data **)&pdcpId,\
                  sizeof(CmLtePdcpId))) != ROK)
      {
#if (ERRCLASS & ERRCLS_ADD_RES)
         SLogError(pst->srcEnt, pst->srcInst, pst->srcProcId,
               __FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
               (ErrVal)EPJU040, (ErrVal)0, "SGetMsg() failed");
#endif /*  ERRCLASS & ERRCLS_ADD_RES  */
         RETVALUE(ret1);
      }
      if (cmUnpkLtePdcpId(pdcpId, mBuf) != ROK)
      {
         SPutSBuf(pst->region, pst->pool, (Data *)pdcpId, sizeof(CmLtePdcpId));
         SPutMsg(mBuf);
#if (ERRCLASS & ERRCLS_ADD_RES)
         SLogError(pst->srcEnt, pst->srcInst, pst->srcProcId,
               __FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
               (ErrVal)EPJU027, (ErrVal)0, "Unpacking failed");
#endif
         RETVALUE(RFAILED);
      }
   }
   if (SUnpkU32(&sduId, mBuf) != ROK) {
      SPutSBuf(pst->region, pst->pool, (Data *)pdcpId, sizeof(CmLtePdcpId));
      SPutMsg(mBuf);
#if (ERRCLASS & ERRCLS_ADD_RES)
      SLogError(pst->srcEnt, pst->srcInst, pst->srcProcId,
         __FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
         (ErrVal)EPJU028, (ErrVal)0, "Unpacking failed");
#endif
      RETVALUE(RFAILED);
   }
   if (pst->selector == PJU_SEL_LC) {
      S16 msgLen, totalMsgLen;
      CMCHKUNPK(SUnpkS16, &msgLen, mBuf);
      if (SFndLenMsg(mBuf, &totalMsgLen) != ROK)
      {
         /* pju_c_001.main_3 - Memory leak fix */
         SPutSBuf(pst->region, pst->pool, (Data *)pdcpId, sizeof(CmLtePdcpId));
         SPutMsg(mBuf);
         RETVALUE(RFAILED);
      }
      /* pju_c_001.main_2 */
      if (SSegMsg(mBuf, (S16)(totalMsgLen-msgLen), &buff) != ROK)
      {
         /* pju_c_001.main_3 - Memory leak fix */
         SPutSBuf(pst->region, pst->pool, (Data *)pdcpId, sizeof(CmLtePdcpId));
         SPutMsg(mBuf);
         RETVALUE(RFAILED);
      }  
   }
   SPutMsg(mBuf);
   RETVALUE((*func)(pst, spId, pdcpId, sduId, buff));

}

#ifdef ANSI
PUBLIC S16 cmPkPjuDatCfm
(
Pst* pst,
SuId suId,
CmLtePdcpId * pdcpId,
PjuDatCfmInfo * datCfm
)
#else
PUBLIC S16 cmPkPjuDatCfm(pst, suId, pdcpId, datCfm)
Pst* pst;
SuId suId;
CmLtePdcpId * pdcpId;
PjuDatCfmInfo * datCfm;
#endif
{
   Buffer *mBuf = NULLP;
   TRC3(cmPkPjuDatCfm)

   if (SGetMsg(pst->region, pst->pool, &mBuf) != ROK) {
#if (ERRCLASS & ERRCLS_ADD_RES)
      SLogError(pst->srcEnt, pst->srcInst, pst->srcProcId,
         __FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
         (ErrVal)EPJU029, (ErrVal)0, "Packing failed");
#endif
      SPutSBuf(pst->region, pst->pool, (Data *)pdcpId, sizeof(CmLtePdcpId));
      SPutSBuf(pst->region, pst->pool, (Data *)datCfm, sizeof(PjuDatCfmInfo));
      RETVALUE(RFAILED);
   }
   if (pst->selector == PJU_SEL_LC) {
      if (cmPkPjuDatCfmInfo(datCfm, mBuf) != ROK) {
#if (ERRCLASS & ERRCLS_ADD_RES)
         SLogError(pst->srcEnt, pst->srcInst, pst->srcProcId,
            __FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
            (ErrVal)EPJU030, (ErrVal)0, "Packing failed");
#endif
         SPutSBuf(pst->region, pst->pool, (Data *)pdcpId, sizeof(CmLtePdcpId));
         SPutSBuf(pst->region, pst->pool, (Data *)datCfm, sizeof(PjuDatCfmInfo));
         SPutMsg(mBuf);
         RETVALUE(RFAILED);
      }
   }

   if (pst->selector == PJU_SEL_LC) {
      if (cmPkLtePdcpId(pdcpId, mBuf) != ROK) {
#if (ERRCLASS & ERRCLS_ADD_RES)
         SLogError(pst->srcEnt, pst->srcInst, pst->srcProcId,
            __FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
            (ErrVal)EPJU032, (ErrVal)0, "Packing failed");
#endif
         SPutSBuf(pst->region, pst->pool, (Data *)pdcpId, sizeof(CmLtePdcpId));
         SPutSBuf(pst->region, pst->pool, (Data *)datCfm, sizeof(PjuDatCfmInfo));
         SPutMsg(mBuf);
         RETVALUE(RFAILED);
      }
   }

   if (SPkS16(suId, mBuf) != ROK) {
#if (ERRCLASS & ERRCLS_ADD_RES)
      SLogError(pst->srcEnt, pst->srcInst, pst->srcProcId,
         __FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
         (ErrVal)EPJU034, (ErrVal)0, "Packing failed");
#endif
      SPutSBuf(pst->region, pst->pool, (Data *)pdcpId, sizeof(CmLtePdcpId));
      SPutSBuf(pst->region, pst->pool, (Data *)datCfm, sizeof(PjuDatCfmInfo));
      SPutMsg(mBuf);
      RETVALUE(RFAILED);
   }

   /* Free Confirm Structure after packing */
   SPutSBuf(pst->region, pst->pool, (Data *)pdcpId, sizeof(CmLtePdcpId));
   SPutSBuf(pst->region, pst->pool, (Data *)datCfm, sizeof(PjuDatCfmInfo));

   pst->event = (Event) EVTPJUDATCFM;
   RETVALUE(SPstTsk(pst,mBuf));
}

#ifdef ANSI
PUBLIC S16 cmUnpkPjuDatCfm
(
PjuDatCfm func,
Pst *pst,
Buffer *mBuf
)
#else
PUBLIC S16 cmUnpkPjuDatCfm(func, pst, mBuf)
PjuDatCfm func;
Pst *pst;
Buffer *mBuf;
#endif
{
   S16   ret1;
   SuId suId;
   CmLtePdcpId *pdcpId = NULLP;
   PjuDatCfmInfo *datCfm = NULLP;
   
   TRC3(cmUnpkPjuDatCfm)

   if (SUnpkS16(&suId, mBuf) != ROK) {
      SPutMsg(mBuf);
#if (ERRCLASS & ERRCLS_ADD_RES)
      SLogError(pst->srcEnt, pst->srcInst, pst->srcProcId,
         __FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
         (ErrVal)EPJU037, (ErrVal)0, "Unpacking failed");
#endif
      RETVALUE(RFAILED);
   }


   if (pst->selector == PJU_SEL_LC) 
   {
      if((ret1 = SGetSBuf(pst->region, pst->pool, (Data **)&pdcpId,\
                  sizeof(CmLtePdcpId))) != ROK)
      {
#if (ERRCLASS & ERRCLS_ADD_RES)
         SLogError(pst->srcEnt, pst->srcInst, pst->srcProcId,
               __FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
               (ErrVal)EPJU040, (ErrVal)0, "SGetMsg() failed");
#endif /*  ERRCLASS & ERRCLS_ADD_RES  */
         RETVALUE(ret1);
      }

      if (cmUnpkLtePdcpId(pdcpId, mBuf) != ROK)
      {
         SPutSBuf(pst->region, pst->pool, (Data *)pdcpId, sizeof(CmLtePdcpId));
         SPutMsg(mBuf);
#if (ERRCLASS & ERRCLS_ADD_RES)
         SLogError(pst->srcEnt, pst->srcInst, pst->srcProcId,
               __FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
               (ErrVal)EPJU039, (ErrVal)0, "Unpacking failed");
#endif
         RETVALUE(RFAILED);
      }
   }

   if (pst->selector == PJU_SEL_LC) 
   {
      if((ret1 = SGetSBuf(pst->region, pst->pool, (Data **)&datCfm,\
                  sizeof(PjuDatCfmInfo))) != ROK)
      {
#if (ERRCLASS & ERRCLS_ADD_RES)
         SLogError(pst->srcEnt, pst->srcInst, pst->srcProcId,
               __FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
               (ErrVal)EPJU040, (ErrVal)0, "SGetMsg() failed");
#endif /*  ERRCLASS & ERRCLS_ADD_RES  */
         RETVALUE(ret1);
      }

      cmMemset((U8 *)(datCfm), 0, sizeof(PjuDatCfmInfo));

      if (cmUnpkPjuDatCfmInfo(datCfm, mBuf) != ROK) {
         SPutSBuf(pst->region, pst->pool, (Data *)pdcpId, sizeof(CmLtePdcpId));
         SPutSBuf(pst->region, pst->pool, (Data *)datCfm, sizeof(PjuDatCfmInfo));
         SPutMsg(mBuf);
#if (ERRCLASS & ERRCLS_ADD_RES)
         SLogError(pst->srcEnt, pst->srcInst, pst->srcProcId,
               __FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
               (ErrVal)EPJU041, (ErrVal)0, "Unpacking failed");
#endif
         RETVALUE(RFAILED);
      }
   }
   SPutMsg(mBuf);
   RETVALUE((*func)(pst, suId, pdcpId, datCfm));
}

#ifdef ANSI
PUBLIC S16 cmPkPjuDatInd
(
Pst* pst,
SuId suId,
CmLtePdcpId * pdcpId,
Buffer * buff
)
#else
PUBLIC S16 cmPkPjuDatInd(pst, suId, pdcpId, buff)
Pst* pst;
SuId suId;
CmLtePdcpId * pdcpId;
Buffer * buff;
#endif
{
   Buffer *mBuf = NULLP;
   MsgLen msgLen = 0;
   TRC3(cmPkPjuDatInd)

   if (SGetMsg(pst->region, pst->pool, &mBuf) != ROK) {
#if (ERRCLASS & ERRCLS_ADD_RES)
      SLogError(pst->srcEnt, pst->srcInst, pst->srcProcId,
         __FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
         (ErrVal)EPJU042, (ErrVal)0, "Packing failed");
#endif
      SPutSBuf(pst->region, pst->pool, (Data *)pdcpId, sizeof(CmLtePdcpId));
      SPutMsg(buff);
      RETVALUE(RFAILED);
   }
   if (pst->selector == PJU_SEL_LC) {
      if (SFndLenMsg(buff, &msgLen) != ROK) {
#if (ERRCLASS & ERRCLS_ADD_RES)
         SLogError(pst->srcEnt, pst->srcInst, pst->srcProcId,
            __FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
            (ErrVal)EPJU043, (ErrVal)0, "Packing failed");
#endif
         SPutSBuf(pst->region, pst->pool, (Data *)pdcpId, sizeof(CmLtePdcpId));
         SPutMsg(buff);
         SPutMsg(mBuf);
         RETVALUE(RFAILED);
      }
      if (SCatMsg(mBuf, buff, M1M2) != ROK) {
#if (ERRCLASS & ERRCLS_ADD_RES)
         SLogError(pst->srcEnt, pst->srcInst, pst->srcProcId,
            __FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
            (ErrVal)EPJU044, (ErrVal)0, "Packing failed");
#endif
         SPutSBuf(pst->region, pst->pool, (Data *)pdcpId, sizeof(CmLtePdcpId));
         SPutMsg(buff);
         SPutMsg(mBuf);
         RETVALUE(RFAILED);
      }
      SPutMsg(buff);
      CMCHKPK(SPkS16, msgLen, mBuf);

   }

   if (pst->selector == PJU_SEL_LC) {
      if (cmPkLtePdcpId(pdcpId, mBuf) != ROK) {
#if (ERRCLASS & ERRCLS_ADD_RES)
         SLogError(pst->srcEnt, pst->srcInst, pst->srcProcId,
            __FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
            (ErrVal)EPJU046, (ErrVal)0, "Packing failed");
#endif
         SPutSBuf(pst->region, pst->pool, (Data *)pdcpId, sizeof(CmLtePdcpId));
         SPutMsg(mBuf);
         RETVALUE(RFAILED);
      }
   }

   if (SPkS16(suId, mBuf) != ROK) {
#if (ERRCLASS & ERRCLS_ADD_RES)
      SLogError(pst->srcEnt, pst->srcInst, pst->srcProcId,
         __FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
         (ErrVal)EPJU048, (ErrVal)0, "Packing failed");
#endif
      SPutSBuf(pst->region, pst->pool, (Data *)pdcpId, sizeof(CmLtePdcpId));
      SPutMsg(mBuf);
      RETVALUE(RFAILED);
   }

   /* Free Confirm Structure after packing */
   SPutSBuf(pst->region, pst->pool, (Data *)pdcpId, sizeof(CmLtePdcpId));

   pst->event = (Event) EVTPJUDATIND;
   RETVALUE(SPstTsk(pst,mBuf));
}

#ifdef ANSI
PUBLIC S16 cmUnpkPjuDatInd
(
PjuDatInd func,
Pst *pst,
Buffer *mBuf
)
#else
PUBLIC S16 cmUnpkPjuDatInd(func, pst, mBuf)
PjuDatInd func;
Pst *pst;
Buffer *mBuf;
#endif
{
   S16   ret1;
   SuId suId;
   CmLtePdcpId *pdcpId = NULLP;
   Buffer *buff = NULLP;
   
   TRC3(cmUnpkPjuDatInd)

   if (SUnpkS16(&suId, mBuf) != ROK) {
      SPutMsg(mBuf);
#if (ERRCLASS & ERRCLS_ADD_RES)
      SLogError(pst->srcEnt, pst->srcInst, pst->srcProcId,
         __FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
         (ErrVal)EPJU050, (ErrVal)0, "Unpacking failed");
#endif
      RETVALUE(RFAILED);
   }

   if (pst->selector == PJU_SEL_LC) 
   {
      if((ret1 = SGetSBuf(pst->region, pst->pool, (Data **)&pdcpId,\
                  sizeof(CmLtePdcpId))) != ROK)
      {
#if (ERRCLASS & ERRCLS_ADD_RES)
         SLogError(pst->srcEnt, pst->srcInst, pst->srcProcId,
               __FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
               (ErrVal)EPJU040, (ErrVal)0, "SGetMsg() failed");
#endif /*  ERRCLASS & ERRCLS_ADD_RES  */
         RETVALUE(ret1);
      }

      if (cmUnpkLtePdcpId(pdcpId, mBuf) != ROK)
      {
         SPutSBuf(pst->region, pst->pool, (Data *)pdcpId, sizeof(CmLtePdcpId));
         SPutMsg(mBuf);
#if (ERRCLASS & ERRCLS_ADD_RES)
         SLogError(pst->srcEnt, pst->srcInst, pst->srcProcId,
               __FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
               (ErrVal)EPJU052, (ErrVal)0, "Unpacking failed");
#endif
         RETVALUE(RFAILED);
      }
   }
   if (pst->selector == PJU_SEL_LC) {
      S16 msgLen, totalMsgLen;
      CMCHKUNPK(SUnpkS16, &msgLen, mBuf);
      if (SFndLenMsg(mBuf, &totalMsgLen) != ROK)
      {
         /* pju_c_001.main_3 - Memory leak fix */
         SPutSBuf(pst->region, pst->pool, (Data *)pdcpId, sizeof(CmLtePdcpId));
         SPutMsg(mBuf);
         RETVALUE(RFAILED);
      }  
      /* pju_c_001.main_2 */
      if (SSegMsg(mBuf, (S16)(totalMsgLen-msgLen), &buff) != ROK)
      {
         /* pju_c_001.main_3 - Memory leak fix */
         SPutSBuf(pst->region, pst->pool, (Data *)pdcpId, sizeof(CmLtePdcpId));
         SPutMsg(mBuf);
         RETVALUE(RFAILED);
      }
   }

   SPutMsg(mBuf);
   RETVALUE((*func)(pst, suId, pdcpId, buff));
}

#ifdef ANSI
PUBLIC S16 cmPkPjuStaInd
(
Pst* pst,
SuId suId,
CmLtePdcpId * pdcpId,
PjuStaIndInfo * staInd,
Buffer * buff
)
#else
PUBLIC S16 cmPkPjuStaInd(pst, suId, pdcpId, staInd, buff)
Pst* pst;
SuId suId;
CmLtePdcpId * pdcpId;
PjuStaIndInfo * staInd;
Buffer * buff;
#endif
{
   Buffer *mBuf = NULLP;
   MsgLen msgLen = 0;
   TRC3(cmPkPjuStaInd)

   if (SGetMsg(pst->region, pst->pool, &mBuf) != ROK) {
#if (ERRCLASS & ERRCLS_ADD_RES)
      SLogError(pst->srcEnt, pst->srcInst, pst->srcProcId,
         __FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
         (ErrVal)EPJU053, (ErrVal)0, "Packing failed");
#endif
      SPutSBuf(pst->region, pst->pool, (Data *)pdcpId, sizeof(CmLtePdcpId));
      SPutSBuf(pst->region, pst->pool, (Data *)staInd, sizeof(PjuStaIndInfo));
      SPutMsg(buff);
      RETVALUE(RFAILED);
   }
   if (pst->selector == PJU_SEL_LC) {
      if ( buff != NULLP )
      {
         if (SFndLenMsg(buff, &msgLen) != ROK) {
#if (ERRCLASS & ERRCLS_ADD_RES)
            SLogError(pst->srcEnt, pst->srcInst, pst->srcProcId,
               __FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
               (ErrVal)EPJU054, (ErrVal)0, "Packing failed");
#endif
            SPutSBuf(pst->region, pst->pool, (Data *)pdcpId, sizeof(CmLtePdcpId));
            SPutSBuf(pst->region, pst->pool, (Data *)staInd, sizeof(PjuStaIndInfo));
            SPutMsg(buff);
            RETVALUE(RFAILED);
         }
         if (SCatMsg(mBuf, buff, M1M2) != ROK) {
#if (ERRCLASS & ERRCLS_ADD_RES)
            SLogError(pst->srcEnt, pst->srcInst, pst->srcProcId,
               __FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
               (ErrVal)EPJU055, (ErrVal)0, "Packing failed");
#endif
            SPutSBuf(pst->region, pst->pool, (Data *)pdcpId, sizeof(CmLtePdcpId));
            SPutSBuf(pst->region, pst->pool, (Data *)staInd, sizeof(PjuStaIndInfo));
            SPutMsg(buff);
            RETVALUE(RFAILED);
         }
         SPutMsg(buff);
      }
      else
      {
         msgLen = 0;
      }
      CMCHKPK(SPkS16, msgLen, mBuf);
   }
   if (pst->selector == PJU_SEL_LC) {
      if (cmPkPjuStaIndInfo(staInd, mBuf) != ROK) {
#if (ERRCLASS & ERRCLS_ADD_RES)
         SLogError(pst->srcEnt, pst->srcInst, pst->srcProcId,
            __FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
            (ErrVal)EPJU054, (ErrVal)0, "Packing failed");
#endif
         SPutSBuf(pst->region, pst->pool, (Data *)pdcpId, sizeof(CmLtePdcpId));
         SPutSBuf(pst->region, pst->pool, (Data *)staInd, sizeof(PjuStaIndInfo));
         SPutMsg(mBuf);
         RETVALUE(RFAILED);
      }
   }

   if (pst->selector == PJU_SEL_LC) {
      if (cmPkLtePdcpId(pdcpId, mBuf) != ROK) {
#if (ERRCLASS & ERRCLS_ADD_RES)
         SLogError(pst->srcEnt, pst->srcInst, pst->srcProcId,
            __FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
            (ErrVal)EPJU056, (ErrVal)0, "Packing failed");
#endif
         SPutSBuf(pst->region, pst->pool, (Data *)pdcpId, sizeof(CmLtePdcpId));
         SPutSBuf(pst->region, pst->pool, (Data *)staInd, sizeof(PjuStaIndInfo));
         SPutMsg(mBuf);
         RETVALUE(RFAILED);
      }
   }

   if (SPkS16(suId, mBuf) != ROK) {
#if (ERRCLASS & ERRCLS_ADD_RES)
      SLogError(pst->srcEnt, pst->srcInst, pst->srcProcId,
         __FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
         (ErrVal)EPJU058, (ErrVal)0, "Packing failed");
#endif
      SPutSBuf(pst->region, pst->pool, (Data *)pdcpId, sizeof(CmLtePdcpId));
      SPutSBuf(pst->region, pst->pool, (Data *)staInd, sizeof(PjuStaIndInfo));
      SPutMsg(mBuf);
      RETVALUE(RFAILED);
   }

   /* Free Indication Structure after packing */
   SPutSBuf(pst->region, pst->pool, (Data *)pdcpId, sizeof(CmLtePdcpId));
   SPutSBuf(pst->region, pst->pool, (Data *)staInd, sizeof(PjuStaIndInfo));

   pst->event = (Event) EVTPJUSTAIND;
   RETVALUE(SPstTsk(pst,mBuf));
}

#ifdef ANSI
PUBLIC S16 cmUnpkPjuStaInd
(
PjuStaInd func,
Pst *pst,
Buffer *mBuf
)
#else
PUBLIC S16 cmUnpkPjuStaInd(func, pst, mBuf)
PjuStaInd func;
Pst *pst;
Buffer *mBuf;
#endif
{
   S16   ret1;
   SuId suId;
   CmLtePdcpId *pdcpId = NULLP;
   PjuStaIndInfo *staInd = NULLP;
   Buffer *buff = NULLP;
   
   TRC3(cmUnpkPjuStaInd)

   if (SUnpkS16(&suId, mBuf) != ROK) {
      SPutMsg(mBuf);
#if (ERRCLASS & ERRCLS_ADD_RES)
      SLogError(pst->srcEnt, pst->srcInst, pst->srcProcId,
         __FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
         (ErrVal)EPJU061, (ErrVal)0, "Unpacking failed");
#endif
      RETVALUE(RFAILED);
   }

   if (pst->selector == PJU_SEL_LC) 
   {
      if((ret1 = SGetSBuf(pst->region, pst->pool, (Data **)&pdcpId,\
                  sizeof(CmLtePdcpId))) != ROK)
      {
#if (ERRCLASS & ERRCLS_ADD_RES)
         SLogError(pst->srcEnt, pst->srcInst, pst->srcProcId,
               __FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
               (ErrVal)EPJU040, (ErrVal)0, "SGetMsg() failed");
#endif /*  ERRCLASS & ERRCLS_ADD_RES  */
         RETVALUE(ret1);
      }

      if (cmUnpkLtePdcpId(pdcpId, mBuf) != ROK)
      {
         SPutSBuf(pst->region, pst->pool, (Data *)pdcpId, sizeof(CmLtePdcpId));
         SPutMsg(mBuf);
#if (ERRCLASS & ERRCLS_ADD_RES)
         SLogError(pst->srcEnt, pst->srcInst, pst->srcProcId,
               __FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
               (ErrVal)EPJU063, (ErrVal)0, "Unpacking failed");
#endif
         RETVALUE(RFAILED);
      }
   }

   if (pst->selector == PJU_SEL_LC) 
   {
      if((ret1 = SGetSBuf(pst->region, pst->pool, (Data **)&staInd,\
                  sizeof(PjuStaIndInfo))) != ROK)
      {
#if (ERRCLASS & ERRCLS_ADD_RES)
         SLogError(pst->srcEnt, pst->srcInst, pst->srcProcId,
               __FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
               (ErrVal)EPJU040, (ErrVal)0, "SGetMsg() failed");
#endif /*  ERRCLASS & ERRCLS_ADD_RES  */
         RETVALUE(ret1);
      }

      if (cmUnpkPjuStaIndInfo(staInd, mBuf) != ROK)
      {
         SPutSBuf(pst->region, pst->pool, (Data *)pdcpId, sizeof(CmLtePdcpId));
         SPutSBuf(pst->region, pst->pool, (Data *)staInd, sizeof(PjuStaIndInfo));
         SPutMsg(mBuf);
#if (ERRCLASS & ERRCLS_ADD_RES)
         SLogError(pst->srcEnt, pst->srcInst, pst->srcProcId,
               __FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
               (ErrVal)EPJU065, (ErrVal)0, "Unpacking failed");
#endif
         RETVALUE(RFAILED);
      }
   }
   if (pst->selector == PJU_SEL_LC) {
      S16 msgLen, totalMsgLen;
      CMCHKUNPK(SUnpkS16, &msgLen, mBuf);
      if ( msgLen != 0 )
      {
         if (SFndLenMsg(mBuf, &totalMsgLen) != ROK)
            RETVALUE(RFAILED);
         /* pju_c_001.main_2 */
         if (SSegMsg(mBuf, (S16)(totalMsgLen-msgLen), &buff) != ROK)
            RETVALUE(RFAILED);
      }
   }

   SPutMsg(mBuf);
   RETVALUE((*func)(pst, suId, pdcpId, staInd, buff));
}

#ifdef ANSI
PUBLIC S16 cmPkPjuDatFwdReq
(
Pst* pst,
SpId spId,
CmLtePdcpId * pdcpId,
PjuDatFwdReqInfo * datFwdReq
)
#else
PUBLIC S16 cmPkPjuDatFwdReq(pst, spId, pdcpId, datFwdReq)
Pst* pst;
SpId spId;
CmLtePdcpId * pdcpId;
PjuDatFwdReqInfo * datFwdReq;
#endif
{
   Buffer *mBuf = NULLP;
   TRC3(cmPkPjuDatFwdReq)

   if (SGetMsg(pst->region, pst->pool, &mBuf) != ROK) {
#if (ERRCLASS & ERRCLS_ADD_RES)
      SLogError(pst->srcEnt, pst->srcInst, pst->srcProcId,
         __FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
         (ErrVal)EPJU066, (ErrVal)0, "Packing failed");
#endif
      SPutSBuf(pst->region, pst->pool, (Data *)pdcpId, sizeof(CmLtePdcpId));
      SPutSBuf(pst->region, pst->pool, (Data *)datFwdReq, sizeof(PjuDatFwdReqInfo));
      RETVALUE(RFAILED);
   }
   if (pst->selector == PJU_SEL_LC) {
      if (cmPkPjuDatFwdReqInfo(datFwdReq, mBuf) != ROK) {
#if (ERRCLASS & ERRCLS_ADD_RES)
         SLogError(pst->srcEnt, pst->srcInst, pst->srcProcId,
            __FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
            (ErrVal)EPJU067, (ErrVal)0, "Packing failed");
#endif
         SPutSBuf(pst->region, pst->pool, (Data *)pdcpId, sizeof(CmLtePdcpId));
         SPutSBuf(pst->region, pst->pool, (Data *)datFwdReq, sizeof(PjuDatFwdReqInfo));
         SPutMsg(mBuf);
         RETVALUE(RFAILED);
      }
      SPutSBuf(pst->region, pst->pool, (Data *)datFwdReq->datFwdInfo,
            (datFwdReq->numSdus * sizeof(PjuDatFwdInfo)) );
   }

   if (pst->selector == PJU_SEL_LC) {
      if (cmPkLtePdcpId(pdcpId, mBuf) != ROK) {
#if (ERRCLASS & ERRCLS_ADD_RES)
         SLogError(pst->srcEnt, pst->srcInst, pst->srcProcId,
            __FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
            (ErrVal)EPJU069, (ErrVal)0, "Packing failed");
#endif
         SPutSBuf(pst->region, pst->pool, (Data *)pdcpId, sizeof(CmLtePdcpId));
         SPutSBuf(pst->region, pst->pool, (Data *)datFwdReq, sizeof(PjuDatFwdReqInfo));
         SPutMsg(mBuf);
         RETVALUE(RFAILED);
      }
   }

   if (SPkS16(spId, mBuf) != ROK) {
#if (ERRCLASS & ERRCLS_ADD_RES)
      SLogError(pst->srcEnt, pst->srcInst, pst->srcProcId,
         __FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
         (ErrVal)EPJU071, (ErrVal)0, "Packing failed");
#endif
      SPutSBuf(pst->region, pst->pool, (Data *)pdcpId, sizeof(CmLtePdcpId));
      SPutSBuf(pst->region, pst->pool, (Data *)datFwdReq, sizeof(PjuDatFwdReqInfo));
      SPutMsg(mBuf);
      RETVALUE(RFAILED);
   }

   /* Free Request Structure after packing */
   SPutSBuf(pst->region, pst->pool, (Data *)pdcpId, sizeof(CmLtePdcpId));
   SPutSBuf(pst->region, pst->pool, (Data *)datFwdReq, sizeof(PjuDatFwdReqInfo));

   pst->event = (Event) EVTPJUDATFWDREQ;
   RETVALUE(SPstTsk(pst,mBuf));
}

#ifdef ANSI
PUBLIC S16 cmUnpkPjuDatFwdReq
(
PjuDatFwdReq func,
Pst *pst,
Buffer *mBuf
)
#else
PUBLIC S16 cmUnpkPjuDatFwdReq(func, pst, mBuf)
PjuDatFwdReq func;
Pst *pst;
Buffer *mBuf;
#endif
{
   S16   ret1;
   SpId spId;
   CmLtePdcpId *pdcpId = NULLP;
   PjuDatFwdReqInfo *datFwdReq = NULLP;
   
   TRC3(cmUnpkPjuDatFwdReq)

   if (SUnpkS16(&spId, mBuf) != ROK) {
      SPutMsg(mBuf);
#if (ERRCLASS & ERRCLS_ADD_RES)
      SLogError(pst->srcEnt, pst->srcInst, pst->srcProcId,
         __FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
         (ErrVal)EPJU074, (ErrVal)0, "Unpacking failed");
#endif
      RETVALUE(RFAILED);
   }

   if (pst->selector == PJU_SEL_LC) 
   {
      if((ret1 = SGetSBuf(pst->region, pst->pool, (Data **)&pdcpId,\
                  sizeof(CmLtePdcpId))) != ROK)
      {
#if (ERRCLASS & ERRCLS_ADD_RES)
         SLogError(pst->srcEnt, pst->srcInst, pst->srcProcId,
               __FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
               (ErrVal)EPJU040, (ErrVal)0, "SGetMsg() failed");
#endif /*  ERRCLASS & ERRCLS_ADD_RES  */
         RETVALUE(ret1);
      }

      if (cmUnpkLtePdcpId(pdcpId, mBuf) != ROK)
      {
         SPutSBuf(pst->region, pst->pool, (Data *)pdcpId, sizeof(CmLtePdcpId));
         SPutMsg(mBuf);
#if (ERRCLASS & ERRCLS_ADD_RES)
         SLogError(pst->srcEnt, pst->srcInst, pst->srcProcId,
               __FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
               (ErrVal)EPJU076, (ErrVal)0, "Unpacking failed");
#endif
         RETVALUE(RFAILED);
      }
   }

   if (pst->selector == PJU_SEL_LC) 
   {
      if((ret1 = SGetSBuf(pst->region, pst->pool, (Data **)&datFwdReq,\
                  sizeof(PjuDatFwdReqInfo))) != ROK)
      {
#if (ERRCLASS & ERRCLS_ADD_RES)
         SLogError(pst->srcEnt, pst->srcInst, pst->srcProcId,
               __FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
               (ErrVal)EPJU040, (ErrVal)0, "SGetMsg() failed");
#endif /*  ERRCLASS & ERRCLS_ADD_RES  */
         RETVALUE(ret1);
      }

      if (cmUnpkPjuDatFwdReqInfo(pst, datFwdReq, mBuf) != ROK)
      {
         SPutSBuf(pst->region, pst->pool, (Data *)pdcpId, sizeof(CmLtePdcpId));
         SPutSBuf(pst->region, pst->pool, (Data *)datFwdReq, sizeof(PjuDatFwdReqInfo));
         SPutMsg(mBuf);
#if (ERRCLASS & ERRCLS_ADD_RES)
         SLogError(pst->srcEnt, pst->srcInst, pst->srcProcId,
               __FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
               (ErrVal)EPJU078, (ErrVal)0, "Unpacking failed");
#endif
         RETVALUE(RFAILED);
      }
   }
   SPutMsg(mBuf);
   RETVALUE((*func)(pst, spId, pdcpId, datFwdReq));
}

#ifdef ANSI
PUBLIC S16 cmPkPjuDatFwdInd
(
Pst* pst,
SuId suId,
CmLtePdcpId * pdcpId,
PjuDatFwdIndInfo * datFwdInd
)
#else
PUBLIC S16 cmPkPjuDatFwdInd(pst, suId, pdcpId, datFwdInd)
Pst* pst;
SuId suId;
CmLtePdcpId * pdcpId;
PjuDatFwdIndInfo * datFwdInd;
#endif
{
   Buffer *mBuf = NULLP;
   TRC3(cmPkPjuDatFwdInd)

   if (SGetMsg(pst->region, pst->pool, &mBuf) != ROK) {
#if (ERRCLASS & ERRCLS_ADD_RES)
      SLogError(pst->srcEnt, pst->srcInst, pst->srcProcId,
         __FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
         (ErrVal)EPJU079, (ErrVal)0, "Packing failed");
#endif
      SPutSBuf(pst->region, pst->pool, (Data *)pdcpId, sizeof(CmLtePdcpId));
      SPutSBuf(pst->region, pst->pool, (Data *)datFwdInd, sizeof(PjuDatFwdIndInfo));
      RETVALUE(RFAILED);
   }
   if (pst->selector == PJU_SEL_LC) 
   {
      if (cmPkPjuDatFwdReqInfo(datFwdInd, mBuf) != ROK) {
#if (ERRCLASS & ERRCLS_ADD_RES)
         SLogError(pst->srcEnt, pst->srcInst, pst->srcProcId,
            __FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
            (ErrVal)EPJU080, (ErrVal)0, "Packing failed");
#endif
         SPutSBuf(pst->region, pst->pool, (Data *)pdcpId, sizeof(CmLtePdcpId));
         SPutSBuf(pst->region, pst->pool, (Data *)datFwdInd, sizeof(PjuDatFwdIndInfo));
         SPutMsg(mBuf);
         RETVALUE(RFAILED);
      }
      /* pju_c_001.main_4 if numSdus == 0, then do not free the memory */
      if(datFwdInd->numSdus != 0)
      {
         SPutSBuf(pst->region, pst->pool, (Data *)datFwdInd->datFwdInfo,
                 (datFwdInd->numSdus * sizeof(PjuDatFwdInfo)) );
      }
   }

   if (pst->selector == PJU_SEL_LC) {
      if (cmPkLtePdcpId(pdcpId, mBuf) != ROK) {
#if (ERRCLASS & ERRCLS_ADD_RES)
         SLogError(pst->srcEnt, pst->srcInst, pst->srcProcId,
            __FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
            (ErrVal)EPJU082, (ErrVal)0, "Packing failed");
#endif
         SPutSBuf(pst->region, pst->pool, (Data *)pdcpId, sizeof(CmLtePdcpId));
         SPutSBuf(pst->region, pst->pool, (Data *)datFwdInd, sizeof(PjuDatFwdIndInfo));
         SPutMsg(mBuf);
         RETVALUE(RFAILED);
      }
   }

   if (SPkS16(suId, mBuf) != ROK) {
#if (ERRCLASS & ERRCLS_ADD_RES)
      SLogError(pst->srcEnt, pst->srcInst, pst->srcProcId,
         __FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
         (ErrVal)EPJU084, (ErrVal)0, "Packing failed");
#endif
      SPutSBuf(pst->region, pst->pool, (Data *)pdcpId, sizeof(CmLtePdcpId));
      SPutSBuf(pst->region, pst->pool, (Data *)datFwdInd, sizeof(PjuDatFwdIndInfo));
      SPutMsg(mBuf);
      RETVALUE(RFAILED);
   }

   /* Free Confirm Structure after packing */
   SPutSBuf(pst->region, pst->pool, (Data *)pdcpId, sizeof(CmLtePdcpId));
   SPutSBuf(pst->region, pst->pool, (Data *)datFwdInd, sizeof(PjuDatFwdIndInfo));

   pst->event = (Event) EVTPJUDATFWDIND;
   RETVALUE(SPstTsk(pst,mBuf));
}

#ifdef ANSI
PUBLIC S16 cmUnpkPjuDatFwdInd
(
PjuDatFwdInd func,
Pst *pst,
Buffer *mBuf
)
#else
PUBLIC S16 cmUnpkPjuDatFwdInd(func, pst, mBuf)
PjuDatFwdInd func;
Pst *pst;
Buffer *mBuf;
#endif
{
   S16   ret1;
   SuId suId;
   CmLtePdcpId *pdcpId = NULLP;
   PjuDatFwdIndInfo *datFwdInd = NULLP;
   
   TRC3(cmUnpkPjuDatFwdInd)

   if (SUnpkS16(&suId, mBuf) != ROK) {
      SPutMsg(mBuf);
#if (ERRCLASS & ERRCLS_ADD_RES)
      SLogError(pst->srcEnt, pst->srcInst, pst->srcProcId,
         __FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
         (ErrVal)EPJU087, (ErrVal)0, "Unpacking failed");
#endif
      RETVALUE(RFAILED);
   }

   if (pst->selector == PJU_SEL_LC) 
   {
      if((ret1 = SGetSBuf(pst->region, pst->pool, (Data **)&pdcpId,\
                  sizeof(CmLtePdcpId))) != ROK)
      {
#if (ERRCLASS & ERRCLS_ADD_RES)
         SLogError(pst->srcEnt, pst->srcInst, pst->srcProcId,
               __FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
               (ErrVal)EPJU040, (ErrVal)0, "SGetMsg() failed");
#endif /*  ERRCLASS & ERRCLS_ADD_RES  */
         RETVALUE(ret1);
      }

      if (cmUnpkLtePdcpId(pdcpId, mBuf) != ROK)
      {
         SPutSBuf(pst->region, pst->pool, (Data *)pdcpId, sizeof(CmLtePdcpId));
         SPutMsg(mBuf);
#if (ERRCLASS & ERRCLS_ADD_RES)
         SLogError(pst->srcEnt, pst->srcInst, pst->srcProcId,
               __FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
               (ErrVal)EPJU089, (ErrVal)0, "Unpacking failed");
#endif
         RETVALUE(RFAILED);
      }
   }

   if (pst->selector == PJU_SEL_LC) 
   {
      if((ret1 = SGetSBuf(pst->region, pst->pool, (Data **)&datFwdInd,\
                  sizeof(PjuDatFwdIndInfo))) != ROK)
      {
#if (ERRCLASS & ERRCLS_ADD_RES)
         SLogError(pst->srcEnt, pst->srcInst, pst->srcProcId,
               __FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
               (ErrVal)EPJU040, (ErrVal)0, "SGetMsg() failed");
#endif /*  ERRCLASS & ERRCLS_ADD_RES  */
         RETVALUE(ret1);
      }

      if (cmUnpkPjuDatFwdReqInfo(pst, datFwdInd, mBuf) != ROK)
      {
         SPutSBuf(pst->region, pst->pool, (Data *)pdcpId, sizeof(CmLtePdcpId));
         SPutSBuf(pst->region, pst->pool, (Data *)datFwdInd, sizeof(PjuDatFwdIndInfo));
         SPutMsg(mBuf);
#if (ERRCLASS & ERRCLS_ADD_RES)
         SLogError(pst->srcEnt, pst->srcInst, pst->srcProcId,
               __FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
               (ErrVal)EPJU091, (ErrVal)0, "Unpacking failed");
#endif
         RETVALUE(RFAILED);
      }
   }
   SPutMsg(mBuf);
   RETVALUE((*func)(pst, suId, pdcpId, datFwdInd));
}


/***********************************************************
*
*     Func : cmPkPjuDatCfmInfoSta
*
*
*     Desc :   Data confirmation status parameters
*
*
*     Ret  : S16
*
*     Notes:
*
*     File  : 
*
**********************************************************/
#ifdef ANSI
PUBLIC S16 cmPkPjuDatCfmInfoSta
(
PjuDatCfmSta *param,
Buffer *mBuf
)
#else
PUBLIC S16 cmPkPjuDatCfmInfoSta(param, mBuf)
PjuDatCfmSta *param;
Buffer *mBuf;
#endif
{

   TRC3(cmPkPjuDatCfmInfoSta)

   CMCHKPK(SPkU8, param->status, mBuf);
   CMCHKPK(SPkU32, param->sduId, mBuf);
   RETVALUE(ROK);
}



/***********************************************************
*
*     Func : cmUnpkPjuDatCfmInfoSta
*
*
*     Desc :   Data confirmation status parameters
*
*
*     Ret  : S16
*
*     Notes:
*
*     File  : 
*
**********************************************************/
#ifdef ANSI
PUBLIC S16 cmUnpkPjuDatCfmInfoSta
(
PjuDatCfmSta *param,
Buffer *mBuf
)
#else
PUBLIC S16 cmUnpkPjuDatCfmInfoSta(param, mBuf)
PjuDatCfmSta *param;
Buffer *mBuf;
#endif
{

   TRC3(cmUnpkPjuDatCfmInfoSta)

   CMCHKUNPK(SUnpkU32, &param->sduId, mBuf);
   CMCHKUNPK(SUnpkU8, &param->status, mBuf);
   RETVALUE(ROK);
}



/***********************************************************
*
*     Func : cmPkPjuDatCfmInfo
*
*
*     Desc :   Data confirmation parameters
*
*
*     Ret  : S16
*
*     Notes:
*
*     File  : 
*
**********************************************************/
#ifdef ANSI
PUBLIC S16 cmPkPjuDatCfmInfo
(
PjuDatCfmInfo *param,
Buffer *mBuf
)
#else
PUBLIC S16 cmPkPjuDatCfmInfo(param, mBuf)
PjuDatCfmInfo *param;
Buffer *mBuf;
#endif
{

   S32 i;
   TRC3(cmPkPjuDatCfmInfo)

   for (i=param->numSdus-1; i >= 0; i--) {
      CMCHKPK(cmPkPjuDatCfmInfoSta, &param->cfmSta[i], mBuf);
   }
   CMCHKPK(SPkU16, param->numSdus, mBuf);
   RETVALUE(ROK);
}



/***********************************************************
*
*     Func : cmUnpkPjuDatCfmInfo
*
*
*     Desc :   Data confirmation parameters
*
*
*     Ret  : S16
*
*     Notes:
*
*     File  : 
*
**********************************************************/
#ifdef ANSI
PUBLIC S16 cmUnpkPjuDatCfmInfo
(
PjuDatCfmInfo *param,
Buffer *mBuf
)
#else
PUBLIC S16 cmUnpkPjuDatCfmInfo(param, mBuf)
PjuDatCfmInfo *param;
Buffer *mBuf;
#endif
{

   S32 i;
   TRC3(cmUnpkPjuDatCfmInfo)

   CMCHKUNPK(SUnpkU16, &param->numSdus, mBuf);
   for (i=0; i<param->numSdus; i++) {
      CMCHKUNPK(cmUnpkPjuDatCfmInfoSta, &param->cfmSta[i], mBuf);
   }
   RETVALUE(ROK);
}



/***********************************************************
*
*     Func : cmPkPjuStaIndInfo
*
*
*     Desc :   Status indication parameters
*
*
*     Ret  : S16
*
*     Notes:
*
*     File  : 
*
**********************************************************/
#ifdef ANSI
PUBLIC S16 cmPkPjuStaIndInfo
(
PjuStaIndInfo *param,
Buffer *mBuf
)
#else
PUBLIC S16 cmPkPjuStaIndInfo(param, mBuf)
PjuStaIndInfo *param;
Buffer *mBuf;
#endif
{

   TRC3(cmPkPjuStaIndInfo)

   CMCHKPK(SPkU8, param->cause, mBuf);
   RETVALUE(ROK);
}



/***********************************************************
*
*     Func : cmUnpkPjuStaIndInfo
*
*
*     Desc :   Status indication parameters
*
*
*     Ret  : S16
*
*     Notes:
*
*     File  : 
*
**********************************************************/
#ifdef ANSI
PUBLIC S16 cmUnpkPjuStaIndInfo
(
PjuStaIndInfo *param,
Buffer *mBuf
)
#else
PUBLIC S16 cmUnpkPjuStaIndInfo(param, mBuf)
PjuStaIndInfo *param;
Buffer *mBuf;
#endif
{

   TRC3(cmUnpkPjuStaIndInfo)

   CMCHKUNPK(SUnpkU8, &param->cause, mBuf);
   RETVALUE(ROK);
}



/***********************************************************
*
*     Func : cmPkPjuDatFwdInfo
*
*
*     Desc :   Data Forward Information
*
*
*     Ret  : S16
*
*     Notes:
*
*     File  : 
*
**********************************************************/
#ifdef ANSI
PUBLIC S16 cmPkPjuDatFwdInfo
(
PjuDatFwdInfo *param,
Buffer *mBuf
)
#else
PUBLIC S16 cmPkPjuDatFwdInfo(param, mBuf)
PjuDatFwdInfo *param;
Buffer *mBuf;
#endif
{

   S16 msgLen;
   TRC3(cmPkPjuDatFwdInfo)

   if (SFndLenMsg(param->sdu, &msgLen) != ROK)
      RETVALUE(RFAILED);
   if (SCatMsg(mBuf, param->sdu, M1M2) != ROK)
      RETVALUE(RFAILED);
   SPutMsg(param->sdu);
   CMCHKPK(SPkS16, msgLen, mBuf);

   CMCHKPK(SPkU16, param->sn, mBuf);
   CMCHKPK(SPkU32, param->sduId, mBuf);
   RETVALUE(ROK);
}



/***********************************************************
*
*     Func : cmUnpkPjuDatFwdInfo
*
*
*     Desc :   Data Forward Information
*
*
*     Ret  : S16
*
*     Notes:
*
*     File  : 
*
**********************************************************/
#ifdef ANSI
PUBLIC S16 cmUnpkPjuDatFwdInfo
(
PjuDatFwdInfo *param,
Buffer *mBuf
)
#else
PUBLIC S16 cmUnpkPjuDatFwdInfo(param, mBuf)
PjuDatFwdInfo *param;
Buffer *mBuf;
#endif
{

   S16 msgLen, totalMsgLen;
   TRC3(cmUnpkPjuDatFwdInfo)

   CMCHKUNPK(SUnpkU32, &param->sduId, mBuf);
   CMCHKUNPK(SUnpkU16, &param->sn, mBuf);
   CMCHKUNPK(SUnpkS16, &msgLen, mBuf);
   if (SFndLenMsg(mBuf, &totalMsgLen) != ROK)
      RETVALUE(RFAILED);
   /* pju_c_001.main_2 */
   if (SSegMsg(mBuf, (S16)(totalMsgLen-msgLen), &param->sdu) != ROK)
      RETVALUE(RFAILED);
   RETVALUE(ROK);
}



/***********************************************************
*
*     Func : cmPkPjuDatFwdReqInfo
*
*
*     Desc :   Data Forward Request parameters for PDCP 
            entity mapped to RLC AM
*
*
*     Ret  : S16
*
*     Notes:
*
*     File  : 
*
**********************************************************/
#ifdef ANSI
PUBLIC S16 cmPkPjuDatFwdReqInfo
(
PjuDatFwdReqInfo *param,
Buffer *mBuf
)
#else
PUBLIC S16 cmPkPjuDatFwdReqInfo(param, mBuf)
PjuDatFwdReqInfo *param;
Buffer *mBuf;
#endif
{
   S32  numSdus = 0;

   TRC3(cmPkPjuDatFwdReqInfo)

   while ( numSdus < param->numSdus )
   {
      CMCHKPK(cmPkPjuDatFwdInfo, &(param->datFwdInfo[numSdus]), mBuf);
      numSdus++;
   }
   CMCHKPK(SPkU16, param->numSdus, mBuf);
   CMCHKPK(SPkU8, param->dir, mBuf);
   RETVALUE(ROK);
}



/***********************************************************
*
*     Func : cmUnpkPjuDatFwdReqInfo
*
*
*     Desc :   Data Forward Request parameters for PDCP 
            entity mapped to RLC AM
*
*
*     Ret  : S16
*
*     Notes:
*
*     File  : 
*
**********************************************************/
#ifdef ANSI
PUBLIC S16 cmUnpkPjuDatFwdReqInfo
(
Pst *pst,
PjuDatFwdReqInfo *param,
Buffer *mBuf
)
#else
PUBLIC S16 cmUnpkPjuDatFwdReqInfo(pst,param, mBuf)
Pst *pst;
PjuDatFwdReqInfo *param;
Buffer *mBuf;
#endif
{
   S16   ret1;
   S32  numSdus = 0;

   TRC3(cmUnpkPjuDatFwdReqInfo)

   CMCHKUNPK(SUnpkU8, &param->dir, mBuf);
   CMCHKUNPK(SUnpkU16, &param->numSdus, mBuf);
   /* pju_c_001.main_4 if numSdus == 0, then return ROK */
   if(param->numSdus == 0)
   {
      RETVALUE(ROK);
   }
   if((ret1 = SGetSBuf(pst->region, pst->pool, (Data **)&param->datFwdInfo,\
               (sizeof(PjuDatFwdInfo)* param->numSdus ))) != ROK)
   {
#if (ERRCLASS & ERRCLS_ADD_RES)
         SLogError(pst->srcEnt, pst->srcInst, pst->srcProcId,
               __FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
               (ErrVal)EPJU040, (ErrVal)0, "SGetMsg() failed");
#endif /*  ERRCLASS & ERRCLS_ADD_RES  */
      RETVALUE(ret1);
   }
   numSdus = param->numSdus - 1;
    while ( numSdus >= 0 )
   {
      CMCHKUNPK(cmUnpkPjuDatFwdInfo, &(param->datFwdInfo[numSdus]), mBuf);
      numSdus--;
   }

   RETVALUE(ROK);
}

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* (defined(LCPJU) || defined(LWLCPJU)) */


/********************************************************************30**
  
         End of file:     pju.c@@/main/5 - Fri Sep  2 19:27:53 2011
  
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
/main/1      ---      dm   1. LTE RRC Initial Release.
/main/2      ---      gk   1. LTE RLC PDCP Release 2.1
/main/3      ---      pju_c_001.main_2  nm   1. Typecast arg with S16 for SSegMsg
/main/4      ---      pju_c_001.main_3  sv   1. Fixed memory leaks 
                                                in error cases.
/main/4      pju_c_001.main_4 ap  1. Added support for sending Data Fwd indication 
                                     even numSdu is zero                                                
/main/5      pju_c_001.main_5 ap  1. ccpu00120534, fixed the problem of passing 
                                     suId and spId as argument.
*********************************************************************91*/
