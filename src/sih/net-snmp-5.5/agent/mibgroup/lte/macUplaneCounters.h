/*
 * Note: this file originally auto-generated by mib2c using
 *  : mib2c.iterate.conf 19302 2010-08-13 12:19:42Z dts12 $
 */
#ifndef MACUPLANECOUNTERS_H
#define MACUPLANECOUNTERS_H

/* function declarations */
void init_macUplaneCounters(void);
void initialize_table_macUplaneCountersTable(void);

/* column number definitions for table macUplaneCountersTable */
       #define COLUMN_MAC_UPLANE_COUNTERS_INDEX		1
       #define COLUMN_NUMOFSDUTXRLCMACIF		2
       #define COLUMN_NUMOFSDURXRLCMACIF		3
       #define COLUMN_NUMSDUDROPRLCMACIF		4
       #define COLUMN_NUMOFPDUTXL2L1IF			5
       #define COLUMN_NUMOFPDURXL2L1IF			6
       #define COLUMN_NUMPDUDROPL2L1IF			7
       #define COLUMN_MAC_UPLANE_COUNTERS_LASTCOLLECTEDAT 8

#define MAC_UPLANE_COUNTERS_MAX_COL COLUMN_MAC_UPLANE_COUNTERS_LASTCOLLECTEDAT

#define MAC_UPLANE_COUNTERS_LCOLLECT_BUF_LEN_MAX 18

    /* Typical data structure for a row entry */
typedef struct macUplaneCountersTable_entry {
    /* Index values */
    unsigned int index;

    /* Column values */
    unsigned int numOfSDUTxRlcMacIf;
    unsigned int numOfSDURxRlcMacIf;
    unsigned int numSDUDropRlcMacIf;
    unsigned int numOfPduTxL2L1If;
    unsigned int numOfPduRxL2L1If;
    unsigned int numPduDropL2L1If;
    char lastCollectedAt[MAC_UPLANE_COUNTERS_LCOLLECT_BUF_LEN_MAX];
    size_t lastCollectedAt_len;

    /* Illustrate using a simple linked list */
    int   valid;
    struct macUplaneCountersTable_entry *next;
}macUplaneCountersTableEntry;

#endif /* MACCOUNTERS_H */
