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
#include "sctpGenCounters.h"

/* function declarations */
Netsnmp_Node_Handler sctpGenCountersTable_handler;
Netsnmp_First_Data_Point  sctpGenCountersTable_get_first_data_point;
Netsnmp_Next_Data_Point   sctpGenCountersTable_get_next_data_point;

/* globals */
struct sctpGenCountersTable_entry  *sctpGenCountersTable_head = NULL;
static TBOOL g_sihSctpGenCountersGetReqProcessed = FALSE;
static UINT32 g_prevSctpGenCountersReqCount = 0;
static UINT32 g_sctpGenCountersFirstCall = 0;

/* free_sctpGenCountersTable */
/**
* @details 
*          This is the routine which frees the sctp counters entry.
*/
void free_sctpGenCountersTable(void)
{
  sctpGenCountersTableEntry  * current = sctpGenCountersTable_head;

  if (current) 
  {
  for (current = sctpGenCountersTable_head->next ; current != NULL; current = current->next) {
    SNMP_FREE( current );  
    }
  }
   
  SNMP_FREE(sctpGenCountersTable_head);
  sctpGenCountersTable_head = NULL;

  return;
}


/**
* @details 
*          This is the routine which handles get response from SIH Get Processor
*          and prepare the SNMP GET response for EMS.
* @param tlvArray contains pointer to TLV array.
*/
TBOOL sendSctpGenCountersSnmpGetResponse (
  IO TlvArray    *tlvArray
  )
{
  int  totalNumRow, totalNumCol, i=0, colmn=0;
  sctpGenCountersTableEntry * prevPtr = NULL;

  printTLV(tlvArray);

  /* Total no. of Colmns */
  totalNumCol = SCTP_GEN_COUNTERS_MAX_COL;
  /* Total no. of Row = (No. of TLV)/(No. of Columns) */
  totalNumRow = tlvArray->numTlvs/totalNumCol;

  logInfo("Total No. of Row %d & Col %d", totalNumRow, totalNumCol);
  for (i=1;  i <= totalNumRow;  i++)
      {
      sctpGenCountersTableEntry *ptr=(sctpGenCountersTableEntry *)malloc(sizeof(sctpGenCountersTableEntry));

      clearBytes (ptr, sizeof(sctpGenCountersTableEntry));

      /* Fill Data in TLV */
      for(colmn = 1 ; colmn <= totalNumCol ; colmn++)
        {
        if (colmn EQ COLUMN_SCTP_GEN_COUNTERS_INDEX)
           {
            ptr->index =  getUint32FromArray(tlvArray->tlvInfo[colmn-1].data);
            continue;
           }
        if (colmn EQ COLUMN_NOINITTX)
           {
            ptr->noInitTx  = getUint32FromArray(tlvArray->tlvInfo[colmn-1].data);
            continue;
           }
        if (colmn EQ COLUMN_NOINITRETX)
           {
            ptr->noInitReTx  = getUint32FromArray(tlvArray->tlvInfo[colmn-1].data);
            continue;
           }
        if (colmn EQ COLUMN_NOINITRX)
           {
            ptr->noInitRx  = getUint32FromArray(tlvArray->tlvInfo[colmn-1].data);
            continue;
           }
        if (colmn EQ COLUMN_NOIACKTX)
           {
            ptr->noIAckTx  = getUint32FromArray(tlvArray->tlvInfo[colmn-1].data);
            continue;
           }
        if (colmn EQ COLUMN_NOIACKRX)
           {
            ptr->noIAckRx  = getUint32FromArray(tlvArray->tlvInfo[colmn-1].data);
            continue;
           }
        if (colmn EQ COLUMN_NOSHDWNTX)
           {
            ptr->noShDwnTx  = getUint32FromArray(tlvArray->tlvInfo[colmn-1].data);
            continue;
           }
        if (colmn EQ COLUMN_NOSHDWNRETX)
           {
            ptr->noShDwnReTx  = getUint32FromArray(tlvArray->tlvInfo[colmn-1].data);
            continue;
           }
        if (colmn EQ COLUMN_NOSHDWNRX)
           {
            ptr->noShDwnRx  = getUint32FromArray(tlvArray->tlvInfo[colmn-1].data);
            continue;
           }
        if (colmn EQ COLUMN_NOSHDWNACKTX)
           {
            ptr->noShDwnAckTx  = getUint32FromArray(tlvArray->tlvInfo[colmn-1].data);
            continue;
           }
        if (colmn EQ COLUMN_NOSHDWNACKRETX)
           {
            ptr->noShDwnAckReTx  = getUint32FromArray(tlvArray->tlvInfo[colmn-1].data);
            continue;
           }
        if (colmn EQ COLUMN_NOSHDWNACKRX)
           {
            ptr->noShDwnAckRx  = getUint32FromArray(tlvArray->tlvInfo[colmn-1].data);
            continue;
           }
        if (colmn EQ COLUMN_NOCOOKIETX)
           {
            ptr->noCookieTx  = getUint32FromArray(tlvArray->tlvInfo[colmn-1].data);
            continue;
           }
        if (colmn EQ COLUMN_NOCOOKIERETX)
           {
            ptr->noCookieReTx  = getUint32FromArray(tlvArray->tlvInfo[colmn-1].data);
            continue;
           }
        if (colmn EQ COLUMN_NOCOOKIERX)
           {
            ptr->noCookieRx  = getUint32FromArray(tlvArray->tlvInfo[colmn-1].data);
            continue;
           }
        if (colmn EQ COLUMN_NOCKACKTX)
           {
            ptr->noCkAckTx  = getUint32FromArray(tlvArray->tlvInfo[colmn-1].data);
            continue;
           }
        if (colmn EQ COLUMN_NOCKACKRX)
           {
            ptr->noCkAckRx  = getUint32FromArray(tlvArray->tlvInfo[colmn-1].data);
            continue;
           }
        if (colmn EQ COLUMN_NODATATX)
           {
            ptr->noDataTx  = getUint32FromArray(tlvArray->tlvInfo[colmn-1].data);
            continue;
           }
        if (colmn EQ COLUMN_NODATARETX)
           {
            ptr->noDataReTx  = getUint32FromArray(tlvArray->tlvInfo[colmn-1].data);
            continue;
           }
        if (colmn EQ COLUMN_NODATARX)
           {
            ptr->noDataRx  = getUint32FromArray(tlvArray->tlvInfo[colmn-1].data);
            continue;
           }
        if (colmn EQ COLUMN_NODACKTX)
           {
            ptr->noDAckTx  = getUint32FromArray(tlvArray->tlvInfo[colmn-1].data);
            continue;
           }
        if (colmn EQ COLUMN_NODACKRX)
           {
            ptr->noDAckRx  = getUint32FromArray(tlvArray->tlvInfo[colmn-1].data);
            continue;
           }
        if (colmn EQ COLUMN_NOSHDWNCMPLTTX)
           {
            ptr->noShDwnCmpltTx  = getUint32FromArray(tlvArray->tlvInfo[colmn-1].data);
            continue;
           }
        if (colmn EQ COLUMN_NOSHDWNCMPLTRX)
           {
            ptr->noShDwnCmpltRx  = getUint32FromArray(tlvArray->tlvInfo[colmn-1].data);
            continue;
           }
        if (colmn EQ COLUMN_NOHBEATTX)
           {
            ptr->noHBeatTx  = getUint32FromArray(tlvArray->tlvInfo[colmn-1].data);
            continue;
           }
        if (colmn EQ COLUMN_NOHBEATRX)
           {
            ptr->noHBeatRx  = getUint32FromArray(tlvArray->tlvInfo[colmn-1].data);
            continue;
           }
        if (colmn EQ COLUMN_NOHBACKTX)
           {
            ptr->noHBAckTx  = getUint32FromArray(tlvArray->tlvInfo[colmn-1].data);
            continue;
           }
        if (colmn EQ COLUMN_NOHBACKRX)
           {
            ptr->noHBAckRx  = getUint32FromArray(tlvArray->tlvInfo[colmn-1].data);
            continue;
           }
        if (colmn EQ COLUMN_NOABORTTX)
           {
            ptr->noAbortTx  = getUint32FromArray(tlvArray->tlvInfo[colmn-1].data);
            continue;
           }
        if (colmn EQ COLUMN_NOABORTRX)
           {
            ptr->noAbortRx  = getUint32FromArray(tlvArray->tlvInfo[colmn-1].data);
            continue;
           }
        if (colmn EQ COLUMN_NOERRORTX)
           {
            ptr->noErrorTx  = getUint32FromArray(tlvArray->tlvInfo[colmn-1].data);
            continue;
           }
        if (colmn EQ COLUMN_NOERRORRX)
           {
            ptr->noErrorRx  = getUint32FromArray(tlvArray->tlvInfo[colmn-1].data);
            continue;
           }
        if (colmn EQ COLUMN_NOECNETX)
           {
            ptr->noEcneTx  = getUint32FromArray(tlvArray->tlvInfo[colmn-1].data);
            continue;
           }
        if (colmn EQ COLUMN_NOECNERETX)
           {
            ptr->noEcneReTx  = getUint32FromArray(tlvArray->tlvInfo[colmn-1].data);
            continue;
           }
        if (colmn EQ COLUMN_NOECNERX)
           {
            ptr->noEcneRx  = getUint32FromArray(tlvArray->tlvInfo[colmn-1].data);
            continue;
           }
        if (colmn EQ COLUMN_NOCWRTX)
           {
            ptr->noCwrTx  = getUint32FromArray(tlvArray->tlvInfo[colmn-1].data);
            continue;
           }
        if (colmn EQ COLUMN_NOCWRRX)
           {
            ptr->noCwrRx  = getUint32FromArray(tlvArray->tlvInfo[colmn-1].data);
            continue;
           }
        if (colmn EQ COLUMN_NOQUERYTX)
           {
            ptr->noQueryTx  = getUint32FromArray(tlvArray->tlvInfo[colmn-1].data);
            continue;
           }
        if (colmn EQ COLUMN_NOQUERYRETX)
           {
            ptr->noQueryReTx  = getUint32FromArray(tlvArray->tlvInfo[colmn-1].data);
            continue;
           }
        if (colmn EQ COLUMN_NOQUERYRSPRX)
           {
            ptr->noQueryRspRx  = getUint32FromArray(tlvArray->tlvInfo[colmn-1].data);
            continue;
           }
	if (colmn EQ COLUMN_SCTP_GEN_COUNTERS_LASTCOLLECTEDAT)
           {
            clearBytes (ptr->lastCollectedAt,SCTP_GEN_COUNTERS_LCOLLECT_BUF_LEN_MAX);
            copyBytes (tlvArray->tlvInfo[colmn-1].data, tlvArray->tlvInfo[colmn-1].len, ptr->lastCollectedAt);
            ptr->lastCollectedAt_len = strlen(ptr->lastCollectedAt);
            continue;
           }
        }

       ptr->next = NULL;

      if (prevPtr == NULL) {
           sctpGenCountersTable_head =  prevPtr = ptr;
       }

      prevPtr->next = ptr;
      prevPtr = ptr;
     }
    prevPtr->next = NULL;

    if (sctpGenCountersTable_head)
        return TRUE;
    else
        return FALSE;
}


