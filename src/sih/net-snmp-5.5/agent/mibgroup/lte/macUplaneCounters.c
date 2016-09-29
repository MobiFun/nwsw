/*
 * Note: this file originally auto-generated by mib2c using
 *  : mib2c.iterate.conf 19302 2010-08-13 12:19:42Z dts12 $
 */

#include <net-snmp/net-snmp-config.h>
#include <net-snmp/net-snmp-includes.h>
#include <net-snmp/agent/net-snmp-agent-includes.h>

#include "bs_modules.h"
#include "all_cfg.h"
#include "all_cmn.h"
#include "tlv_enc_dec.h"

#include "sih.h"
#include "syslog.h"
#include "macUplaneCounters.h"

/* function declarations */
Netsnmp_Node_Handler macUplaneCountersTable_handler;
Netsnmp_First_Data_Point  macUplaneCountersTable_get_first_data_point;
Netsnmp_Next_Data_Point   macUplaneCountersTable_get_next_data_point;

/* globals */
struct macUplaneCountersTable_entry  *macUplaneCountersTable_head = NULL;
static TBOOL g_sihMacUplaneCountersGetReqProcessed = FALSE;
static UINT32 g_prevMacUplaneCountersReqCount = 0;
static UINT32 g_macUplaneCountersFirstCall = 0;

/* free_macUplaneCountersTable */
/**
* @details 
*          This is the routine which frees the mac counters entry.
*/
void free_macUplaneCountersTable(void)
{
  macUplaneCountersTableEntry  * current = macUplaneCountersTable_head;

  if (current) 
  {
  for (current = macUplaneCountersTable_head->next ; current != NULL; current = current->next) {
    SNMP_FREE( current );  
    }
  }
   
  SNMP_FREE(macUplaneCountersTable_head);
  macUplaneCountersTable_head = NULL;
  return;
}


/**
* @details 
*          This is the routine which handles get response from SIH Get Processor
*          and prepare the SNMP GET response for EMS.
* @param tlvArray contains pointer to TLV array.
*/
TBOOL sendMacUplaneCountersSnmpGetResponse (
  IO TlvArray    *tlvArray
  )
{
  int  totalNumRow, totalNumCol, i=0, colmn=0;
  macUplaneCountersTableEntry * prevPtr = NULL;

  printTLV(tlvArray);

  /* Total no. of Colmns */
  totalNumCol = MAC_UPLANE_COUNTERS_MAX_COL;
  /* Total no. of Row = (No. of TLV)/(No. of Columns) */
  totalNumRow = tlvArray->numTlvs/totalNumCol;

  logInfo("Total No. of Row %d & Col %d", totalNumRow, totalNumCol);
  for (i=1;  i <= totalNumRow;  i++)
      {
      macUplaneCountersTableEntry *ptr=(macUplaneCountersTableEntry *)malloc(sizeof(macUplaneCountersTableEntry));

      clearBytes (ptr, sizeof(macUplaneCountersTableEntry));

      /* Fill Data in TLV */
      for(colmn = 1 ; colmn <= totalNumCol ; colmn++)
        {
        if (colmn EQ COLUMN_MAC_UPLANE_COUNTERS_INDEX)
           {
            ptr->index =  getUint32FromArray(tlvArray->tlvInfo[colmn-1].data);
            continue;
           }
        if (colmn EQ COLUMN_NUMOFSDUTXRLCMACIF)
           {
            ptr->numOfSDUTxRlcMacIf =  getUint32FromArray(tlvArray->tlvInfo[colmn-1].data);
            continue;
           }
        if (colmn EQ COLUMN_NUMOFSDURXRLCMACIF)
           {
            ptr->numOfSDURxRlcMacIf = getUint32FromArray(tlvArray->tlvInfo[colmn-1].data);
            continue;
           }
        if (colmn EQ COLUMN_NUMSDUDROPRLCMACIF)
           {
            ptr->numSDUDropRlcMacIf = getUint32FromArray(tlvArray->tlvInfo[colmn-1].data);
            continue;
           }
        if (colmn EQ COLUMN_NUMOFPDUTXL2L1IF)
           {
            ptr->numOfPduTxL2L1If = getUint32FromArray(tlvArray->tlvInfo[colmn-1].data);
            continue;
           }
        if (colmn EQ COLUMN_NUMOFPDURXL2L1IF)
           {
            ptr->numOfPduRxL2L1If = getUint32FromArray(tlvArray->tlvInfo[colmn-1].data);
            continue;
           }
        if (colmn EQ COLUMN_NUMPDUDROPL2L1IF)
           {
            ptr-> numPduDropL2L1If = getUint32FromArray(tlvArray->tlvInfo[colmn-1].data);
            continue;
           }
        if (colmn EQ COLUMN_MAC_UPLANE_COUNTERS_LASTCOLLECTEDAT)
           {
            clearBytes (ptr->lastCollectedAt, MAC_UPLANE_COUNTERS_LCOLLECT_BUF_LEN_MAX);
            copyBytes (tlvArray->tlvInfo[colmn-1].data, tlvArray->tlvInfo[colmn-1].len, ptr->lastCollectedAt);
            ptr->lastCollectedAt_len = strlen(ptr->lastCollectedAt);
            continue;
           }
        }

       ptr->next = NULL;

      if (prevPtr == NULL) {
           macUplaneCountersTable_head =  prevPtr = ptr;
       }

      prevPtr->next = ptr;
      prevPtr = ptr;
     }
    prevPtr->next = NULL;

    if (macUplaneCountersTable_head)
        return TRUE;
    else
        return FALSE;
}


