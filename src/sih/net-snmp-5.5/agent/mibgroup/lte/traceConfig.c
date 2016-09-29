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
#include "traceConfig.h"


/* function declarations */
Netsnmp_Node_Handler traceConfigTable_handler;
Netsnmp_First_Data_Point  traceConfigTable_get_first_data_point;
Netsnmp_Next_Data_Point   traceConfigTable_get_next_data_point;

/* Globals */
struct traceConfigTable_entry  *traceConfigTable_head = NULL;
static TBOOL g_sihTraceConfigSetReqProcessed = FALSE;
static TBOOL g_sihTraceConfigGetReqProcessed = FALSE;
static UINT32 g_prevTraceConfigReqCount = 0;
static UINT32 g_traceConfigFirstCall = 0;

/* free_traceConfigTable */
/**
* @details 
*          This is the routine which frees the trace config entry.
*/
void free_traceConfigTable(void)
{
  traceConfigTableEntry  * current = traceConfigTable_head;

  if (current)
  {
  for (current = traceConfigTable_head->next ; current != NULL; current = current->next) {
    SNMP_FREE( current );
    }

  SNMP_FREE(traceConfigTable_head);
  traceConfigTable_head = NULL;
  }
  return;
}

/**
* @details 
*          This is the routine which handles get response from SIH Get Processor
*          and prepare the SNMP GET response for EMS.
* @param tlvArray contains pointer to TLV array.
*/
TBOOL sendTraceConfigSnmpGetResponse (
  IO TlvArray    *tlvArray
  )
{
  int  totalNumRow, totalNumCol, i=0, colmn=0;
  traceConfigTableEntry * prevPtr = NULL;

  printTLV(tlvArray);

  /* Total no. of Colmns */
  totalNumCol = TRACE_CONFIG_MAX_COL;
  /* Total no. of Row = (No. of TLV)/(No. of Columns) */
  totalNumRow = tlvArray->numTlvs/totalNumCol;

  for (i=1;  i <= totalNumRow;  i++)
      {
      traceConfigTableEntry *ptr=(traceConfigTableEntry *)malloc(sizeof(traceConfigTableEntry));

      clearBytes(ptr,sizeof(traceConfigTableEntry));

      /* Fill Data in TLV */
      for(colmn = 1 ; colmn <= totalNumCol ; colmn++)
        {
       if (colmn EQ COLUMN_TRACEPROFILEID)
           {
            ptr->traceProfileId = getUint32FromArray(tlvArray->tlvInfo[colmn-1].data);
            continue;
           }
       if (colmn EQ COLUMN_TRACERRC)
           {
            ptr->traceRRC = getUint8FromArray(tlvArray->tlvInfo[colmn-1].data);
            continue;
           }
       if (colmn EQ COLUMN_TRACES1AP)
           {
            ptr->traceS1AP = getUint8FromArray(tlvArray->tlvInfo[colmn-1].data);
            continue;
           }
       if (colmn EQ COLUMN_TRACEX2AP)
           {
            ptr->traceX2AP = getUint8FromArray(tlvArray->tlvInfo[colmn-1].data);
            continue;
           }
       if (colmn EQ COLUMN_TRACERESERVED1)
           {
            ptr->traceReserved1 = getUint8FromArray(tlvArray->tlvInfo[colmn-1].data);
            continue;
           }
       if (colmn EQ COLUMN_TRACERESERVED2)
           {
            ptr->traceReserved2 = getUint8FromArray(tlvArray->tlvInfo[colmn-1].data);
            continue;
           }
       if (colmn EQ COLUMN_TRACERESERVED3)
           {
            ptr->traceReserved3 = getUint8FromArray(tlvArray->tlvInfo[colmn-1].data);
            continue;
           }
       if (colmn EQ COLUMN_TRACEDEST)
           {
            ptr->traceDestination = getUint32FromArray(tlvArray->tlvInfo[colmn-1].data);
            continue;
           }
       if (colmn EQ COLUMN_TRACEDESTUDPPORT)
           {
            ptr->traceDestUdpPort = getUint16FromArray(tlvArray->tlvInfo[colmn-1].data);
            continue;
           }
       if (colmn EQ COLUMN_TRACEDESTIPADDRESS)
           {
            clearBytes (ptr->traceDestIpAddress, MAX_SIZE_TRACEDESTIPADDRESS);
            if (tlvArray->tlvInfo[colmn-1].len > 0)
                {
                 copyBytes (tlvArray->tlvInfo[colmn-1].data, tlvArray->tlvInfo[colmn-1].len, ptr->traceDestIpAddress);
                }
            ptr->traceDestIpAddress_len = strlen(ptr->traceDestIpAddress);
            continue;
           }
        }

       ptr->next = NULL;

      if (prevPtr == NULL) {
           traceConfigTable_head =  prevPtr = ptr;
       }

      prevPtr->next = ptr;
      prevPtr = ptr;
     }
    prevPtr->next = NULL;

    if (traceConfigTable_head)
        return TRUE;
    else
        return FALSE;
}


