/*
 * Note: this file originally auto-generated by mib2c using
 *  : mib2c.iterate.conf 17695 2009-07-21 12:22:18Z dts12 $
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

#include "fileDownload.h"

/* function declarations */
Netsnmp_Node_Handler fileDownloadTable_handler;
Netsnmp_First_Data_Point  fileDownloadTable_get_first_data_point;
Netsnmp_Next_Data_Point   fileDownloadTable_get_next_data_point;

/* globals */
fileDownloadTableEntry  *fileDownloadTable_head = NULL;
static TBOOL g_sihFileDownloadSetReqProcessed = FALSE;
static TBOOL g_sihFileDownloadGetReqProcessed = FALSE;
static UINT32 g_prevFileDownloadReqCount = 0;
static UINT32 g_fileDownloadFirstCall = 0;

/* free_fileDownloadTable */
/**
* @details 
*          This is the routine which frees the file download entry. 
*/
void free_fileDownloadTable(void)
{
  fileDownloadTableEntry  * current = fileDownloadTable_head;

  if (current) 
  {
  for (current = fileDownloadTable_head->next ; current != NULL; current = current->next) {
    SNMP_FREE( current );  
    }
   
  SNMP_FREE(fileDownloadTable_head);
  fileDownloadTable_head = NULL;
  }
  return;
}


/**
* @details 
*          This is the routine which handles get response from SIH Get Processor
*          and prepare the SNMP GET response for EMS. 
* @param tlvArray contains pointer to TLV array.     
*/
TBOOL sendFileDownloadSnmpGetResponse (
  IO TlvArray    *tlvArray
  )
{
  int  totalNumRow, totalNumCol, i=0, colmn=0;
  fileDownloadTableEntry * prevPtr = NULL;

  printTLV(tlvArray);

  logInfo("%s","sendFileDownloadSnmpGetResponse");
  /* Total no. of Colmns */
  totalNumCol = FILE_DOWNLOAD_MAX_COL;
  /* Total no. of Row = (No. of TLV)/(No. of Columns) */
  totalNumRow = tlvArray->numTlvs/totalNumCol;

  logInfo("Total No. of Row %d & Col %d", totalNumRow, totalNumCol);
  for (i=1;  i <= totalNumRow;  i++) 
      {
      fileDownloadTableEntry *ptr=(fileDownloadTableEntry *)malloc(sizeof(fileDownloadTableEntry));

      clearBytes (ptr, sizeof(fileDownloadTableEntry));

      /* Fill Data in TLV */
      for(colmn = 1 ; colmn <= totalNumCol ; colmn++)
        {
        if (colmn EQ COLUMN_INDEX)
           {
            ptr->index =  getUint32FromArray(tlvArray->tlvInfo[colmn-1].data);
            continue;
           }
        if (colmn EQ COLUMN_FILEDOWNLOADCOMMAND)
           {
            ptr->fileDownloadCommand =  getUint32FromArray(tlvArray->tlvInfo[colmn-1].data);
            continue;
           }
        if (colmn EQ COLUMN_DOWNLOADFILENAME)
           {
            clearBytes (ptr->fileName, BUFFER_LEN_MAX);
	    if (tlvArray->tlvInfo[colmn-1].len > 0)
            {
            	copyBytes (tlvArray->tlvInfo[colmn-1].data, tlvArray->tlvInfo[colmn-1].len, ptr->fileName);
            }
	    ptr->fileName_len = strlen(ptr->fileName); 
            continue;
           }
        if (colmn EQ COLUMN_LOGINID)
           {
            clearBytes (ptr->loginId, BUFFER_LEN_MAX);
	    if (tlvArray->tlvInfo[colmn-1].len > 0)
            {
            	copyBytes (tlvArray->tlvInfo[colmn-1].data, tlvArray->tlvInfo[colmn-1].len, ptr->loginId);
            }
            ptr->loginId_len = strlen(ptr->loginId); 
            continue;
           }
        if (colmn EQ COLUMN_PASSWORD)
           {
            clearBytes (ptr->password, BUFFER_LEN_MAX);
	    if (tlvArray->tlvInfo[colmn-1].len > 0)
            {
            	copyBytes (tlvArray->tlvInfo[colmn-1].data, tlvArray->tlvInfo[colmn-1].len, ptr->password);
            }
            ptr->password_len = strlen(ptr->password); 
            continue;
           }
        if (colmn EQ COLUMN_SOURCEIP)
           {
            ptr->sourceIp =  inet_addr(tlvArray->tlvInfo[colmn-1].data);
            continue;
           }
        if (colmn EQ COLUMN_PROTOCOLUSED)
           {
            ptr->protocolUsed =  getUint32FromArray(tlvArray->tlvInfo[colmn-1].data);
            continue;
           }
        if (colmn EQ COLUMN_SOURCEPORT)
           {
            ptr->sourcePort =  getUint32FromArray(tlvArray->tlvInfo[colmn-1].data);
            continue;
           }
        if (colmn EQ COLUMN_MAXDOWNLOADSPEED)
           {
            ptr->maxDownloadSpeed =  getUint32FromArray(tlvArray->tlvInfo[colmn-1].data);
            continue;
           }
        if (colmn EQ COLUMN_SCHEDULEDDATEANDTIME)
           {
            clearBytes (ptr->scheduleDateAndTime, SCHEDULE_DATEANDTIME_LEN);
	    if (tlvArray->tlvInfo[colmn-1].len > 0)
            {
            	copyBytes (tlvArray->tlvInfo[colmn-1].data, tlvArray->tlvInfo[colmn-1].len, ptr->scheduleDateAndTime);
            }
            ptr->scheduleDateAndTime_len = strlen(ptr->scheduleDateAndTime);
            continue;
           }
        }

       ptr->next = NULL;

      if (prevPtr == NULL) {
           fileDownloadTable_head =  prevPtr = ptr;
       }

      prevPtr->next = ptr;
      prevPtr = ptr;
     }
    prevPtr->next = NULL;

    if (fileDownloadTable_head)
        return TRUE;
    else
        return FALSE;
}


