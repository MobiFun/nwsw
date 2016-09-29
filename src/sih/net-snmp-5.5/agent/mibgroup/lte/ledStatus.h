/*
 * Note: this file originally auto-generated by mib2c using
 *  : mib2c.iterate.conf 19302 2010-08-13 12:19:42Z dts12 $
 */
#ifndef LEDSTATUS_H
#define LEDSTATUS_H

/* function declarations */
void init_ledStatus(void);
void initialize_table_ledStatusTable(void);

/* column number definitions for table ledStatusTable */
       #define COLUMN_LED_STATUS_INDEX		1
       #define COLUMN_LEDID		2
       #define COLUMN_LEDCOLOR		3

#define LED_STATUS_MAX_COL COLUMN_LEDCOLOR

    /* Typical data structure for a row entry */
typedef struct ledStatusTable_entry {
    /* Index values */
    unsigned int index;

    /* Column values */
    unsigned int ledId;
    unsigned int ledColor;

    /* Illustrate using a simple linked list */
    int   valid;
    struct ledStatusTable_entry *next;
}LedStatusTableEntry;

#endif /* LEDSTATUS_H */