/** Initializes the macUplaneCounters module */
void
init_macUplaneCounters(void)
{
  /* here we initialize all the tables we're planning on supporting */
    initialize_table_macUplaneCountersTable();
}

/* Determine the first/last column names */

/** Initialize the macUplaneCountersTable table by defining its contents and how it's structured */
void
initialize_table_macUplaneCountersTable(void)
{
    const oid macUplaneCountersTable_oid[] = {1,3,6,1,4,1,9344,501,MACUPLANECOUNTERS,1};
    const size_t macUplaneCountersTable_oid_len   = OID_LENGTH(macUplaneCountersTable_oid);
    netsnmp_handler_registration    *reg;
    netsnmp_iterator_info           *iinfo;
    netsnmp_table_registration_info *table_info;

    DEBUGMSGTL(("macUplaneCounters:init", "initializing table macUplaneCountersTable\n"));

    reg = netsnmp_create_handler_registration(
              "macUplaneCountersTable",     macUplaneCountersTable_handler,
              macUplaneCountersTable_oid, macUplaneCountersTable_oid_len,
              HANDLER_CAN_RONLY
              );

    table_info = SNMP_MALLOC_TYPEDEF( netsnmp_table_registration_info );
    netsnmp_table_helper_add_indexes(table_info,
                           ASN_UNSIGNED,  /* index: index */
                           0);
    table_info->min_column = COLUMN_MAC_UPLANE_COUNTERS_INDEX;
    table_info->max_column = COLUMN_MAC_UPLANE_COUNTERS_LASTCOLLECTEDAT;
    
    iinfo = SNMP_MALLOC_TYPEDEF( netsnmp_iterator_info );
    iinfo->get_first_data_point = macUplaneCountersTable_get_first_data_point;
    iinfo->get_next_data_point  = macUplaneCountersTable_get_next_data_point;
    iinfo->table_reginfo        = table_info;
    
    netsnmp_register_table_iterator( reg, iinfo );

    /* Initialise the contents of the table here */
}


#ifndef PWAV_SIH
/* create a new row in the (unsorted) table */
struct macUplaneCountersTable_entry *
macUplaneCountersTable_createEntry(
                 u_long  index,
                ) {
    struct macUplaneCountersTable_entry *entry;

    entry = SNMP_MALLOC_TYPEDEF(struct macUplaneCountersTable_entry);
    if (!entry)
        return NULL;

    entry->index = index;
    entry->next = macUplaneCountersTable_head;
    macUplaneCountersTable_head = entry;
    return entry;
}

/* remove a row from the table */
void
macUplaneCountersTable_removeEntry( struct macUplaneCountersTable_entry *entry ) {
    struct macUplaneCountersTable_entry *ptr, *prev;

    if (!entry)
        return;    /* Nothing to remove */

    for ( ptr  = macUplaneCountersTable_head, prev = NULL;
          ptr != NULL;
          prev = ptr, ptr = ptr->next ) {
        if ( ptr == entry )
            break;
    }
    if ( !ptr )
        return;    /* Can't find it */

    if ( prev == NULL )
        macUplaneCountersTable_head = ptr->next;
    else
        prev->next = ptr->next;

    SNMP_FREE( entry );   /* XXX - release any other internal resources */
}
#endif /* PWAV_SIH */