/**
* @details 
*          This routine sends file download set request to BSM, it uses SIH Set 
*          Processor to create and send the message to BSM. 
* @param sihTagInfo is used to have the tag information for set request.
*/
TBOOL sendFileDownloadSnmpSetRequest ( 
      TagInfo  sihTagInfo
      )
{
   TlvArray tlvArray;
   fileDownloadTableEntry * setReqData = NULL;   
   TBOOL status = FALSE;
   int colmn = 0, maxCols = 0;
   unsigned char * ipAddr;
   struct in_addr ip;
 
   clearBytes(&tlvArray,sizeof(tlvArray));
    
   setReqData = fileDownloadTable_head;

  /* max columns in row */
   maxCols = FILE_DOWNLOAD_MAX_COL;

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
       if (((sihTagInfo.colId EQ CONFIG_COL_ID_NONE) && (colmn EQ COLUMN_INDEX)) || 
           (sihTagInfo.colId EQ COLUMN_INDEX))
           {
           putUint32InArray( setReqData->index, tlvArray.tlvInfo[colmn-1].data );
           tlvArray.tlvInfo[colmn-1].len = sizeof(setReqData->index); 
           tlvArray.numTlvs++;
           if (sihTagInfo.colId EQ COLUMN_INDEX)
               break;
           }
       if (((sihTagInfo.colId EQ CONFIG_COL_ID_NONE) && (colmn EQ COLUMN_FILEDOWNLOADCOMMAND)) || 
           (sihTagInfo.colId EQ COLUMN_FILEDOWNLOADCOMMAND))
           {
           putUint32InArray( setReqData->fileDownloadCommand, tlvArray.tlvInfo[colmn-1].data );
           tlvArray.tlvInfo[colmn-1].len = sizeof(setReqData->fileDownloadCommand); 
           tlvArray.numTlvs++;
           if (sihTagInfo.colId EQ COLUMN_FILEDOWNLOADCOMMAND)
               break;
           }
       if (((sihTagInfo.colId EQ CONFIG_COL_ID_NONE) && (colmn EQ COLUMN_DOWNLOADFILENAME)) || 
           (sihTagInfo.colId EQ COLUMN_DOWNLOADFILENAME))
           {
           clearBytes (tlvArray.tlvInfo[colmn-1].data, sizeof(tlvArray.tlvInfo[colmn-1].data));
           copyBytes (setReqData->fileName, strlen(setReqData->fileName), 
                      tlvArray.tlvInfo[colmn-1].data);
           tlvArray.tlvInfo[colmn-1].len = STRING_LEN_MAX; 
           tlvArray.numTlvs++;
           if (sihTagInfo.colId EQ COLUMN_DOWNLOADFILENAME)
               break;
           }
       if (((sihTagInfo.colId EQ CONFIG_COL_ID_NONE) && (colmn EQ COLUMN_LOGINID)) ||
           (sihTagInfo.colId EQ COLUMN_LOGINID))
           {
           clearBytes (tlvArray.tlvInfo[colmn-1].data, sizeof(tlvArray.tlvInfo[colmn-1].data));
           copyBytes (setReqData->loginId, strlen(setReqData->loginId), 
                      tlvArray.tlvInfo[colmn-1].data);
           tlvArray.tlvInfo[colmn-1].len = STRING_LEN_MAX;     
           tlvArray.numTlvs++;
           if (sihTagInfo.colId EQ COLUMN_LOGINID)
               break;
           }       
       if (((sihTagInfo.colId EQ CONFIG_COL_ID_NONE) && (colmn EQ COLUMN_PASSWORD)) ||
           (sihTagInfo.colId EQ COLUMN_PASSWORD))
           {
           clearBytes (tlvArray.tlvInfo[colmn-1].data, sizeof(tlvArray.tlvInfo[colmn-1].data));
           copyBytes (setReqData->password, strlen(setReqData->password), 
                      tlvArray.tlvInfo[colmn-1].data);           
           tlvArray.tlvInfo[colmn-1].len = STRING_LEN_MAX;     
           tlvArray.numTlvs++;
           if (sihTagInfo.colId EQ COLUMN_PASSWORD)
               break;
           }         
       if (((sihTagInfo.colId EQ CONFIG_COL_ID_NONE) && (colmn EQ COLUMN_SOURCEIP)) || 
           (sihTagInfo.colId EQ COLUMN_SOURCEIP))
           {
           ip.s_addr = setReqData->sourceIp;
           ipAddr = inet_ntoa(ip);
           clearBytes (tlvArray.tlvInfo[colmn-1].data, sizeof(tlvArray.tlvInfo[colmn-1].data));
           copyBytes (ipAddr, strlen(ipAddr), tlvArray.tlvInfo[colmn-1].data);
           tlvArray.tlvInfo[colmn-1].len = SRCIP_ADDR_LEN_MAX; 
           tlvArray.numTlvs++;
           if (sihTagInfo.colId EQ COLUMN_SOURCEIP)
               break;
           }       
       if (((sihTagInfo.colId EQ CONFIG_COL_ID_NONE) && (colmn EQ COLUMN_PROTOCOLUSED)) ||
           (sihTagInfo.colId EQ COLUMN_PROTOCOLUSED))
           {
           putUint32InArray( setReqData->protocolUsed, tlvArray.tlvInfo[colmn-1].data );
           tlvArray.tlvInfo[colmn-1].len = sizeof(setReqData->protocolUsed);     
           tlvArray.numTlvs++;
           if (sihTagInfo.colId EQ COLUMN_PROTOCOLUSED)
               break;
           }             
       if (((sihTagInfo.colId EQ CONFIG_COL_ID_NONE) && (colmn EQ COLUMN_SOURCEPORT)) || 
           (sihTagInfo.colId EQ COLUMN_SOURCEPORT))
           {
           putUint32InArray( setReqData->sourcePort, tlvArray.tlvInfo[colmn-1].data);
           tlvArray.tlvInfo[colmn-1].len = sizeof(setReqData->sourcePort); 
           tlvArray.numTlvs++;
           if (sihTagInfo.colId EQ COLUMN_SOURCEPORT)
               break;
           }
       if (((sihTagInfo.colId EQ CONFIG_COL_ID_NONE) && (colmn EQ COLUMN_MAXDOWNLOADSPEED)) ||
           (sihTagInfo.colId EQ COLUMN_MAXDOWNLOADSPEED))
           {
           putUint32InArray( setReqData->maxDownloadSpeed, tlvArray.tlvInfo[colmn-1].data );
           tlvArray.tlvInfo[colmn-1].len = sizeof(setReqData->maxDownloadSpeed);     
           tlvArray.numTlvs++;
           if (sihTagInfo.colId EQ COLUMN_MAXDOWNLOADSPEED)
               break;
           }        
       if (((sihTagInfo.colId EQ CONFIG_COL_ID_NONE) && (colmn EQ COLUMN_SCHEDULEDDATEANDTIME)) ||
           (sihTagInfo.colId EQ COLUMN_SCHEDULEDDATEANDTIME))
           {
           clearBytes (tlvArray.tlvInfo[colmn-1].data, sizeof(tlvArray.tlvInfo[colmn-1].data));
           copyBytes (setReqData->scheduleDateAndTime, strlen(setReqData->scheduleDateAndTime),
                      tlvArray.tlvInfo[colmn-1].data);
           tlvArray.tlvInfo[colmn-1].len = SCHEDULE_DATEANDTIME_LEN;
           tlvArray.numTlvs++;
           if (sihTagInfo.colId EQ COLUMN_SCHEDULEDDATEANDTIME)
               break;
           }
       }

   /* Invoke SIH Set Processor */
   status = sihSetProcessor (sihTagInfo,  &tlvArray);
   if (status EQ TRUE)
     {
      g_sihFileDownloadSetReqProcessed = TRUE;
      g_sihFileDownloadGetReqProcessed = FALSE;
      /* free the resources */      
      free_fileDownloadTable(); 
     }

   return status;
}


