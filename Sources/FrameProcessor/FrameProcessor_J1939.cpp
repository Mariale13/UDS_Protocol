
/******************************************************************************
  Project       :  Auto-SAT_Tools
  FileName      :  FrameProcessor_J1939.cpp
  Description   :  Source file for CFrameProcessor_J1939 class.
  $Log:   X:/Archive/Sources/FrameProcessor/FrameProcessor_J1939.cpv  $

      Rev 1.9   02 Dec 2011 20:16:48   rac2kor
   Removed hard coding of version information
   string in the log file by accepting it as a parameter
   from the application / client.

      Rev 1.8   15 Apr 2011 19:20:32   rac2kor
   Inserted RBEI Copyright information text into the file header.

      Rev 1.7   28 Dec 2010 18:53:24   CANMNTTM
   rac2kor:
   1. DoInitialisation function now only clears the message buffer.
   2. Setting of the action event of the thread is now done in the
   constructor, as there there is no way the same is going to change
   throughout the lifetime of the process.

      Rev 1.6   23 Dec 2010 16:53:30   CANMNTTM
   macro MAX_MSG_LEN_J1939 is used instead of MAX_DATA_LEN_J1939.

      Rev 1.5   14 Dec 2010 19:35:06   CANMNTTM
   BugFix: Data size is set appropriately.

      Rev 1.4   14 Dec 2010 17:24:40   rac2kor
   Function has been called as a parameter of ASSERT(...).
   As a result in release mode this doesn't compile and is an error.

      Rev 1.3   14 Dec 2010 10:21:52   CANMNTTM
   BugFix: To get number of  messages inside the buffer GetMsgCount( ) is used instead of GetBufferSize( )

      Rev 1.2   13 Dec 2010 19:38:16   CANMNTTM
   DLC value is set properly.

      Rev 1.1   10 Dec 2010 22:28:50   rac2kor
   Bugfixing done: Return value of DIL_GetInterface(...)
   was wrongly asserted against S_OK.

      Rev 1.0   06 Dec 2010 18:53:02   rac2kor


  Author(s)     :  Ratnadip Choudhury
  Date Created  :  1.12.2010
  Modified By   :
  Copyright (c) 2011, Robert Bosch Engineering and Business Solutions.  All rights reserved.
******************************************************************************/

#include "FrameProcessor_stdafx.h"
#include "Datatypes/MsgBufAll_DataTypes.h"
#include "DIL_Interface/DIL_Interface_extern.h"
#include "FrameProcessor_J1939.h"
#include "LogObjectJ1939.h"
#include "Utility\MultiLanguageSupport.h"
//#include "../Application/GettextBusmaster.h"


USHORT CFrameProcessor_J1939::ushCalculateStrLen(bool bForHex, USHORT ushLength)
{
    USHORT Result = 0;

    if (bForHex)
    {
        Result = ushLength * 3 + 1;
    }
    else
    {
        Result = ushLength * 4 + 1;
    }

    return Result;
}

CFrameProcessor_J1939::CFrameProcessor_J1939()
{
    // Get hold of J1939 DIL interface
    HRESULT Result = DIL_GetInterface(J1939, (void**) &m_pouDIL_J1939);
    ASSERT(S_OK == Result);
    ASSERT(NULL != m_pouDIL_J1939);

    m_sJ1939ProcParams.m_pILog = NULL;
    m_sJ1939ProcParams.dwClientID = 0x0;

    // Allocate necessary amount of memory.

    m_sJ1939Data.m_unDLC = MAX_DATA_LEN_J1939;
    m_sJ1939Data.m_pbyData = new BYTE[m_sJ1939Data.m_unDLC];// For basic data object
    ASSERT(NULL != m_sJ1939Data.m_pbyData);

    // For raw data bytes. It should be equal to the size of m_sJ1939Data
    m_pbyJ1939Data = new BYTE[m_sJ1939Data.unGetSize()];
    ASSERT(NULL != m_pbyJ1939Data);

    USHORT Length = ushCalculateStrLen(true, MAX_DATA_LEN_J1939);
    m_sCurrFormatDat.m_pcDataHex = new char[Length];
    ASSERT(NULL != m_sCurrFormatDat.m_pcDataHex);
    memset(m_sCurrFormatDat.m_pcDataHex, '\0', Length * sizeof(char));
    Length = ushCalculateStrLen(false, MAX_DATA_LEN_J1939);
    m_sCurrFormatDat.m_pcDataDec = new char[Length];
    ASSERT(NULL != m_sCurrFormatDat.m_pcDataDec);
    memset(m_sCurrFormatDat.m_pcDataDec, '\0', Length * sizeof(char));
    m_sDataCopyThread.m_hActionEvent = m_ouVSEBufJ1939.hGetNotifyingEvent();
    // PTV [1.6.4]
    m_bIsJ1939DataLogged = FALSE;
}

