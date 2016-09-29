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
#include "moduleStatus.h"

/* function declarations */
Netsnmp_Node_Handler moduleStatusTable_handler;
Netsnmp_First_Data_Point  moduleStatusTable_get_first_data_point;
Netsnmp_Next_Data_Point   moduleStatusTable_get_next_data_point;

/* globals */
struct moduleStatusTable_entry  *moduleStatusTable_head = NULL;
static TBOOL g_sihModuleStatusGetReqProcessed = FALSE;
static UINT32 g_prevModuleStatusReqCount = 0;
static UINT32 g_moduleStatusFirstCall = 0;

/* free_moduleStatusTable */
/**
* @details 
*          This is the routine which frees the module status entry.
*/
void free_moduleStatusTable(void)
{
  struct moduleStatusTable_entry *current = moduleStatusTable_head;
  struct moduleStatusTable_entry *temp = NULL;

  if(current)
  {
    while(current->next != NULL)
    {
        temp = current->next;
        SNMP_FREE( current );
        current = temp;
    }

    SNMP_FREE( current );
    moduleStatusTable_head = NULL;
  }
  return;
}


/**
* @details 
*          This is the routine which handles get response from SIH Get Processor
*          and prepare the SNMP GET response for EMS.
* @param tlvArray contains pointer to TLV array.
*/
TBOOL sendModuleStatusSnmpGetResponse (
  IO TlvArray    *tlvArray
  )
{
  int  totalNumRow, totalNumCol, i=0, colmn=0, pos=0;
  struct moduleStatusTable_entry * prevPtr = NULL;

  printTLV(tlvArray);

  /* Total no. of Colmns */
  totalNumCol = MODULE_STATUS_MAX_COL;
  /* Total no. of Row = (No. of TLV)/(No. of Columns) */
  totalNumRow = tlvArray->numTlvs/totalNumCol;

  logInfo("Total No. of Row %d & Col %d", totalNumRow, totalNumCol);
  for (i=1;  i <= totalNumRow;  i++)
      {
      struct moduleStatusTable_entry *ptr=(struct moduleStatusTable_entry *)malloc(sizeof(struct moduleStatusTable_entry));

      clearBytes (ptr, sizeof(struct moduleStatusTable_entry));

      /* Fill Data in TLV */
      for(colmn = 1 ; colmn <= totalNumCol ; colmn++, pos++)
        {
        if (colmn EQ COLUMN_MODULE_STATUS_INDEX)
           {
            ptr->index =  getUint32FromArray(tlvArray->tlvInfo[pos].data);
            continue;
           }
        if (colmn EQ COLUMN_MODULENAME)
           {
            ptr->moduleName =  getUint32FromArray(tlvArray->tlvInfo[pos].data);
            continue;
           }
        if (colmn EQ COLUMN_MODULEDESC)
           {
            clearBytes (ptr->moduleDesc, MODULEDESC_BUFFER_LEN_MAX);
            copyBytes (tlvArray->tlvInfo[pos].data, tlvArray->tlvInfo[pos].len, ptr->moduleDesc);
            ptr->moduleDesc_len = strlen(ptr->moduleDesc);
            continue;
           }
        if (colmn EQ COLUMN_MODULE_STATUS_VERSION)
           {
            clearBytes (ptr->version, MODULE_STATUS_VERSION_BUFFER_LEN_MAX);
            copyBytes (tlvArray->tlvInfo[pos].data, tlvArray->tlvInfo[pos].len, ptr->version);
            ptr->version_len = strlen(ptr->version);
            continue;
           }
        if (colmn EQ COLUMN_ISRUNNING)
           {
            ptr->isRunning =  getUint8FromArray(tlvArray->tlvInfo[pos].data);
            continue;
           }
        if (colmn EQ COLUMN_PROCEESSORTASKID)
           {
            ptr->proceessOrTaskId =  getUint32FromArray(tlvArray->tlvInfo[pos].data);
            continue;
           }
        if (colmn EQ COLUMN_MEMUSAGE)
           {
            ptr->memUsage =  getUint32FromArray(tlvArray->tlvInfo[pos].data);
            continue;
           }
        if (colmn EQ COLUMN_STARTEDAT)
           {
            clearBytes (ptr->startedAt, STARTEDAT_BUFFER_LEN_MAX);
            copyBytes (tlvArray->tlvInfo[pos].data, tlvArray->tlvInfo[pos].len, ptr->startedAt);
            ptr->startedAt_len = strlen(ptr->startedAt);
            continue;
           }
        if (colmn EQ COLUMN_NUMRESTARTSDUETOERRORS)
           {
            ptr->numRestartsDueToErrors =  getUint32FromArray(tlvArray->tlvInfo[pos].data);
            continue;
           }
        if (colmn EQ COLUMN_RESTARTREASON)
           {
            ptr->restartReason =  getUint32FromArray(tlvArray->tlvInfo[pos].data);
            continue;
           }
        if (colmn EQ COLUMN_LASTHEALTHCHECKAT)
           {
            clearBytes (ptr->lastHealthCheckAt, LASTHEALTHCHECKAT_BUFFER_LEN_MAX);
            copyBytes (tlvArray->tlvInfo[pos].data, tlvArray->tlvInfo[pos].len, ptr->lastHealthCheckAt);
            ptr->lastHealthCheckAt_len = strlen(ptr->lastHealthCheckAt);
            continue;
           }
        }

       ptr->next = NULL;

      if (prevPtr == NULL) {
           moduleStatusTable_head =  prevPtr = ptr;
       }

      prevPtr->next = ptr;
      prevPtr = ptr;
     }
    prevPtr->next = NULL;

    if (moduleStatusTable_head)
        return TRUE;
    else
        return FALSE;
}