/* Example iterator hook routines - using 'get_next' to do most of the work */
netsnmp_variable_list *
macUplaneCountersTable_get_first_data_point(void **my_loop_context,
                          void **my_data_context,
                          netsnmp_variable_list *put_index_data,
                          netsnmp_iterator_info *mydata)
{
    TagInfo sihTagInfo;
    TBOOL status = FALSE;


    if (g_sihMacUplaneCountersGetReqProcessed EQ FALSE)
        {
        /* Set SIH Get Request Tag Info */
        sihTagInfo.tableId = MACUPLANECOUNTERS;
        sihTagInfo.rowId = CONFIG_ROW_ID_NONE;
        sihTagInfo.colId = CONFIG_COL_ID_NONE;

        status = sihGetProcessor (sihTagInfo);
        if ( status EQ TRUE )
            {
            logInfo("%s","Get Processor Succedded");
            if (macUplaneCountersTable_head)
                {
		g_macUplaneCountersFirstCall = 1;
                g_sihMacUplaneCountersGetReqProcessed = TRUE;
                }
            }
        else
            logError("%s","Get Processor Failed");
        }

    *my_loop_context = macUplaneCountersTable_head;
    return macUplaneCountersTable_get_next_data_point(my_loop_context, my_data_context,
                                    put_index_data,  mydata );
}

netsnmp_variable_list *
macUplaneCountersTable_get_next_data_point(void **my_loop_context,
                          void **my_data_context,
                          netsnmp_variable_list *put_index_data,
                          netsnmp_iterator_info *mydata)
{
    struct macUplaneCountersTable_entry *entry = (struct macUplaneCountersTable_entry *)*my_loop_context;
    netsnmp_variable_list *idx = put_index_data;

    if ( entry ) {
        snmp_set_var_typed_integer( idx, ASN_UNSIGNED, entry->index );
        idx = idx->next_variable;
        *my_data_context = (void *)entry;
        *my_loop_context = (void *)entry->next;
        return put_index_data;
    } else {
        return NULL;
    }
}


