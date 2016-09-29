/*
 * Note: this file originally auto-generated by mib2c using
 *  : mib2c.iterate.conf 19302 2010-08-13 12:19:42Z dts12 $
 */
#ifndef ENBSTATUS_H
#define ENBSTATUS_H

/* function declarations */
void init_enbStatus(void);
void initialize_table_enbStatusTable(void);

/* column number definitions for table enbStatusTable */
       #define COLUMN_ENBSTATUS_INDEX			1
       #define COLUMN_ENBSTATUS_ENODEBID		2
       #define COLUMN_ENBSTATUS_VENDORNAME		3
       #define COLUMN_ENBSTATUS_ENODEBIPADDRESS		4
       #define COLUMN_ENBSTATUS_CONNECTEDTOMME		5
       #define COLUMN_ENBSTATUS_ADMINSTATE		6
       #define COLUMN_ENBSTATUS_OPERATIONALSTATE	7
       #define COLUMN_NUMOFUESCONNECTED			8
       #define COLUMN_AGGREGATEDLTHROUGHPUTKBPS		9
       #define COLUMN_AGGREGATEULTHROUHPUTKBPS		10
       #define COLUMN_LOADSTATE				11
       #define COLUMN_NUMACTIVECRITICALALARMS		12
       #define COLUMN_NUMACTIVEMAJORALARMS		13
       #define COLUMN_ENBSTATUS_UPTIME			14
       #define COLUMN_ENBSTATUS_LOCALTIME		15
       #define COLUMN_ENBSTATUS_LOCALTIMEOFFSETFROMUTC	16
       #define COLUMN_MIBVERSION			17
       #define COLUMN_ENBSTATUS_DATEANDTIMESOURCE	18

#define ENB_STATUS_MAX_COL	COLUMN_ENBSTATUS_DATEANDTIMESOURCE
#define ENB_STATUS_VENDORNAME_LEN_MAX	33
#define ENB_STATUS_LOCALTIME_LEN_MAX	18
#define ENB_STATUS_LOCALTIMEOFFSETFROMUTC_LEN_MAX	7
#define ENB_STATUS_MIBVERSION_LEN_MAX		20

    /* Typical data structure for a row entry */
struct enbStatusTable_entry {
    /* Index values */
    unsigned int index;

    /* Column values */
    unsigned int enodebId;
    char vendorName[ENB_STATUS_VENDORNAME_LEN_MAX];
    size_t vendorName_len;
    in_addr_t enodebIpAddress;
    unsigned int connectedToMME;
    unsigned int adminState;
    unsigned int operationalState;
    unsigned int numOfUesConnected;
    unsigned int aggregateDLThroughputKbps;
    unsigned int aggregateULThrouhputKbps;
    unsigned int loadState;
    unsigned int numActiveCriticalAlarms;
    unsigned int numActiveMajorAlarms;
    unsigned int uptime;
    char localTime[ENB_STATUS_LOCALTIME_LEN_MAX];
    size_t localTime_len;
    char localTimeOffsetFromUTC[ENB_STATUS_LOCALTIMEOFFSETFROMUTC_LEN_MAX];
    size_t localTimeOffsetFromUTC_len;
    char mibVersion[ENB_STATUS_MIBVERSION_LEN_MAX];
    size_t mibVersion_len;
    unsigned int dateAndTimeSource;

    /* Illustrate using a simple linked list */
    int   valid;
    struct enbStatusTable_entry *next;
};

#endif /* ENBSTATUS_H */