/** Initializes the moduleStatus module */
void
init_moduleStatus(void)
{
  /* here we initialize all the tables we're planning on supporting */
    initialize_table_moduleStatusTable();
}

/*  # Determine the first/last column names */

/** Initialize the moduleStatusTable table by defining its contents and how it's structured */
void
initialize_table_moduleStatusTable(void)
{
    const oid moduleStatusTable_oid[] = {1,3,6,1,4,1,9344,501,MODULESTATUS,1};
    const size_t moduleStatusTable_oid_len   = OID_LENGTH(moduleStatusTable_oid);
    netsnmp_handler_registration    *reg;
    netsnmp_iterator_info           *iinfo;
    netsnmp_table_registration_info *table_info;

    DEBUGMSGTL(("moduleStatus:init", "initializing table moduleStatusTable\n"));

    reg = netsnmp_create_handler_registration(
              "moduleStatusTable",     moduleStatusTable_handler,
              moduleStatusTable_oid, moduleStatusTable_oid_len,
              HANDLER_CAN_RONLY
              );

    table_info = SNMP_MALLOC_TYPEDEF( netsnmp_table_registration_info );
    netsnmp_table_helper_add_indexes(table_info,
                           ASN_INTEGER,  /* index: moduleName */
                           0);
    table_info->min_column = COLUMN_MODULE_STATUS_INDEX;
    table_info->max_column = COLUMN_LASTHEALTHCHECKAT;
    
    iinfo = SNMP_MALLOC_TYPEDEF( netsnmp_iterator_info );
    iinfo->get_first_data_point = moduleStatusTable_get_first_data_point;
    iinfo->get_next_data_point  = moduleStatusTable_get_next_data_point;
    iinfo->table_reginfo        = table_info;
    
    netsnmp_register_table_iterator( reg, iinfo );

    /* Initialise the contents of the table here */
}


#ifndef PWAV_SIH
/* create a new row in the (unsorted) table */
struct moduleStatusTable_entry *
moduleStatusTable_createEntry(
                 long  moduleName,
                ) {
    struct moduleStatusTable_entry *entry;

    entry = SNMP_MALLOC_TYPEDEF(struct moduleStatusTable_entry);
    if (!entry)
        return NULL;

    entry->moduleName = moduleName;
    entry->next = moduleStatusTable_head;
    moduleStatusTable_head = entry;
    return entry;
}

/* remove a row from the table */
void
moduleStatusTable_removeEntry( struct moduleStatusTable_entry *entry ) {
    struct moduleStatusTable_entry *ptr, *prev;

    if (!entry)
        return;    /* Nothing to remove */

    for ( ptr  = moduleStatusTable_head, prev = NULL;
          ptr != NULL;
          prev = ptr, ptr = ptr->next ) {
        if ( ptr == entry )
            break;
    }
    if ( !ptr )
        return;    /* Can't find it */

    if ( prev == NULL )
        moduleStatusTable_head = ptr->next;
    else
        prev->next = ptr->next;

    SNMP_FREE( entry );   /* XXX - release any other internal resources */
}
#endif /* PWAV_SIH */


