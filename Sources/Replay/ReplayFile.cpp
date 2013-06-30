/*
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

/**
 * \file      ReplayFile.cpp
 * \brief     Implementation file for CReplayFile class
 * \author    Raja N
 * \copyright Copyright (c) 2011, Robert Bosch Engineering and Business Solutions. All rights reserved.
 *
 * Implementation file for CReplayFile class
 */

#include "Replay_stdafx.h"             // For standard Include
#include "ReplayFile.h"         // For replay file class declaration
#include <Shlwapi.h>
#include "Utility\UtilFunctions.h"


#define MSG_GET_CONFIGPATH  10000

/*******************************************************************************
  Function Name  : CReplayFile
  Description    : Standard default constructor
  Member of      : CReplayFile
  Functionality  : This will initialise local variables
  Author(s)      : Raja N
  Date Created   : 16.7.2005
  Modifications  :
*******************************************************************************/
CReplayFile::CReplayFile()
    : m_omStrFileName( STR_EMPTY ),
      m_nTimeMode( 0 ),
      m_unMsgTimeDelay( 1 ),
      m_nReplayMode( 0 ),
      m_unCycleTimeDelay( 1 ),
      m_bEnabled( TRUE ),
      m_bInteractive( FALSE ),
      m_ouReplayMsgType( DIR_ALL )
{
    m_sFilterApplied.vClear();
}

/*******************************************************************************
  Function Name  : CReplayFile
  Description    : Standard Copy constructor
  Member of      : CReplayFile
  Functionality  : This will copy local variables with the reference object data
  Author(s)      : Raja N
  Date Created   : 16.7.2005
  Modifications  :
*******************************************************************************/
CReplayFile::CReplayFile(const CReplayFile& ouRef)
{
    // Copy the content of the reference object
    vCopyContent( ouRef );
}

/*******************************************************************************
  Function Name  : operator =
  Input(s)       : ouRef - Object that is to be copied
  Output         : CReplayFile& - Self reference for operator cascading
  Functionality  : Overloaded '=' operator. This will copy RHS object data.
  Member of      : CReplayFile
  Author(s)      : Raja N
  Date Created   : 16.7.2005
  Modifications  :
*******************************************************************************/
CReplayFile& CReplayFile::operator = ( const CReplayFile& ouRef)
{
    if( this != &ouRef )
    {
        vCopyContent( ouRef );
    }
    else
    {
        // Self Assignment
        ASSERT( FALSE );
    }
    // Return current object pointer
    return *this;
}

/*******************************************************************************
  Function Name  : vCopyContent
  Input(s)       : ouRef - Object that has to be copied
  Output         : -
  Functionality  : To copy the content of the reference object.
  Member of      : CReplayFile
  Author(s)      : Raja N
  Date Created   : 16.7.2005
  Modifications  :
*******************************************************************************/
void CReplayFile::vCopyContent(const CReplayFile& ouRef)
{
    // Assign File Name
    m_omStrFileName = ouRef.m_omStrFileName;
    // Time Mode
    m_nTimeMode = ouRef.m_nTimeMode;
    // Specified Delay
    m_unMsgTimeDelay = ouRef.m_unMsgTimeDelay;
    // Repaly Mode
    m_nReplayMode = ouRef.m_nReplayMode;
    // Cyclic Delay
    m_unCycleTimeDelay = ouRef.m_unCycleTimeDelay;
    // Enable Option
    m_bEnabled = ouRef.m_bEnabled;
    // Interactive Option
    m_bInteractive = ouRef.m_bInteractive;
    //Replay Message Type
    m_ouReplayMsgType = ouRef.m_ouReplayMsgType;
    // Copy Filter
    m_sFilterApplied.bClone(ouRef.m_sFilterApplied);
}

/*******************************************************************************
  Function Name  : ~CReplayFile
  Description    : Standard Destructor
  Member of      : CReplayFile
  Functionality  : -
  Author(s)      : Raja N
  Date Created   : 16.7.2005
  Modifications  :
*******************************************************************************/
CReplayFile::~CReplayFile()
{

}

