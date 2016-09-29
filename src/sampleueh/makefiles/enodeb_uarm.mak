#/********************************************************************16**
#
#                         (c) COPYRIGHT 2009 by 
#                         Continuous Computing Corporation.
#                         All rights reserved.
#
#     This software is confidential and proprietary to Continuous Computing 
#     Corporation (CCPU).  No part of this software may be reproduced,
#     stored, transmitted, disclosed or used in any form or by any means
#     other than as expressly provided by the written Software License 
#     Agreement between CCPU and its licensee.
#
#     CCPU warrants that for a period, as provided by the written
#     Software License Agreement between CCPU and its licensee, this
#     software will perform substantially to CCPU specifications as
#     published at the time of shipment, exclusive of any updates or 
#     upgrades, and the media used for delivery of this software will be 
#     free from defects in materials and workmanship.  CCPU also warrants 
#     that has the corporate authority to enter into and perform under the   
#     Software License Agreement and it is the copyright owner of the software 
#     as originally delivered to its licensee.
#
#     CCPU MAKES NO OTHER WARRANTIES, EXPRESS OR IMPLIED, INCLUDING
#     WITHOUT LIMITATION WARRANTIES OF MERCHANTABILITY OR FITNESS FOR
#     A PARTICULAR PURPOSE WITH REGARD TO THIS SOFTWARE, SERVICE OR ANY RELATED
#     MATERIALS.
#
#     IN NO EVENT SHALL CCPU BE LIABLE FOR ANY INDIRECT, SPECIAL,
#     CONSEQUENTIAL DAMAGES, OR PUNITIVE DAMAGES IN CONNECTION WITH OR ARISING
#     OUT OF THE USE OF, OR INABILITY TO USE, THIS SOFTWARE, WHETHER BASED
#     ON BREACH OF CONTRACT, TORT (INCLUDING NEGLIGENCE), PRODUCT
#     LIABILITY, OR OTHERWISE, AND WHETHER OR NOT IT HAS BEEN ADVISED
#     OF THE POSSIBILITY OF SUCH DAMAGE.
#
#                       Restricted Rights Legend
#
#     This software and all related materials licensed hereby are
#     classified as "restricted computer software" as defined in clause
#     52.227-19 of the Federal Acquisition Regulation ("FAR") and were
#     developed entirely at private expense for nongovernmental purposes,
#     are commercial in nature and have been regularly used for
#     nongovernmental purposes, and, to the extent not published and
#     copyrighted, are trade secrets and confidential and are provided
#     with all rights reserved under the copyright laws of the United
#     States.  The government's rights to the software and related
#     materials are limited and restricted as provided in clause
#     52.227-19 of the FAR.
#
#                    IMPORTANT LIMITATION(S) ON USE
#
#     The use of this software is limited to the use set
#     forth in the written Software License Agreement between CCPU and
#     its Licensee. Among other things, the use of this software
#     may be limited to a particular type of Designated Equipment, as 
#     defined in such Software License Agreement.
#     Before any installation, use or transfer of this software, please
#     consult the written Software License Agreement or contact CCPU at
#     the location set forth below in order to confirm that you are
#     engaging in a permissible use of the software.
#
#                    Continuous Computing Corporation
#                    9380, Carroll Park Drive
#                    San Diego, CA-92121, USA
#
#                    Tel: +1 (858) 882 8800
#                    Fax: +1 (858) 777 3388
#
#                    Email: support@trillium.com
#                    Web: http://www.ccpu.com
#
#*********************************************************************17*/
#
#********************************************************************20**
#
#       Name:   LTE eNodeB Sample Application
#
#       Type:   make file
#
#       Desc:   Compile, assemble and link product software for
#               various configurations. Further comments are
#               embedded in the file. 
#
#               This file supports a variety of environments and
#               build targets. The default build target will be the
#               portable target for the Linu 2.4.x with GNU C(gcc)
#
#       Env:    Linux 2.4.x with GNU C (gcc)
#
#               BUILD TARGETS:
#
#               clean            remove all object files
#   
#       File:  stack.mak
#
#
#       Prg:   an
#
#********************************************************************21*/
#=======================================================================

#-----------------------------------------------------------------------
# macro for output file name and makefile name
#
ifndef OCTEON_ROOT
OCTEON_ROOT=/usr/local/Cavium_Networks/1.9.0/OCTEON-SDK
endif
ifndef MATCHED_MODEL
MATCHED_MODEL=OCTEON_CN56XX
endif
ROOT=enodeb
ROOT1=enb_se
ROOT2=enb_seum
SY_DIR=./obj
SY_DIR1=./obj1
SY_DIR2=./obj2

SY_LIB_DIR=.
STOPTS=-DENB 

OUTNAME=$(ROOT)
OUTNAME1=$(ROOT1)
OUTNAME2=$(ROOT2)

#-----------------------------------------------------------------------
# macros for td specific options
#

# MTSS defines
MTOPTS=-DSS -DSS_MT -DCONRD -UNOFILESYS
MTOPTS2=-DSS -DSS_MT -DCONRD

#cavium includes
OBJ_DIR=./obj1
ifeq (${BLDENV}, cav_seum_perf)
OBJ_DIR=./obj2
endif
ifeq (${BLDENV}, cav_seum_e2e)
OBJ_DIR=./obj2
endif
CVMX_CONFIG_DIR=../cavsdk/config
CVMX_INC=-I$(OCTEON_ROOT)/target/include -I$(CVMX_CONFIG_DIR)
CFLAGS_GLOBAL += $(CVMX_INC)
CFLAGS_GLOBAL += $(OCTEON_CPPFLAGS_GLOBAL_ADD)

#cavium defines
#  application config check and rules