/**
* @details 
*          This routine sends enodeb config set request to BSM, it uses SIH Set
*          Processor to create and send the message to BSM.
* @param sihTagInfo is used to have the tag information for set request.
*/
TBOOL sendTraceConfigSnmpSetRequest (
      IO TagInfo  sihTagInfo,
      IO TlvArray *tlvArray
      )
{
   traceConfigTableEntry * setReqData = NULL;
   TBOOL status = FALSE;
   int colmn = 0;
   unsigned char * ipAddr;
   struct in_addr ip;

   setReqData = traceConfigTable_head;

   /* Fill Data in TLV */
   for(colmn = 0; colmn < tlvArray->numTlvs; colmn++)
   {
       if(tlvArray->tlvInfo[colmn].tag.colId EQ COLUMN_TRACEPROFILEID)
       {
           putUint32InArray( setReqData->traceProfileId, tlvArray->tlvInfo[colmn].data );
           tlvArray->tlvInfo[colmn].len = sizeof(setReqData->traceProfileId);
       }
       if(tlvArray->tlvInfo[colmn].tag.colId EQ COLUMN_TRACERRC)
       {
           putUint8InArray( setReqData->traceRRC, tlvArray->tlvInfo[colmn].data );
           tlvArray->tlvInfo[colmn].len = sizeof(setReqData->traceRRC);
       }
       if(tlvArray->tlvInfo[colmn].tag.colId EQ COLUMN_TRACES1AP)
       {
           putUint8InArray( setReqData->traceS1AP, tlvArray->tlvInfo[colmn].data );
           tlvArray->tlvInfo[colmn].len = sizeof(setReqData->traceS1AP);
       }
       if(tlvArray->tlvInfo[colmn].tag.colId EQ COLUMN_TRACEX2AP)
       {
           putUint8InArray( setReqData->traceX2AP, tlvArray->tlvInfo[colmn].data );
           tlvArray->tlvInfo[colmn].len = sizeof(setReqData->traceX2AP);
       }
       if(tlvArray->tlvInfo[colmn].tag.colId EQ COLUMN_TRACERESERVED1)
       {
           putUint8InArray( setReqData->traceReserved1, tlvArray->tlvInfo[colmn].data );
           tlvArray->tlvInfo[colmn].len = sizeof(setReqData->traceReserved1);
       }
       if(tlvArray->tlvInfo[colmn].tag.colId EQ COLUMN_TRACERESERVED2)
       {
           putUint8InArray( setReqData->traceReserved2, tlvArray->tlvInfo[colmn].data );
           tlvArray->tlvInfo[colmn].len = sizeof(setReqData->traceReserved2);
       }
       if(tlvArray->tlvInfo[colmn].tag.colId EQ COLUMN_TRACERESERVED3)
       {
           putUint8InArray( setReqData->traceReserved3, tlvArray->tlvInfo[colmn].data );
           tlvArray->tlvInfo[colmn].len = sizeof(setReqData->traceReserved3);
       }
       if(tlvArray->tlvInfo[colmn].tag.colId EQ COLUMN_TRACEDEST)
       {
           putUint32InArray( setReqData->traceDestination, tlvArray->tlvInfo[colmn].data );
           tlvArray->tlvInfo[colmn].len = sizeof(setReqData->traceDestination);
       }
       if(tlvArray->tlvInfo[colmn].tag.colId EQ COLUMN_TRACEDESTUDPPORT)
       {
           putUint16InArray( setReqData->traceDestUdpPort, tlvArray->tlvInfo[colmn].data );
           tlvArray->tlvInfo[colmn].len = sizeof(setReqData->traceDestUdpPort);
       }
       if(tlvArray->tlvInfo[colmn].tag.colId EQ COLUMN_TRACEDESTIPADDRESS)
       {
           copyBytes (setReqData->traceDestIpAddress, strlen(setReqData->traceDestIpAddress), tlvArray->tlvInfo[colmn].data);
           tlvArray->tlvInfo[colmn].len = MAX_SIZE_TRACEDESTIPADDRESS;
       }
   }

   /* Invoke SIH Set Processor */
   status = sihSetProcessor (sihTagInfo, tlvArray);
   if (status EQ TRUE)
     {
      g_sihTraceConfigSetReqProcessed = TRUE;
      g_sihTraceConfigGetReqProcessed = FALSE;
      /* free the resources */
      free_traceConfigTable();
     }
   return status;
}