/** Initializes the sctpGenCounters module */
void
init_sctpGenCounters(void)
{
  /* here we initialize all the tables we're planning on supporting */
    initialize_table_sctpGenCountersTable();
}

/*  Determine the first/last column names */

/** Initialize the sctpGenCountersTable table by defining its contents and how it's structured */
void
initialize_table_sctpGenCountersTable(void)
{
    const oid sctpGenCountersTable_oid[] = {1,3,6,1,4,1,9344,501,SCTPGENCOUNTERS,1};
    const size_t sctpGenCountersTable_oid_len   = OID_LENGTH(sctpGenCountersTable_oid);
    netsnmp_handler_registration    *reg;
    netsnmp_iterator_info           *iinfo;
    netsnmp_table_registration_info *table_info;

    DEBUGMSGTL(("sctpGenCounters:init", "initializing table sctpGenCountersTable\n"));

    reg = netsnmp_create_handler_registration(
              "sctpGenCountersTable",     sctpGenCountersTable_handler,
              sctpGenCountersTable_oid, sctpGenCountersTable_oid_len,
              HANDLER_CAN_RONLY
              );

    table_info = SNMP_MALLOC_TYPEDEF( netsnmp_table_registration_info );
    netsnmp_table_helper_add_indexes(table_info,
                           ASN_UNSIGNED,  /* index: index */
                           0);
    table_info->min_column = COLUMN_SCTP_GEN_COUNTERS_INDEX;
    table_info->max_column = COLUMN_SCTP_GEN_COUNTERS_LASTCOLLECTEDAT;
    
    iinfo = SNMP_MALLOC_TYPEDEF( netsnmp_iterator_info );
    iinfo->get_first_data_point = sctpGenCountersTable_get_first_data_point;
    iinfo->get_next_data_point  = sctpGenCountersTable_get_next_data_point;
    iinfo->table_reginfo        = table_info;
    
    netsnmp_register_table_iterator( reg, iinfo );

    /* Initialise the contents of the table here */
}