/* Example iterator hook routines - using 'get_next' to do most of the work */
netsnmp_variable_list *
moduleStatusTable_get_first_data_point(void **my_loop_context,
                          void **my_data_context,
                          netsnmp_variable_list *put_index_data,
                          netsnmp_iterator_info *mydata)
{
    TagInfo sihTagInfo;
    TBOOL status = FALSE;


    if (g_sihModuleStatusGetReqProcessed EQ FALSE)
        {
        /* Set SIH Get Request Tag Info */
        sihTagInfo.tableId = MODULESTATUS;
        sihTagInfo.rowId = CONFIG_ROW_ID_NONE;
        sihTagInfo.colId = CONFIG_COL_ID_NONE;

        status = sihGetProcessor (sihTagInfo);
        if ( status EQ TRUE )
            {
            logInfo("%s","Get Processor Succedded");
            if (moduleStatusTable_head)
                {
                g_moduleStatusFirstCall = 1;
                g_sihModuleStatusGetReqProcessed = TRUE;
                }
            }
        else
            logError("Get Processor Failed",NULL);
        }

    *my_loop_context = moduleStatusTable_head;
    return moduleStatusTable_get_next_data_point(my_loop_context, my_data_context,
                                    put_index_data,  mydata );
}

netsnmp_variable_list *
moduleStatusTable_get_next_data_point(void **my_loop_context,
                          void **my_data_context,
                          netsnmp_variable_list *put_index_data,
                          netsnmp_iterator_info *mydata)
{
    struct moduleStatusTable_entry *entry = (struct moduleStatusTable_entry *)*my_loop_context;
    netsnmp_variable_list *idx = put_index_data;

    if ( entry ) {
        snmp_set_var_typed_integer( idx, ASN_INTEGER, entry->moduleName );
        idx = idx->next_variable;
        *my_data_context = (void *)entry;
        *my_loop_context = (void *)entry->next;
        return put_index_data;
    } else {
        return NULL;
    }
}