/** Initializes the traceConfig module */
void
init_traceConfig(void)
{
  /* here we initialize all the tables we're planning on supporting */
    initialize_table_traceConfigTable();
}

/*  # Determine the first/last column names */

/** Initialize the traceConfigTable table by defining its contents and how it's structured */
void
initialize_table_traceConfigTable(void)
{
    const oid traceConfigTable_oid[] = {1,3,6,1,4,1,9344,501,TRACECONFIG,1};
    const size_t traceConfigTable_oid_len   = OID_LENGTH(traceConfigTable_oid);
    netsnmp_handler_registration    *reg;
    netsnmp_iterator_info           *iinfo;
    netsnmp_table_registration_info *table_info;

    DEBUGMSGTL(("traceConfig:init", "initializing table traceConfigTable\n"));

    reg = netsnmp_create_handler_registration(
              "traceConfigTable",     traceConfigTable_handler,
              traceConfigTable_oid, traceConfigTable_oid_len,
              HANDLER_CAN_RWRITE
              );

    table_info = SNMP_MALLOC_TYPEDEF( netsnmp_table_registration_info );
    netsnmp_table_helper_add_indexes(table_info,
                           ASN_INTEGER,  /* index: traceProfileId */
                           0);
    table_info->min_column = COLUMN_TRACEPROFILEID;
    table_info->max_column = TRACE_CONFIG_MAX_COL;
    
    iinfo = SNMP_MALLOC_TYPEDEF( netsnmp_iterator_info );
    iinfo->get_first_data_point = traceConfigTable_get_first_data_point;
    iinfo->get_next_data_point  = traceConfigTable_get_next_data_point;
    iinfo->table_reginfo        = table_info;
    
    netsnmp_register_table_iterator( reg, iinfo );

    /* Initialise the contents of the table here */
}


#ifndef PWAV_SIH
/* create a new row in the (unsorted) table */
struct traceConfigTable_entry *
traceConfigTable_createEntry(
                 long  traceProfileId,
                ) {
    struct traceConfigTable_entry *entry;

    entry = SNMP_MALLOC_TYPEDEF(struct traceConfigTable_entry);
    if (!entry)
        return NULL;

    entry->traceProfileId = traceProfileId;
    entry->next = traceConfigTable_head;
    traceConfigTable_head = entry;
    return entry;
}

/* remove a row from the table */
void
traceConfigTable_removeEntry( struct traceConfigTable_entry *entry ) {
    struct traceConfigTable_entry *ptr, *prev;

    if (!entry)
        return;    /* Nothing to remove */

    for ( ptr  = traceConfigTable_head, prev = NULL;
          ptr != NULL;
          prev = ptr, ptr = ptr->next ) {
        if ( ptr == entry )
            break;
    }
    if ( !ptr )
        return;    /* Can't find it */

    if ( prev == NULL )
        traceConfigTable_head = ptr->next;
    else
        prev->next = ptr->next;

    SNMP_FREE( entry );   /* XXX - release any other internal resources */
}
#endif /* PWAV_SIH */