CFrameProcessor_J1939::~CFrameProcessor_J1939()
{
    DELETE_ARRAY(m_pbyJ1939Data);
    DELETE_ARRAY(m_sJ1939Data.m_pbyData);

    vEmptyLogObjArray(m_omLogListTmp);
    vEmptyLogObjArray(m_omLogObjectArray);
}

BOOL CFrameProcessor_J1939::InitInstance(void)
{
    BOOL Result = this->CFrameProcessor_Common::InitInstance();

    return Result;
}

int CFrameProcessor_J1939::ExitInstance(void)
{
    int Result = this->CFrameProcessor_Common::ExitInstance();
    m_ouVSEBufJ1939.vClearMessageBuffer();

    return Result;
}

CBaseLogObject* CFrameProcessor_J1939::CreateNewLogObj(const CString& omStrVersion)
{
    CLogObjectJ1939* pLogObj = NULL;
    CString strVersion = CString(m_sJ1939ProcParams.m_acVersion);
    if (strVersion.IsEmpty())
    {
        strVersion = omStrVersion;
    }
    pLogObj = new CLogObjectJ1939(strVersion);
    return (static_cast<CBaseLogObject*> (pLogObj));
}

void CFrameProcessor_J1939::DeleteLogObj(CBaseLogObject*& pouLogObj)
{
    CLogObjectJ1939* pLogObj = static_cast<CLogObjectJ1939*> (pouLogObj);
    if (NULL != pLogObj)
    {
        delete pLogObj;
        pouLogObj = NULL;
    }
    else
    {
        ASSERT(FALSE);
    }
}

void CFrameProcessor_J1939::CreateTimeModeMapping(SYSTEMTIME& CurrSysTime,
        UINT64& unAbsTime)
{
    if (NULL != m_pouDIL_J1939)
    {
        //WON'T COMPILE !!
        m_pouDIL_J1939->DILIJ_GetTimeModeMapping(CurrSysTime, unAbsTime);
    }
}

void CFrameProcessor_J1939::vRetrieveDataFromBuffer(void)
{
    static INT nType = 0;
    static HRESULT Result = 0;

    while (m_ouVSEBufJ1939.GetMsgCount() > 0)
    {
        INT nSize = MAX_MSG_LEN_J1939;
        // First read the J1939 message
        Result = m_ouVSEBufJ1939.ReadFromBuffer(nType, m_pbyJ1939Data, nSize);
        if (Result == ERR_READ_MEMORY_SHORT)
        {
            CString omBuf;
            omBuf.Format(_("ERR_READ_MEMORY_SHORT  nSize: %d"), nSize);
            //AfxMessageBox(omBuf);
        }
        else if (Result == EMPTY_APP_BUFFER)
        {
            //AfxMessageBox(_T("EMPTY_APP_BUFFER"));
        }

        if (m_bLogEnabled == TRUE)
        {
            //check for new logging session
            if(m_bResetAbsTime == TRUE)
            {
                //update msg reset flag
                m_ouFormatMsgJ1939.m_bResetMsgAbsTime = m_bResetAbsTime;
                m_ouFormatMsgJ1939.m_LogSysTime = m_LogSysTime;
                m_bResetAbsTime = FALSE;
            }
            // Save it into the J1939 message structure
            m_sJ1939Data.vSetDataStream(m_pbyJ1939Data);

            // Format current J1939 frame in the necessary settings
            m_ouFormatMsgJ1939.vFormatJ1939DataMsg(&m_sJ1939Data,
                                                   &m_sCurrFormatDat, m_bExprnFlag_Log);

            USHORT ushBlocks = (USHORT) (m_omLogObjectArray.GetSize());
            for (USHORT i = 0; i < ushBlocks; i++)
            {
                CBaseLogObject* pouLogObjBase = m_omLogObjectArray.GetAt(i);
                CLogObjectJ1939* pouLogObjCon = static_cast<CLogObjectJ1939*> (pouLogObjBase);
                BOOL bIsDataLogged = pouLogObjCon->bLogData(m_sCurrFormatDat);

                // PTV [1.6.4]
                if(bIsDataLogged == TRUE)
                {
                    m_bIsJ1939DataLogged = TRUE;
                }
            }
        }
    }
}