/** handles requests for the moduleStatusTable table */
int
moduleStatusTable_handler(
    netsnmp_mib_handler               *handler,
    netsnmp_handler_registration      *reginfo,
    netsnmp_agent_request_info        *reqinfo,
    netsnmp_request_info              *requests) {

    netsnmp_request_info       *request = NULL;
    netsnmp_table_request_info *table_info = NULL;
    struct moduleStatusTable_entry          *table_entry = NULL;
    int loopCount = 0;

    DEBUGMSGTL(("moduleStatus:handler", "Processing request (%d)\n", reqinfo->mode));

    switch (reqinfo->mode) {
        /*
         * Read-support (also covers GetNext requests)
         */
    case MODE_GET:
        for (request=requests; request; request=request->next) {

            loopCount++;
            if (request->processed != 0) /*Processed all requests, rows and colmns */
                {
                g_prevModuleStatusReqCount = 0;
                g_sihModuleStatusGetReqProcessed = FALSE;
                /* free the resources */
                free_moduleStatusTable();
                /* Here loopCount is decremented, Due to calling of free function two times in SNMP-GETNEXT
                 * on single parameter -- 20/07/11 */
                loopCount--;
                continue;
                }

            /* perform anything here that you need to do before each
               request is processed. */

            /* the following extracts the my_data_context pointer set in
               the loop functions above.  You can then use the results to
               help return data for the columns of the traceConfigTable table
               in question */

            table_entry = (struct moduleStatusTable_entry *)
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
            case COLUMN_MODULE_STATUS_INDEX:
                if ( !table_entry ) {
                    netsnmp_set_request_error(reqinfo, request,
                                              SNMP_NOSUCHINSTANCE);
                    continue;
                }
                snmp_set_var_typed_integer( request->requestvb, ASN_UNSIGNED,
                                            table_entry->index);
                break;
            case COLUMN_MODULENAME:
                if ( !table_entry ) {
                    netsnmp_set_request_error(reqinfo, request,
                                              SNMP_NOSUCHINSTANCE);
                    continue;
                }
                snmp_set_var_typed_integer( request->requestvb, ASN_INTEGER,
                                            table_entry->moduleName);
                break;
            case COLUMN_MODULEDESC:
                if ( !table_entry ) {
                    netsnmp_set_request_error(reqinfo, request,
                                              SNMP_NOSUCHINSTANCE);
                    continue;
                }
                snmp_set_var_typed_value( request->requestvb, ASN_OCTET_STR,
                                          table_entry->moduleDesc,
                                          table_entry->moduleDesc_len);
                break;
            case COLUMN_MODULE_STATUS_VERSION:
                if ( !table_entry ) {
                    netsnmp_set_request_error(reqinfo, request,
                                              SNMP_NOSUCHINSTANCE);
                    continue;
                }
                snmp_set_var_typed_value( request->requestvb, ASN_OCTET_STR,
                                          table_entry->version,
                                          table_entry->version_len);
                break;
            case COLUMN_ISRUNNING:
                if ( !table_entry ) {
                    netsnmp_set_request_error(reqinfo, request,
                                              SNMP_NOSUCHINSTANCE);
                    continue;
                }
                snmp_set_var_typed_integer( request->requestvb, ASN_INTEGER,
                                            table_entry->isRunning);
                break;
            case COLUMN_PROCEESSORTASKID:
                if ( !table_entry ) {
                    netsnmp_set_request_error(reqinfo, request,
                                              SNMP_NOSUCHINSTANCE);
                    continue;
                }
                snmp_set_var_typed_integer( request->requestvb, ASN_UNSIGNED,
                                            table_entry->proceessOrTaskId);
                break;
            case COLUMN_MEMUSAGE:
                if ( !table_entry ) {
                    netsnmp_set_request_error(reqinfo, request,
                                              SNMP_NOSUCHINSTANCE);
                    continue;
                }
                snmp_set_var_typed_integer( request->requestvb, ASN_UNSIGNED,
                                            table_entry->memUsage);
                break;
            case COLUMN_STARTEDAT:
                if ( !table_entry ) {
                    netsnmp_set_request_error(reqinfo, request,
                                              SNMP_NOSUCHINSTANCE);
                    continue;
                }
                snmp_set_var_typed_value( request->requestvb, ASN_OCTET_STR,
                                          table_entry->startedAt,
                                          table_entry->startedAt_len);
                break;
            case COLUMN_NUMRESTARTSDUETOERRORS:
                if ( !table_entry ) {
                    netsnmp_set_request_error(reqinfo, request,
                                              SNMP_NOSUCHINSTANCE);
                    continue;
                }
                snmp_set_var_typed_integer( request->requestvb, ASN_GAUGE,
                                            table_entry->numRestartsDueToErrors);
                break;
            case COLUMN_RESTARTREASON:
                if ( !table_entry ) {
                    netsnmp_set_request_error(reqinfo, request,
                                              SNMP_NOSUCHINSTANCE);
                    continue;
                }
                snmp_set_var_typed_integer( request->requestvb, ASN_INTEGER,
                                            table_entry->restartReason);
                break;
            case COLUMN_LASTHEALTHCHECKAT:
                if ( !table_entry ) {
                    netsnmp_set_request_error(reqinfo, request,
                                              SNMP_NOSUCHINSTANCE);
                    continue;
                }
                snmp_set_var_typed_value( request->requestvb, ASN_OCTET_STR,
                                          table_entry->lastHealthCheckAt,
                                          table_entry->lastHealthCheckAt_len);
                break;
            default:
                netsnmp_set_request_error(reqinfo, request,
                                          SNMP_NOSUCHOBJECT);
                break;
            }
        }
         if(g_moduleStatusFirstCall EQ 1)
        {
           g_moduleStatusFirstCall = 0;
           g_prevModuleStatusReqCount = loopCount;
          // This will occur in case of, when SNMP-GET or SNMP-GETNEXT of Single Parameter occurred.
          // In the first call itself loopCount becomes one.
          // In General Case, the loopCount will be always > 1 because every table contains index column to fetch the row
          // and remaining columns contain table attributes.
          if(loopCount EQ 1)
          {
            //Setting it to false in case of SNMP-GET request for single parameter.
            g_sihModuleStatusGetReqProcessed = FALSE;
            // Free all resources.
            free_moduleStatusTable();
          }
        }
        else if(loopCount > 1 || g_prevModuleStatusReqCount > 1)
        {
          //update g_prevReqCount with current loopCount for next call of Table_handler
          g_prevModuleStatusReqCount = loopCount;
          //nothing to be done because previous request contain more than one column.
        }
        else if(loopCount EQ 1)
        {
          //Setting it to false in case of SNMP-GET request for single parameter.
          g_sihModuleStatusGetReqProcessed = FALSE;
          // Free all resources.
          free_moduleStatusTable();
        }
        break;

    }
    return SNMP_ERR_NOERROR;
}