#ifndef PWAV_SIH
/* create a new row in the (unsorted) table */
struct sctpGenCountersTable_entry *
sctpGenCountersTable_createEntry(
                 u_long  index,
                ) {
    struct sctpGenCountersTable_entry *entry;

    entry = SNMP_MALLOC_TYPEDEF(struct sctpGenCountersTable_entry);
    if (!entry)
        return NULL;

    entry->index = index;
    entry->next = sctpGenCountersTable_head;
    sctpGenCountersTable_head = entry;
    return entry;
}

/* remove a row from the table */
void
sctpGenCountersTable_removeEntry( struct sctpGenCountersTable_entry *entry ) {
    struct sctpGenCountersTable_entry *ptr, *prev;

    if (!entry)
        return;    /* Nothing to remove */

    for ( ptr  = sctpGenCountersTable_head, prev = NULL;
          ptr != NULL;
          prev = ptr, ptr = ptr->next ) {
        if ( ptr == entry )
            break;
    }
    if ( !ptr )
        return;    /* Can't find it */

    if ( prev == NULL )
        sctpGenCountersTable_head = ptr->next;
    else
        prev->next = ptr->next;

    SNMP_FREE( entry );   /* XXX - release any other internal resources */
}
#endif /* PWAV_SIH */


/* Example iterator hook routines - using 'get_next' to do most of the work */
netsnmp_variable_list *
sctpGenCountersTable_get_first_data_point(void **my_loop_context,
                          void **my_data_context,
                          netsnmp_variable_list *put_index_data,
                          netsnmp_iterator_info *mydata)
{
    TagInfo sihTagInfo;
    TBOOL status = FALSE;


    if (g_sihSctpGenCountersGetReqProcessed EQ FALSE)
        {
        /* Set SIH Get Request Tag Info */
        sihTagInfo.tableId = SCTPGENCOUNTERS;
        sihTagInfo.rowId = CONFIG_ROW_ID_NONE;
        sihTagInfo.colId = CONFIG_COL_ID_NONE;

        status = sihGetProcessor (sihTagInfo);
        if ( status EQ TRUE )
            {
            logInfo("%s","Get Processor Succedded");
            if (sctpGenCountersTable_head)
                {
		g_sctpGenCountersFirstCall = 1;
                g_sihSctpGenCountersGetReqProcessed = TRUE;
                }
            }
        else
            logError("%s","Get Processor Failed");
        }

    *my_loop_context = sctpGenCountersTable_head;
    return sctpGenCountersTable_get_next_data_point(my_loop_context, my_data_context,
                                    put_index_data,  mydata );
}