/** handles requests for the macUplaneCountersTable table */
int
macUplaneCountersTable_handler(
    netsnmp_mib_handler               *handler,
    netsnmp_handler_registration      *reginfo,
    netsnmp_agent_request_info        *reqinfo,
    netsnmp_request_info              *requests) {

    netsnmp_request_info       *request = NULL;
    netsnmp_table_request_info *table_info = NULL;
    struct macUplaneCountersTable_entry          *table_entry = NULL;
    int loopCount = 0;

    DEBUGMSGTL(("macUplaneCounters:handler", "Processing request (%d)\n", reqinfo->mode));

    switch (reqinfo->mode) {
        /*
         * Read-support (also covers GetNext requests)
         */
    case MODE_GET:
        for (request=requests; request; request=request->next) {

            loopCount++;
            if (request->processed != 0) /*Processed all requests, rows and colmns */
                {
		g_prevMacUplaneCountersReqCount = 0;
                g_sihMacUplaneCountersGetReqProcessed = FALSE;
                /* free the resources */
                free_macUplaneCountersTable();
                /* Here loopCount is decremented, Due to calling of free function two times in SNMP-GETNEXT
                 * on single parameter -- 20/07/11 */
                loopCount--;
                continue;
                }

            /* perform anything here that you need to do before each
               request is processed. */

            /* the following extracts the my_data_context pointer set in
               the loop functions above.  You can then use the results to
               help return data for the columns of the macUplaneCountersTable table
               in question */

            table_entry = (struct macUplaneCountersTable_entry *)
                              netsnmp_extract_iterator_context(request);

            if ( table_entry == NULL) {
                if (reqinfo->mode == MODE_GET) {
                    netsnmp_set_request_error(reqinfo, request,SNMP_NOSUCHINSTANCE);
                    continue;
                }
                /* XXX: no row existed, if you support creation and this is a
                   set, start dealing with it here, else continue */
            }

            table_info  =     netsnmp_extract_table_info(      request);

            /* table_info->colnum contains the column number requested */
            /* table_info->indexes contains a linked list of snmp variable
               bindings for the indexes of the table.  Values in the list
               have been set corresponding to the indexes of the
               request */
            if (table_info==NULL) {
                continue;
            }
    
            switch (table_info->colnum) {
            case COLUMN_MAC_UPLANE_COUNTERS_INDEX:
                if ( !table_entry ) {
                    netsnmp_set_request_error(reqinfo, request,
                                              SNMP_NOSUCHINSTANCE);
                    continue;
                }
                snmp_set_var_typed_integer( request->requestvb, ASN_UNSIGNED,
                                            table_entry->index);
                break;
            case COLUMN_NUMOFSDUTXRLCMACIF:
                if ( !table_entry ) {
                    netsnmp_set_request_error(reqinfo, request,
                                              SNMP_NOSUCHINSTANCE);
                    continue;
                }
                snmp_set_var_typed_integer( request->requestvb, ASN_GAUGE,
                                            table_entry->numOfSDUTxRlcMacIf);
                break;
            case COLUMN_NUMOFSDURXRLCMACIF:
                if ( !table_entry ) {
                    netsnmp_set_request_error(reqinfo, request,
                                              SNMP_NOSUCHINSTANCE);
                    continue;
                }
                snmp_set_var_typed_integer( request->requestvb, ASN_GAUGE,
                                            table_entry->numOfSDURxRlcMacIf);
                break;
            case COLUMN_NUMSDUDROPRLCMACIF:
                if ( !table_entry ) {
                    netsnmp_set_request_error(reqinfo, request,
                                              SNMP_NOSUCHINSTANCE);
                    continue;
                }
                snmp_set_var_typed_integer( request->requestvb, ASN_GAUGE,
                                            table_entry->numSDUDropRlcMacIf);
                break;
            case COLUMN_NUMOFPDUTXL2L1IF:
                if ( !table_entry ) {
                    netsnmp_set_request_error(reqinfo, request,
                                              SNMP_NOSUCHINSTANCE);
                    continue;
                }
                snmp_set_var_typed_integer( request->requestvb, ASN_GAUGE,
                                            table_entry->numOfPduTxL2L1If);
                break;
            case COLUMN_NUMOFPDURXL2L1IF:
                if ( !table_entry ) {
                    netsnmp_set_request_error(reqinfo, request,
                                              SNMP_NOSUCHINSTANCE);
                    continue;
                }
                snmp_set_var_typed_integer( request->requestvb, ASN_GAUGE,
                                            table_entry->numOfPduRxL2L1If);
                break;
            case COLUMN_NUMPDUDROPL2L1IF:
                if ( !table_entry ) {
                    netsnmp_set_request_error(reqinfo, request,
                                              SNMP_NOSUCHINSTANCE);
                    continue;
                }
                snmp_set_var_typed_integer( request->requestvb, ASN_GAUGE,
                                            table_entry->numPduDropL2L1If);
                break;
	     case COLUMN_MAC_UPLANE_COUNTERS_LASTCOLLECTEDAT:
                if ( !table_entry ) {
                    netsnmp_set_request_error(reqinfo, request,
                                              SNMP_NOSUCHINSTANCE);
                    continue;
                }
                snmp_set_var_typed_value( request->requestvb, ASN_OCTET_STR,
                                          table_entry->lastCollectedAt,
                                          table_entry->lastCollectedAt_len);
                break;
            default:
                netsnmp_set_request_error(reqinfo, request,
                                          SNMP_NOSUCHOBJECT);
                break;
            }
        }
	if(g_macUplaneCountersFirstCall EQ 1)
         {
            g_macUplaneCountersFirstCall = 0;
            g_prevMacUplaneCountersReqCount = loopCount;
            // This will occur in case of, when SNMP-GET or SNMP-GETNEXT of Single Parameter occurred.
            // In the first call itself loopCount becomes one.
            // In General Case, the loopCount will be always > 1 because every table contains 
            // index column to fetch the row and remaining columns contain table attributes.
            if(loopCount EQ 1)
            {
               //Setting it to false in case of SNMP-GET request for single parameter.
               g_sihMacUplaneCountersGetReqProcessed = FALSE;
               // Free all resources.
               free_macUplaneCountersTable();
            }
         }
         else if(loopCount > 1 || g_prevMacUplaneCountersReqCount > 1)
         {
            //update g_prevReqCount with current loopCount for next call of Table_handler
            g_prevMacUplaneCountersReqCount = loopCount;
         }
         else if(loopCount EQ 1)
         {
            //Setting it to false in case of SNMP-GET request for single parameter.
            g_sihMacUplaneCountersGetReqProcessed = FALSE;
            // Free all resources.
            free_macUplaneCountersTable();
         }
        break;

    }
    return SNMP_ERR_NOERROR;
}
