//-------------------------------------------------------------------------------------------
/** @file ltemac.h
 *
 * @brief This file defines the structures, constants and variables
 *        used in the implementation of an LTE MAC
 * @author Mindspeed Technologies
 * @version $Revision: 1.9 $
 *
 * COPYRIGHT&copy; 2008-2011 Mindspeed Technologies.
 * ALL RIGHTS RESERVED
 *
 * This is Unpublished Proprietary Source Code of Mindspeed Technologies
 **/
//-------------------------------------------------------------------------------------------


#ifndef LTEMAC_H
#define LTEMAC_H
//#include "ltypes.h"

#define LTEMAXBUFFSIZE 17250 * 8
#define LTECTLMAXSIZE 200
// Array for IQ samples for a subFrame calculated as (assuming 20 MHz BW)
// (2048 + 160)*2 + (2048 + 144)*2 = 30720 Samples
// The In/Out samples are stored as pairs of fixed point 16 bits, two's
// complement notation, so we need 4*30720 bytes or 122880 bytes for 
// each case
#define LTEIQMAX 122880 
#define MAXCHPERSF 100
#define MAXMAP 30

typedef struct ResBlockMappingDescriptor
{
    U8 startRb;
    U8 numRb;
} RBALLOC, *PRBALLOC;

typedef struct TransportChannelDescriptor
{

    // scTxRbInfo contains the number of pair of RB's used for codeword0 and
    // codeword1 and are updated during txSdu processing
    U16 scTxRbInfo[2];
    // pFecTxOut is an array of pointers for each subchannel Fec Block Output
    U8* pFecTxOut[2];
    // pFecTxIn is an array of pointers for each subchannel Fec Block Input
    U8* pFECTxIn[2];
    // fecOutBits is an array with the number of output bits present
    // in the tx FEC out(Expected ie G0,G1 in Matlab code)
    U32 fecInOutBits[2];
    // pFecRxIn is an array of pointers for each subchannel receive Fec In
    U8* pFecRxIn[2];
    // RbMap is used for debugging purposes as it is easy to visualize the
    // Pair of resource blocks mapping
    // This is a bit map for RB's allocation for this channel, it is the result
    // of processing the allocations of both subchannels
    // The bit map is as follows
    // RbMap[0] RB0, RB1,..., RB31
    // RbMap[1] RB32,......., RB63
    // RbMap[2] RB64,.........RB95
    // RbMap[3] RB96.. RB99
    // Although 36.211 and 36.213 have 110 as maximum number of RB's in certain
    // tables, the 36.804 document that specifies the BS Tx & Rx only requires
    // 100 RB's for 20 MHz Bandwith, 110 corresponds to 22 MHz BW which it
    // is not currently listed as a required BW
    U32 Qm;
    //  TB bytes
    U32 tbBytes[2];
    MAPPINGINFO map;
    //    U16 numRbMaps;
    //    RBALLOC rBAssignment[MAXMAP];

} TRCHDESC, *PTRCHDESC;

typedef struct LteMacDescriptor
{
    U16 numREinRBPairsAllControl;
    U16 numREinRBPairsMid6_Broadcast;
    U16 numREinRBPairsMid6_Ssync;
    U16 numREinRBPairsMid6_Psync;
    U16 frameNumber;
    U16 subFrameNumber;
    U16 rBIndexMid6Lo;
    U16 rBIndexMid6Hi;
    // Index to next available RB pair, valid for contiguous allocations
    U16 nextAvailableRB;
    //  Index to next available CCE entry
    U16 nextAvailableCce;
    TRCHDESC tCh[MAXCHPERSF];
    // Points to the Phy Initialization structure used in the PHY_INIT command
    PINITPARM pInit;
} LTEMACDESC, *PLTEMACDESC;

#endif /* LTEMAC_H */