/*******************************************************************************
  Function Name  : unGetConfigSizeOfCommonMembers
  Input(s)       :
  Output         : size of members common to all object
  Functionality  : To get size of members common to all object
  Member of      : CReplayFile
  Author(s)      : Anish kumar
  Date Created   : 08.07.2010
  Modifications  :
*******************************************************************************/
UINT CReplayFile::unGetConfigSizeOfCommonMembers()
{
    UINT unSize = 0;
    unSize += sizeof(m_nTimeMode);
    unSize += sizeof(m_unMsgTimeDelay);
    unSize += sizeof(m_nReplayMode);
    unSize += sizeof(m_unCycleTimeDelay);
    unSize += sizeof(m_bEnabled);
    unSize += sizeof(m_bInteractive);
    unSize += (sizeof(char) * MAX_PATH); // To store the path
    unSize += sizeof(eDirection);           //To store Replay Message Direction
    return unSize;
}

UINT CReplayFile::unGetConfigSize()
{
    UINT unSize = 0;
    unSize += unGetConfigSizeOfCommonMembers();
    unSize += m_sFilterApplied.unGetSize();
    return unSize;
}
/*******************************************************************************
  Function Name  : pbySaveConfig
  Input(s)       : pointer to buffer where the config will be saved
  Output         : End pointer
  Functionality  : To save config data
  Member of      : CReplayFile
  Author(s)      : Anish kumar
  Date Created   : 08.07.2010
  Modifications  :
*******************************************************************************/
BYTE* CReplayFile::pbySaveConfig(BYTE* pDesBuffer)
{
    memcpy(pDesBuffer, &m_nTimeMode, sizeof(m_nTimeMode));
    pDesBuffer += sizeof(m_nTimeMode);
    memcpy(pDesBuffer, &m_unMsgTimeDelay, sizeof(m_unMsgTimeDelay));
    pDesBuffer += sizeof(m_unMsgTimeDelay);
    memcpy(pDesBuffer, &m_nReplayMode, sizeof(m_nReplayMode));
    pDesBuffer += sizeof(m_nReplayMode);
    memcpy(pDesBuffer, &m_unCycleTimeDelay, sizeof(m_unCycleTimeDelay));
    pDesBuffer += sizeof(m_unCycleTimeDelay);
    memcpy(pDesBuffer, &m_bEnabled, sizeof(m_bEnabled));
    pDesBuffer += sizeof(m_bEnabled);
    memcpy(pDesBuffer, &m_bInteractive, sizeof(m_bInteractive));
    pDesBuffer += sizeof(m_bInteractive);
    //Save file name size
    char acName[MAX_PATH] = {_T('\0')};
    //Tobias- venkat
    strcpy_s(acName, MAX_PATH, m_omStrFileName.GetBuffer(MAX_PATH));
    memcpy(pDesBuffer, acName, sizeof(char) * MAX_PATH);
    pDesBuffer += sizeof(char) * MAX_PATH;
    //Message Direction
    memcpy(pDesBuffer, &m_ouReplayMsgType, sizeof(eDirection));
    pDesBuffer += sizeof(m_ouReplayMsgType);
    //To store filters
    //This function will copy into the destination bufffer and increment the pointer
    pDesBuffer = m_sFilterApplied.pbGetConfigData(pDesBuffer);
    return pDesBuffer;
}