/** Initializes the fileDownload module */
void
init_fileDownload(void)
{
  /* here we initialize all the tables we're planning on supporting */
    initialize_table_fileDownloadTable();
}


/** Initialize the fileDownloadTable table by defining its contents and how it's structured */
void
initialize_table_fileDownloadTable(void)
{
    const oid fileDownloadTable_oid[] = {1,3,6,1,4,1,9344,501,FILEDOWNLOAD,1};
    const size_t fileDownloadTable_oid_len   = OID_LENGTH(fileDownloadTable_oid);
    netsnmp_handler_registration    *reg;
    netsnmp_iterator_info           *iinfo;
    netsnmp_table_registration_info *table_info;

    DEBUGMSGTL(("fileDownload:init", "initializing table fileDownloadTable\n"));

    reg = netsnmp_create_handler_registration(
              "fileDownloadTable",     fileDownloadTable_handler,
              fileDownloadTable_oid, fileDownloadTable_oid_len,
              HANDLER_CAN_RWRITE
              );

    table_info = SNMP_MALLOC_TYPEDEF( netsnmp_table_registration_info );
    netsnmp_table_helper_add_indexes(table_info,
                           ASN_UNSIGNED,  /* index: index */
                           0);
    table_info->min_column = COLUMN_INDEX;
    table_info->max_column = FILE_DOWNLOAD_MAX_COL;
    
    iinfo = SNMP_MALLOC_TYPEDEF( netsnmp_iterator_info );
    iinfo->get_first_data_point = fileDownloadTable_get_first_data_point;
    iinfo->get_next_data_point  = fileDownloadTable_get_next_data_point;
    iinfo->table_reginfo        = table_info;
    
    netsnmp_register_table_iterator( reg, iinfo );

    /* Initialise the contents of the table here */
}