/* STARTS FUNCTIONS WHOSE LOGICS ARE IMPLEMENTED IN THIS CLASS  */
// To initialise this module
HRESULT CFrameProcessor_J1939::FPJ1_DoInitialisation(CParamLoggerJ1939* psInitParams)
{
    HRESULT hResult = S_FALSE;

    if (psInitParams != NULL)
    {
        m_sJ1939ProcParams = *psInitParams;
        ASSERT(NULL != m_sJ1939ProcParams.m_pILog);

        m_ouVSEBufJ1939.vClearMessageBuffer();
        //m_sDataCopyThread.m_hActionEvent = m_ouVSEBufJ1939.hGetNotifyingEvent();

        if (this->CFrameProcessor_Common::DoInitialisation() == S_OK)
        {
            if (NULL != m_pouDIL_J1939)
            {
                hResult = m_pouDIL_J1939->DILIJ_ManageMsgBuf(MSGBUF_ADD,
                          m_sJ1939ProcParams.dwClientID, &m_ouVSEBufJ1939);
                if (S_OK != hResult)
                {
                    ASSERT(FALSE);
                }
            }
        }
        else
        {
            ASSERT(FALSE);
        }
    }
    else
    {
        ASSERT(FALSE);
    }

    return hResult;
}

// To modify the filtering scheme of a logging block
HRESULT CFrameProcessor_J1939::FPJ1_ApplyFilteringScheme(USHORT ushLogBlkID,
        const SFILTERAPPLIED_J1939& sFilterObj)
{
    HRESULT hResult = S_FALSE;

    if (bIsEditingON())
    {
        if (m_omLogListTmp.GetSize() > ushLogBlkID)
        {
            CBaseLogObject* pouBaseLogObj = m_omLogListTmp.GetAt(ushLogBlkID);
            CLogObjectJ1939* pLogObj = static_cast<CLogObjectJ1939*> (pouBaseLogObj);

            if (NULL != pLogObj)
            {
                pLogObj->SetFilterInfo(sFilterObj);
                hResult = S_OK;
            }
            else
            {
                ASSERT(FALSE);
            }
        }
    }

    return hResult;
}

// Getter for the filtering scheme of a logging block
HRESULT CFrameProcessor_J1939::FPJ1_GetFilteringScheme(USHORT ushLogBlk,
        SFILTERAPPLIED_J1939& sFilterObj)
{
    HRESULT hResult = S_FALSE;

    CBaseLogObject* pouBaseLogObj = FindLoggingBlock(ushLogBlk);
    CLogObjectJ1939* pouLogObj = static_cast<CLogObjectJ1939*> (pouBaseLogObj);

    if (NULL != pouLogObj)
    {
        pouLogObj->GetFilterInfo(sFilterObj);
        hResult = S_OK;
    }
    else
    {
        ASSERT(FALSE);
    }

    return hResult;
}

/* ENDS FUNCTIONS WHOSE LOGICS ARE IMPLEMENTED IN THIS CLASS  */


// USE COMMON BASE CLASS ALIAS FUNCTIONS: START
/* Call to enable/disable logging for a particular block. Having ushBlk equal
to FOR_ALL, signifies the operation to be performed for all the blocks */
HRESULT CFrameProcessor_J1939::FPJ1_EnableLoggingBlock(USHORT ushBlk, BOOL bEnable)
{
    return EnableLoggingBlock(ushBlk, bEnable);
}

// To enable/disable logging
HRESULT CFrameProcessor_J1939::FPJ1_EnableLogging(BOOL bEnable)
{
    return EnableLogging(bEnable, J1939);
}

void CFrameProcessor_J1939::FPJ1_vCloseLogFile(void)
{
    USHORT ushBlocks = (USHORT) (m_omLogObjectArray.GetSize());

    CBaseLogObject* pouCurrLogObj = NULL;
    for (USHORT i = 0; i < ushBlocks; i++)
    {
        pouCurrLogObj = m_omLogObjectArray.GetAt(i);

        if (pouCurrLogObj != NULL)
        {
            pouCurrLogObj->bStopOnlyLogging();
        }
    }
}

/* Call to enable/disable logging for a particular block. Having ushBlk equal
to FOR_ALL, signifies the operation to be performed for all the blocks */
HRESULT CFrameProcessor_J1939::FPJ1_EnableFilter(USHORT ushBlk, BOOL bEnable)
{
    return EnableFilter(ushBlk, bEnable);
}

// Query function - current logging status (OFF/ON).
BOOL CFrameProcessor_J1939::FPJ1_IsLoggingON(void)
{
    return IsLoggingON();
}

// PTV [1.6.4]
BOOL CFrameProcessor_J1939::FPJ1_IsJ1939DataLogged(void)
{
    return IsJ1939DataLogged();
}

