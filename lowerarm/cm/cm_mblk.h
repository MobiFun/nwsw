/********************************************************************16**

                         (c) COPYRIGHT 2006 by 
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
                    9380, Carroll Park Drive
                    San Diego, CA-92121, USA

                    Tel: +1 (858) 882 8800
                    Fax: +1 (858) 777 3388

                    Email: support@trillium.com
                    Web: http://www.ccpu.com

*********************************************************************17*/

/********************************************************************20**
  
     Name:     common memory allocation library 
  
     Type:     C include file
  
     Desc:     Hash Defines 
 
     File:     cm_mblk.h
  
     Sid:      cm_mblk.h@@/main/5 - Mon Nov 17 15:55:41 2008
    
     Prg:      mp
 
*********************************************************************21*/
  
#ifndef __CMMBLKH__
#define __CMMBLKH__

/* Hash defines for alignment bytes */
#define CM_ALIGNBOUNDARY_FOUR     4   /* Align 32 bit */ 
#define CM_ALIGNBOUNDARY_EIGHT    8   /* Align 64 bit */

/* Macro Definitions */

#define CM_INIT_MEMCP(_memCp,_maxSize,_sMem) \
{ \
  _memCp->first = NULLP; \
  _memCp->last = NULLP; \
  _memCp->count = 0;\
  _memCp->memCb.maxSize = _maxSize;\
  _memCp->memCb.memAllocated = 0; \
  _memCp->memCb.initPtr = NULLP; \
  _memCp->memCb.runPtr = NULLP; \
  _memCp->memCb.sMem.region = _sMem->region; \
  _memCp->memCb.sMem.pool = _sMem->pool; \
}

#define CM_INIT_MEMCPVAR(_memCp) \
{ \
  _memCp->first = NULLP; \
  _memCp->last = NULLP; \
  _memCp->count = 0;\
  _memCp->memCb.memAllocated = 0; \
  _memCp->memCb.initPtr = NULLP; \
  _memCp->memCb.runPtr = NULLP; \
}

#ifdef ALIGN_64BIT
#define CM_ALIGN_SIZE(_size) \
{ \
  if( (_size % CM_ALIGNBOUNDARY_EIGHT) ) \
     _size = _size + ( CM_ALIGNBOUNDARY_EIGHT -  \
                  (_size % CM_ALIGNBOUNDARY_EIGHT)); \
}
#else
#define CM_ALIGN_SIZE(_size) \
{ \
  if( (_size % CM_ALIGNBOUNDARY_FOUR) ) \
    _size = _size + ( CM_ALIGNBOUNDARY_FOUR -  \
                  (_size % CM_ALIGNBOUNDARY_FOUR)); \
}
#endif  /* ALIGN_64BIT */

#define CMCHKUNPKPTR(func, val, ptr, mBuf) \
   { \
      S16 ret; \
      if ((ret = func(val, ptr, mBuf)) != ROK) \
         RETVALUE(ret); \
   }

#define CMGETMBLK(ptr, size, pptr) \
   { \
      S16 ret; \
      ret = cmGetMem( ptr, size, pptr); \
      if (ret != ROK) \
      { \
          RETVALUE(RFAILED); \
      } \
   }

#endif /* __CMMBLKH__ */

 
/********************************************************************30**
  
         End of file:     cm_mblk.h@@/main/5 - Mon Nov 17 15:55:41 2008
  
*********************************************************************31*/


/********************************************************************40**
  
        Notes:
  
*********************************************************************41*/

/********************************************************************50**

*********************************************************************51*/


/********************************************************************60**
  
        Revision history:
  
*********************************************************************61*/
  
/********************************************************************80**

  version    pat  init                   description
----------- ----- ----  ------------------------------------------------

*********************************************************************81*/
/********************************************************************90**
 
     ver       pat    init                  description
------------ -------- ---- ----------------------------------------------
1.1          ---      mp   1. initial release.
/main/2      ---      mg   1. Added Pointer pack macros.
/main/3      ---       mj   1. MPLS release 1.2 changes. 
/main/4      ---      rbabu 1. Updated copyright information
/main/5      ---      ch   1. Updated copyright SSI Multicore Support release.
*********************************************************************91*/