BOOL CReplayFile::pbySaveConfig(xmlNodePtr pxmlNodePtr)         //replay is the parent node coming to this function
{
    const char* omcVarChar ;

    //<Log_File_Path>path</Log_File_Path>

    string omPath, omStrConfigFolder;
    char configPath[MAX_PATH];
    AfxGetMainWnd()->SendMessage(10000, (WPARAM)configPath, 0);
    CUtilFunctions::nGetBaseFolder(configPath, omStrConfigFolder );
    CUtilFunctions::MakeRelativePath(omStrConfigFolder.c_str(), (char*)m_omStrFileName.GetBuffer(MAX_PATH), omPath);


    omcVarChar = omPath.c_str();



    xmlNodePtr pPath = xmlNewChild(pxmlNodePtr, NULL, BAD_CAST DEF_LOG_PATH,BAD_CAST omcVarChar);
    xmlAddChild(pxmlNodePtr, pPath);

    //<IsEnabled>bool</IsEnabled>
    CString     csEnabled;
    csEnabled.Format("%d",m_bEnabled);
    omcVarChar = csEnabled;
    xmlNodePtr pEnabled = xmlNewChild(pxmlNodePtr, NULL, BAD_CAST DEF_ENABLED,BAD_CAST omcVarChar);
    xmlAddChild(pxmlNodePtr, pEnabled);

    //<Retain_Recored_Time>bool</Retain_Recored_Time>
    CString     csRetainTimeRec;
    csRetainTimeRec.Format("%d",m_nTimeMode);
    omcVarChar = csRetainTimeRec;
    xmlNodePtr pRetainTimeRec = xmlNewChild(pxmlNodePtr, NULL, BAD_CAST DEF_RETAIN_RECORD_TIME,BAD_CAST omcVarChar);
    xmlAddChild(pxmlNodePtr, pRetainTimeRec);

    //<Message_Delay>1</Message_Delay>
    CString     csMsgDelay;
    csMsgDelay.Format("%u",m_unMsgTimeDelay);
    omcVarChar = csMsgDelay;
    xmlNodePtr pMsgDelay = xmlNewChild(pxmlNodePtr, NULL, BAD_CAST DEF_MSG_DELAY,BAD_CAST omcVarChar);
    xmlAddChild(pxmlNodePtr, pMsgDelay);

    // <Is_Cyclic_Mode>bool</Is_Cyclic_Mode>
    CString     csCyclicMode;
    csCyclicMode.Format("%d",m_nReplayMode);
    omcVarChar = csCyclicMode;
    xmlNodePtr pCyclicMode = xmlNewChild(pxmlNodePtr, NULL, BAD_CAST DEF_CYCLIC_MODE,BAD_CAST omcVarChar);
    xmlAddChild(pxmlNodePtr, pCyclicMode);

    //<Cyclic_Delay>int</Cyclic_Delay>
    CString     csCyclicDelay;
    csCyclicDelay.Format("%u",m_unCycleTimeDelay);
    omcVarChar = csCyclicDelay;
    xmlNodePtr pCyclicDelay = xmlNewChild(pxmlNodePtr, NULL, BAD_CAST DEF_CYCLIC_DELAY,BAD_CAST omcVarChar);
    xmlAddChild(pxmlNodePtr, pCyclicDelay);

    //  <IsInteractive>FALSE</IsInteractive>
    CString     csIterative;
    csIterative.Format("%u",m_bInteractive);
    omcVarChar = csIterative;
    xmlNodePtr pIterative = xmlNewChild(pxmlNodePtr, NULL, BAD_CAST DEF_INTERACTIVE,BAD_CAST omcVarChar);
    xmlAddChild(pxmlNodePtr, pIterative);

    //<Replay_Message_Direction>ALL</Replay_Message_Direction>
    CString     csMsgDirection;
    if(m_ouReplayMsgType == DIR_RX)
    {
        csMsgDirection = "RX";
    }
    else if(m_ouReplayMsgType == DIR_TX)
    {
        csMsgDirection = "TX";
    }
    else
    {
        csMsgDirection = "ALL";
    }

    omcVarChar = csMsgDirection;
    xmlNodePtr pDirection = xmlNewChild(pxmlNodePtr, NULL, BAD_CAST DEF_REPLAY_MSG_DIRECTN,BAD_CAST omcVarChar);
    xmlAddChild(pxmlNodePtr, pDirection);

    //<Filter>FILTER_NAME</Filter>
    CString     csFilter;
    for(int iCnt=0; iCnt < m_sFilterApplied.m_ushTotal; iCnt++)
    {
        csFilter.Format("%s",((m_sFilterApplied.m_psFilters)+iCnt)->m_sFilterName.m_acFilterName);
        omcVarChar = csFilter;
        xmlNodePtr pFilter = xmlNewChild(pxmlNodePtr, NULL, BAD_CAST DEF_FILTER,BAD_CAST omcVarChar);
        xmlAddChild(pxmlNodePtr, pFilter);
    }

    return TRUE;
}
/*******************************************************************************
  Function Name  : pbyLoadConfig
  Input(s)       : pointer to buffer from where the config will be loaded
  Output         : End pointer
  Functionality  : To load config data
  Member of      : CReplayFile
  Author(s)      : Anish kumar
  Date Created   : 08.07.2010
  Modifications  :
*******************************************************************************/
BYTE* CReplayFile::pbyLoadConfig(BYTE* pSrcBuffer, INT nSectionVersion)
{
    if (NULL == pSrcBuffer)
    {
        return NULL;
    }
    memcpy(&m_nTimeMode, pSrcBuffer, sizeof(m_nTimeMode));
    pSrcBuffer += sizeof(m_nTimeMode);
    memcpy(&m_unMsgTimeDelay, pSrcBuffer, sizeof(m_unMsgTimeDelay));
    pSrcBuffer += sizeof(m_unMsgTimeDelay);
    memcpy(&m_nReplayMode, pSrcBuffer, sizeof(m_nReplayMode));
    pSrcBuffer += sizeof(m_nReplayMode);
    memcpy(&m_unCycleTimeDelay, pSrcBuffer, sizeof(m_unCycleTimeDelay));
    pSrcBuffer += sizeof(m_unCycleTimeDelay);
    memcpy(&m_bEnabled, pSrcBuffer, sizeof(m_bEnabled));
    pSrcBuffer += sizeof(m_bEnabled);
    memcpy(&m_bInteractive, pSrcBuffer, sizeof(m_bInteractive));
    pSrcBuffer += sizeof(m_bInteractive);
    //Save file name size
    char acName[MAX_PATH] = {_T('\0')};
    memcpy(acName, pSrcBuffer, sizeof(char) * MAX_PATH);
    pSrcBuffer += (sizeof(char) * MAX_PATH);
    //Save file name
    m_omStrFileName.Format("%s", acName);

    if( nSectionVersion >= REPLAY_MANAGER_REPLAY_MSG_ADD )
    {
        memcpy(&m_ouReplayMsgType, pSrcBuffer, sizeof(m_ouReplayMsgType));
        pSrcBuffer += sizeof(m_ouReplayMsgType);
    }
    ////To store filters
    bool bRet = false;
    pSrcBuffer = m_sFilterApplied.pbSetConfigData(pSrcBuffer, bRet);
    return pSrcBuffer;
}
int CReplayFile::nLoadXMLConfig(xmlNodePtr pNodePtr)
{
    int nRetValue = S_OK;
    xmlNodePtr pTempNode = pNodePtr;
    CStringArray omStrFilters;
    while( NULL != pNodePtr )
    {
        if ((!xmlStrcmp(pNodePtr->name, (const xmlChar*)"Log_File_Path")))
        {
            xmlChar* key = xmlNodeListGetString(pNodePtr->doc, pNodePtr->xmlChildrenNode, 1);
            if(NULL != key)
            {
                if(PathIsRelative((char*)key) == TRUE)
                {
                    string omStrConfigFolder;
                    string omPath;
                    char configPath[MAX_PATH];
                    AfxGetMainWnd()->SendMessage(MSG_GET_CONFIGPATH, (WPARAM)configPath, 0);
                    CUtilFunctions::nGetBaseFolder(configPath, omStrConfigFolder );
                    char chAbsPath[MAX_PATH];
                    PathCombine(chAbsPath, omStrConfigFolder.c_str(), (char*)key);
                    m_omStrFileName = chAbsPath;
                }
                else
                {
                    m_omStrFileName = (char*)key;
                }
                xmlFree(key);
            }
        }

        if ((!xmlStrcmp(pNodePtr->name, (const xmlChar*)"IsEnabled")))
        {
            xmlChar* key = xmlNodeListGetString(pNodePtr->doc, pNodePtr->xmlChildrenNode, 1);
            if(NULL != key)
            {
                m_bEnabled = xmlUtils::bGetBooleanValue((char*)key);
                xmlFree(key);
            }
        }


        if ((!xmlStrcmp(pNodePtr->name, (const xmlChar*)"Retain_Recored_Time")))
        {
            xmlChar* key = xmlNodeListGetString(pNodePtr->doc, pNodePtr->xmlChildrenNode, 1);
            if(NULL != key)
            {
                m_nTimeMode = xmlUtils::bGetBooleanValue((char*)key);
                xmlFree(key);
            }
        }
        if ((!xmlStrcmp(pNodePtr->name, (const xmlChar*)"Message_Delay")))
        {
            xmlChar* key = xmlNodeListGetString(pNodePtr->doc, pNodePtr->xmlChildrenNode, 1);
            if(NULL != key)
            {
                m_unMsgTimeDelay = atoi((char*)key);
                xmlFree(key);
            }
        }

        if ((!xmlStrcmp(pNodePtr->name, (const xmlChar*)"Is_Cyclic_Mode")))
        {
            xmlChar* key = xmlNodeListGetString(pNodePtr->doc, pNodePtr->xmlChildrenNode, 1);
            if(NULL != key)
            {
                m_nReplayMode = (int)xmlUtils::bGetBooleanValue((char*)key);
                xmlFree(key);
            }
        }


        if ((!xmlStrcmp(pNodePtr->name, (const xmlChar*)"Cyclic_Delay")))
        {
            xmlChar* key = xmlNodeListGetString(pNodePtr->doc, pNodePtr->xmlChildrenNode, 1);
            if(NULL != key)
            {
                m_unCycleTimeDelay = atoi((char*)key);
                xmlFree(key);
            }
        }
        if ((!xmlStrcmp(pNodePtr->name, (const xmlChar*)"IsInteractive")))
        {
            xmlChar* key = xmlNodeListGetString(pNodePtr->doc, pNodePtr->xmlChildrenNode, 1);
            if(NULL != key)
            {
                m_bInteractive = (int)xmlUtils::bGetBooleanValue((char*)key);
                xmlFree(key);
            }
        }
        if ((!xmlStrcmp(pNodePtr->name, (const xmlChar*)"Replay_Message_Direction")))
        {
            xmlChar* key = xmlNodeListGetString(pNodePtr->doc, pNodePtr->xmlChildrenNode, 1);
            if(NULL != key)
            {
                this->m_ouReplayMsgType = xmlUtils::bGetDirection((char*)key);
                xmlFree(key);
            }
        }

        if ((!xmlStrcmp(pNodePtr->name, (const xmlChar*)"Filter")))
        {
            xmlChar* key = xmlNodeListGetString(pNodePtr->doc, pNodePtr->xmlChildrenNode, 1);
            if(NULL != key)
            {
                omStrFilters.Add((char*)key);
                xmlFree(key);
            }
        }
        pNodePtr = pNodePtr->next;
    }
    //Filters

    if( omStrFilters.GetSize() > 0 && pTempNode != NULL)
    {
        SFILTERAPPLIED_CAN sFilterApplied;
        if( sFilterApplied.nSetXMLConfigData(pTempNode->doc) == S_OK)
        {
            sFilterApplied.nGetFiltersFromName(m_sFilterApplied, omStrFilters);
            /*
            int nRealFilters = 0;
            for(int i = 0; i < omStrFilters.GetSize(); i++)
            {
                int nIndex = GetFilterNameIndex((LPCSTR)omStrFilters.GetAt(i), sFilterApplied);
                if(nIndex >= 0)
                {
                    nRealFilters++;
                }
                else
                {
                    omStrFilters.RemoveAt(i);
                }
            }
            m_sFilterApplied.m_bEnabled = sFilterApplied.m_bEnabled;
            m_sFilterApplied.m_ushTotal = omStrFilters.GetSize();
            m_sFilterApplied.m_psFilters = new SFILTERSET[m_sFilterApplied.m_ushTotal];

            for(int i = 0; i < omStrFilters.GetSize(); i++)
            {
                int nIndex = GetFilterNameIndex((LPCSTR)omStrFilters.GetAt(i), sFilterApplied);
                if(nIndex >= 0)
                {
                    m_sFilterApplied.m_psFilters[i].bClone(sFilterApplied.m_psFilters[nIndex]);
                }
            }*/
        }

    }
    return nRetValue;
}
void CReplayFile::GetFileName(CString& omStrFileName)
{
    omStrFileName = m_omStrFileName;
}
/*******************************************************************************
  Function Name  : bisConfigChanged
  Input(s)       : pointer to buffer where the last config was saved
                   it will be incremented to point next replay file
  Output         : BOOL
  Functionality  : To find if config is modified
  Member of      : CReplayFile
  Author(s)      : Anish kumar
  Date Created   : 08.07.2010
  Modifications  :
*******************************************************************************/
BOOL CReplayFile::bisConfigChanged(BYTE*& pSrcBuffer)
{
    BOOL bReturn = TRUE;
    if (NULL != pSrcBuffer)
    {
        memcpy(&m_nTimeModeTmp, pSrcBuffer, sizeof(m_nTimeModeTmp));
        pSrcBuffer += sizeof(m_nTimeModeTmp);
        if (m_nTimeMode != m_nTimeModeTmp)
        {
            bReturn = FALSE;
        }
        if (bReturn)
        {
            memcpy(&m_unTimeDelTmp, pSrcBuffer, sizeof(m_unTimeDelTmp));
            pSrcBuffer += sizeof(m_unTimeDelTmp);
            if (m_unMsgTimeDelay != m_unTimeDelTmp)
            {
                bReturn = FALSE;
            }
        }
        if (bReturn)
        {
            memcpy(&m_nRepModeTmp, pSrcBuffer, sizeof(m_nRepModeTmp));
            pSrcBuffer += sizeof(m_nRepModeTmp);
            if (m_nReplayMode != m_nRepModeTmp)
            {
                bReturn = FALSE;
            }
        }
        if (bReturn)
        {
            memcpy(&m_unCycleTimeTmp, pSrcBuffer, sizeof(m_unCycleTimeTmp));
            pSrcBuffer += sizeof(m_unCycleTimeTmp);
            if (m_unCycleTimeDelay != m_unCycleTimeTmp)
            {
                bReturn = FALSE;
            }
        }
        if (bReturn)
        {
            memcpy(&m_bEnabledTmp, pSrcBuffer, sizeof(m_bEnabledTmp));
            pSrcBuffer += sizeof(m_bEnabledTmp);
            if (m_bEnabled != m_bEnabledTmp)
            {
                bReturn = FALSE;
            }
        }
        if (bReturn)
        {
            memcpy(&m_bInteractiveTmp, pSrcBuffer, sizeof(m_bInteractiveTmp));
            pSrcBuffer += sizeof(m_bInteractiveTmp);
            if (m_bInteractive != m_bInteractiveTmp)
            {
                bReturn = FALSE;
            }
        }
        int nNameSize = 0;
        if (bReturn)
        {
            memcpy(&nNameSize, pSrcBuffer, sizeof(nNameSize));
            pSrcBuffer += sizeof(nNameSize);
            if (nNameSize != m_omStrFileName.GetLength())
            {
                bReturn = FALSE;
            }
        }
        if (bReturn)
        {
            m_omNameTmp = pSrcBuffer;
            pSrcBuffer += (nNameSize + 1);
            if (m_omStrFileName != m_omNameTmp)
            {
                bReturn = FALSE;
            }
        }
        UINT nFilterSize = 0;
        if (bReturn)
        {
            memcpy(&nFilterSize, pSrcBuffer, sizeof(UINT));
            //To Store no. of filters
            pSrcBuffer += sizeof (UINT);
            if (nFilterSize != m_sFilterApplied.unGetSize())
            {
                bReturn = FALSE;
            }
        }
        if (bReturn)
        {
            BYTE* pbData = new BYTE[nFilterSize];
            m_sFilterApplied.pbGetConfigData(pbData);
            if (memcmp((void*)pSrcBuffer, (void*)pbData, nFilterSize) != 0)
            {
                bReturn = FALSE;
            }
        }
    }
    return bReturn;
}