netsnmp_variable_list *
sctpGenCountersTable_get_next_data_point(void **my_loop_context,
                          void **my_data_context,
                          netsnmp_variable_list *put_index_data,
                          netsnmp_iterator_info *mydata)
{
    struct sctpGenCountersTable_entry *entry = (struct sctpGenCountersTable_entry *)*my_loop_context;
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


/** handles requests for the sctpGenCountersTable table */
int
sctpGenCountersTable_handler(
    netsnmp_mib_handler               *handler,
    netsnmp_handler_registration      *reginfo,
    netsnmp_agent_request_info        *reqinfo,
    netsnmp_request_info              *requests) {

    netsnmp_request_info       *request = NULL;
    netsnmp_table_request_info *table_info = NULL;
    struct sctpGenCountersTable_entry          *table_entry = NULL;
    int loopCount = 0;

    DEBUGMSGTL(("sctpGenCounters:handler", "Processing request (%d)\n", reqinfo->mode));

    switch (reqinfo->mode) {
        /*
         * Read-support (also covers GetNext requests)
         */
    case MODE_GET:
        for (request=requests; request; request=request->next) {

            loopCount++;
            if (request->processed != 0) /*Processed all requests, rows and colmns */
                {
		g_prevSctpGenCountersReqCount = 0;
                g_sihSctpGenCountersGetReqProcessed = FALSE;
                /* free the resources */
                free_sctpGenCountersTable();
                /* Here loopCount is decremented, Due to calling of free function two times in SNMP-GETNEXT
                 * on single parameter -- 20/07/11 */
                loopCount--;
                continue;
                }

            /* perform anything here that you need to do before each
               request is processed. */

            /* the following extracts the my_data_context pointer set in
               the loop functions above.  You can then use the results to
               help return data for the columns of the sctpGenCountersTable table
               in question */

            table_entry = (struct sctpGenCountersTable_entry *)
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
            case COLUMN_SCTP_GEN_COUNTERS_INDEX:
                if ( !table_entry ) {
                    netsnmp_set_request_error(reqinfo, request,
                                              SNMP_NOSUCHINSTANCE);
                    continue;
                }
                snmp_set_var_typed_integer( request->requestvb, ASN_UNSIGNED,
                                            table_entry->index);
                break;
            case COLUMN_NOINITTX:
                if ( !table_entry ) {
                    netsnmp_set_request_error(reqinfo, request,
                                              SNMP_NOSUCHINSTANCE);
                    continue;
                }
                snmp_set_var_typed_integer( request->requestvb, ASN_COUNTER,
                                            table_entry->noInitTx);
                break;
            case COLUMN_NOINITRETX:
                if ( !table_entry ) {
                    netsnmp_set_request_error(reqinfo, request,
                                              SNMP_NOSUCHINSTANCE);
                    continue;
                }
                snmp_set_var_typed_integer( request->requestvb, ASN_COUNTER,
                                            table_entry->noInitReTx);
                break;
            case COLUMN_NOINITRX:
                if ( !table_entry ) {
                    netsnmp_set_request_error(reqinfo, request,
                                              SNMP_NOSUCHINSTANCE);
                    continue;
                }
                snmp_set_var_typed_integer( request->requestvb, ASN_COUNTER,
                                            table_entry->noInitRx);
                break;
            case COLUMN_NOIACKTX:
                if ( !table_entry ) {
                    netsnmp_set_request_error(reqinfo, request,
                                              SNMP_NOSUCHINSTANCE);
                    continue;
                }
                snmp_set_var_typed_integer( request->requestvb, ASN_COUNTER,
                                            table_entry->noIAckTx);
                break;
            case COLUMN_NOIACKRX:
                if ( !table_entry ) {
                    netsnmp_set_request_error(reqinfo, request,
                                              SNMP_NOSUCHINSTANCE);
                    continue;
                }
                snmp_set_var_typed_integer( request->requestvb, ASN_COUNTER,
                                            table_entry->noIAckRx);
                break;
            case COLUMN_NOSHDWNTX:
                if ( !table_entry ) {
                    netsnmp_set_request_error(reqinfo, request,
                                              SNMP_NOSUCHINSTANCE);
                    continue;
                }
                snmp_set_var_typed_integer( request->requestvb, ASN_COUNTER,
                                            table_entry->noShDwnTx);
                break;
            case COLUMN_NOSHDWNRETX:
                if ( !table_entry ) {
                    netsnmp_set_request_error(reqinfo, request,
                                              SNMP_NOSUCHINSTANCE);
                    continue;
                }
                snmp_set_var_typed_integer( request->requestvb, ASN_COUNTER,
                                            table_entry->noShDwnReTx);
                break;
            case COLUMN_NOSHDWNRX:
                if ( !table_entry ) {
                    netsnmp_set_request_error(reqinfo, request,
                                              SNMP_NOSUCHINSTANCE);
                    continue;
                }
                snmp_set_var_typed_integer( request->requestvb, ASN_COUNTER,
                                            table_entry->noShDwnRx);
                break;
            case COLUMN_NOSHDWNACKTX:
                if ( !table_entry ) {
                    netsnmp_set_request_error(reqinfo, request,
                                              SNMP_NOSUCHINSTANCE);
                    continue;
                }
                snmp_set_var_typed_integer( request->requestvb, ASN_COUNTER,
                                            table_entry->noShDwnAckTx);
                break;
            case COLUMN_NOSHDWNACKRETX:
                if ( !table_entry ) {
                    netsnmp_set_request_error(reqinfo, request,
                                              SNMP_NOSUCHINSTANCE);
                    continue;
                }
                snmp_set_var_typed_integer( request->requestvb, ASN_COUNTER,
                                            table_entry->noShDwnAckReTx);
                break;
            case COLUMN_NOSHDWNACKRX:
                if ( !table_entry ) {
                    netsnmp_set_request_error(reqinfo, request,
                                              SNMP_NOSUCHINSTANCE);
                    continue;
                }
                snmp_set_var_typed_integer( request->requestvb, ASN_COUNTER,
                                            table_entry->noShDwnAckRx);
                break;
            case COLUMN_NOCOOKIETX:
                if ( !table_entry ) {
                    netsnmp_set_request_error(reqinfo, request,
                                              SNMP_NOSUCHINSTANCE);
                    continue;
                }
                snmp_set_var_typed_integer( request->requestvb, ASN_COUNTER,
                                            table_entry->noCookieTx);
                break;
            case COLUMN_NOCOOKIERETX:
                if ( !table_entry ) {
                    netsnmp_set_request_error(reqinfo, request,
                                              SNMP_NOSUCHINSTANCE);
                    continue;
                }
                snmp_set_var_typed_integer( request->requestvb, ASN_COUNTER,
                                            table_entry->noCookieReTx);
                break;
            case COLUMN_NOCOOKIERX:
                if ( !table_entry ) {
                    netsnmp_set_request_error(reqinfo, request,
                                              SNMP_NOSUCHINSTANCE);
                    continue;
                }
                snmp_set_var_typed_integer( request->requestvb, ASN_COUNTER,
                                            table_entry->noCookieRx);
                break;
            case COLUMN_NOCKACKTX:
                if ( !table_entry ) {
                    netsnmp_set_request_error(reqinfo, request,
                                              SNMP_NOSUCHINSTANCE);
                    continue;
                }
                snmp_set_var_typed_integer( request->requestvb, ASN_COUNTER,
                                            table_entry->noCkAckTx);
                break;
            case COLUMN_NOCKACKRX:
                if ( !table_entry ) {
                    netsnmp_set_request_error(reqinfo, request,
                                              SNMP_NOSUCHINSTANCE);
                    continue;
                }
                snmp_set_var_typed_integer( request->requestvb, ASN_COUNTER,
                                            table_entry->noCkAckRx);
                break;
            case COLUMN_NODATATX:
                if ( !table_entry ) {
                    netsnmp_set_request_error(reqinfo, request,
                                              SNMP_NOSUCHINSTANCE);
                    continue;
                }
                snmp_set_var_typed_integer( request->requestvb, ASN_COUNTER,
                                            table_entry->noDataTx);
                break;
            case COLUMN_NODATARETX:
                if ( !table_entry ) {
                    netsnmp_set_request_error(reqinfo, request,
                                              SNMP_NOSUCHINSTANCE);
                    continue;
                }
                snmp_set_var_typed_integer( request->requestvb, ASN_COUNTER,
                                            table_entry->noDataReTx);
                break;
            case COLUMN_NODATARX:
                if ( !table_entry ) {
                    netsnmp_set_request_error(reqinfo, request,
                                              SNMP_NOSUCHINSTANCE);
                    continue;
                }
                snmp_set_var_typed_integer( request->requestvb, ASN_COUNTER,
                                            table_entry->noDataRx);
                break;
            case COLUMN_NODACKTX:
                if ( !table_entry ) {
                    netsnmp_set_request_error(reqinfo, request,
                                              SNMP_NOSUCHINSTANCE);
                    continue;
                }
                snmp_set_var_typed_integer( request->requestvb, ASN_COUNTER,
                                            table_entry->noDAckTx);
                break;
            case COLUMN_NODACKRX:
                if ( !table_entry ) {
                    netsnmp_set_request_error(reqinfo, request,
                                              SNMP_NOSUCHINSTANCE);
                    continue;
                }
                snmp_set_var_typed_integer( request->requestvb, ASN_COUNTER,
                                            table_entry->noDAckRx);
                break;
            case COLUMN_NOSHDWNCMPLTTX:
                if ( !table_entry ) {
                    netsnmp_set_request_error(reqinfo, request,
                                              SNMP_NOSUCHINSTANCE);
                    continue;
                }
                snmp_set_var_typed_integer( request->requestvb, ASN_COUNTER,
                                            table_entry->noShDwnCmpltTx);
                break;
            case COLUMN_NOSHDWNCMPLTRX:
                if ( !table_entry ) {
                    netsnmp_set_request_error(reqinfo, request,
                                              SNMP_NOSUCHINSTANCE);
                    continue;
                }
                snmp_set_var_typed_integer( request->requestvb, ASN_COUNTER,
                                            table_entry->noShDwnCmpltRx);
                break;
            case COLUMN_NOHBEATTX:
                if ( !table_entry ) {
                    netsnmp_set_request_error(reqinfo, request,
                                              SNMP_NOSUCHINSTANCE);
                    continue;
                }
                snmp_set_var_typed_integer( request->requestvb, ASN_COUNTER,
                                            table_entry->noHBeatTx);
                break;
            case COLUMN_NOHBEATRX:
                if ( !table_entry ) {
                    netsnmp_set_request_error(reqinfo, request,
                                              SNMP_NOSUCHINSTANCE);
                    continue;
                }
                snmp_set_var_typed_integer( request->requestvb, ASN_COUNTER,
                                            table_entry->noHBeatRx);
                break;
            case COLUMN_NOHBACKTX:
                if ( !table_entry ) {
                    netsnmp_set_request_error(reqinfo, request,
                                              SNMP_NOSUCHINSTANCE);
                    continue;
                }
                snmp_set_var_typed_integer( request->requestvb, ASN_COUNTER,
                                            table_entry->noHBAckTx);
                break;
            case COLUMN_NOHBACKRX:
                if ( !table_entry ) {
                    netsnmp_set_request_error(reqinfo, request,
                                              SNMP_NOSUCHINSTANCE);
                    continue;
                }
                snmp_set_var_typed_integer( request->requestvb, ASN_COUNTER,
                                            table_entry->noHBAckRx);
                break;
            case COLUMN_NOABORTTX:
                if ( !table_entry ) {
                    netsnmp_set_request_error(reqinfo, request,
                                              SNMP_NOSUCHINSTANCE);
                    continue;
                }
                snmp_set_var_typed_integer( request->requestvb, ASN_COUNTER,
                                            table_entry->noAbortTx);
                break;
            case COLUMN_NOABORTRX:
                if ( !table_entry ) {
                    netsnmp_set_request_error(reqinfo, request,
                                              SNMP_NOSUCHINSTANCE);
                    continue;
                }
                snmp_set_var_typed_integer( request->requestvb, ASN_COUNTER,
                                            table_entry->noAbortRx);
                break;
            case COLUMN_NOERRORTX:
                if ( !table_entry ) {
                    netsnmp_set_request_error(reqinfo, request,
                                              SNMP_NOSUCHINSTANCE);
                    continue;
                }
                snmp_set_var_typed_integer( request->requestvb, ASN_COUNTER,
                                            table_entry->noErrorTx);
                break;
            case COLUMN_NOERRORRX:
                if ( !table_entry ) {
                    netsnmp_set_request_error(reqinfo, request,
                                              SNMP_NOSUCHINSTANCE);
                    continue;
                }
                snmp_set_var_typed_integer( request->requestvb, ASN_COUNTER,
                                            table_entry->noErrorRx);
                break;
            case COLUMN_NOECNETX:
                if ( !table_entry ) {
                    netsnmp_set_request_error(reqinfo, request,
                                              SNMP_NOSUCHINSTANCE);
                    continue;
                }
                snmp_set_var_typed_integer( request->requestvb, ASN_COUNTER,
                                            table_entry->noEcneTx);
                break;
            case COLUMN_NOECNERETX:
                if ( !table_entry ) {
                    netsnmp_set_request_error(reqinfo, request,
                                              SNMP_NOSUCHINSTANCE);
                    continue;
                }
                snmp_set_var_typed_integer( request->requestvb, ASN_COUNTER,
                                            table_entry->noEcneReTx);
                break;
            case COLUMN_NOECNERX:
                if ( !table_entry ) {
                    netsnmp_set_request_error(reqinfo, request,
                                              SNMP_NOSUCHINSTANCE);
                    continue;
                }
                snmp_set_var_typed_integer( request->requestvb, ASN_COUNTER,
                                            table_entry->noEcneRx);
                break;
            case COLUMN_NOCWRTX:
                if ( !table_entry ) {
                    netsnmp_set_request_error(reqinfo, request,
                                              SNMP_NOSUCHINSTANCE);
                    continue;
                }
                snmp_set_var_typed_integer( request->requestvb, ASN_COUNTER,
                                            table_entry->noCwrTx);
                break;
            case COLUMN_NOCWRRX:
                if ( !table_entry ) {
                    netsnmp_set_request_error(reqinfo, request,
                                              SNMP_NOSUCHINSTANCE);
                    continue;
                }
                snmp_set_var_typed_integer( request->requestvb, ASN_COUNTER,
                                            table_entry->noCwrRx);
                break;
            case COLUMN_NOQUERYTX:
                if ( !table_entry ) {
                    netsnmp_set_request_error(reqinfo, request,
                                              SNMP_NOSUCHINSTANCE);
                    continue;
                }
                snmp_set_var_typed_integer( request->requestvb, ASN_COUNTER,
                                            table_entry->noQueryTx);
                break;
            case COLUMN_NOQUERYRETX:
                if ( !table_entry ) {
                    netsnmp_set_request_error(reqinfo, request,
                                              SNMP_NOSUCHINSTANCE);
                    continue;
                }
                snmp_set_var_typed_integer( request->requestvb, ASN_COUNTER,
                                            table_entry->noQueryReTx);
                break;
            case COLUMN_NOQUERYRSPRX:
                if ( !table_entry ) {
                    netsnmp_set_request_error(reqinfo, request,
                                              SNMP_NOSUCHINSTANCE);
                    continue;
                }
                snmp_set_var_typed_integer( request->requestvb, ASN_COUNTER,
                                            table_entry->noQueryRspRx);
                break;
	    case COLUMN_SCTP_GEN_COUNTERS_LASTCOLLECTEDAT:
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
	if(g_sctpGenCountersFirstCall EQ 1)
         {
            g_sctpGenCountersFirstCall = 0;
            g_prevSctpGenCountersReqCount = loopCount;
            // This will occur in case of, when SNMP-GET or SNMP-GETNEXT of Single Parameter occurred.
            // In the first call itself loopCount becomes one.
            // In General Case, the loopCount will be always > 1 because every table contains 
            // index column to fetch the row and remaining columns contain table attributes.
            if(loopCount EQ 1)
            {
               //Setting it to false in case of SNMP-GET request for single parameter.
               g_sihSctpGenCountersGetReqProcessed = FALSE;
               // Free all resources.
               free_sctpGenCountersTable();
            }
         }
         else if(loopCount > 1 || g_prevSctpGenCountersReqCount > 1)
         {
            //update g_prevReqCount with current loopCount for next call of Table_handler
            g_prevSctpGenCountersReqCount = loopCount;
         }
         else if(loopCount EQ 1)
         {
            //Setting it to false in case of SNMP-GET request for single parameter.
            g_sihSctpGenCountersGetReqProcessed = FALSE;
            // Free all resources.
            free_sctpGenCountersTable();
         }
        break;

    }
    return SNMP_ERR_NOERROR;
}