#ifndef PWAV_SIH
/* create a new row in the (unsorted) table */
struct fileDownloadTable_entry *
fileDownloadTable_createEntry(
                 long  fileDownloadCommand
                ) {
    struct fileDownloadTable_entry *entry;

    entry = SNMP_MALLOC_TYPEDEF(struct fileDownloadTable_entry);
    if (!entry)
        return NULL;

    entry->fileDownloadCommand = fileDownloadCommand;
    entry->next = fileDownloadTable_head;
    fileDownloadTable_head = entry;
    return entry;
}

/* remove a row from the table */
void
fileDownloadTable_removeEntry( struct fileDownloadTable_entry *entry ) {
    struct fileDownloadTable_entry *ptr, *prev;

    if (!entry)
        return;    /* Nothing to remove */

    for ( ptr  = fileDownloadTable_head, prev = NULL;
          ptr != NULL;
          prev = ptr, ptr = ptr->next ) {
        if ( ptr == entry )
            break;
    }
    if ( !ptr )
        return;    /* Can't find it */

    if ( prev == NULL )
        fileDownloadTable_head = ptr->next;
    else
        prev->next = ptr->next;

    SNMP_FREE( entry );   /* XXX - release any other internal resources */
}
#endif /* PWAV_SIH */


/* Example iterator hook routines - using 'get_next' to do most of the work */
netsnmp_variable_list *
fileDownloadTable_get_first_data_point(void **my_loop_context,
                          void **my_data_context,
                          netsnmp_variable_list *put_index_data,
                          netsnmp_iterator_info *mydata)
{
    TagInfo sihTagInfo;
    TBOOL status = FALSE;

    if (g_sihFileDownloadGetReqProcessed EQ FALSE)
        {
        /* Set SIH Get Request Tag Info */
        sihTagInfo.tableId = FILEDOWNLOAD;
        sihTagInfo.rowId = CONFIG_ROW_ID_NONE; 
        sihTagInfo.colId = CONFIG_COL_ID_NONE;                

        status = sihGetProcessor (sihTagInfo);
        if ( status EQ TRUE )
            {
            logInfo("%s","Get Processor Succedded");
            if (fileDownloadTable_head)
                {
		g_fileDownloadFirstCall = 1;	
                g_sihFileDownloadGetReqProcessed = TRUE;
                }
            }
        else
            logError("%s","Get Processor Failed");
        }        

    *my_loop_context = fileDownloadTable_head;

    return fileDownloadTable_get_next_data_point(my_loop_context, my_data_context,
                                    put_index_data,  mydata );
}