/* Example iterator hook routines - using 'get_next' to do most of the work */
netsnmp_variable_list *
traceConfigTable_get_first_data_point(void **my_loop_context,
                          void **my_data_context,
                          netsnmp_variable_list *put_index_data,
                          netsnmp_iterator_info *mydata)
{
    TagInfo sihTagInfo;
    TBOOL status = FALSE;

    if (g_sihTraceConfigGetReqProcessed EQ FALSE)
        {
        /* Set SIH Get Request Tag Info */
        sihTagInfo.tableId = TRACECONFIG;
        sihTagInfo.rowId = CONFIG_ROW_ID_NONE;
        sihTagInfo.colId = CONFIG_COL_ID_NONE;

        status = sihGetProcessor (sihTagInfo);
        if ( status EQ TRUE )
            {
            logInfo("%s","Get Processor Succedded");
            if (traceConfigTable_head)
                {
		g_traceConfigFirstCall = 1;
                g_sihTraceConfigGetReqProcessed = TRUE;
                }
            }
        else
            logError("%s","Get Processor Failed");
        }

    *my_loop_context = traceConfigTable_head;
    return traceConfigTable_get_next_data_point(my_loop_context, my_data_context,
                                    put_index_data,  mydata );
}

netsnmp_variable_list *
traceConfigTable_get_next_data_point(void **my_loop_context,
                          void **my_data_context,
                          netsnmp_variable_list *put_index_data,
                          netsnmp_iterator_info *mydata)
{
    struct traceConfigTable_entry *entry = (struct traceConfigTable_entry *)*my_loop_context;
    netsnmp_variable_list *idx = put_index_data;

    if ( entry ) {
        snmp_set_var_typed_integer( idx, ASN_INTEGER, entry->traceProfileId );
        idx = idx->next_variable;
        *my_data_context = (void *)entry;
        *my_loop_context = (void *)entry->next;
        return put_index_data;
    } else {
        return NULL;
    }
}