CVMX_CONFIG = ../cavsdk/config/cvmx-config.h
CVMX_OTHER_CONFIGS := ../cavsdk/config/*-config.h
CVMX_OTHER_CONFIGS := $(shell echo $(CVMX_OTHER_CONFIGS) | sed 's/config\/cvmx-config.h//')

$(CVMX_CONFIG): $(CVMX_OTHER_CONFIGS)
	cvmx-config $(CVMX_OTHER_CONFIGS)

#  special rule to re-compile if important environment variables change
MATCH=${findstring DUSE_RUNTIME_MODEL_CHECKS=1, ${OCTEON_CPPFLAGS_GLOBAL_ADD}}
ifeq (${MATCH}, DUSE_RUNTIME_MODEL_CHECKS=1)
#  We are using runtime model detection, so use "runtime" as model to avoid
#    a re-compile if only OCTEON_MODEL used for simulation changes
MADE_WITH_OCTEON_MODEL = "runtime"
else
MADE_WITH_OCTEON_MODEL = $(OCTEON_MODEL)
endif
#  set special filename for target and change any spaces in it to commas
MADE_WITH = $(shell echo "$(OBJ_DIR)/made_with-OCTEON_MODEL=$(MADE_WITH_OCTEON_MODEL)=-OCTEON_CPPFLAGS_GLOBAL_ADD=$(OCTEON_CPPFLAGS_GLOBAL_ADD)=." | sed 's/\ /,/g')

# Add profile-feedback flags.
ifdef FDO_PASS
ifeq ($(FDO_PASS), 1)
CFLAGS_GLOBAL += -fprofile-generate
LDFLAGS_GLOBAL += -fprofile-generate
else # 1
ifeq ($(FDO_PASS), 2)
CFLAGS_GLOBAL += -fprofile-use
LDFLAGS_GLOBAL += -fprofile-use
else # 2
$(error FDO_PASS should either be 1 or 2)
endif # 2
endif # 1
endif # FDO_PASS

ifdef OCTEON_DISABLE_BACKTRACE
CFLAGS_GLOBAL += -fno-asynchronous-unwind-tables -DOCTEON_DISABLE_BACKTRACE
endif

ifndef OCTEON_TARGET
ifeq (${BLDENV}, cav_se_perf)
  OCTEON_TARGET=cvmx_64
else
ifeq (${BLDENV},cav_se_e2e)
  OCTEON_TARGET=cvmx_64
else
  OCTEON_TARGET=linux_64
endif
endif
endif

SUPPORTED_TARGETS=linux_64 linux_n32 linux_uclibc linux_o32 cvmx_n32 cvmx_64
ifeq ($(findstring $(OCTEON_TARGET), $(SUPPORTED_TARGETS)),)
    ${error Invalid value for OCTEON_TARGET. Supported values: ${SUPPORTED_TARGETS}}
endif

ifeq (${OCTEON_TARGET},linux_64)
    PREFIX=-linux_64
    CFLAGS_GLOBAL += -DOCTEON_TARGET=${OCTEON_TARGET} -mabi=64 -march=octeon -msoft-float -Dmain=appmain
    ASFLAGS_GLOBAL += -DOCTEON_TARGET=${OCTEON_TARGET} -mabi=64 -march=octeon -msoft-float -Dmain=appmain
    LDFLAGS_GLOBAL += -mabi=64 -static -lrt -Xlinker -T -Xlinker $(OCTEON_ROOT)/target/lib/cvmx-shared-linux.ld
    #LDFLAGS_GLOBAL += -Xlinker $(OBJ_DIR)/libcvmx.a

else # linux_64
ifeq (${OCTEON_TARGET},linux_n32)
    PREFIX=-linux_n32
    CFLAGS_GLOBAL += -DOCTEON_TARGET=${OCTEON_TARGET} -mabi=n32 -march=octeon -msoft-float -Dmain=appmain
    ASFLAGS_GLOBAL += -DOCTEON_TARGET=${OCTEON_TARGET} -mabi=n32 -march=octeon -msoft-float -Dmain=appmain
    LDFLAGS_GLOBAL += -mabi=n32 -static -lrt -Xlinker -T -Xlinker $(OCTEON_ROOT)/target/lib/cvmx-shared-linux-n32.ld
else # linux_n32
ifeq (${OCTEON_TARGET},linux_uclibc)
    PREFIX=-linux_uclibc
    CFLAGS_GLOBAL += -DOCTEON_TARGET=${OCTEON_TARGET} -muclibc -march=octeon -msoft-float -Dmain=appmain
    ASFLAGS_GLOBAL += -DOCTEON_TARGET=${OCTEON_TARGET} -muclibc -march=octeon -msoft-float -Dmain=appmain
    LDFLAGS_GLOBAL += -muclibc -static -lrt -Xlinker -T -Xlinker $(OCTEON_ROOT)/target/lib/cvmx-shared-linux-n32.ld
else # linux_uclibc
ifeq (${OCTEON_TARGET},linux_o32)
    PREFIX=-linux_o32
    CFLAGS_GLOBAL += -DOCTEON_TARGET=${OCTEON_TARGET} -mabi=32 -march=octeon -msoft-float -Dmain=appmain
    ASFLAGS_GLOBAL += -DOCTEON_TARGET=${OCTEON_TARGET} -mabi=32 -march=octeon -msoft-float -Dmain=appmain
    LDFLAGS_GLOBAL += -mabi=32 -static -lrt -Xlinker -T -Xlinker $(OCTEON_ROOT)/target/lib/cvmx-shared-linux-o32.ld
else # linux_o32
ifeq (${OCTEON_TARGET},cvmx_n32)
    CFLAGS_GLOBAL += -DOCTEON_TARGET=cvmx_n32 -mabi=n32
    ASFLAGS_GLOBAL += -DOCTEON_TARGET=cvmx_n32 -mabi=n32
    LDFLAGS_GLOBAL += -mabi=n32
    PREFIX=-cvmx_n32
else # cvmx_n32
ifeq (${OCTEON_TARGET},cvmx_64)
    CFLAGS_GLOBAL += -DOCTEON_TARGET=cvmx_64
    ASFLAGS_GLOBAL += -DOCTEON_TARGET=cvmx_64
else # cvmx_64
    ${error Invalid value for OCTEON_TARGET. Supported values: ${SUPPORTED_TARGETS}}
endif # cvmx_64
endif # cvmx_n32
endif # linux_o32
endif # linux_uclibc
endif # linux_n32
endif # linux_64

CFLAGS_GLOBAL += -DOCTEON_MODEL=$(MATCHED_MODEL)

CVMXOPTS += $(CFLAGS_GLOBAL)

# MOS defines
MSOPTS=-DMS 

#SSI Files 
SSOPTS=-DSS

CMENBOPTS= -DCMINET_BSDCOMPAT -DSS_TICKS_SEC=1000 -DCMFILE_REORG_1 -DCM_INET2 \
           -DCMFILE_REORG_2 -DSSINT2 -DCMKV2 -DHI_MULTI_THREADED \
          -DSS_PERF -DSS_WL_REGION=1 -DNO_ERRCLS -DNOERRCHK -DxSS_M_PROTO_REGION \
          -DFP_RELEASE6 -DCCPU_DEBUG_TRACE -DCCPU_MEAS_CPU -DDEBUGP \
          -DLTE_ENB_PERF -UVE_PERF_MEAS -DCM_PASN_DBG -DREL_850 \
          -ULTE_START_UL_DL_DATA

# EnodeB Performance Compile Options
CMENBPERFOPTS=-DCMINET_BSDCOMPAT -DSS_TICKS_SEC=1000 -DCMFILE_REORG_1 -DCM_INET2 \
           -DCMFILE_REORG_2 -DSSINT2 -DCMKV2 -DHI_MULTI_THREADED \
          -DSS_PERF -DSS_WL_REGION=1 -DNO_ERRCLS -DNOERRCHK -DxSS_M_PROTO_REGION \
          -DFP_RELEASE6 -DCCPU_DEBUG_TRACE -DCCPU_MEAS_CPU -DDEBUGP \
          -ULTE_ENB_PERF -DVE_PERF_MEAS -DCM_PASN_DBG -DREL_850 \
          -DVE_PERF_DL_DATA -UVE_PERF_UL_DATA -DRGR_RRM_TICK -ULTE_ENB_PAL \
			 -DSLES9_PLUS
    
# EnodeB End to End Compile Options
CMENBE2EOPTS=-DCMINET_BSDCOMPAT -DSS_TICKS_SEC=1000 -DCMFILE_REORG_1 -DCM_INET2 \
          -DCMFILE_REORG_2 -DSSINT2 -DCMKV2 -DHI_MULTI_THREADED \
          -DSS_PERF -DSS_WL_REGION=1 -DNO_ERRCLS -DNOERRCHK -DxSS_M_PROTO_REGION \
          -DFP_RELEASE6 -DCCPU_DEBUG_TRACE -DCCPU_MEAS_CPU -DDEBUGP \
          -ULTE_ENB_PERF -UVE_PERF_MEAS -UCM_PASN_DBG -DREL_850 \
          -ULTE_START_UL_DL_DATA -DSS_FLOAT -DRGR_RRM_TICK -ULTE_ENB_PAL -UVE_WITHOUT_CNE\
          -DSLES9_PLUS -DSS_4GMX_UCORE -DSS_DRVR_SUPPORT -DVE_SIBS_ENBLD -DLTE_CL_BLD_KONTRON -DVE_UM_MODE -DVE_UM_MODE_BI_DIR -DHI_NO_CHK_RES -DMSPD -DLSZV1\
          -DDISABLE_RRCSEC -UVE_DL_CQI

CMENBPERFSEOPTS= -DCMINET_BSDCOMPAT -DSS_TICKS_SEC=100 -DCMFILE_REORG_1 -DCM_INET2 \
           -DCMFILE_REORG_2 -DSSINT2 -DCMKV2 -DHI_MULTI_THREADED \
          -DSS_PERF -DSS_WL_REGION=1 -UNO_ERRCLS -UNOERRCHK -DSS_M_PROTO_REGION \
          -DFP_RELEASE6 -DCCPU_DEBUG_TRACE -DCCPU_MEAS_CPU  -DDEBUG \
          -DLTE_ENB_PERF -DREL_850 -DVE_PERF_DL_DATA -UVE_PERF_UL_DATA -DVE_PERF_MEAS \
          -DLTE_ENB_PAL

CMENBE2ESEOPTS= -DCMINET_BSDCOMPAT -DSS_TICKS_SEC=100 -DCMFILE_REORG_1 -DCM_INET2 \
           -DCMFILE_REORG_2 -DSSINT2 -DCMKV2 -DHI_MULTI_THREADED \
          -DSS_PERF -DSS_WL_REGION=1 -UNO_ERRCLS -UNOERRCHK -DSS_M_PROTO_REGION \
          -DFP_RELEASE6 -DCCPU_DEBUG_TRACE -DCCPU_MEAS_CPU  -DDEBUG \
          -ULTE_ENB_PERF -DREL_850 -DLTE_ENB_PAL -DSS_FLOAT

CMENBPERFSEUMOPTS= -DCMINET_BSDCOMPAT -DSS_TICKS_SEC=100 -DCMFILE_REORG_1 -DCM_INET2 \
           -DCMFILE_REORG_2 -DSSINT2 -DCMKV2 -DHI_MULTI_THREADED \
          -DSS_PERF -DSS_WL_REGION=1 -DNO_ERRCLS -DNOERRCHK -DSS_M_PROTO_REGION\
          -DFP_RELEASE6 -DCCPU_DEBUG_TRACE -DCCPU_MEAS_CPU -DDEBUGP \
          -DLTE_ENB_PERF -DVE_PERF_DL_DATA -UVE_PERF_UL_DATA -DVE_PERF_MEAS -DLTE_ENB_PAL

CMENBE2ESEUMOPTS= -DCMINET_BSDCOMPAT -DSS_TICKS_SEC=100 -DCMFILE_REORG_1 -DCM_INET2 \
           -DCMFILE_REORG_2 -DSSINT2 -DCMKV2 -DHI_MULTI_THREADED \
          -DSS_PERF -DSS_WL_REGION=1 -DNO_ERRCLS -DNOERRCHK -DSS_M_PROTO_REGION\
          -DFP_RELEASE6 -DCCPU_DEBUG_TRACE -DCCPU_MEAS_CPU -DDEBUGP \
          -DLTE_ENB_PERF  -DLTE_ENB_PAL

# The options below are used to pass on to the actual product compilations
# Just this one variable is passed for individual compilations and hence
# all the needed options are added from various other defined option macros

#ALL PAL layer options
TFUOPTS=-DTFU_VER_2 -UTFU_TDD -DLCTFU
TFUOPTS1=-DTFU_VER_2 -UTFU_TDD 
TFUOPTS2=-DTF -DLCRGLITFU -DLCTFU -DLCTFUITFU
LTFOPTS=-DSM -DLCTFMILTF -DLCLTF
TFOPTS=-DTF $(TFUOPTS) $(LTFOPTS) -DDL_STS -UTTI_PROC -URLC_STATUS_GEN \
        -UUL_PROC
TFOPTS1=-DTF $(TFUOPTS1) $(LTFOPTS) -DDL_STS -UTTI_PROC -URLC_STATUS_GEN \
        -UUL_PROC
TFOPTS2=-DTF $(TFUOPTS2) $(LTFOPTS) -DDL_STS -UTTI_PROC -URLC_STATUS_GEN \
        -UUL_PROC


# All LTE MAC product options
CRGOPTS= -DLCNHLICRG -DLCCRG -DLCRGUICRG
CRGOPTS1= -DLCNHLICRG -DLCCRG -DLCRGUICRG -DRG
RGUOPTS=-DRG -DKW -DLCKWLIRGU -DLCRGU -DLCRGUIRGU -DCCPU_OPT
LRGOPTS=-DLCSMMILRG -DLCLRG -DSM -DRG -DLCRGMILRG
RGROPTS=-URG -DLCVELIRGR -DLCRGR -DLWLCVELIRGR -DLCRGUIRGR
RGRSEUMOPTS=-DLCVELIRGR -DLCRGR -DLWLCVELIRGR -DLCRGUIRGR
RGOPTS=-DRG $(RGUOPTS) $(CRGOPTS) $(LRGOPTS) $(RGROPTS) $(TFUOPTS) -URG_DEBUG -UTTI_PROC -UPROC_DL -USCH_TTI_PROC -UTOM -UPAL_MAC_PROC -UUL_PROC_MAC -DLCRGLITFU

RGOPTS1=-DRG $(RGUOPTS) $(CRGOPTS1) $(LRGOPTS) $(RGROPTS) $(TFUOPTS) -URG_DEBUG -UTTI_PROC -UPROC_DL -USCH_TTI_PROC -UTOM -UPAL_MAC_PROC -UUL_PROC_MAC 

# All LTE EGTP-U product options
EGTOPTS=-DEG -DLCEGT -DLCEGUIEGT -DCCPU_OPT -DLCEULIEGT
LEGOPTS=-DLCSMMILEG -DLCLEG -DSM -DEG -DLCEGMILEG -DLCSMEGMILEG
EGOPTS=-DEG $(EGTOPTS) $(LEGOPTS) -UEG_DEBUG -DLCEGLIHIT 


#All Convergence Layer options
LCLOPTS=-DLCLCL -DSM -DCL
CLOPTS=-DCL $(LCLOPTS) -DLCCLMILCL 

#All RLC product options
CKWOPTS=-DLCNHLICKW -DLCCKW -DLCKWUICKW
CKWOPTS1=-DLCNHLICKW -DLCCKW -DLCKWUICKW -DKW
CKWOPTS2=-DLCNHLICKW -DLCCKW -DLCKWUICKW -DKW -DNH
KWUOPTS=-DKW -DNH -DLCKWUIKWU -DLCKWU -DLCNHLIKWU -DCCPU_OPT
LKWOPTS=-DSM -DKW -DLCKWMILKW -DLCLKW
LPJOPTS=-DSM -DPJ -DLCPJMILPJ -DLCLPJ
PJUVEOPTS=-UPJ -DNH -DLCPJU  -DLCVELIPJU -DLCPJUIPJU
PJUVESEUMOPTS=-DNH -DLCPJU  -DLCVELIPJU -DLCPJUIPJU
PJUVEOPTS2=-DPJ -DLCPJU -DVE -DPX -DNX -DNH -DLCVELIPJU -DLCPJUIPJU
PJUNHOPTS=-DPJ  -DLCPJU  -DLCNHLIPJU -DLCPJUIPJU
PJUNHOPTS2=-DPJ -DLCPJU -DVE -DPX -DNX -DNH -DLCNHLIPJU -DLCPJUIPJU
CPJOPTS=-DLCCPJ -DNH -DKW -DLCNHLICPJ -DLCPJUICPJ
KWOPTS=-DKW $(CKWOPTS) $(KWUOPTS) $(CPJOPTS) $(LPJOPTS) $(LKWOPTS) $(RGUOPTS) $(PJUNHOPTS) $(PJUVEOPTS) -URLC_STATUS_GEN -UUL_PROC_MAC
KWOPTS1=-DKW $(CKWOPTS1) $(KWUOPTS) $(CPJOPTS) $(LPJOPTS) $(LKWOPTS) $(RGUOPTS) $(PJUNHOPTS) $(PJUVEOPTS) -URLC_STATUS_GEN -UUL_PROC_MAC
KWOPTS2=-DKW $(CKWOPTS2) $(KWUOPTS) $(CPJOPTS) $(LPJOPTS) $(LKWOPTS) $(RGUOPTS) $(PJUNHOPTS2) $(PJUVEOPTS2) -URLC_STATUS_GEN -UUL_PROC_MAC
#ALL RRC product options
LNHOPTS=-DNH -DSM -DLCLNH -DLCSMMILNH -DLCSMNHMILNH -DLCNHMILNH
NHUOPTS=-DLCNHU -DVE -DNX -DLCVELINHU -DLCNHUINHU
NHOPTS=-DNH $(LNHOPTS) $(NHUOPTS) $(CRGOPTS) $(CKWOPTS) $(CPJOPTS) $(KWUOPTS) $(PJUNHOPTS)\
       -DRX -DRNC_INTEG_CCPU 
NHOPTS1=-DNH $(LNHOPTS) $(NHUOPTS) $(CRGOPTS1) $(CKWOPTS1) $(CPJOPTS) $(KWUOPTS) $(PJUNHOPTS)\
       -DRX -DRNC_INTEG_CCPU 

#ALL RRC application product options
LVEOPTS=-DVE -DSM -DLCLVE -DLCSMMILVE -DLCVEMILVE
CTFOPTS=-DLCCTF -DVE -DLCVELICTF -DLCTFUICTF
CTFOPTS1=-DLCCTF -DLCVELICTF -DLCTFUICTF
VEOPTS=-DVE $(LVEOPTS) $(NHUOPTS) $(CTFOPTS) $(RGROPTS) $(PJUVEOPTS) $(SZTOPTS) $(EGTOPTS) 
VESEUMOPTS=-DVE $(LVEOPTS) $(NHUOPTS) $(CTFOPTS) $(RGRSEUMOPTS) $(PJUVESEUMOPTS) $(SZTOPTS) $(EGTOPTS) 
VEOPTS1=-DVE $(LVEOPTS) $(NHUOPTS) $(CTFOPTS1) $(RGROPTS) $(PJUVEOPTS) 

#All product options
SZTOPTS=-DSZ -DUZ -DLCUZLISZT -DLWLCUZLISZT -DLCSZUISZT -DLCSZT -DPTSZUISZT
LSZOPTS=-DLCLSZ -DLCSCT -DSM -DSZ -DLCSMSZMILSZ  -DLCSZMILSZ
LSZOPTS2=-DLCLSZ -DSM -DSZ -DLCSMSZMILSZ  -DLCSZMILSZ 

TFOPTS=-DTF $(CTFOPTS) $(RGROPTS) -DLCTFU -DLCTFUITFU -DTFU_VER_2 -UTFU_TDD -DRG -DLTE_PAL_ENB
TFOPTS1=-DTF $(CTFOPTS1) $(RGROPTS)

#All SCTP product options
LSBOPTS=-DLCLSB -DSM -DSB -DLSB4  -DLSB8 -DHI_REL_1_4
SCTOPTS=-DSB  -DLCSBUISCT -DLCSZLISCT
HITOPTS=-DLCSBLIHIT -DLCHIUIHIT -DHI_REL_1_4 -DLCEGLIHIT 
SBOPTS = $(LSBOPTS) $(HITOPTS) $(SCTOPTS) -DLCSBMILSB -DLCSMSBMILSB 

SZOPTS=-DSZ $(SZTOPTS) $(LSZOPTS) $(SCTOPTS) -DLCSZMILSZ -DSM \
	  -DSZ_ENB -DSZ_DYNAMIC_PEER -DSZ_USTA
SZOPTS2=-DSZ $(SZTOPTS) $(LSZOPTS2) $(SCTOPTS) -DLCSZMILSZ -DSM \
	  -DSZ_ENB -DSZ_DYNAMIC_PEER -DSZ_USTA

#All TUCL product options
LHIOPTS=-DLCHIMILHI -DSM -DLCLHI
HIOPTS= $(LHIOPTS) $(HITOPTS) -DHI -DLCHIMILHI -DSM -DLCSMHIMILHI -DEG -DHI_NO_CHK_RES

#ALL RRC product options
LRXOPTS=-DRX -DSM -DLCLRX

SMOPTS= -DLCSMMILCL -DLCCLMILCL

#-----------------------------------------------------------------------
# macros for sw and include directories
#
CODEPATH=../../
MTSE_DIR=$(CODEPATH)/mt
MTSEUM_DIR=$(CODEPATH)/mt
CM_DIR=$(CODEPATH)/cm
VE_DIR=$(CODEPATH)/lteenb/src
TF_DIR=$(CODEPATH)/ltepal
YS_DIR=$(CODEPATH)/ltecl
#MS_DIR=$(CODEPATH)/ltephy/mindSpeed/inc/
MS_CM_DIR=$(CODEPATH)ltephy/mindSpeed/uppercore/Common/
MS_PHYLTE_DIR=$(CODEPATH)ltephy/mindSpeed/uppercore/PHY/LTE/C-Code/
MS_TP_DIR=$(CODEPATH)ltephy/mindSpeed/uppercore/TurnerProject/
MS_TP_BSP_DIR=$(CODEPATH)ltephy/mindSpeed/uppercore/TurnerProject/BSP/
NH_DIR=$(CODEPATH)/lterrc
SZ_DIR=$(CODEPATH)/s1ap
SB_DIR=$(CODEPATH)/sctp
HI_DIR=$(CODEPATH)/tucl
EG_DIR=$(CODEPATH)/egtpu
RG_DIR=$(CODEPATH)/ltemac
KW_DIR=$(CODEPATH)/lterlcpdcp
CL2_DIR=$(CODEPATH)/cl2
CM_CMINC=../../common/include

CM_LIB_DIR=.
VE_LIB_DIR=.

BLD_LNX_CM_OBJS=$(SY_DIR)/liblnxcm.a
BLD_LNX_CM_PERF_OBJS=$(SY_DIR)/liblnxcmuu.a
BLD_LNX_SS_OBJS=$(SY_DIR)/liblnxmt.a
BLD_LNX_KW_OBJS=$(SY_DIR)/liblnxkw.a
BLD_LNX_RG_OBJS=$(SY_DIR)/liblnxrg.a
BLD_LNX_YS_OBJS=$(SY_DIR)/liblnxys.a
BLD_LNX_YS_MS_OBJS=$(SY_DIR)/liblnxysms.a
BLD_LNX_VE_PERF_OBJS=$(SY_DIR)/liblnxperfve.a
BLD_LNX_VE_UU_PERF_OBJS=$(SY_DIR)/liblnxperfveuu.a
BLD_LNX_VE_E2E_OBJS=$(SY_DIR)/liblnxe2eve.a
BLD_LNX_NH_OBJS=$(SY_DIR)/liblnxnh.a
BLD_LNX_SZ_OBJS=$(SY_DIR)/liblnxsz.a
BLD_LNX_SB_OBJS=$(SY_DIR)/liblnxsb.a
BLD_LNX_HI_OBJS=$(SY_DIR)/liblnxhi.a
BLD_LNX_EG_OBJS=$(SY_DIR)/liblnxeg.a

BLD_CAV_CM_OBJS=$(SY_DIR1)/libcavcm.a
BLD_CAV_SS_OBJS=$(SY_DIR1)/libcavmt.a
BLD_CAV_KW_OBJS=$(SY_DIR1)/libcavkw.a
BLD_CAV_RG_OBJS=$(SY_DIR1)/libcavrg.a
BLD_CAV_YS_OBJS=$(SY_DIR1)/libcavys.a
BLD_CAV_VE_OBJS=$(SY_DIR1)/libcavve.a
BLD_CAV_NH_OBJS=$(SY_DIR1)/libcavnh.a
BLD_CAV_SZ_OBJS=$(SY_DIR1)/libcavsz.a
BLD_CAV_SB_OBJS=$(SY_DIR1)/libcavsb.a
BLD_CAV_HI_OBJS=$(SY_DIR1)/libcavhi.a
BLD_CAV_EG_OBJS=$(SY_DIR1)/libcaveg.a

BLD_CAV_CM_OBJS2=$(SY_DIR2)/libcavcm2.a
BLD_CAV_CM_OBJS2_PERF=$(SY_DIR2)/libcavcm2uu.a
BLD_CAV_SS_OBJS2=$(SY_DIR2)/libcavmt2.a
BLD_CAV_VE_PERF_OBJS2=$(SY_DIR2)/libcavperfve2.a
BLD_CAV_VE_PERF_OBJS2_UU=$(SY_DIR2)/libcavperfve2uu.a
BLD_CAV_VE_E2E_OBJS2=$(SY_DIR2)/libcave2eve2.a
BLD_CAV_NH_OBJS2=$(SY_DIR2)/libcavnh2.a
BLD_CAV_SZ_OBJS2=$(SY_DIR2)/libcavsz2.a
BLD_CAV_SB_OBJS2=$(SY_DIR2)/libcavsb2.a
BLD_CAV_HI_OBJS2=$(SY_DIR2)/libcavhi2.a
BLD_CAV_EG_OBJS2=$(SY_DIR2)/libcaveg2.a
# make the list for .x and .h dependent

# common include files(no makedepend)
CM_INC=\
	$(CM_DIR)/envdep.h         $(CM_DIR)/envind.h         \
	$(CM_DIR)/envopt.h                                    \
	$(CM_DIR)/gen.h            $(CM_DIR)/gen.x            \
	$(CM_DIR)/cm_hash.x        $(CM_DIR)/cm_hash.h        \
	$(CM_DIR)/cm_lte.x        $(CM_DIR)/cm_lte.h        \
	$(CM_DIR)/cm5.h            $(CM_DIR)/cm5.x            \
	$(CM_DIR)/cm_umts.h        $(CM_DIR)/cm_umts.x        \
	$(CM_DIR)/cm_err.h         $(CM_DIR)/cm_lib.x         \
	$(CM_DIR)/cm_mblk.h        $(CM_DIR)/cm_mblk.x        \
	$(CM_DIR)/cm_tkns.h        $(CM_DIR)/cm_tkns.x        \
	$(CM_DIR)/cm_llist.h       $(CM_DIR)/cm_llist.x       \
	$(CM_DIR)/cm_hash.h        $(CM_DIR)/cm_hash.x        \
	$(CM_DIR)/cm_inet.h        $(CM_DIR)/cm_inet.x        \
	$(CM_DIR)/cm_gen.h         $(CM_DIR)/cm_gen.x         \
	$(CM_DIR)/cm_tpt.h         $(CM_DIR)/cm_tpt.x         \
	$(CM_DIR)/cm_dns.h         $(CM_DIR)/cm_dns.x         \
	$(CM_DIR)/ssi.h            $(CM_DIR)/ssi.x            \
	$(CM_DIR)/nhu.h            $(CM_DIR)/nhu.x            \
	$(CM_DIR)/crg.h            $(CM_DIR)/crg.x            \
	$(CM_DIR)/egt.h            $(CM_DIR)/egt.x            \
	$(CM_DIR)/hit.h            $(CM_DIR)/hit.x            \
	$(CM_DIR)/kwu.h            $(CM_DIR)/kwu.x            \
	$(CM_DIR)/szt.h            $(CM_DIR)/szt.x            \
	$(CM_DIR)/sct.h            $(CM_DIR)/sct.x            \
	$(CM_DIR)/tfu.h            $(CM_DIR)/tfu.x            \
	$(CM_DIR)/rgr.h            $(CM_DIR)/rgr.x            \
	$(CM_DIR)/pju.h            $(CM_DIR)/pju.x            \
	$(CM_DIR)/cm_pint.h        $(CM_DIR)/cm_pint.x        \
	$(CM_DIR)/cm_perr.h                                   \
        $(CM_DIR)/nhu_pk.x         $(CM_DIR)/nhu_unpk.x       \
        $(CM_DIR)/nhu_asn.h         $(CM_DIR)/nhu_asn.x       \
        $(CM_DIR)/szt_pk.x         $(CM_DIR)/szt_unpk.x       \
        $(CM_DIR)/szt_asn.h         $(CM_DIR)/szt_asn.x       \
        $(CM_DIR)/leg.h            $(CM_DIR)/leg.x            \
        $(CM_DIR)/lhi.h            $(CM_DIR)/lhi.x            \
        $(CM_DIR)/lkw.h            $(CM_DIR)/lkw.x            \
        $(CM_DIR)/lnh.h            $(CM_DIR)/lnh.x            \
        $(CM_DIR)/lys.h            $(CM_DIR)/lys.x            \
        $(CM_DIR)/lrg.h            $(CM_DIR)/lrg.x            \
        $(CM_DIR)/lsb.h            $(CM_DIR)/lsb.x            \
        $(CM_DIR)/lsz.h            $(CM_DIR)/lsz.x            \
        $(CM_DIR)/lve.h            $(CM_DIR)/lve.x            \
        $(CM_DIR)/lpj.h            $(CM_DIR)/lpj.x            \
	$(CM_DIR)/cm_pasn.x                                   \
	$(CM_DIR)/cm_mem.h         $(CM_DIR)/cm_mem.x         \

CM_INC1=\
        $(CM_DIR)/envdep.h         $(CM_DIR)/envind.h         \
        $(CM_DIR)/envopt.h                                    \
        $(CM_DIR)/gen.h            $(CM_DIR)/gen.x            \
        $(CM_DIR)/cm_hash.x        $(CM_DIR)/cm_hash.h        \
        $(CM_DIR)/cm_lte.x        $(CM_DIR)/cm_lte.h        \
        $(CM_DIR)/cm5.h            $(CM_DIR)/cm5.x            \
        $(CM_DIR)/cm_umts.h        $(CM_DIR)/cm_umts.x        \
        $(CM_DIR)/cm_err.h         $(CM_DIR)/cm_lib.x         \
        $(CM_DIR)/cm_mblk.h        $(CM_DIR)/cm_mblk.x        \
        $(CM_DIR)/cm_tkns.h        $(CM_DIR)/cm_tkns.x        \
        $(CM_DIR)/cm_llist.h       $(CM_DIR)/cm_llist.x       \
        $(CM_DIR)/cm_hash.h        $(CM_DIR)/cm_hash.x        \
        $(CM_DIR)/cm_inet.h        $(CM_DIR)/cm_inet.x        \
        $(CM_DIR)/cm_gen.h         $(CM_DIR)/cm_gen.x         \
        $(CM_DIR)/cm_tpt.h         $(CM_DIR)/cm_tpt.x         \
        $(CM_DIR)/cm_dns.h         $(CM_DIR)/cm_dns.x         \
        $(CM_DIR)/ssi.h            $(CM_DIR)/ssi.x            \
        $(CM_DIR)/nhu.h            $(CM_DIR)/nhu.x            \
        $(CM_DIR)/crg.h            $(CM_DIR)/crg.x            \
        $(CM_DIR)/kwu.h            $(CM_DIR)/kwu.x            \
        $(CM_DIR)/tfu.h            $(CM_DIR)/tfu.x            \
        $(CM_DIR)/rgr.h            $(CM_DIR)/rgr.x            \
        $(CM_DIR)/pju.h            $(CM_DIR)/pju.x            \
        $(CM_DIR)/cm_pint.h        $(CM_DIR)/cm_pint.x        \
        $(CM_DIR)/cm_perr.h                                   \
        $(CM_DIR)/lkw.h            $(CM_DIR)/lkw.x            \
        $(CM_DIR)/lrg.h            $(CM_DIR)/lrg.x            \
        $(CM_DIR)/lpj.h            $(CM_DIR)/lpj.x            \
        $(CM_DIR)/cm_pasn.h        $(CM_DIR)/cm_pasn.x        \
        $(CM_DIR)/cm_mem.h         $(CM_DIR)/cm_mem.x         \

# Addign this for the Lib Rule 
CM_INC2=\
	$(CM_LIB_DIR)/envdep.h         $(CM_LIB_DIR)/envind.h         \
	$(CM_LIB_DIR)/envopt.h                                    \
	$(CM_LIB_DIR)/gen.h            $(CM_LIB_DIR)/gen.x            \
	$(CM_LIB_DIR)/cm_hash.x        $(CM_LIB_DIR)/cm_hash.h        \
	$(CM_LIB_DIR)/cm_lte.x        $(CM_LIB_DIR)/cm_lte.h        \
	$(CM_LIB_DIR)/cm5.h            $(CM_LIB_DIR)/cm5.x            \
	$(CM_LIB_DIR)/cm_err.h         $(CM_LIB_DIR)/cm_lib.x         \
	$(CM_LIB_DIR)/cm_mblk.h        $(CM_LIB_DIR)/cm_mblk.x        \
	$(CM_LIB_DIR)/cm_tkns.h        $(CM_LIB_DIR)/cm_tkns.x        \
	$(CM_LIB_DIR)/cm_llist.h       $(CM_LIB_DIR)/cm_llist.x       \
	$(CM_LIB_DIR)/cm_hash.h        $(CM_LIB_DIR)/cm_hash.x        \
	$(CM_LIB_DIR)/cm_inet.h        $(CM_LIB_DIR)/cm_inet.x        \
	$(CM_LIB_DIR)/cm_tpt.h         $(CM_LIB_DIR)/cm_tpt.x         \
	$(CM_LIB_DIR)/ssi.h            $(CM_LIB_DIR)/ssi.x            \
	$(CM_LIB_DIR)/nhu.h            $(CM_LIB_DIR)/nhu.x            \
	$(CM_LIB_DIR)/egt.h            $(CM_LIB_DIR)/egt.x            \
	$(CM_LIB_DIR)/szt.h            $(CM_LIB_DIR)/szt.x            \
	$(CM_LIB_DIR)/rgr.h            $(CM_LIB_DIR)/rgr.x            \
	$(CM_LIB_DIR)/cm_perr.h                                   \
        $(CM_LIB_DIR)/nhu_pk.x         $(CM_LIB_DIR)/nhu_unpk.x       \
        $(CM_LIB_DIR)/nhu_asn.h         $(CM_LIB_DIR)/nhu_asn.x       \
        $(CM_LIB_DIR)/szt_pk.x         $(CM_LIB_DIR)/szt_unpk.x       \
        $(CM_LIB_DIR)/szt_asn.h         $(CM_LIB_DIR)/szt_asn.x       \
        $(CM_LIB_DIR)/leg.h            $(CM_LIB_DIR)/leg.x            \
        $(CM_LIB_DIR)/lhi.h            $(CM_LIB_DIR)/lhi.x            \
        $(CM_LIB_DIR)/lkw.h            $(CM_LIB_DIR)/lkw.x            \
        $(CM_LIB_DIR)/lnh.h            $(CM_LIB_DIR)/lnh.x            \
        $(CM_LIB_DIR)/lys.h            $(CM_LIB_DIR)/lys.x            \
        $(CM_LIB_DIR)/lrg.h            $(CM_LIB_DIR)/lrg.x            \
        $(CM_LIB_DIR)/lsb.h            $(CM_LIB_DIR)/lsb.x            \
        $(CM_LIB_DIR)/lsz.h            $(CM_LIB_DIR)/lsz.x            \
        $(CM_LIB_DIR)/lve.h            $(CM_LIB_DIR)/lve.x            \
        $(CM_LIB_DIR)/lpj.h            $(CM_LIB_DIR)/lpj.x            \
	$(CM_LIB_DIR)/cm_pasn.x                                   \
	$(CM_LIB_DIR)/cm_mem.h         $(CM_LIB_DIR)/cm_mem.x         \

#-----------------------------------------------------------------------
# macros for compile and link options

LNXENV=-DSUNOS -DSS -DSS_MT -DANSI -D_GNU_SOURCE -DSS_LINUX -D_REENTRANT -D__EXTENSIONS__ \
    -DDEBUGNOEXIT -DDEBUG_LEVEL=$(DEBUG)
CAVENV=-DSUNOS -DSS -DSS_MT -DANSI -D_GNU_SOURCE -DSS_LINUX -D_REENTRANT -D__EXTENSIONS__ \
    -DDEBUGNOEXIT -DALIGN_64BIT -DBIT_64  $(CVMXOPTS) 

# compiler:
QUANTIFY=/usr/ccpu/rational/releases/quantify.i386_linux2.7.0/quantify

ifeq (linux,$(findstring linux,$(OCTEON_TARGET)))
    LNXCC = mips64-octeon-linux-gnu-gcc
    AR = mips64-octeon-linux-gnu-ar
    LNXLD = mips64-octeon-linux-gnu-ld
    STRIP = mips64-octeon-linux-gnu-strip
    OBJDUMP = mips64-octeon-linux-gnu-objdump
    NM = mips64-octeon-linux-gnu-nm
else
    LNXCC = mipsisa64-octeon-elf-gcc
    AR = mipsisa64-octeon-elf-ar
    LNXLD = mipsisa64-octeon-elf-ld
    STRIP = mipsisa64-octeon-elf-strip
    OBJDUMP = mipsisa64-octeon-elf-objdump
    NM = mipsisa64-octeon-elf-nm
endif

ARCH = cortex-a9

LNX_CC="arm-none-linux-gnueabi-gcc -mcpu=$(ARCH) "
LNX_LD="arm-none-linux-gnueabi-gcc -mcpu=$(ARCH) "
#LNX_CC="arm-none-linux-gnueabi-gcc -mcpu=$(ARCH) -pg -static"
#LNX_LD="arm-none-linux-gnueabi-gcc -mcpu=$(ARCH) -pg -static"
#LNX_CC="gcc -m32"
#LNX_LD="gcc -m32"
#LNX_CC="purify --cache-dir=/tmp gcc -m32"
#LNX_LD="purify --cache-dir=/tmp gcc -m32"
CAVCOPTS=" -g3 -pipe -Wall -Wno-comment -Wshadow -Wcast-qual \
-Wstrict-prototypes -Wmissing-prototypes -fno-strict-aliasing"

#LNXCOPTS=" -g3 -pipe -pedantic -Wall -Wno-comment -Wshadow -Wcast-qual \
-Wstrict-prototypes -Wmissing-prototypes -fno-strict-aliasing"
LNXCOPTS=" -g -pipe -pedantic -Wall -Wno-comment -Wshadow -Wcast-qual \
-Wstrict-prototypes -Wmissing-prototypes -fno-strict-aliasing"


# object files:
# o     UNIX
OBJ=o                                    # object files, sunos
MS_PATH=$(CODEPATH)ltephy/mindSpeed/uppercore/TurnerProject/

#######################################################################
# library path
CAVLIBPATH= -L$(OCTEON_ROOT)/target/lib -L$(OBJ_DIR)
# for linking with TurnerProject
LNXMSLIBPATH= -L$(MS_PATH)/OSAL/LINUX -L$(MS_PATH)/BSP/LINUX -L$(MS_PATH)/PHYSTUB/STUB
# for linking with only phystub
LNXLIBPATH= -L$(MS_PATH)/OSAL/LINUX
#######################################################################

# linker options:
CAVSELOPTS=$(LDFLAGS_GLOBAL) -Xlinker -lm $(OBJ_DIR)/libcvmx.a 
CAVSEUMLOPTS=$(LDFLAGS_GLOBAL) -Xlinker $(OBJ_DIR)/libcvmx.a -lpthread -lc -lnss_files -lnss_dns -lresolv -lm

#path for  MS libs and objs
#LNXLOPTS= -Wl,-Map,zzz.map -lpthread -lnsl -lrt -lm  -lbsp -losal -lphystub
# for linking with TurnerProject
LNXMSLOPTS= -lnsl -lm  -lbsp -losal -lphystub -lrt -lpthread
# for linking with only phystub 
LNXLOPTS=-lpthread -lnsl -lrt -lm -losal
#LNXLOPTS= -lpthread -lnsl -lrt -lmac -lm  -lbsp -losal -lphystub
#LNXLOPTS= /root/v1_4-pre3/upper-core/app/*.o -L/root/v1_4-pre3/upper-core/app/BSP/LINUX/libbsp.a -L/root/v1_4-pre3/upper-core/app/BSP/LINUX/libbsp.a -L/root/v1_4-pre3/upper-core/app/OSAL/LINUX/libosal.a -L/root/v1_4-pre3/upper-core/app/LTE/STUB/libmac.a -L/root/v1_4-pre3/upper-core/app/PHYSTUB/STUB/libphystub.a -lpthread -lnsl -lrt -lm 
#LOPTS=$(LDFLAGS_GLOBAL) -lpthread -lnsl -lc -lnss_files -lnss_dns -lresolv

# include options:
#LNXIOPTS  =  -I$(MTSEUM_DIR) -I$(CM_DIR) -I$(EG_DIR) -I$(SZ_DIR) -I$(KW_DIR) -I$(MS_DIR)
LNXIOPTS  =  -I$(MTSEUM_DIR) -I$(CM_DIR) -I$(EG_DIR) -I$(SZ_DIR) -I$(KW_DIR) -I$(MS_CM_DIR) -I$(MS_PHYLTE_DIR) -I$(MS_TP_DIR) -I$(MS_TP_BSP_DIR)

LNXLIBIOPTS  =  -I$(CM_LIB_DIR)
CAVIOPTS  =  -I$(CM_DIR) -I$(MTSE_DIR) -I$(EG_DIR) -I$(SZ_DIR) -I$(KW_DIR) $(CVMX_INC) -I$(TF_DIR)
CAVIOPTS2  =  -I$(MTSEUM_DIR) -I$(CM_DIR) -I$(EG_DIR) -I$(SZ_DIR) $(CVMX_INC) -I$(CM_CMINC)

# compiler options:

# MTSS defines
CCLNXMTOPTS=$(DEBUGPOPTS) $(LNXENV) $(MTOPTS) $(CMOPTS) $(STOPTS) -DOP_SSI_SAM
CCCAVSEMTOPTS=$(DEBUGPOPTS) $(CAVENV) $(MTOPTS) $(CMOPTS) $(STOPTS) -DOP_SSI_SAM -DSS_CAVIUM
CCCAVSEUMMTOPTS=$(DEBUGPOPTS) $(CAVENV) $(MTOPTS2) $(CMOPTS) $(STOPTS) -DOP_SSI_SAM -DSS_SEUM_CAVIUM

# SM defines 
CCLNXSMOPTS= $(LNXENV) $(SMOPTS) $(CMOPTS) $(STOPTS)
CCCAVSESMOPTS= $(CAVENV) $(SMOPTS) $(CMOPTS) $(STOPTS) -DSS_CAVIUM
CCCAVSEUMSMOPTS= $(CAVENV) $(SMOPTS) $(CMOPTS) $(STOPTS)

# NH defines 
CCLNXNHOPTS=  $(LNXENV) $(NHOPTS) $(CMOPTS) $(STNHOPTS)
CCCAVSENHOPTS=  $(CAVENV) $(NHOPTS) $(CMOPTS) $(STNHOPTS) -DSS_CAVIUM
CCCAVSEUMNHOPTS=  $(CAVENV) $(NHOPTS) $(CMOPTS) $(STNHOPTS)

#KW defines
CCLNXKWOPTS= $(LNXENV) $(KWOPTS)  $(STOPTS) $(CMOPTS)
CCCAVSEKWOPTS= $(CAVENV) $(KWOPTS)  $(STOPTS) $(CMOPTS) -DSS_CAVIUM
CCCAVSEUMKWOPTS= $(CAVENV) $(KWOPTS)  $(STOPTS) $(CMOPTS)

#RG defines 
CCLNXRGOPTS= $(LNXENV) $(RGOPTS)  $(STOPTS) $(CMOPTS)
CCCAVSERGOPTS= $(CAVENV) $(RGOPTS)  $(STOPTS) $(CMOPTS) -DSS_CAVIUM
CCCAVSEUMRGOPTS= $(CAVENV) $(RGOPTS)  $(STOPTS) $(CMOPTS)

#TF defines 
CCLNXTFOPTS= $(LNXENV) $(TFOPTS)  $(STOPTS) $(CMOPTS) -DUSE_SSI_TMR -DYS_IP_CFG_FRM_HDR_FILE -DUSE_UDP
CCLNXMSTFOPTS= $(LNXENV) $(TFOPTS)  $(STOPTS) $(CMOPTS) -DYS_MS_PHY
CCCAVSETFOPTS= $(CAVENV) $(TFOPTS1)  $(STOPTS) $(CMOPTS) -DSS_CAVIUM
CCCAVSEUMTFOPTS= $(CAVENV) $(TFOPTS1)  $(STOPTS) $(CMOPTS)

#EG defines 
CCLNXEGOPTS= $(LNXENV) $(EGOPTS)  $(STOPTS) $(CMOPTS)
CCCAVSEEGOPTS= $(CAVENV) $(EGOPTS)  $(STOPTS) $(CMOPTS) -DSS_CAVIUM
CCCAVSEUMEGOPTS= $(CAVENV) $(EGOPTS)  $(STOPTS) $(CMOPTS)

#SZ defines
CCLNXSZOPTS= $(LNXENV) $(SZOPTS)  $(STOPTS) $(CMOPTS)
CCCAVSESZOPTS= $(CAVENV) $(SZOPTS)  $(STOPTS) $(CMOPTS) -DSS_CAVIUM
CCCAVSEUMSZOPTS= $(CAVENV) $(SZOPTS)  $(STOPTS) $(CMOPTS)

#VE defines 
CCLNXVEOPTS= $(LNXENV) $(VEOPTS)  $(STOPTS) $(CMOPTS)
CCCAVSEVEOPTS= $(CAVENV) $(VEOPTS)  $(STOPTS) $(CMOPTS) -DSS_CAVIUM
CCCAVSEUMVEOPTS= $(CAVENV) $(VESEUMOPTS)  $(STOPTS) $(CMOPTS) -DSS_SEUM_CAVIUM

#SB defines 
CCLNXSBOPTS= $(LNXENV) $(SBOPTS)  $(STOPTS) $(CMOPTS)
CCCAVSESBOPTS= $(CAVENV) $(SBOPTS)  $(STOPTS) $(CMOPTS) -DSS_CAVIUM
CCCAVSEUMSBOPTS= $(CAVENV) $(SBOPTS)  $(STOPTS) $(CMOPTS)

#HI defines 
CCLNXHIOPTS= $(LNXENV) $(HIOPTS)  $(STOPTS) $(CMOPTS)
CCCAVSEHIOPTS= $(CAVENV) $(HIOPTS)  $(STOPTS) $(CMOPTS) -DSS_CAVIUM
CCCAVSEUMHIOPTS= $(CAVENV) $(HIOPTS)  $(STOPTS) $(CMOPTS)

#common options 
CCLNXCMOPTS= $(LNXENV) $(CMOPTS) $(SMOPTS) \
         $(MKUOPTS) $(CMKOPTS)  \
         $(CRLOPTS) $(RLUOPTS)  $(STOPTS) \
         $(CTCOPTS) $(TCUOPTS) $(LRXOPTS) 

CCCAVSECMOPTS= $(CAVENV) $(CMOPTS) $(SMOPTS) \
         $(MKUOPTS) $(CMKOPTS)  \
         $(CRLOPTS) $(RLUOPTS)  $(STOPTS) \
         $(CTCOPTS) $(TCUOPTS) $(LRXOPTS) -DSS_CAVIUM

CCCAVSEUMCMOPTS= $(CAVENV) $(CMOPTS) $(SMOPTS) \
         $(MKUOPTS) $(CMKOPTS)  \
         $(CRLOPTS) $(RLUOPTS)  $(STOPTS) \
         $(CTCOPTS) $(TCUOPTS) $(LRXOPTS)

ifeq (${BLDENV}, cav_se_perf)
COMPILE = $(CC) $(CFLAGS_GLOBAL) $(CFLAGS_LOCAL) -MD -c -o $@ $<

ASSEMBLE = $(CC) $(ASFLAGS_GLOBAL) $(ASFLAGS_LOCAL) -MD -c -o $@ $<

dir := $(OCTEON_ROOT)/executive
include $(dir)/cvmx.mk
endif

ifeq (${BLDENV}, cav_seum_perf)
COMPILE = $(CC) $(CFLAGS_GLOBAL) $(CFLAGS_LOCAL) -MD -c -o $@ $<

ASSEMBLE = $(CC) $(ASFLAGS_GLOBAL) $(ASFLAGS_LOCAL) -MD -c -o $@ $<

dir := $(OCTEON_ROOT)/executive
include $(dir)/cvmx.mk
endif

ifeq (${BLDENV}, cav_se_e2e)
COMPILE = $(CC) $(CFLAGS_GLOBAL) $(CFLAGS_LOCAL) -MD -c -o $@ $<

ASSEMBLE = $(CC) $(ASFLAGS_GLOBAL) $(ASFLAGS_LOCAL) -MD -c -o $@ $<

dir := $(OCTEON_ROOT)/executive
include $(dir)/cvmx.mk
endif


ifeq (${BLDENV}, cav_seum_e2e)
COMPILE = $(CC) $(CFLAGS_GLOBAL) $(CFLAGS_LOCAL) -MD -c -o $@ $<

ASSEMBLE = $(CC) $(ASFLAGS_GLOBAL) $(ASFLAGS_LOCAL) -MD -c -o $@ $<

dir := $(OCTEON_ROOT)/executive
include $(dir)/cvmx.mk
endif

LNXPRDENB_PERF_OBJS: \
    $(BLD_LNX_YS_OBJS) \
    $(BLD_LNX_CM_PERF_OBJS) \
    $(BLD_LNX_SS_OBJS) \
    $(BLD_LNX_KW_OBJS) \
    $(BLD_LNX_RG_OBJS) \
    $(BLD_LNX_NH_OBJS) \
    $(BLD_LNX_VE_PERF_OBJS) 
 
LNXPRDENB_E2E_OBJS: \
    $(BLD_LNX_YS_OBJS) \
    $(BLD_LNX_CM_OBJS) \
    $(BLD_LNX_SS_OBJS) \
    $(BLD_LNX_KW_OBJS) \
    $(BLD_LNX_RG_OBJS) \
    $(BLD_LNX_NH_OBJS) \
    $(BLD_LNX_SZ_OBJS) \
    $(BLD_LNX_SB_OBJS) \
    $(BLD_LNX_HI_OBJS) \
    $(BLD_LNX_VE_E2E_OBJS) \
    $(BLD_LNX_EG_OBJS)

LNXPRDENB_E2E_MS_OBJS: \
    $(BLD_LNX_CM_OBJS) \
    $(BLD_LNX_SS_OBJS) \
    $(BLD_LNX_NH_OBJS) \
    $(BLD_LNX_SZ_OBJS) \
    $(BLD_LNX_SB_OBJS) \
    $(BLD_LNX_HI_OBJS) \
    $(BLD_LNX_VE_E2E_OBJS) \
    $(BLD_LNX_EG_OBJS)


CAVPRDENB_PERF_SE_OBJS: \
    $(CVMX_CONFIG) \
    $(BLD_CAV_YS_OBJS) \
    $(BLD_CAV_CM_OBJS) \
    $(BLD_CAV_SS_OBJS) \
    $(BLD_CAV_KW_OBJS) \
    $(BLD_CAV_RG_OBJS) \
    $(LIBS_LIST)

CAVPRDENB_PERF_SEUM_OBJS: \
    $(BLD_CAV_CM_OBJS2_PERF) \
    $(BLD_CAV_SS_OBJS2) \
    $(BLD_CAV_NH_OBJS2) \
    $(BLD_CAV_VE_PERF_OBJS2) \
    $(LIBS_LIST)

CAVPRDENB_E2E_SE_OBJS: \
    $(CVMX_CONFIG) \
    $(BLD_CAV_YS_OBJS) \
    $(BLD_CAV_CM_OBJS) \
    $(BLD_CAV_SS_OBJS) \
    $(BLD_CAV_KW_OBJS) \
    $(BLD_CAV_RG_OBJS) \
    $(LIBS_LIST)

CAVPRDENB_E2E_SEUM_OBJS: \
    $(BLD_CAV_CM_OBJS2) \
    $(BLD_CAV_SS_OBJS2) \
    $(BLD_CAV_NH_OBJS2) \
    $(BLD_CAV_SZ_OBJS2) \
    $(BLD_CAV_SB_OBJS2) \
    $(BLD_CAV_HI_OBJS2) \
    $(BLD_CAV_VE_E2E_OBJS2) \
    $(BLD_CAV_EG_OBJS2) \
    $(LIBS_LIST)



CAVENBBIN1=enb_se
CAVENBBIN2=enb_seum
LNXENBPERFBIN=enb_perf
LNXENBE2EBIN=enb_e2e
LNXENBE2EBIN_MS=enb_e2e_ms
CAVENBPERFSEBIN=enb_se_perf
CAVENBPERFSEUMBIN=enb_seum_perf
CAVENBPERFSEUMBINUU=enb_seum_perf_uu
CAVENBE2ESEBIN=enb_se_e2e
CAVENBE2ESEUMBIN=enb_seum_e2e
LNXLIBBIN=lnxlib

BASENAME=enodeb_uarm
MAKENAME=$(BASENAME).mak

BINLNXPERF=lnx_perf
BINLNXE2E=lnx_e2e
BINLNXE2E_MS=lnx_e2e_ms
BINCAVPERFSE=cav_se_perf
BINCAVPERFSEUM=cav_seum_perf
BINCAVE2ESE=cav_se_e2e
BINCAVE2ESEUM=cav_seum_e2e
BINCAVACC=cav_acc
BINLNXLIB=lnx_lib

BLDSE=se
BLDSEUM=seum
#
#
# Linux Default GNU acceptance test build 
#
acc:
	@$(MAKE) -f $(MAKENAME) $(BLDENV) 

#	@echo "Linux Performance Build"
$(BINLNXPERF):
	@$(MAKE) -f $(MAKENAME) $(LNXENBPERFBIN) CC=$(LNX_CC) \
	LL=$(LNX_LD) COPTS=$(LNXCOPTS) CMOPTS='$(CMENBPERFOPTS)' 

#	@echo "Linux End To End Build"
$(BINLNXE2E):
	@$(MAKE) -f $(MAKENAME) $(LNXENBE2EBIN) CC=$(LNX_CC) \
	LL=$(LNX_LD) COPTS=$(LNXCOPTS) CMOPTS='$(CMENBE2EOPTS)' 

#	@echo "Linux End To End Build"
$(BINLNXE2E_MS):
	@$(MAKE) -f $(MAKENAME) $(LNXENBE2EBIN_MS) CC=$(LNX_CC) \
	LL=$(LNX_LD) COPTS=$(LNXCOPTS) CMOPTS='$(CMENBE2EOPTS)' 


$(BINCAVPERFSE):
	@$(MAKE) -f $(MAKENAME) $(CAVENBPERFSEBIN) CC=$(LNXCC) \
	LL=$(LNXLD) COPTS=$(CAVCOPTS) CMOPTS='$(CMENBPERFSEOPTS)' 

$(BINCAVPERFSEUM):
	@$(MAKE) -f $(MAKENAME) $(CAVENBPERFSEUMBIN) CC=$(LNXCC) \
	LL=$(LNXLD) COPTS=$(CAVCOPTS) CMOPTS='$(CMENBPERFSEUMOPTS)' 

$(BINCAVE2ESE):
	@$(MAKE) -f $(MAKENAME) $(CAVENBE2ESEBIN) CC=$(LNXCC) \
	LL=$(LNXLD) COPTS=$(CAVCOPTS) CMOPTS='$(CMENBE2ESEOPTS)' 

$(BINCAVE2ESEUM):
	@$(MAKE) -f $(MAKENAME) $(CAVENBE2ESEUMBIN) CC=$(LNXCC) \
	LL=$(LNXLD) COPTS=$(CAVCOPTS) CMOPTS='$(CMENBE2ESEUMOPTS)' 

$(BINLNXLIB):
	@$(MAKE) -f $(MAKENAME) $(LNXLIBBIN) CC=$(LNX_CC) \
	LL=$(LNX_LD) COPTS=$(LNXCOPTS) CMOPTS='$(CMENBE2EOPTS)' 

$(LNXENBPERFBIN):LNXPRDENB_PERF_OBJS 
	$(CC)  $(SY_DIR)/*.o $(LNXLIBPATH) $(LNXLOPTS) -o ./$(SY_DIR)/$(OUTNAME)

$(LNXENBE2EBIN):LNXPRDENB_E2E_OBJS 
	$(CC)  $(SY_DIR)/*.o $(LNXLIBPATH) $(LNXLOPTS) -o ./$(SY_DIR)/$(OUTNAME)

$(LNXENBE2EBIN_MS):LNXPRDENB_E2E_MS_OBJS 
	$(CC)  $(SY_DIR)/*.o $(MS_PATH)/ctrlmsg.o $(MS_PATH)/svsr.o $(MS_PATH)/diags.o \
	$(MS_PATH)/console.o $(MS_PATH)/phyapi.o $(MS_PATH)/spusched.o \
	$(LNXMSLIBPATH) $(LNXMSLOPTS) -o ./$(SY_DIR)/$(OUTNAME)

$(CAVENBPERFSEBIN):CAVPRDENB_PERF_SE_OBJS
	$(CC)  $(SY_DIR1)/*.o $(CAVLIBPATH) $(CAVSELOPTS) -o ./$(SY_DIR1)/$(OUTNAME1)

$(CAVENBPERFSEUMBIN):CAVPRDENB_PERF_SEUM_OBJS 
	$(CC)  $(SY_DIR2)/*.o $(CAVLIBPATH) $(CAVSEUMLOPTS) -o ./$(SY_DIR2)/$(OUTNAME2)

$(CAVENBE2ESEBIN):CAVPRDENB_E2E_SE_OBJS 
	$(CC)  $(SY_DIR1)/*.o $(CAVLIBPATH) $(CAVSELOPTS) -o ./$(SY_DIR1)/$(OUTNAME1)

$(CAVENBE2ESEUMBIN):CAVPRDENB_E2E_SEUM_OBJS 
	$(CC)  $(SY_DIR2)/*.o $(CAVLIBPATH) $(CAVSEUMLOPTS) -o ./$(SY_DIR2)/$(OUTNAME2)

# Portable compilation -> Point to acc directly
# Portability is not supported
pt:acc

$(BINLNXPT):
	@$(MAKE) -f $(MAKENAME) $(ENBBIN)  CC=$(LNXCC) \
	LL=$(LNXLD) COPTS=$(LNXCOPTS) CMOPTS='$(CMENBOPTS)'

#-----------------------------------------------------------------------
# UNIX link - absolute object file depends on relocatable object
# (.o) files

#
# Making Common Libraries
#
$(BLD_LNX_CM_OBJS):
	@$(MAKE) -f cm.mak $(SY_DIR)/libcm.a COPTS=$(LNXCOPTS) \
	LOPTS='$(LNXLOPTS)' POPTS='$(CCLNXCMOPTS)' IOPTS='$(LNXIOPTS)' IN_DIR='$(CM_DIR)' \
	OUT_DIR='$(SY_DIR)' OBJ='$(OBJ)' CC='$(CC)' LL='$(LL)'  CM_INC='$(CM_INC)'

$(BLD_LNX_CM_PERF_OBJS):
	@$(MAKE) -f cm.uu.mak $(SY_DIR)/libcmuu.a COPTS=$(LNXCOPTS) \
	LOPTS='$(LNXLOPTS)' POPTS='$(CCLNXCMOPTS)' IOPTS='$(LNXIOPTS)' IN_DIR='$(CM_DIR)' \
	OUT_DIR='$(SY_DIR)' OBJ='$(OBJ)' CC='$(CC)' LL='$(LL)'  CM_INC='$(CM_INC)'

$(BLD_CAV_CM_OBJS):
	@$(MAKE) -f cm.se.mak $(SY_DIR1)/libcm.a COPTS=$(CAVCOPTS) \
	LOPTS='$(CAVLOPTS)' POPTS='$(CCCAVSECMOPTS)' IOPTS='$(CAVIOPTS)' IN_DIR='$(CM_DIR)' \
	OUT_DIR='$(SY_DIR1)' OBJ='$(OBJ)' CC='$(CC)' LL='$(LL)'  CM_INC='$(CM_INC1)'

$(BLD_CAV_CM_OBJS2):
	@$(MAKE) -f cm.mak $(SY_DIR2)/libcm.a COPTS=$(CAVCOPTS) \
	LOPTS='$(CAVLOPTS)' POPTS='$(CCCAVSEUMCMOPTS)' IOPTS='$(CAVIOPTS2)' IN_DIR='$(CM_DIR)' \
	OUT_DIR='$(SY_DIR2)' OBJ='$(OBJ)' CC='$(CC)' LL='$(LL)'  CM_INC='$(CM_INC1)'

$(BLD_CAV_CM_OBJS2_PERF):
	@$(MAKE) -f cm.uu.mak $(SY_DIR2)/libcmuu.a COPTS=$(CAVCOPTS) \
        LOPTS='$(CAVLOPTS)' POPTS='$(CCCAVSEUMCMOPTS)' IOPTS='$(CAVIOPTS2)' IN_DIR='$(CM_DIR)' \
        OUT_DIR='$(SY_DIR2)' OBJ='$(OBJ)' CC='$(CC)' LL='$(LL)'  CM_INC='$(CM_INC1)'
#
#Making eNodeB Sample application
#
$(BLD_LNX_VE_PERF_OBJS):
	@$(MAKE) -f ve.mak $(SY_DIR)/libperflnxveuu.a COPTS=$(LNXCOPTS) \
	LOPTS='$(LNXLOPTS)' POPTS='$(CCLNXVEOPTS)' SMPOPTS='$(CCLNXSMOPTS)' \
	HIOPTS='$(HIOPTS)' SBOPTS='$(SBOPTS)' IOPTS='$(LNXIOPTS)' IN_DIR='$(VE_DIR)' \
	OUT_DIR='$(SY_DIR)' OBJ='$(OBJ)' CC='$(CC)' LL='$(LL)' CM_INC='$(CM_INC)' 

$(BLD_LNX_VE_E2E_OBJS):
	@$(MAKE) -f ve.mak $(SY_DIR)/libe2elnxve.a COPTS=$(LNXCOPTS) \
	LOPTS='$(LNXLOPTS)' POPTS='$(CCLNXVEOPTS)' SMPOPTS='$(CCLNXSMOPTS)' \
	HIOPTS='$(HIOPTS)' SBOPTS='$(SBOPTS)' IOPTS='$(LNXIOPTS)' IN_DIR='$(VE_DIR)' \
	OUT_DIR='$(SY_DIR)' OBJ='$(OBJ)' CC='$(CC)' LL='$(LL)' CM_INC='$(CM_INC)' 

$(LNXLIBBIN):
	@$(MAKE) -f ve.mak $(SY_LIB_DIR)/libe2elnxve.a COPTS=$(LNXCOPTS) \
	LOPTS='$(LNXLOPTS)' POPTS='$(CCLNXVEOPTS)' SMPOPTS='$(CCLNXSMOPTS)' \
	HIOPTS='$(HIOPTS)' SBOPTS='$(SBOPTS)' IOPTS='$(LNXLIBIOPTS)' IN_DIR='$(VE_LIB_DIR)' \
	OUT_DIR='$(SY_LIB_DIR)' OBJ='$(OBJ)' CC='$(CC)' LL='$(LL)' CM_INC='$(CM_INC2)' 

$(BLD_CAV_VE_OBJS):
	@$(MAKE) -f ve.mak $(SY_DIR1)/libve.a COPTS=$(CAVCOPTS) \
	LOPTS='$(CAVLOPTS)' POPTS='$(CCCAVSEVEOPTS)' SMPOPTS='$(CCCAVSESMOPTS)' \
	HIOPTS='$(HIOPTS)' SBOPTS='$(SBOPTS)' IOPTS='$(CAVIOPTS)' IN_DIR='$(VE_DIR)' \
	OUT_DIR='$(SY_DIR1)' OBJ='$(OBJ)' CC='$(CC)' LL='$(LL)' CM_INC='$(CM_INC1)' 

$(BLD_CAV_VE_PERF_OBJS2):
	@$(MAKE) -f ve.mak $(SY_DIR2)/libperfcavveuu.a BLD='$(BLDSEUM)' COPTS=$(CAVCOPTS) \
	LOPTS='$(CAVLOPTS)' POPTS='$(CCCAVSEUMVEOPTS)' SMPOPTS='$(CCCAVSEUMSMOPTS)' \
	HIOPTS='$(HIOPTS)' SBOPTS='$(SBOPTS)' IOPTS='$(CAVIOPTS2)' IN_DIR='$(VE_DIR)' \
	OUT_DIR='$(SY_DIR2)' OBJ='$(OBJ)' CC='$(CC)' LL='$(LL)' CM_INC='$(CM_INC1)' 

$(BLD_CAV_VE_E2E_OBJS2):
	@$(MAKE) -f ve.mak $(SY_DIR2)/libe2ecavve.a BLD='$(BLDSEUM)' COPTS=$(CAVCOPTS) \
	LOPTS='$(CAVLOPTS)' POPTS='$(CCCAVSEUMVEOPTS)' SMPOPTS='$(CCCAVSEUMSMOPTS)' \
	HIOPTS='$(HIOPTS)' SBOPTS='$(SBOPTS)' IOPTS='$(CAVIOPTS2)' IN_DIR='$(VE_DIR)' \
	OUT_DIR='$(SY_DIR2)' OBJ='$(OBJ)' CC='$(CC)' LL='$(LL)' CM_INC='$(CM_INC1)' 

#
#Making LTE RRC Layer
#
$(BLD_LNX_NH_OBJS):
	@$(MAKE) -f nh.mak $(SY_DIR)/libnh.a COPTS=$(LNXCOPTS) \
	LOPTS='$(LNXLOPTS)' POPTS='$(CCLNXNHOPTS)' IOPTS='$(LNXIOPTS)' IN_DIR='$(NH_DIR)' \
	OUT_DIR='$(SY_DIR)' OBJ='$(OBJ)' CC='$(CC)' LL='$(LL)' CM_INC='$(CM_INC)'

$(BLD_CAV_NH_OBJS):
	@$(MAKE) -f nh.mak $(SY_DIR1)/libnh.a COPTS=$(CAVCOPTS) \
	LOPTS='$(CAVLOPTS)' POPTS='$(CCCAVSENHOPTS)' IOPTS='$(CAVIOPTS)' IN_DIR='$(NH_DIR)' \
	OUT_DIR='$(SY_DIR1)' OBJ='$(OBJ)' CC='$(CC)' LL='$(LL)' CM_INC='$(CM_INC1)'

$(BLD_CAV_NH_OBJS2):
	@$(MAKE) -f nh.mak $(SY_DIR2)/libnh.a COPTS=$(CAVCOPTS) \
	LOPTS='$(CAVLOPTS)' POPTS='$(CCCAVSEUMNHOPTS)' IOPTS='$(CAVIOPTS2)' IN_DIR='$(NH_DIR)' \
	OUT_DIR='$(SY_DIR2)' OBJ='$(OBJ)' CC='$(CC)' LL='$(LL)' CM_INC='$(CM_INC1)'

#
#Making RLC Layer
#
$(BLD_LNX_KW_OBJS):
	@$(MAKE) -f kw.mak $(SY_DIR)/libkw.a COPTS=$(LNXCOPTS) \
	LOPTS='$(LNXLOPTS)' POPTS='$(CCLNXKWOPTS)' IOPTS='$(LNXIOPTS)' IN_DIR='$(KW_DIR)' \
	OUT_DIR='$(SY_DIR)' OBJ='$(OBJ)' CC='$(CC)' LL='$(LL)' CM_INC='$(CM_INC)'
	
$(BLD_CAV_KW_OBJS):
	@$(MAKE) -f kw.mak $(SY_DIR1)/libkw.a COPTS=$(CAVCOPTS) \
	LOPTS='$(CAVLOPTS)' POPTS='$(CCCAVSEKWOPTS)' IOPTS='$(CAVIOPTS)' IN_DIR='$(KW_DIR)' \
	OUT_DIR='$(SY_DIR1)' OBJ='$(OBJ)' CC='$(CC)' LL='$(LL)' CM_INC='$(CM_INC1)'
   
$(BLD_CAV_KW_OBJS2):
	@$(MAKE) -f kw.mak $(SY_DIR2)/libkw.a COPTS=$(CAVCOPTS) \
	LOPTS='$(CAVLOPTS)' POPTS='$(CCCAVSEUMKWOPTS)' IOPTS='$(CAVIOPTS2)' IN_DIR='$(KW_DIR)' \
	OUT_DIR='$(SY_DIR2)' OBJ='$(OBJ)' CC='$(CC)' LL='$(LL)' CM_INC='$(CM_INC1)'

#
#Making MAC Layer
#
$(BLD_LNX_RG_OBJS):
	@$(MAKE) -f rg.mak $(SY_DIR)/librg.a COPTS=$(LNXCOPTS) \
	LOPTS='$(LNXLOPTS)' POPTS='$(CCLNXRGOPTS)' IOPTS='$(LNXIOPTS)' IN_DIR='$(RG_DIR)' \
	OUT_DIR='$(SY_DIR)' OBJ='$(OBJ)' CC='$(CC)' LL='$(LL)' CM_INC='$(CM_INC)'
	
$(BLD_CAV_RG_OBJS):
	@$(MAKE) -f rg.mak $(SY_DIR1)/librg.a COPTS=$(CAVCOPTS) \
	LOPTS='$(CAVLOPTS)' POPTS='$(CCCAVSERGOPTS)' IOPTS='$(CAVIOPTS)' IN_DIR='$(RG_DIR)' \
	OUT_DIR='$(SY_DIR1)' OBJ='$(OBJ)' CC='$(CC)' LL='$(LL)' CM_INC='$(CM_INC1)'

$(BLD_CAV_RG_OBJS2):
	@$(MAKE) -f rg.mak $(SY_DIR2)/librg.a COPTS=$(CAVCOPTS) \
	LOPTS='$(CAVLOPTS)' POPTS='$(CCCAVSEUMRGOPTS)' IOPTS='$(CAVIOPTS2)' IN_DIR='$(RG_DIR)' \
	OUT_DIR='$(SY_DIR2)' OBJ='$(OBJ)' CC='$(CC)' LL='$(LL)' CM_INC='$(CM_INC1)'
#
#Making CL Layer
#
$(BLD_LNX_TF_OBJS):
	@$(MAKE) -f tf.mak $(SY_DIR)/libtf.a COPTS=$(LNXCOPTS) \
	LOPTS='$(LNXLOPTS)' POPTS='$(CCLNXTFOPTS)' IOPTS='$(LNXIOPTS)' IN_DIR='$(TF_DIR)' \
	OUT_DIR='$(SY_DIR)' OBJ='$(OBJ)' CC='$(CC)' LL='$(LL)' CM_INC='$(CM_INC)'
	
$(BLD_CAV_TF_OBJS):
	@$(MAKE) -f tf.mak $(SY_DIR1)/libtf.a COPTS=$(CAVCOPTS) \
	LOPTS='$(CAVLOPTS)' POPTS='$(CCCAVSETFOPTS)' IOPTS='$(CAVIOPTS)' IN_DIR='$(TF_DIR)' \
	OUT_DIR='$(SY_DIR1)' OBJ='$(OBJ)' CC='$(CC)' LL='$(LL)' CM_INC='$(CM_INC1)'

$(BLD_CAV_TF_OBJS2):
	@$(MAKE) -f tf.mak $(SY_DIR2)/libtf.a COPTS=$(CAVCOPTS) \
	LOPTS='$(CAVLOPTS)' POPTS='$(CCCAVSEUMTFOPTS)' IOPTS='$(CAVIOPTS2)' IN_DIR='$(TF_DIR)' \
	OUT_DIR='$(SY_DIR2)' OBJ='$(OBJ)' CC='$(CC)' LL='$(LL)' CM_INC='$(CM_INC1)'

$(BLD_LNX_YS_OBJS):
	@$(MAKE) -f ys.mak $(SY_DIR)/liblnxys.a COPTS=$(LNXCOPTS) \
	LOPTS='$(LNXLOPTS)' POPTS='$(CCLNXTFOPTS)' IOPTS='$(LNXIOPTS)' IN_DIR='$(YS_DIR)' \
	OUT_DIR='$(SY_DIR)' OBJ='$(OBJ)' CC='$(CC)' LL='$(LL)' CM_INC='$(CM_INC)'
	
$(BLD_LNX_YS_MS_OBJS):
	@$(MAKE) -f ys.mak $(SY_DIR)/liblnxysms.a COPTS=$(LNXCOPTS) \
	LOPTS='$(LNXLOPTS)' POPTS='$(CCLNXMSTFOPTS)' IOPTS='$(LNXIOPTS)' IN_DIR='$(YS_DIR)' \
	OUT_DIR='$(SY_DIR)' OBJ='$(OBJ)' CC='$(CC)' LL='$(LL)' CM_INC='$(CM_INC)'
	
$(BLD_CAV_YS_OBJS):
	@$(MAKE) -f ys.mak $(SY_DIR1)/libcavys.a COPTS=$(CAVCOPTS) \
	LOPTS='$(CAVLOPTS)' POPTS='$(CCCAVSETFOPTS)' IOPTS='$(CAVIOPTS)' IN_DIR='$(YS_DIR)' \
	OUT_DIR='$(SY_DIR1)' OBJ='$(OBJ)' CC='$(CC)' LL='$(LL)' CM_INC='$(CM_INC1)'

$(BLD_CAV_YS_OBJS2):
	@$(MAKE) -f ys.mak $(SY_DIR2)/libcavys.a COPTS=$(CAVCOPTS) \
	LOPTS='$(CAVLOPTS)' POPTS='$(CCCAVSEUMTFOPTS)' IOPTS='$(CAVIOPTS2)' IN_DIR='$(YS_DIR)' \
	OUT_DIR='$(SY_DIR2)' OBJ='$(OBJ)' CC='$(CC)' LL='$(LL)' CM_INC='$(CM_INC1)'
#
#
#Making S1AP Layer
#
$(BLD_LNX_SZ_OBJS):
	@$(MAKE) -f sz.mak $(SY_DIR)/libsz.a COPTS=$(LNXCOPTS) \
	LOPTS='$(LOPTS)' POPTS='$(CCLNXSZOPTS)' IOPTS='$(LNXIOPTS)' IN_DIR='$(SZ_DIR)' \
	OUT_DIR='$(SY_DIR)' OBJ='$(OBJ)' CC='$(CC)' LL='$(LL)' CM_INC='$(CM_INC)'
	
$(BLD_CAV_SZ_OBJS):
	@$(MAKE) -f sz.mak $(SY_DIR1)/libsz.a COPTS=$(CAVCOPTS) \
	LOPTS='$(LOPTS)' POPTS='$(CCCAVSESZOPTS)' IOPTS='$(CAVIOPTS)' IN_DIR='$(SZ_DIR)' \
	OUT_DIR='$(SY_DIR1)' OBJ='$(OBJ)' CC='$(CC)' LL='$(LL)' CM_INC='$(CM_INC1)'
	
$(BLD_CAV_SZ_OBJS2):
	@$(MAKE) -f sz.mak $(SY_DIR2)/libsz.a COPTS=$(CAVCOPTS) \
	LOPTS='$(LOPTS)' POPTS='$(CCCAVSEUMSZOPTS)' IOPTS='$(CAVIOPTS2)' IN_DIR='$(SZ_DIR)' \
	OUT_DIR='$(SY_DIR2)' OBJ='$(OBJ)' CC='$(CC)' LL='$(LL)' CM_INC='$(CM_INC1)'
#
#Making SCTP Layer
#
$(BLD_LNX_SB_OBJS):
	@$(MAKE) -f sb.mak $(SY_DIR)/libsb.a COPTS=$(LNXCOPTS) \
	LOPTS='$(LOPTS)' POPTS='$(CCLNXSBOPTS)' IOPTS='$(LNXIOPTS)' IN_DIR='$(SB_DIR)' \
	OUT_DIR='$(SY_DIR)' OBJ='$(OBJ)' CC='$(CC)' LL='$(LL)' CM_INC='$(CM_INC)'
	
$(BLD_CAV_SB_OBJS):
	@$(MAKE) -f sb.mak $(SY_DIR1)/libsb.a COPTS=$(CAVCOPTS) \
	LOPTS='$(LOPTS)' POPTS='$(CCCAVSESBOPTS)' IOPTS='$(CAVIOPTS)' IN_DIR='$(SB_DIR)' \
	OUT_DIR='$(SY_DIR1)' OBJ='$(OBJ)' CC='$(CC)' LL='$(LL)' CM_INC='$(CM_INC1)'
	
$(BLD_CAV_SB_OBJS2):
	@$(MAKE) -f sb.mak $(SY_DIR2)/libsb.a COPTS=$(CAVCOPTS) \
	LOPTS='$(LOPTS)' POPTS='$(CCCAVSEUMSBOPTS)' IOPTS='$(CAVIOPTS2)' IN_DIR='$(SB_DIR)' \
	OUT_DIR='$(SY_DIR2)' OBJ='$(OBJ)' CC='$(CC)' LL='$(LL)' CM_INC='$(CM_INC1)'
#
#Making TUCL Layer
#
$(BLD_LNX_HI_OBJS):
	@$(MAKE) -f hi.mak $(SY_DIR)/libhi.a COPTS=$(LNXCOPTS) \
	LOPTS='$(LOPTS)' POPTS='$(CCLNXHIOPTS)' IOPTS='$(LNXIOPTS)' IN_DIR='$(HI_DIR)' \
	OUT_DIR='$(SY_DIR)' OBJ='$(OBJ)' CC='$(CC)' LL='$(LL)' CM_INC='$(CM_INC)'
	
$(BLD_CAV_HI_OBJS):
	@$(MAKE) -f hi.mak $(SY_DIR1)/libhi.a COPTS=$(CAVCOPTS) \
	LOPTS='$(LOPTS)' POPTS='$(CCCAVSEHIOPTS)' IOPTS='$(CAVIOPTS)' IN_DIR='$(HI_DIR)' \
	OUT_DIR='$(SY_DIR1)' OBJ='$(OBJ)' CC='$(CC)' LL='$(LL)' CM_INC='$(CM_INC1)'

$(BLD_CAV_HI_OBJS2):
	@$(MAKE) -f hi.mak $(SY_DIR2)/libhi.a COPTS=$(CAVCOPTS) \
	LOPTS='$(LOPTS)' POPTS='$(CCCAVSEUMHIOPTS)' IOPTS='$(CAVIOPTS2)' IN_DIR='$(HI_DIR)' \
	OUT_DIR='$(SY_DIR2)' OBJ='$(OBJ)' CC='$(CC)' LL='$(LL)' CM_INC='$(CM_INC1)'
#
#Making eGTP Layer
#
$(BLD_LNX_EG_OBJS):
	@$(MAKE) -f eg.mak $(SY_DIR)/libeg.a COPTS=$(LNXCOPTS) \
	LOPTS='$(LOPTS)' POPTS='$(CCLNXEGOPTS)' IOPTS='$(LNXIOPTS)' IN_DIR='$(EG_DIR)' \
	OUT_DIR='$(SY_DIR)' OBJ='$(OBJ)' CC='$(CC)' LL='$(LL)' CM_INC='$(CM_INC)'

$(BLD_CAV_EG_OBJS):
	@$(MAKE) -f eg.mak $(SY_DIR1)/libeg.a COPTS=$(CAVCOPTS) \
	LOPTS='$(LOPTS)' POPTS='$(CCCAVSEEGOPTS)' IOPTS='$(CAVIOPTS)' IN_DIR='$(EG_DIR)' \
	OUT_DIR='$(SY_DIR1)' OBJ='$(OBJ)' CC='$(CC)' LL='$(LL)' CM_INC='$(CM_INC1)'

$(BLD_CAV_EG_OBJS2):
	@$(MAKE) -f eg.mak $(SY_DIR2)/libeg.a COPTS=$(CAVCOPTS) \
	LOPTS='$(LOPTS)' POPTS='$(CCCAVSEUMEGOPTS)' IOPTS='$(CAVIOPTS2)' IN_DIR='$(EG_DIR)' \
	OUT_DIR='$(SY_DIR2)' OBJ='$(OBJ)' CC='$(CC)' LL='$(LL)' CM_INC='$(CM_INC1)'

#
# making Multi Threaded SSI
#
$(BLD_LNX_SS_OBJS):
	@$(MAKE) -f mt.mak $(SY_DIR)/libmt.a COPTS=$(LNXCOPTS) \
	LOPTS='$(LOPTS)' POPTS='$(CCLNXMTOPTS)' IOPTS='$(LNXIOPTS)' IN_DIR='$(MTSEUM_DIR)' \
   OUT_DIR='$(SY_DIR)' OBJ='$(OBJ)' CC='$(CC)' LL='$(LL)'  \
   CM_INC='$(CM_INC)'

$(BLD_CAV_SS_OBJS):
	@$(MAKE) -f mtse.mak $(SY_DIR1)/libmt.a COPTS=$(CAVCOPTS) \
	LOPTS='$(LOPTS)' POPTS='$(CCCAVSEMTOPTS)' IOPTS='$(CAVIOPTS)' IN_DIR='$(MTSE_DIR)' \
   OUT_DIR='$(SY_DIR1)' OBJ='$(OBJ)' CC='$(CC)' LL='$(LL)'  \
   CM_INC='$(CM_INC1)'

$(BLD_CAV_SS_OBJS2):
	@$(MAKE) -f mt.mak $(SY_DIR2)/libmt.a COPTS=$(CAVCOPTS) \
	LOPTS='$(LOPTS)' POPTS='$(CCCAVSEUMMTOPTS)' IOPTS='$(CAVIOPTS2)' IN_DIR='$(MTSEUM_DIR)' \
   OUT_DIR='$(SY_DIR2)' OBJ='$(OBJ)' CC='$(CC)' LL='$(LL)'  \
   CM_INC='$(CM_INC1)'

clean:
	\rm -f $(SY_DIR)/$(OUTNAME) $(SY_DIR)/* $(SY_DIR1)/* $(SY_LIB_DIR)/*.o $(SY_LIB_DIR)/*.a $(SY_DIR2)/* $(SY_DIR1)/*.a $(SY_DIR2)/*.a $(SY_DIR)/*.a $(SY_DIR)/core $(SY_DIR)/*~ \
	$(SY_DIR)/*.bak $(SY_DIR)/*.err 


# DO NOT DELETE THIS LINE -- make depend depends on it.