netsnmp_variable_list *
fileDownloadTable_get_next_data_point(void **my_loop_context,
                          void **my_data_context,
                          netsnmp_variable_list *put_index_data,
                          netsnmp_iterator_info *mydata)
{
    struct fileDownloadTable_entry *entry = (struct fileDownloadTable_entry *)*my_loop_context;
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


/** handles requests for the fileDownloadTable table */
int
fileDownloadTable_handler(
    netsnmp_mib_handler               *handler,
    netsnmp_handler_registration      *reginfo,
    netsnmp_agent_request_info        *reqinfo,
    netsnmp_request_info              *requests) {

    netsnmp_request_info       *request = NULL;
    netsnmp_table_request_info *table_info = NULL;
    struct fileDownloadTable_entry          *table_entry = NULL;
    int loopCount = 0;
    int ret;
    TagInfo sihTagInfo;
    int noofColms = 0, rowIndex = 0, colId = 0;
    TBOOL status = FALSE;    

    DEBUGMSGTL(("fileDownload:handler", "Processing request (%d)\n", reqinfo->mode));

    switch (reqinfo->mode) {
        /*
         * Read-support (also covers GetNext requests)
         */
    case MODE_GET:
        for (request=requests; request; request=request->next) {

            loopCount++;
            if (request->processed != 0) 
            /* When Processed all requests, rows and colmns */
            {
	    g_prevFileDownloadReqCount = 0;	
            g_sihFileDownloadGetReqProcessed = FALSE;
            /* free the resources */
            free_fileDownloadTable();
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

            table_entry = (struct fileDownloadTable_entry *)
                              netsnmp_extract_iterator_context(request);

            if ( table_entry == NULL) {
                if (reqinfo->mode == MODE_GET) {
                    netsnmp_set_request_error(reqinfo, request,SNMP_NOSUCHINSTANCE);
                    continue;
                }
                /* XXX: no row existed, if you support creation and this is a
                   set, start dealing with it here, else continue */
            }

            table_info  =     netsnmp_extract_table_info(request);

            /* table_info->colnum contains the column number requested */
            /* table_info->indexes contains a linked list of snmp variable
               bindings for the indexes of the table.  Values in the list
               have been set corresponding to the indexes of the
               request */
            if (table_info==NULL) {
                continue;
            }    

            switch (table_info->colnum) {
            case COLUMN_INDEX:
                if ( !table_entry ) {
                    netsnmp_set_request_error(reqinfo, request,
                                              SNMP_NOSUCHINSTANCE);
                    continue;
                }
                snmp_set_var_typed_integer( request->requestvb, ASN_UNSIGNED,
                                            table_entry->index);
                break;
            case COLUMN_FILEDOWNLOADCOMMAND:
                if ( !table_entry ) {
                    netsnmp_set_request_error(reqinfo, request,
                                              SNMP_NOSUCHINSTANCE);
                    continue;
                }
                snmp_set_var_typed_integer( request->requestvb, ASN_INTEGER,
                                            table_entry->fileDownloadCommand);
                break;
            case COLUMN_DOWNLOADFILENAME:
                if ( !table_entry ) {
                    netsnmp_set_request_error(reqinfo, request,
                                              SNMP_NOSUCHINSTANCE);
                    continue;
                }
                snmp_set_var_typed_value( request->requestvb, ASN_OCTET_STR,
                                 (u_char*)table_entry->fileName,
                                          table_entry->fileName_len);
                break;
            case COLUMN_LOGINID:
                if ( !table_entry ) {
                    netsnmp_set_request_error(reqinfo, request,
                                              SNMP_NOSUCHINSTANCE);
                    continue;
                }
                snmp_set_var_typed_value( request->requestvb, ASN_OCTET_STR,
                                 (u_char*)table_entry->loginId,
                                          table_entry->loginId_len);
                break;
            case COLUMN_PASSWORD:
                if ( !table_entry ) {
                    netsnmp_set_request_error(reqinfo, request,
                                              SNMP_NOSUCHINSTANCE);
                    continue;
                }
                snmp_set_var_typed_value( request->requestvb, ASN_OCTET_STR,
                                 (u_char*)table_entry->password,
                                          table_entry->password_len);
                break;
            case COLUMN_SOURCEIP:
                if ( !table_entry ) {
                    netsnmp_set_request_error(reqinfo, request,
                                              SNMP_NOSUCHINSTANCE);
                    continue;
                }
                snmp_set_var_typed_integer( request->requestvb, ASN_IPADDRESS,
                                            table_entry->sourceIp);
                break;
            case COLUMN_PROTOCOLUSED:
                if ( !table_entry ) {
                    netsnmp_set_request_error(reqinfo, request,
                                              SNMP_NOSUCHINSTANCE);
                    continue;
                }
                snmp_set_var_typed_integer( request->requestvb, ASN_INTEGER,
                                            table_entry->protocolUsed);
                break;
            case COLUMN_SOURCEPORT:
                if ( !table_entry ) {
                    netsnmp_set_request_error(reqinfo, request,
                                              SNMP_NOSUCHINSTANCE);
                    continue;
                }
                snmp_set_var_typed_integer( request->requestvb, ASN_INTEGER,
                                            table_entry->sourcePort);
                break;
            case COLUMN_MAXDOWNLOADSPEED:
                if ( !table_entry ) {
                    netsnmp_set_request_error(reqinfo, request,
                                              SNMP_NOSUCHINSTANCE);
                    continue;
                }
                snmp_set_var_typed_integer( request->requestvb, ASN_INTEGER,
                                            table_entry->maxDownloadSpeed);
                break;
            case COLUMN_SCHEDULEDDATEANDTIME:
                if ( !table_entry ) {
                    netsnmp_set_request_error(reqinfo, request,
                                              SNMP_NOSUCHINSTANCE);
                    continue;
                }
                snmp_set_var_typed_value( request->requestvb, ASN_OCTET_STR,
                                 (u_char*)table_entry->scheduleDateAndTime,
                                          table_entry->scheduleDateAndTime_len);
                break;
            default:
                netsnmp_set_request_error(reqinfo, request,
                                          SNMP_NOSUCHOBJECT);
                break;
            }
        }
         if(g_fileDownloadFirstCall EQ 1)
         {
            g_fileDownloadFirstCall = 0;
            g_prevFileDownloadReqCount = loopCount;
            // This will occur in case of, when SNMP-GET or SNMP-GETNEXT of Single Parameter occurred.
            // In the first call itself loopCount becomes one.
            // In General Case, the loopCount will be always > 1 because every table contains 
            // index column to fetch the row and remaining columns contain table attributes.
            if(loopCount EQ 1)
            {
               //Setting it to false in case of SNMP-GET request for single parameter.
               g_sihFileDownloadGetReqProcessed = FALSE;
               // Free all resources.
               free_fileDownloadTable();
            }
         }
         else if(loopCount > 1 || g_prevFileDownloadReqCount > 1)
         {
            //update g_prevReqCount with current loopCount for next call of Table_handler
            g_prevFileDownloadReqCount = loopCount;
         }
         else if(loopCount EQ 1)
         {
            //Setting it to false in case of SNMP-GET request for single parameter.
            g_sihFileDownloadGetReqProcessed = FALSE;
            // Free all resources.
            free_fileDownloadTable();
         }
        break;

        /*
         * Write-support
         */
    case MODE_SET_RESERVE1:
#ifndef PWAV_SIH        
        for (request=requests; request; request=request->next) {
            table_entry = (struct fileDownloadTable_entry *)
                              netsnmp_extract_iterator_context(request);
            table_info  =     netsnmp_extract_table_info(request);
    
            switch (table_info->colnum) {
            case COLUMN_INDEX:
                /* or possibly 'netsnmp_check_vb_int_range' */
                ret = netsnmp_check_vb_int( request->requestvb );
                if ( ret != SNMP_ERR_NOERROR ) {
                    netsnmp_set_request_error( reqinfo, request, ret );
                    return SNMP_ERR_NOERROR;
                }
                break;
            case COLUMN_FILEDOWNLOADCOMMAND:
                /* or possibly 'netsnmp_check_vb_int_range' */
                ret = netsnmp_check_vb_int( request->requestvb );
                if ( ret != SNMP_ERR_NOERROR ) {
                    netsnmp_set_request_error( reqinfo, request, ret );
                    return SNMP_ERR_NOERROR;
                }
                break;
            case COLUMN_DOWNLOADFILENAME:
	        /* or possibly 'netsnmp_check_vb_type_and_size' */
                ret = netsnmp_check_vb_type_and_max_size(
                          request->requestvb, ASN_OCTET_STR, sizeof(table_entry->fileName));
                if ( ret != SNMP_ERR_NOERROR ) {
                    netsnmp_set_request_error( reqinfo, request, ret );
                    return SNMP_ERR_NOERROR;
                }
                break;
            case COLUMN_LOGINID:
	        /* or possibly 'netsnmp_check_vb_type_and_size' */
                ret = netsnmp_check_vb_type_and_max_size(
                          request->requestvb, ASN_OCTET_STR, sizeof(table_entry->loginId));
                if ( ret != SNMP_ERR_NOERROR ) {
                    netsnmp_set_request_error( reqinfo, request, ret );
                    return SNMP_ERR_NOERROR;
                }
                break;
            case COLUMN_PASSWORD:
	        /* or possibly 'netsnmp_check_vb_type_and_size' */
                ret = netsnmp_check_vb_type_and_max_size(
                          request->requestvb, ASN_OCTET_STR, sizeof(table_entry->password));
                if ( ret != SNMP_ERR_NOERROR ) {
                    netsnmp_set_request_error( reqinfo, request, ret );
                    return SNMP_ERR_NOERROR;
                }
                break;
            case COLUMN_SOURCEIP:
                /* or possibly 'netsnmp_check_vb_int_range' */
                ret = netsnmp_check_vb_int( request->requestvb );
                if ( ret != SNMP_ERR_NOERROR ) {
                    netsnmp_set_request_error( reqinfo, request, ret );
                    return SNMP_ERR_NOERROR;
                }
                break;
            case COLUMN_PROTOCOLUSED:
                /* or possibly 'netsnmp_check_vb_int_range' */
                ret = netsnmp_check_vb_int( request->requestvb );
                if ( ret != SNMP_ERR_NOERROR ) {
                    netsnmp_set_request_error( reqinfo, request, ret );
                    return SNMP_ERR_NOERROR;
                }
                break;
            case COLUMN_SOURCEPORT:
                /* or possibly 'netsnmp_check_vb_int_range' */
                ret = netsnmp_check_vb_int( request->requestvb );
                if ( ret != SNMP_ERR_NOERROR ) {
                    netsnmp_set_request_error( reqinfo, request, ret );
                    return SNMP_ERR_NOERROR;
                }
                break;
            case COLUMN_MAXDOWNLOADSPEED:
                /* or possibly 'netsnmp_check_vb_int_range' */
                ret = netsnmp_check_vb_int( request->requestvb );
                if ( ret != SNMP_ERR_NOERROR ) {
                    netsnmp_set_request_error( reqinfo, request, ret );
                    return SNMP_ERR_NOERROR;
                }
                break;
            case COLUMN_SCHEDULEDDATEANDTIME:
	        /* or possibly 'netsnmp_check_vb_type_and_size' */
                ret = netsnmp_check_vb_type_and_max_size(
                          request->requestvb, ASN_OCTET_STR, sizeof(table_entry->scheduleDateAndTime));
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
        g_sihFileDownloadSetReqProcessed = FALSE; 
        
        for (request=requests; request; request=request->next) {
            table_entry = (struct fileDownloadTable_entry *)
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
            case COLUMN_INDEX:
                table_entry->index     = *request->requestvb->val.integer;
                noofColms++;
                break;
            case COLUMN_FILEDOWNLOADCOMMAND:
                table_entry->fileDownloadCommand     = *request->requestvb->val.integer;
                noofColms++;
                break;
            case COLUMN_DOWNLOADFILENAME:
                memset( table_entry->fileName, 0,
                        sizeof(table_entry->fileName));
                memcpy( table_entry->fileName,
                        request->requestvb->val.string,
                        request->requestvb->val_len);
                table_entry->fileName_len =
                        request->requestvb->val_len;
                noofColms++;
                break;
            case COLUMN_LOGINID:
                memset( table_entry->loginId, 0,
                        sizeof(table_entry->loginId));
                memcpy( table_entry->loginId,
                        request->requestvb->val.string,
                        request->requestvb->val_len);
                table_entry->loginId_len =
                        request->requestvb->val_len;
                noofColms++;
                break;
            case COLUMN_PASSWORD:
                memset( table_entry->password, 0,
                        sizeof(table_entry->password));
                memcpy( table_entry->password,
                        request->requestvb->val.string,
                        request->requestvb->val_len);
                table_entry->password_len =
                        request->requestvb->val_len;
                noofColms++;
                break;
            case COLUMN_SOURCEIP:
                table_entry->sourceIp     = *request->requestvb->val.integer;
                noofColms++;
                break;
            case COLUMN_PROTOCOLUSED:
                table_entry->protocolUsed     = *request->requestvb->val.integer;
                noofColms++;
                break;
            case COLUMN_SOURCEPORT:
                table_entry->sourcePort     = *request->requestvb->val.integer;
                noofColms++;
                break;
            case COLUMN_MAXDOWNLOADSPEED:
                table_entry->maxDownloadSpeed     = *request->requestvb->val.integer;
                noofColms++;
                break;
            case COLUMN_SCHEDULEDDATEANDTIME:
                memset( table_entry->scheduleDateAndTime, 0,
                        sizeof(table_entry->scheduleDateAndTime));
                memcpy( table_entry->scheduleDateAndTime,
                        request->requestvb->val.string,
                        request->requestvb->val_len);
                table_entry->password_len =
                        request->requestvb->val_len;
                noofColms++;
                break;
            }
        }

        if ((request == NULL) && (g_sihFileDownloadSetReqProcessed EQ FALSE))
            /* Processed all requests and table entry is filled with
             * requested data. */
            {
            /* Set SIH Set Request Tag Info */
            sihTagInfo.tableId = FILEDOWNLOAD;

            sihTagInfo.rowId = rowIndex; 

            if (noofColms EQ 1)
                {
                sihTagInfo.colId = colId;                
                }
            else
                {
                sihTagInfo.colId = CONFIG_COL_ID_NONE;                
                }

            status = sendFileDownloadSnmpSetRequest (sihTagInfo);
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
            table_entry = (struct fileDownloadTable_entry *)
                              netsnmp_extract_iterator_context(request);
            table_info  =     netsnmp_extract_table_info(      request);
    
            switch (table_info->colnum) {
            case COLUMN_INDEX:
                table_entry->index     = table_entry->old_index;
                table_entry->old_index = 0;
                break;
            case COLUMN_FILEDOWNLOADCOMMAND:
                table_entry->fileDownloadCommand     = table_entry->old_fileDownloadCommand;
                table_entry->old_fileDownloadCommand = 0;
                break;
            case COLUMN_DOWNLOADFILENAME:
                memcpy( table_entry->fileName,
                        table_entry->old_fileName,
                        sizeof(table_entry->fileName));
                memset( table_entry->old_fileName, 0,
                        sizeof(table_entry->fileName));
                table_entry->fileName_len =
                        table_entry->old_fileName_len;
                break;
            case COLUMN_LOGINID:
                memcpy( table_entry->loginId,
                        table_entry->old_loginId,
                        sizeof(table_entry->loginId));
                memset( table_entry->old_loginId, 0,
                        sizeof(table_entry->loginId));
                table_entry->loginId_len =
                        table_entry->old_loginId_len;
                break;
            case COLUMN_PASSWORD:
                memcpy( table_entry->password,
                        table_entry->old_password,
                        sizeof(table_entry->password));
                memset( table_entry->old_password, 0,
                        sizeof(table_entry->password));
                table_entry->password_len =
                        table_entry->old_password_len;
                break;
            case COLUMN_SOURCEIP:
                table_entry->sourceIp     = table_entry->old_sourceIp;
                table_entry->old_sourceIp = 0;
                break;
            case COLUMN_PROTOCOLUSED:
                table_entry->protocolUsed     = table_entry->old_protocolUsed;
                table_entry->old_protocolUsed = 0;
                break;
            case COLUMN_SOURCEPORT:
                table_entry->sourcePort     = table_entry->old_sourcePort;
                table_entry->old_sourcePort = 0;
                break;
            case COLUMN_MAXDOWNLOADSPEED:
                table_entry->maxDownloadSpeed     = table_entry->old_maxDownloadSpeed;
                table_entry->old_maxDownloadSpeed = 0;
                break;
            case COLUMN_SCHEDULEDDATEANDTIME:
                memcpy( table_entry->,
                        table_entry->old_scheduleDateAndTime,
                        sizeof(table_entry->scheduleDateAndTime));
                memset( table_entry->old_scheduleDateAndTime, 0,
                        sizeof(table_entry->scheduleDateAndTime));
                table_entry->password_len =
                        table_entry->old_scheduleDateAndTime_len;
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
