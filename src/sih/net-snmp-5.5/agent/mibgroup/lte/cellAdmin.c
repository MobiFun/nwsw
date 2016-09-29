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

#include "cellAdmin.h"

/* Handlers */
Netsnmp_Node_Handler cellAdminTable_handler;
Netsnmp_First_Data_Point  cellAdminTable_get_first_data_point;
Netsnmp_Next_Data_Point   cellAdminTable_get_next_data_point;

/* Globals */
struct cellAdminTable_entry  *cellAdminTable_head = NULL;
static TBOOL g_sihCellAdminSetReqProcessed = FALSE;
static TBOOL g_sihCellAdminGetReqProcessed = FALSE;
static UINT32 g_prevCellAdminReqCount = 0;
static UINT32 g_cellAdminFirstCall = 0;

/* free_cellAdminTable */
/**
* @details 
*          This is the routine which frees the cell admin entry.
*/
void free_cellAdminTable(void)
{
  struct cellAdminTable_entry *current = cellAdminTable_head;

  if (current)
  {
  	for (current = cellAdminTable_head->next ; current != NULL; current = current->next) {
    		SNMP_FREE( current );
    	}

  	SNMP_FREE(cellAdminTable_head);
  	cellAdminTable_head = NULL;
  }
  return;
}

/**
* @details 
*          This is the routine which handles get response from SIH Get Processor
*          and prepare the SNMP GET response for EMS.
* @param tlvArray contains pointer to TLV array.
*/
TBOOL sendCellAdminSnmpGetResponse (
  IO TlvArray    *tlvArray
  )
{
  int  totalNumRow, totalNumCol, i=0, colmn=0, pos=0;
  struct cellAdminTable_entry * prevPtr = NULL;

  printTLV(tlvArray);

  /* Total no. of Colmns */
  totalNumCol = CELLADMIN_MAX_COL;
  /* Total no. of Row = (No. of TLV)/(No. of Columns) */
  totalNumRow = tlvArray->numTlvs/totalNumCol;

  logInfo("Total No. of Row %d & Col %d", totalNumRow, totalNumCol);
  for (i=1;  i <= totalNumRow;  i++)
      {
      struct cellAdminTable_entry *ptr=(struct cellAdminTable_entry *)malloc(sizeof(struct cellAdminTable_entry));

      clearBytes (ptr, sizeof(struct cellAdminTable_entry));

      /* Fill Data in TLV */
      for(colmn = 1 ; colmn <= totalNumCol ; colmn++, pos++)
        {
        if (colmn EQ COLUMN_CELLADMIN_INDEX)
           {
            ptr->index =  getUint32FromArray(tlvArray->tlvInfo[pos].data);
            continue;
           }
        if (colmn EQ COLUMN_CELLADMIN_CELLID)
           {
            ptr->cellId =  getUint32FromArray(tlvArray->tlvInfo[pos].data);
            continue;
           }
        if (colmn EQ COLUMN_CELLADMINCMD)
           {
            ptr->cellAdminCmd =  getUint32FromArray(tlvArray->tlvInfo[pos].data);
            continue;
           }
        }

       ptr->next = NULL;

      if (prevPtr == NULL) {
           cellAdminTable_head =  prevPtr = ptr;
       }

      prevPtr->next = ptr;
      prevPtr = ptr;
     }
    prevPtr->next = NULL;

    if (cellAdminTable_head)
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
TBOOL sendCellAdminSnmpSetRequest (
      TagInfo  sihTagInfo
      )
{
   TlvArray tlvArray;
   cellAdminTableEntry * setReqData = NULL;
   TBOOL status = FALSE;
   int colmn = 0, maxCols = 0;

   clearBytes(&tlvArray,sizeof(tlvArray));

   setReqData = cellAdminTable_head;

  /* max columns in row */
   maxCols = CELLADMIN_MAX_COL;

   /* Set the Tags */
   if (sihTagInfo.colId)
   {
    /* Tag for setting an element in table */
    tlvArray.tlvInfo[0].tag = sihTagInfo;
   }
   else
   {
   /* Tag for setting a row in table */
   for(colmn = 1 ; colmn <= maxCols ; colmn++)
      {
      tlvArray.tlvInfo[colmn-1].tag.tableId  = sihTagInfo.tableId;
      tlvArray.tlvInfo[colmn-1].tag.rowId    = sihTagInfo.rowId;
      tlvArray.tlvInfo[colmn-1].tag.colId    = colmn;
      }
    }

   /* Fill Data in TLV */
   for(colmn = 1; colmn <= maxCols ; colmn++)
      {
       if (((sihTagInfo.colId EQ CONFIG_COL_ID_NONE) &&
           (colmn EQ COLUMN_CELLADMIN_INDEX)) ||
           (sihTagInfo.colId EQ COLUMN_CELLADMIN_INDEX))
           {
           putUint32InArray( setReqData->index, tlvArray.tlvInfo[colmn-1].data );
           tlvArray.tlvInfo[colmn-1].len = sizeof(setReqData->index);
           tlvArray.numTlvs++;
           if (sihTagInfo.colId EQ COLUMN_CELLADMIN_INDEX)
               break;
           }
       if (((sihTagInfo.colId EQ CONFIG_COL_ID_NONE) &&
           (colmn EQ COLUMN_CELLADMIN_CELLID)) ||
           (sihTagInfo.colId EQ COLUMN_CELLADMIN_CELLID))
           {
           putUint32InArray( setReqData->cellId, tlvArray.tlvInfo[colmn-1].data );
           tlvArray.tlvInfo[colmn-1].len = sizeof(setReqData->cellId);
           tlvArray.numTlvs++;
           if (sihTagInfo.colId EQ COLUMN_CELLADMIN_CELLID)
               break;
           }
       if (((sihTagInfo.colId EQ CONFIG_COL_ID_NONE) &&
           (colmn EQ COLUMN_CELLADMINCMD)) ||
           (sihTagInfo.colId EQ COLUMN_CELLADMINCMD))
           {
           putUint32InArray( setReqData->cellAdminCmd, tlvArray.tlvInfo[colmn-1].data );
           tlvArray.tlvInfo[colmn-1].len = sizeof(setReqData->cellAdminCmd);
           tlvArray.numTlvs++;
           if (sihTagInfo.colId EQ COLUMN_CELLADMINCMD)
               break;
           }
       }

   /* Invoke SIH Set Processor */
   status = sihSetProcessor (sihTagInfo,  &tlvArray);
   if (status EQ TRUE)
     {
      g_sihCellAdminSetReqProcessed = TRUE;
      g_sihCellAdminGetReqProcessed = FALSE;
      /* free the resources */
      free_cellAdminTable();
     }
   return status;
}

/** Initializes the cellAdmin module */
void
init_cellAdmin(void)
{
  /* here we initialize all the tables we're planning on supporting */
    initialize_table_cellAdminTable();
}

/*  # Determine the first/last column names */

/** Initialize the cellAdminTable table by defining its contents and how it's structured */
void
initialize_table_cellAdminTable(void)
{
    const oid cellAdminTable_oid[] = {1,3,6,1,4,1,9344,501,CELLADMIN,1};
    const size_t cellAdminTable_oid_len   = OID_LENGTH(cellAdminTable_oid);
    netsnmp_handler_registration    *reg;
    netsnmp_iterator_info           *iinfo;
    netsnmp_table_registration_info *table_info;

    DEBUGMSGTL(("cellAdmin:init", "initializing table cellAdminTable\n"));

    reg = netsnmp_create_handler_registration(
              "cellAdminTable",     cellAdminTable_handler,
              cellAdminTable_oid, cellAdminTable_oid_len,
              HANDLER_CAN_RWRITE
              );

    table_info = SNMP_MALLOC_TYPEDEF( netsnmp_table_registration_info );
    netsnmp_table_helper_add_indexes(table_info,
                           ASN_UNSIGNED,  /* index: index */
                           0);
    table_info->min_column = COLUMN_CELLADMIN_INDEX;
    table_info->max_column = COLUMN_CELLADMINCMD;
    
    iinfo = SNMP_MALLOC_TYPEDEF( netsnmp_iterator_info );
    iinfo->get_first_data_point = cellAdminTable_get_first_data_point;
    iinfo->get_next_data_point  = cellAdminTable_get_next_data_point;
    iinfo->table_reginfo        = table_info;
    
    netsnmp_register_table_iterator( reg, iinfo );

    /* Initialise the contents of the table here */
}


#ifndef PWAV_SIH
/* create a new row in the (unsorted) table */
struct cellAdminTable_entry *
cellAdminTable_createEntry(
                 u_long  index,
                ) {
    struct cellAdminTable_entry *entry;

    entry = SNMP_MALLOC_TYPEDEF(struct cellAdminTable_entry);
    if (!entry)
        return NULL;

    entry->index = index;
    entry->next = cellAdminTable_head;
    cellAdminTable_head = entry;
    return entry;
}

/* remove a row from the table */
void
cellAdminTable_removeEntry( struct cellAdminTable_entry *entry ) {
    struct cellAdminTable_entry *ptr, *prev;

    if (!entry)
        return;    /* Nothing to remove */

    for ( ptr  = cellAdminTable_head, prev = NULL;
          ptr != NULL;
          prev = ptr, ptr = ptr->next ) {
        if ( ptr == entry )
            break;
    }
    if ( !ptr )
        return;    /* Can't find it */

    if ( prev == NULL )
        cellAdminTable_head = ptr->next;
    else
        prev->next = ptr->next;

    SNMP_FREE( entry );   /* XXX - release any other internal resources */
}
#endif /* PWAV_SIH */


/* Example iterator hook routines - using 'get_next' to do most of the work */
netsnmp_variable_list *
cellAdminTable_get_first_data_point(void **my_loop_context,
                          void **my_data_context,
                          netsnmp_variable_list *put_index_data,
                          netsnmp_iterator_info *mydata)
{
    TagInfo sihTagInfo;
    TBOOL status = FALSE;

    if (g_sihCellAdminGetReqProcessed EQ FALSE)
        {
        /* Set SIH Get Request Tag Info */
        sihTagInfo.tableId = CELLADMIN;
        sihTagInfo.rowId = CONFIG_ROW_ID_NONE;
        sihTagInfo.colId = CONFIG_COL_ID_NONE;

        status = sihGetProcessor (sihTagInfo);
        if ( status EQ TRUE )
            {
            logInfo("%s","Get Processor Succedded");
            if (cellAdminTable_head)
                {
                g_cellAdminFirstCall = 1;
                g_sihCellAdminGetReqProcessed = TRUE;
                }
            }
        else
            logError("%s","Get Processor Failed");
        }

    *my_loop_context = cellAdminTable_head;
    return cellAdminTable_get_next_data_point(my_loop_context, my_data_context,
                                    put_index_data,  mydata );
}

netsnmp_variable_list *
cellAdminTable_get_next_data_point(void **my_loop_context,
                          void **my_data_context,
                          netsnmp_variable_list *put_index_data,
                          netsnmp_iterator_info *mydata)
{
    struct cellAdminTable_entry *entry = (struct cellAdminTable_entry *)*my_loop_context;
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


/** handles requests for the cellAdminTable table */
int
cellAdminTable_handler(
    netsnmp_mib_handler               *handler,
    netsnmp_handler_registration      *reginfo,
    netsnmp_agent_request_info        *reqinfo,
    netsnmp_request_info              *requests) {

    netsnmp_request_info       *request = NULL;
    netsnmp_table_request_info *table_info = NULL;
    struct cellAdminTable_entry          *table_entry = NULL;
    int loopCount = 0;
    int ret;
    TagInfo sihTagInfo;
    int noofColms = 0, rowIndex = 0, colId = 0;
    TBOOL status = FALSE;

    logInfo("cellAdmin:handler Processing request (%d)\n", reqinfo->mode);

    switch (reqinfo->mode) {
        /*
         * Read-support (also covers GetNext requests)
         */
    case MODE_GET:
        for (request=requests; request; request=request->next) {

            loopCount++;
            if (request->processed != 0)
            {
             g_prevCellAdminReqCount = 0;
             g_sihCellAdminGetReqProcessed = FALSE;

             /* free the resources */
             free_cellAdminTable();
             /* Here loopCount is decremented, Due to calling of free function two times in SNMP-GETNEXT
              * on single parameter -- 20/07/11 */
             loopCount--;
             continue;
            }

            /* perform anything here that you need to do before each
               request is processed. */

            /* the following extracts the my_data_context pointer set in
               the loop functions above.  You can then use the results to
               help return data for the columns of the enodebConfigTable table
               in question */

            table_entry = (struct cellAdminTable_entry *)
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
            case COLUMN_CELLADMIN_INDEX:
                if ( !table_entry ) {
                    netsnmp_set_request_error(reqinfo, request,
                                              SNMP_NOSUCHINSTANCE);
                    continue;
                }
                snmp_set_var_typed_integer( request->requestvb, ASN_UNSIGNED,
                                            table_entry->index);
                break;
            case COLUMN_CELLADMIN_CELLID:
                if ( !table_entry ) {
                    netsnmp_set_request_error(reqinfo, request,
                                              SNMP_NOSUCHINSTANCE);
                    continue;
                }
                snmp_set_var_typed_integer( request->requestvb, ASN_INTEGER,
                                            table_entry->cellId);
                break;
            case COLUMN_CELLADMINCMD:
                if ( !table_entry ) {
                    netsnmp_set_request_error(reqinfo, request,
                                              SNMP_NOSUCHINSTANCE);
                    continue;
                }
                snmp_set_var_typed_integer( request->requestvb, ASN_INTEGER,
                                            table_entry->cellAdminCmd);
                break;
            default:
                netsnmp_set_request_error(reqinfo, request,
                                          SNMP_NOSUCHOBJECT);
                break;
            }
        }
        if(g_cellAdminFirstCall EQ 1)
        {
           g_cellAdminFirstCall = 0;
           g_prevCellAdminReqCount = loopCount;
           // This will occur in case of, when SNMP-GET or SNMP-GETNEXT of Single Parameter occurred.
           // In the first call itself loopCount becomes one.
           // In General Case, the loopCount will be always > 1 because every table contains
           // index column to fetch the row and remaining columns contain table attributes.
           if(loopCount EQ 1)
           {
             //Setting it to false in case of SNMP-GET request for single parameter.
             g_sihCellAdminGetReqProcessed = FALSE;
             // Free all resources.
             free_cellAdminTable();
           }
        }
        else if(loopCount > 1 || g_prevCellAdminReqCount > 1)
        {
           //update g_prevReqCount with current loopCount for next call of Table_handler
           g_prevCellAdminReqCount = loopCount;
        }
        else if(loopCount EQ 1)
        {
           //Setting it to false in case of SNMP-GET request for single parameter.
           g_sihCellAdminGetReqProcessed = FALSE;
           // Free all resources.
           free_cellAdminTable();
        }
        break;

        /*
         * Write-support
         */
    case MODE_SET_RESERVE1:
#ifndef PWAV_SIH
        for (request=requests; request; request=request->next) {
            table_entry = (struct cellAdminTable_entry *)
                              netsnmp_extract_iterator_context(request);
            table_info  =     netsnmp_extract_table_info(      request);
    
            switch (table_info->colnum) {
            case COLUMN_INDEX:
                /* or possibly 'netsnmp_check_vb_int_range' */
                ret = netsnmp_check_vb_int( request->requestvb );
                if ( ret != SNMP_ERR_NOERROR ) {
                    netsnmp_set_request_error( reqinfo, request, ret );
                    return SNMP_ERR_NOERROR;
                }
                break;
            case COLUMN_CELLADMIN_CELLID:
                /* or possibly 'netsnmp_check_vb_int_range' */
                ret = netsnmp_check_vb_int( request->requestvb );
                if ( ret != SNMP_ERR_NOERROR ) {
                    netsnmp_set_request_error( reqinfo, request, ret );
                    return SNMP_ERR_NOERROR;
                }
                break;
            case COLUMN_CELLADMINCMD:
                /* or possibly 'netsnmp_check_vb_int_range' */
                ret = netsnmp_check_vb_int( request->requestvb );
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

	g_sihCellAdminSetReqProcessed = FALSE;

        for (request=requests; request; request=request->next) {
            table_entry = (struct cellAdminTable_entry *)
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
            case COLUMN_CELLADMIN_INDEX:
                table_entry->index     = *request->requestvb->val.integer;
                break;
            case COLUMN_CELLADMIN_CELLID:
                table_entry->cellId     = *request->requestvb->val.integer;
                break;
            case COLUMN_CELLADMINCMD:
                table_entry->cellAdminCmd     = *request->requestvb->val.integer;
                break;
            }
        }

        if ((request == NULL) && (g_sihCellAdminSetReqProcessed EQ FALSE))
            /* Processed all requests and table entry is filled with
             * requested data. */
            {
            /* Set SIH Set Request Tag Info */
            sihTagInfo.tableId = CELLADMIN;

            sihTagInfo.rowId = rowIndex;

            if (noofColms EQ 1)
                {
                sihTagInfo.colId = colId;
                }
            else
                {
                sihTagInfo.colId = CONFIG_COL_ID_NONE;
                }

            status = sendCellAdminSnmpSetRequest (sihTagInfo);
            if (status EQ TRUE)
                {
                logInfo("%s","SET Succedded");
                }
            else
                netsnmp_set_request_error(reqinfo, requests, SNMP_ERR_GENERR );
            }
        break;

    case MODE_SET_UNDO:
#ifndef PWAV_SIH
        for (request=requests; request; request=request->next) {
            table_entry = (struct cellAdminTable_entry *)
                              netsnmp_extract_iterator_context(request);
            table_info  =     netsnmp_extract_table_info(      request);
    
            switch (table_info->colnum) {
            case COLUMN_INDEX:
                table_entry->index     = table_entry->old_index;
                table_entry->old_index = 0;
                break;
            case COLUMN_CELLADMIN_CELLID:
                table_entry->cellId     = table_entry->old_cellId;
                table_entry->old_cellId = 0;
                break;
            case COLUMN_CELLADMINCMD:
                table_entry->cellAdminCmd     = table_entry->old_cellAdminCmd;
                table_entry->old_cellAdminCmd = 0;
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