/** handles requests for the traceConfigTable table */
int
traceConfigTable_handler(
    netsnmp_mib_handler               *handler,
    netsnmp_handler_registration      *reginfo,
    netsnmp_agent_request_info        *reqinfo,
    netsnmp_request_info              *requests) {

    netsnmp_request_info       *request = NULL;
    netsnmp_table_request_info *table_info = NULL;
    struct traceConfigTable_entry          *table_entry = NULL;
    int loopCount = 0;
    int ret;
    TagInfo sihTagInfo;
    TlvArray tlvArray;
    int rowIndex = 0, colId = 0;
    TBOOL status = FALSE;

    DEBUGMSGTL(("traceConfig:handler", "Processing request (%d)\n", reqinfo->mode));

    switch (reqinfo->mode) {
        /*
         * Read-support (also covers GetNext requests)
         */
    case MODE_GET:
        for (request=requests; request; request=request->next) {

            loopCount++;
            if (request->processed != 0)
            {
	     g_prevTraceConfigReqCount = 0;
             g_sihTraceConfigGetReqProcessed = FALSE;

             /* free the resources */
             free_traceConfigTable();
             /* Here loopCount is decremented, Due to calling of free function two times in SNMP-GETNEXT
              * on single parameter -- 20/07/11 */
             loopCount--;
             continue;
            }

            /* perform anything here that you need to do before each
               request is processed. */

            /* the following extracts the my_data_context pointer set in
               the loop functions above.  You can then use the results to
               help return data for the columns of the enbRegConfigTable table
               in question */

            table_entry = (struct traceConfigTable_entry *)
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
            case COLUMN_TRACEPROFILEID:
                if ( !table_entry ) {
                    netsnmp_set_request_error(reqinfo, request,
                                              SNMP_NOSUCHINSTANCE);
                    continue;
                }
                snmp_set_var_typed_integer( request->requestvb, ASN_INTEGER,
                                            table_entry->traceProfileId);
                break;
            case COLUMN_TRACERRC:
                if ( !table_entry ) {
                    netsnmp_set_request_error(reqinfo, request,
                                              SNMP_NOSUCHINSTANCE);
                    continue;
                }
                snmp_set_var_typed_integer( request->requestvb, ASN_INTEGER,
                                            table_entry->traceRRC);
                break;
            case COLUMN_TRACES1AP:
                if ( !table_entry ) {
                    netsnmp_set_request_error(reqinfo, request,
                                              SNMP_NOSUCHINSTANCE);
                    continue;
                }
                snmp_set_var_typed_integer( request->requestvb, ASN_INTEGER,
                                            table_entry->traceS1AP);
                break;
            case COLUMN_TRACEX2AP:
                if ( !table_entry ) {
                    netsnmp_set_request_error(reqinfo, request,
                                              SNMP_NOSUCHINSTANCE);
                    continue;
                }
                snmp_set_var_typed_integer( request->requestvb, ASN_INTEGER,
                                            table_entry->traceX2AP);
                break;
            case COLUMN_TRACERESERVED1:
                if ( !table_entry ) {
                    netsnmp_set_request_error(reqinfo, request,
                                              SNMP_NOSUCHINSTANCE);
                    continue;
                }
                snmp_set_var_typed_integer( request->requestvb, ASN_INTEGER,
                                            table_entry->traceReserved1);
                break;
            case COLUMN_TRACERESERVED2:
                if ( !table_entry ) {
                    netsnmp_set_request_error(reqinfo, request,
                                              SNMP_NOSUCHINSTANCE);
                    continue;
                }
                snmp_set_var_typed_integer( request->requestvb, ASN_INTEGER,
                                            table_entry->traceReserved2);
                break;
            case COLUMN_TRACERESERVED3:
                if ( !table_entry ) {
                    netsnmp_set_request_error(reqinfo, request,
                                              SNMP_NOSUCHINSTANCE);
                    continue;
                }
                snmp_set_var_typed_integer( request->requestvb, ASN_INTEGER,
                                            table_entry->traceReserved3);
                break;
            case COLUMN_TRACEDEST:
                if ( !table_entry ) {
                    netsnmp_set_request_error(reqinfo, request,
                                              SNMP_NOSUCHINSTANCE);
                    continue;
                }
                snmp_set_var_typed_integer( request->requestvb, ASN_INTEGER,
                                            table_entry->traceDestination);
                break;
            case COLUMN_TRACEDESTUDPPORT:
                if ( !table_entry ) {
                    netsnmp_set_request_error(reqinfo, request,
                                              SNMP_NOSUCHINSTANCE);
                    continue;
                }
                snmp_set_var_typed_integer( request->requestvb, ASN_UNSIGNED,
                                            table_entry->traceDestUdpPort);
                break;
            case COLUMN_TRACEDESTIPADDRESS:
                if ( !table_entry ) {
                    netsnmp_set_request_error(reqinfo, request,
                                              SNMP_NOSUCHINSTANCE);
                    continue;
                }
                snmp_set_var_typed_value( request->requestvb, ASN_OCTET_STR,
                                          table_entry->traceDestIpAddress,
                                          table_entry->traceDestIpAddress_len);
                break;
            default:
                netsnmp_set_request_error(reqinfo, request,
                                          SNMP_NOSUCHOBJECT);
                break;
            }
        }
	if(g_traceConfigFirstCall EQ 1)
         {
            g_traceConfigFirstCall = 0;
            g_prevTraceConfigReqCount = loopCount;
            // This will occur in case of, when SNMP-GET or SNMP-GETNEXT of Single Parameter occurred.
            // In the first call itself loopCount becomes one.
            // In General Case, the loopCount will be always > 1 because every table contains 
            // index column to fetch the row and remaining columns contain table attributes.
            if(loopCount EQ 1)
            {
               //Setting it to false in case of SNMP-GET request for single parameter.
               g_sihTraceConfigGetReqProcessed = FALSE;
               // Free all resources.
               free_traceConfigTable();
            }
         }
         else if(loopCount > 1 || g_prevTraceConfigReqCount > 1)
         {
            //update g_prevReqCount with current loopCount for next call of Table_handler
            g_prevTraceConfigReqCount = loopCount;
         }
         else if(loopCount EQ 1)
         {
            //Setting it to false in case of SNMP-GET request for single parameter.
            g_sihTraceConfigGetReqProcessed = FALSE;
            // Free all resources.
            free_traceConfigTable();
         }
        break;

        /*
         * Write-support
         */
    case MODE_SET_RESERVE1:
#ifndef PWAV_SIH
        for (request=requests; request; request=request->next) {
            table_entry = (struct traceConfigTable_entry *)
                              netsnmp_extract_iterator_context(request);
            table_info  =     netsnmp_extract_table_info(      request);
    
            switch (table_info->colnum) {
            case COLUMN_TRACEPROFILEID:
                /* or possibly 'netsnmp_check_vb_int_range' */
                ret = netsnmp_check_vb_int( request->requestvb );
                if ( ret != SNMP_ERR_NOERROR ) {
                    netsnmp_set_request_error( reqinfo, request, ret );
                    return SNMP_ERR_NOERROR;
                }
                break;
            case COLUMN_TRACERRC:
                /* or possibly 'netsnmp_check_vb_int_range' */
                ret = netsnmp_check_vb_int( request->requestvb );
                if ( ret != SNMP_ERR_NOERROR ) {
                    netsnmp_set_request_error( reqinfo, request, ret );
                    return SNMP_ERR_NOERROR;
                }
                break;
            case COLUMN_TRACES1AP:
                /* or possibly 'netsnmp_check_vb_int_range' */
                ret = netsnmp_check_vb_int( request->requestvb );
                if ( ret != SNMP_ERR_NOERROR ) {
                    netsnmp_set_request_error( reqinfo, request, ret );
                    return SNMP_ERR_NOERROR;
                }
                break;
            case COLUMN_TRACEX2AP:
                /* or possibly 'netsnmp_check_vb_int_range' */
                ret = netsnmp_check_vb_int( request->requestvb );
                if ( ret != SNMP_ERR_NOERROR ) {
                    netsnmp_set_request_error( reqinfo, request, ret );
                    return SNMP_ERR_NOERROR;
                }
                break;
            case COLUMN_TRACERESERVED1:
                /* or possibly 'netsnmp_check_vb_int_range' */
                ret = netsnmp_check_vb_int( request->requestvb );
                if ( ret != SNMP_ERR_NOERROR ) {
                    netsnmp_set_request_error( reqinfo, request, ret );
                    return SNMP_ERR_NOERROR;
                }
                break;
            case COLUMN_TRACERESERVED2:
                /* or possibly 'netsnmp_check_vb_int_range' */
                ret = netsnmp_check_vb_int( request->requestvb );
                if ( ret != SNMP_ERR_NOERROR ) {
                    netsnmp_set_request_error( reqinfo, request, ret );
                    return SNMP_ERR_NOERROR;
                }
                break;
            case COLUMN_TRACERESERVED3:
                /* or possibly 'netsnmp_check_vb_int_range' */
                ret = netsnmp_check_vb_int( request->requestvb );
                if ( ret != SNMP_ERR_NOERROR ) {
                    netsnmp_set_request_error( reqinfo, request, ret );
                    return SNMP_ERR_NOERROR;
                }
                break;
            case COLUMN_TRACEDEST:
                /* or possibly 'netsnmp_check_vb_int_range' */
                ret = netsnmp_check_vb_int( request->requestvb );
                if ( ret != SNMP_ERR_NOERROR ) {
                    netsnmp_set_request_error( reqinfo, request, ret );
                    return SNMP_ERR_NOERROR;
                }
                break;
            case COLUMN_TRACEDESTUDPPORT:
                /* or possibly 'netsnmp_check_vb_int_range' */
                ret = netsnmp_check_vb_int( request->requestvb );
                if ( ret != SNMP_ERR_NOERROR ) {
                    netsnmp_set_request_error( reqinfo, request, ret );
                    return SNMP_ERR_NOERROR;
                }
                break;
            case COLUMN_TRACEDESTIPADDRESS:
                /* or possibly 'netsnmp_check_vb_type_and_size' */
                ret = netsnmp_check_vb_type_and_max_size(
                          request->requestvb, ASN_OCTET_STR, sizeof(table_entry->traceDestIpAddress));
                if ( ret != SNMP_ERR_NOERROR ) {
                    netsnmp_set_request_error( reqinfo, request, ret );
                    return SNMP_ERR_NOERROR;
                }
                break;
            default:
                netsnmp_set_request_error( reqinfo, request,
                                           SNMP_ERR_NOTWRITABLE );
                return SNMP_ERR_NOERROR;
            }
        }
#endif /* PWAV_SIH */
        break;

    case MODE_SET_RESERVE2:
        break;

    case MODE_SET_FREE:
        break;

    case MODE_SET_ACTION:
        g_sihTraceConfigSetReqProcessed = FALSE;

        clearBytes(&sihTagInfo, sizeof(TagInfo));
        clearBytes(&tlvArray, sizeof(TlvArray));

        for (request=requests; request; request=request->next) {
            table_entry = (struct traceConfigTable_entry *)
                              netsnmp_extract_iterator_context(request);
            table_info  =     netsnmp_extract_table_info(      request);

            if(table_entry == NULL){
                logInfo(" %s\n", "table_entry is NULL");
                if (reqinfo->mode == MODE_SET_ACTION) {
                    netsnmp_set_request_error(reqinfo, request,SNMP_NOSUCHINSTANCE);
                    continue;
                }
                /* XXX: no row existed, if you support creation and this is a
                   set, start dealing with it here, else continue */
            }

            if(table_info == NULL){
                logInfo(" %s\n", "table_info is NULL");
                continue;
            }

            colId = table_info->colnum;
            /* set the row index */
            rowIndex = requests->index;   
 
            switch (table_info->colnum) {
            case COLUMN_TRACEPROFILEID:
                table_entry->traceProfileId     = *request->requestvb->val.integer;
                fillTableInfo(&tlvArray.tlvInfo[tlvArray.numTlvs].tag, TRACECONFIG, rowIndex, COLUMN_TRACEPROFILEID);
                tlvArray.numTlvs++;
                break;
            case COLUMN_TRACERRC:
                table_entry->traceRRC     = *request->requestvb->val.integer;
                fillTableInfo(&tlvArray.tlvInfo[tlvArray.numTlvs].tag, TRACECONFIG, rowIndex, COLUMN_TRACERRC);
                tlvArray.numTlvs++;
                break;
            case COLUMN_TRACES1AP:
                table_entry->traceS1AP     = *request->requestvb->val.integer;
                fillTableInfo(&tlvArray.tlvInfo[tlvArray.numTlvs].tag, TRACECONFIG, rowIndex, COLUMN_TRACES1AP);
                tlvArray.numTlvs++;
                break;
            case COLUMN_TRACEX2AP:
                table_entry->traceX2AP     = *request->requestvb->val.integer;
                fillTableInfo(&tlvArray.tlvInfo[tlvArray.numTlvs].tag, TRACECONFIG, rowIndex, COLUMN_TRACEX2AP);
                tlvArray.numTlvs++;
                break;
            case COLUMN_TRACERESERVED1:
                table_entry->traceReserved1     = *request->requestvb->val.integer;
                fillTableInfo(&tlvArray.tlvInfo[tlvArray.numTlvs].tag, TRACECONFIG, rowIndex, COLUMN_TRACERESERVED1);
                tlvArray.numTlvs++;
                break;
            case COLUMN_TRACERESERVED2:
                table_entry->traceReserved2     = *request->requestvb->val.integer;
                fillTableInfo(&tlvArray.tlvInfo[tlvArray.numTlvs].tag, TRACECONFIG, rowIndex, COLUMN_TRACERESERVED2);
                tlvArray.numTlvs++;
                break;
            case COLUMN_TRACERESERVED3:
                table_entry->traceReserved3     = *request->requestvb->val.integer;
                fillTableInfo(&tlvArray.tlvInfo[tlvArray.numTlvs].tag, TRACECONFIG, rowIndex, COLUMN_TRACERESERVED3);
                tlvArray.numTlvs++;
                break;
            case COLUMN_TRACEDEST:
                table_entry->traceDestination     = *request->requestvb->val.integer;
                fillTableInfo(&tlvArray.tlvInfo[tlvArray.numTlvs].tag, TRACECONFIG, rowIndex, COLUMN_TRACEDEST);
                tlvArray.numTlvs++;
                break;
            case COLUMN_TRACEDESTUDPPORT:
                table_entry->traceDestUdpPort     = *request->requestvb->val.integer;
                fillTableInfo(&tlvArray.tlvInfo[tlvArray.numTlvs].tag, TRACECONFIG, rowIndex, COLUMN_TRACEDESTUDPPORT);
                tlvArray.numTlvs++;
                break;
            case COLUMN_TRACEDESTIPADDRESS:
                memset( table_entry->traceDestIpAddress, 0,
                        sizeof(table_entry->traceDestIpAddress));
                memcpy( table_entry->traceDestIpAddress,
                        request->requestvb->val.string,
                        request->requestvb->val_len);
                table_entry->traceDestIpAddress_len =
                        request->requestvb->val_len;
                fillTableInfo(&tlvArray.tlvInfo[tlvArray.numTlvs].tag, TRACECONFIG, rowIndex, COLUMN_TRACEDESTIPADDRESS);
                tlvArray.numTlvs++;
                break;
            }
        }

        if ((request == NULL) && (g_sihTraceConfigSetReqProcessed EQ FALSE))
            /* Processed all requests and table entry is filled with
             * requested data. */
            {
            /* Set SIH Set Request Tag Info */
            sihTagInfo.tableId = TRACECONFIG;

            sihTagInfo.rowId = rowIndex;

            status = sendTraceConfigSnmpSetRequest (sihTagInfo, &tlvArray);
            if (status EQ TRUE)
                {
                syslog (LOG_INFO, "SET Succedded");
                }
            else
                netsnmp_set_request_error(reqinfo, requests, SNMP_ERR_GENERR );
            }

        break;

    case MODE_SET_UNDO:
#ifndef PWAV_SIH
        for (request=requests; request; request=request->next) {
            table_entry = (struct traceConfigTable_entry *)
                              netsnmp_extract_iterator_context(request);
            table_info  =     netsnmp_extract_table_info(      request);
    
            switch (table_info->colnum) {
            case COLUMN_TRACEPROFILEID:
                table_entry->traceProfileId     = table_entry->old_traceProfileId;
                table_entry->old_traceProfileId = 0;
                break;
            case COLUMN_TRACERRC:
                table_entry->traceRRC     = table_entry->old_traceRRC;
                table_entry->old_traceRRC = 0;
                break;
            case COLUMN_TRACES1AP:
                table_entry->traceS1AP     = table_entry->old_traceS1AP;
                table_entry->old_traceS1AP = 0;
                break;
            case COLUMN_TRACEX2AP:
                table_entry->traceX2AP     = table_entry->old_tracePX2AP
                table_entry->old_traceX2AP = 0;
                break;
            case COLUMN_TRACERESERVED1:
                table_entry->traceReserved1     = table_entry->old_traceReserved1;
                table_entry->old_traceReserved1 = 0;
                break;
            case COLUMN_TRACERESERVED2:
                table_entry->traceReserved2     = table_entry->old_traceReserved2;
                table_entry->old_traceReserved2 = 0;
                break;
            case COLUMN_TRACERESERVED3:
                table_entry->traceReserved3     = table_entry->old_traceReserved3;
                table_entry->old_traceReserved3 = 0;
                break;
            case COLUMN_TRACEDEST:
                table_entry->traceDestination     = table_entry->old_traceDestination;
                table_entry->old_traceDestination = 0;
                break;
            case COLUMN_TRACEDESTUDPPORT:
                table_entry->traceDestUdpPort     = table_entry->old_traceDestUdpPort;
                table_entry->old_traceDestUdpPort = 0;
                break;
            case COLUMN_TRACEDESTIPADDRESS:
                memcpy( table_entry->traceDestIpAddress,
                        table_entry->old_traceDestIpAddress,
                        sizeof(table_entry->traceDestIpAddress));
                memset( table_entry->old_traceDestIpAddress, 0,
                        sizeof(table_entry->traceDestIpAddress));
                table_entry->traceDestIpAddress_len =
                        table_entry->old_traceDestIpAddress_len;
                break;
            }
        }
#endif /* PWAV_SIH */
        break;

    case MODE_SET_COMMIT:
        break;
    }
    return SNMP_ERR_NOERROR;
}