BOOL CFrameProcessor_J1939::FPJ1_IsJ1939ThreadBlocked(void)
{
    return IsThreadBlocked();
}

void CFrameProcessor_J1939::FPJ1_DisableJ1939DataLogFlag(void)
{
    DisableJ1939DataLogFlag();
}
// PTV [1.6.4]
// Query function - current filtering status
BOOL CFrameProcessor_J1939::FPJ1_IsFilterON(void)
{
    return IsFilterON();
}

// To log a string
HRESULT CFrameProcessor_J1939::FPJ1_LogString(CString& omStr)
{
    return LogString(omStr);
}

// To add a logging block; must be in editing mode
HRESULT CFrameProcessor_J1939::FPJ1_AddLoggingBlock(const SLOGINFO& sLogObject)
{
    return AddLoggingBlock(sLogObject);
}

// To remove a logging block by its index in the list; editing mode prerequisite
HRESULT CFrameProcessor_J1939::FPJ1_RemoveLoggingBlock(USHORT ushBlk)
{
    return RemoveLoggingBlock(ushBlk);
}

// Getter for total number of logging blocks
USHORT CFrameProcessor_J1939::FPJ1_GetLoggingBlockCount(void)
{
    return GetLoggingBlockCount();
}

// To clear the logging block list
HRESULT CFrameProcessor_J1939::FPJ1_ClearLoggingBlockList(void)
{
    return ClearLoggingBlockList();
}

// Getter for a logging block by specifying its index in the list
HRESULT CFrameProcessor_J1939::FPJ1_GetLoggingBlock(USHORT ushBlk, SLOGINFO& sLogObject)
{
    return GetLoggingBlock(ushBlk, sLogObject);
}

// Setter for a logging block by specifying its index in the list
HRESULT CFrameProcessor_J1939::FPJ1_SetLoggingBlock(USHORT ushBlk,
        const SLOGINFO& sLogObject)
{
    return SetLoggingBlock(ushBlk, sLogObject);
}

// To reset or revoke the modifications made
HRESULT CFrameProcessor_J1939::FPJ1_Reset(void)
{
    return Reset();
}

// To confirm the modifications made
HRESULT CFrameProcessor_J1939::FPJ1_Confirm(void)
{
    return Confirm();
}

// To start logging block editing session
HRESULT CFrameProcessor_J1939::FPJ1_StartEditingSession(void)
{
    return StartEditingSession();
}

// To stop logging block editing session
HRESULT CFrameProcessor_J1939::FPJ1_StopEditingSession(BOOL bConfirm)
{
    return StopEditingSession(bConfirm);
}

// Getter for the logging configuration data
HRESULT CFrameProcessor_J1939::FPJ1_GetConfigData(BYTE** ppvConfigData, UINT& unLength)
{
    return GetConfigData(ppvConfigData, unLength);
}

// PTV XML
// Getter for the logging configuration data
HRESULT CFrameProcessor_J1939::FPJ1_GetConfigData(xmlNodePtr pNodePtr)
{
    return GetConfigData(pNodePtr);
}
// PTV XML

// Setter for the logging configuration data
HRESULT CFrameProcessor_J1939::FPJ1_SetConfigData(BYTE* pvDataStream, const CString& omStrVersion)
{
    return SetConfigData(pvDataStream, omStrVersion);
}
HRESULT CFrameProcessor_J1939::FPJ1_SetConfigData(xmlDocPtr pDoc)
{
    return SetConfigData(pDoc, J1939);
}
// Empty log object
void CFrameProcessor_J1939::vEmptyLogObjArray(CLogObjArray& omLogObjArray)
{
    USHORT ushBlocks = (USHORT) (omLogObjArray.GetSize());

    if (ushBlocks > 0)
    {
        for (USHORT i = 0; i < ushBlocks; i++)
        {
            CBaseLogObject* pouCurrLogObj = omLogObjArray.GetAt(i);
            DeleteLogObj(pouCurrLogObj);
        }
    }
    omLogObjArray.RemoveAll();
}

//Setter for database files associated
HRESULT CFrameProcessor_J1939::FPJ1_SetDatabaseFiles(const CStringArray& omList)
{
    return SetDatabaseFiles(omList);
}

// To update the channel baud rate info to logger
HRESULT CFrameProcessor_J1939::FPJ1_SetChannelBaudRateDetails
(SCONTROLLER_DETAILS* controllerDetails,
 int nNumChannels)
{
    HRESULT hResult = S_OK;
    SetChannelBaudRateDetails(controllerDetails, nNumChannels);
    return hResult;
}

// USE COMMON BASE CLASS ALIAS FUNCTIONS: END
