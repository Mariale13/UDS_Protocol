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
 * \file      SignalWatch_CAN.cpp
 * \author    Ratnadip Choudhury
 * \copyright Copyright (c) 2011, Robert Bosch Engineering and Business Solutions. All rights reserved.
 */
#include "SignalWatch_stdafx.h"
#include "Include/Utils_macro.h"
#include "SignalWatch_CAN.h"
#include "Utility/MsgInterpretation.h"
#include "include/XMLDefines.h"
#include "Utility/XMLUtils.h"

int ReadCANDataBuffer(CSignalWatch_CAN* pSWCan)
{
    ASSERT(pSWCan != NULL);
    while (pSWCan->m_ouCanBufFSE.GetMsgCount() > 0)
    {
        static STCANDATA sCanData;
        INT Result = pSWCan->m_ouCanBufFSE.ReadFromBuffer(&sCanData);
        if (Result == ERR_READ_MEMORY_SHORT)
        {
            TRACE("ERR_READ_MEMORY_SHORT");
        }
        else if (Result == EMPTY_APP_BUFFER)
        {
            TRACE("EMPTY_APP_BUFFER");
        }
        else
        {
            pSWCan->vDisplayInSigWatchWnd(sCanData);
        }

    }
    return 0;
}

DWORD WINAPI SigWatchDataReadThreadProc_C(LPVOID pVoid)
{
    CPARAM_THREADPROC* pThreadParam = (CPARAM_THREADPROC*) pVoid;
    if (pThreadParam == NULL)
    {
        return (DWORD)-1;
    }
    CSignalWatch_CAN* pSWCan = (CSignalWatch_CAN*)pThreadParam->m_pBuffer;
    if (pSWCan == NULL)
    {
        return (DWORD)-1;
    }
    bool bLoopON = true;
    pThreadParam->m_unActionCode = INVOKE_FUNCTION;
    while (bLoopON)
    {
        WaitForSingleObject(pThreadParam->m_hActionEvent, INFINITE);
        switch (pThreadParam->m_unActionCode)
        {
            case INVOKE_FUNCTION:
            {
                ReadCANDataBuffer(pSWCan); // Retrieve message from the driver
            }
            break;
            case EXIT_THREAD:
            {
                bLoopON = false;
            }
            break;
            case CREATE_TIME_MAP:
            {
                //Nothing at this moment
            }
            break;
            default:
            case INACTION:
            {
                // nothing right at this moment
            }
            break;
        }
    }
    SetEvent(pThreadParam->hGetExitNotifyEvent());
    return 0;
}

BOOL CSignalWatch_CAN::InitInstance(void)
{
    InitializeCriticalSection(&m_omCritSecSW);
    m_pouSigWnd = NULL;
    m_pMsgInterPretObj = NULL;
    m_ouReadThread.m_hActionEvent = m_ouCanBufFSE.hGetNotifyingEvent();
    return TRUE;
}

int CSignalWatch_CAN::ExitInstance(void)
{
    m_ouReadThread.bTerminateThread(); // Terminate read thread
    m_ouCanBufFSE.vClearMessageBuffer();//clear can buffer

    if (m_pMsgInterPretObj != NULL) // clear interpretation object
    {
        m_pMsgInterPretObj->vClear();
        delete m_pMsgInterPretObj;
        m_pMsgInterPretObj = NULL;
    }

    DeleteCriticalSection(&m_omCritSecSW); //delete critical section

    if (m_pouSigWnd != NULL)
    {
        m_pouSigWnd->DestroyWindow();
        delete m_pouSigWnd;
        m_pouSigWnd = NULL;
    }
    return TRUE;
}

BOOL CSignalWatch_CAN::bStartSigWatchReadThread()
{
    m_ouReadThread.m_pBuffer = this;
    m_ouReadThread.bStartThread(SigWatchDataReadThreadProc_C);
    return TRUE;
}

void CSignalWatch_CAN::vDisplayInSigWatchWnd(STCANDATA& sCanData)
{
    EnterCriticalSection(&m_omCritSecSW);
    if (m_pMsgInterPretObj != NULL)
    {
        static CString omMsgName;
        static CStringArray omSigNames, omRawValues, omPhyValues;
        if (m_pMsgInterPretObj->bInterpretMsgSigList(sCanData.m_uDataInfo.m_sCANMsg.m_unMsgID,
                sCanData.m_uDataInfo.m_sCANMsg.m_ucData,omMsgName, omSigNames, omRawValues, omPhyValues, m_bHex))
        {
            if ((m_pouSigWnd != NULL) && (m_pouSigWnd->IsWindowVisible()))
            {
                m_pouSigWnd->vAddMsgSigIntoList(omMsgName,omSigNames, omRawValues, omPhyValues, FALSE);
            }
        }
    }
    LeaveCriticalSection(&m_omCritSecSW);

    //delete the invalid entries
    vDeleteRemovedListEntries();
}

void CSignalWatch_CAN::vDeleteRemovedListEntries()
{
    if ((m_pMsgInterPretObj != NULL) && (m_pouSigWnd != NULL))
    {
        CStringArray strMsgList;
        int inSize = 0;

        //get the number of signals
        int iCount = (int)m_pouSigWnd->m_omSignalList.GetItemCount();

        for(int iIndex = 0; iIndex < iCount ; iIndex++ )
        {
            CString strMsgName = m_pouSigWnd->m_omSignalList.GetItemText(iIndex, 0);
            inSize = (int)strMsgList.GetSize();
            bool bFound = false;

            //check for the unique message name
            for(int inPos = 0; inPos < inSize ; inPos++ )
            {
                if(strMsgName.CompareNoCase(strMsgList.GetAt(inPos)) == 0 )
                {
                    bFound = true;
                    break;
                }
            }

            //add the message in list
            if(bFound == false)
            {
                strMsgList.Add(strMsgName);
            }
        }

        //get the message count
        inSize = (int)strMsgList.GetSize();

        for(int inPos = 0; inPos < inSize ; inPos++ )
        {
            CString strMsgName = strMsgList.GetAt(inPos);
            int iSignalCount = m_pMsgInterPretObj->nGetSignalCount(strMsgName);
            if(iSignalCount == 0 ) //no signal is configured
            {
                if((int)m_pouSigWnd->m_omSignalList.GetItemCount() > 0  )
                {
                    //remove the signal from signal watch window.
                    m_pouSigWnd->SendMessage( WM_REMOVE_SIGNAL, 0, (LPARAM) &strMsgName);
                }
            }
        }
    }
}

HRESULT CSignalWatch_CAN::SW_DoInitialization()
{
    AFX_MANAGE_STATE(AfxGetStaticModuleState());
    //Create the signal watch window for CAN
    if (m_pouSigWnd == NULL)
    {
        m_pouSigWnd = new CSigWatchDlg(AfxGetMainWnd());
        m_pouSigWnd->Create(IDD_DLG_SIGNAL_WATCH, NULL);
        m_pouSigWnd->SetWindowText("Signal Watch - CAN");
    }

    CBaseDIL_CAN* pouDIL_CAN;
    if (DIL_GetInterface(CAN, (void**)&pouDIL_CAN) == S_OK)
    {
        DWORD dwClientId = 0;
        pouDIL_CAN->DILC_RegisterClient(TRUE, dwClientId, CAN_MONITOR_NODE);
        pouDIL_CAN->DILC_ManageMsgBuf(MSGBUF_ADD, dwClientId, &(m_ouCanBufFSE));
    }
    //Start the read thread
    return bStartSigWatchReadThread()? S_OK: S_FALSE;
}

/**
 * \req RS_18_01 Signal watch configuration dialog box
 *
 * Signal watch configuration dialog box
 * lists frames / messages from the loaded database (combo box / list box).
 */
HRESULT CSignalWatch_CAN::SW_ShowAddDelSignalsDlg(CWnd* pParent, CMainEntryList* podMainSubList)
{
    AFX_MANAGE_STATE(AfxGetStaticModuleState());

    CSigWatchAddDelDlg odDlg(pParent, podMainSubList);
    return (HRESULT)odDlg.DoModal();
}

HRESULT CSignalWatch_CAN::SW_ShowSigWatchWnd(CWnd* /*pParent*/, INT nCmd)
{
    if (m_pouSigWnd != NULL)
    {
        return m_pouSigWnd->ShowWindow(nCmd);
    }
    return S_FALSE;
}

HRESULT CSignalWatch_CAN::SW_GetConfigSize(void)
{
    return (sizeof (WINDOWPLACEMENT) + sizeof(UINT) * defSW_LIST_COLUMN_COUNT);
}

HRESULT CSignalWatch_CAN::SW_GetConfigData(void* pbyConfigData)
{
    WINDOWPLACEMENT WndPlace;
    UINT nDebugSize  = 0;
    BYTE* pbyTemp = (BYTE*)pbyConfigData;
    if ((m_pouSigWnd != NULL) && (pbyTemp != NULL))
    {
        m_pouSigWnd->GetWindowPlacement(&WndPlace);
        COPY_DATA(pbyTemp, &WndPlace, sizeof (WINDOWPLACEMENT));
        for (UINT i = 0; i < defSW_LIST_COLUMN_COUNT; i++)
        {
            INT nWidth = m_pouSigWnd->m_omSignalList.GetColumnWidth(i);
            COPY_DATA(pbyTemp, &nWidth, sizeof (INT));
        }
    }
    return S_OK;
}
// PTV XML
HRESULT CSignalWatch_CAN::SW_GetConfigData(xmlNodePtr pNodePtr)
{

    // Setting signal watch window placement and column width
    xmlNodePtr pWndPositn = xmlNewNode(NULL, BAD_CAST DEF_WINDOW_POSITION);
    xmlAddChild(pNodePtr, pWndPositn);

    WINDOWPLACEMENT WndPlace;
    UINT nDebugSize  = 0;
    //BYTE* pbyTemp = (BYTE*)pbyConfigData;
    if ((m_pouSigWnd != NULL))
    {
        m_pouSigWnd->GetWindowPlacement(&WndPlace);
        //COPY_DATA(pbyTemp, &WndPlace, sizeof (WINDOWPLACEMENT));

        BOOL bIsWndwVisible = m_pouSigWnd->IsWindowVisible();

        CString strWindwVisibility = "";

        strWindwVisibility = xmlUtils::nSetWindowVisibility(WndPlace.showCmd);

        // Writing visibility in to xml
        xmlNodePtr pVisibility = xmlNewChild(pWndPositn, NULL, BAD_CAST DEF_VISIBILITY, BAD_CAST strWindwVisibility.GetBuffer(strWindwVisibility.GetLength()));
        xmlAddChild(pWndPositn, pVisibility);

        UINT nFlag = WndPlace.flags;

        CString strWindPlcmnt = "";

        strWindPlcmnt = xmlUtils::nSetWindowVisibility(nFlag);

        // Setting window Placement
        xmlNodePtr pWndwVisibility = xmlNewChild(pWndPositn, NULL, BAD_CAST DEF_WINDOW_PLACEMENT, BAD_CAST strWindPlcmnt.GetBuffer(strWindPlcmnt.GetLength()));
        xmlAddChild(pWndPositn, pWndwVisibility);

        CString strBottomPos = "", strLeftPos = "", strTopPos = "", strRightPos = "";

        strTopPos.Format("%d", WndPlace.rcNormalPosition.top);
        strLeftPos.Format("%d", WndPlace.rcNormalPosition.left);
        strRightPos.Format("%d", WndPlace.rcNormalPosition.right);
        strBottomPos.Format("%d", WndPlace.rcNormalPosition.bottom);

        // Writing co-ordinates of the window in to XML file
        xmlNodePtr pTopPos = xmlNewChild(pWndPositn, NULL, BAD_CAST DEF_TOP, BAD_CAST strTopPos.GetBuffer(strTopPos.GetLength()));
        xmlAddChild(pWndPositn, pTopPos);

        xmlNodePtr pLeftPos = xmlNewChild(pWndPositn, NULL, BAD_CAST DEF_Left,BAD_CAST strLeftPos.GetBuffer(strLeftPos.GetLength()));
        xmlAddChild(pWndPositn, pLeftPos);

        xmlNodePtr pRightPos = xmlNewChild(pWndPositn, NULL, BAD_CAST DEF_Right,BAD_CAST strRightPos.GetBuffer(strRightPos.GetLength()));
        xmlAddChild(pWndPositn, pRightPos);

        xmlNodePtr pBottomPos = xmlNewChild(pWndPositn, NULL, BAD_CAST DEF_Bottom,BAD_CAST strBottomPos.GetBuffer(strBottomPos.GetLength()));
        xmlAddChild(pWndPositn, pBottomPos);

        // Setting Column width in to the xml file
        xmlNodePtr pColWidthPtr = xmlNewNode(NULL, BAD_CAST DEF_Columns_Width);
        xmlAddChild(pNodePtr, pColWidthPtr);

        for (UINT i = 0; i < defSW_LIST_COLUMN_COUNT; i++)
        {
            INT nWidth = m_pouSigWnd->m_omSignalList.GetColumnWidth(i);
            CString strWidth = "";
            strWidth.Format("%d", nWidth);

            if(i == 0)
            {
                // Writing message column value in to xml
                xmlNodePtr pMsgClmnPtr = xmlNewChild(pColWidthPtr, NULL, BAD_CAST DEF_Message_Column, BAD_CAST strWidth.GetBuffer(strWidth.GetLength()));
                xmlAddChild(pColWidthPtr, pMsgClmnPtr);
            }
            if(i == 1)
            {
                // Writing Raw value column value in to xml
                xmlNodePtr pRawValClmn = xmlNewChild(pColWidthPtr, NULL, BAD_CAST DEF_Raw_Val_Column, BAD_CAST strWidth.GetBuffer(strWidth.GetLength()));
                xmlAddChild(pColWidthPtr, pRawValClmn);
            }
            if(i == 2)
            {
                // Writing Raw value column value in to xml
                xmlNodePtr pPhyalClmn = xmlNewChild(pColWidthPtr, NULL, BAD_CAST DEF_Physical_Val_Column, BAD_CAST strWidth.GetBuffer(strWidth.GetLength()));
                xmlAddChild(pColWidthPtr, pPhyalClmn);
            }
            if(i == 3)
            {
                // Writing Raw value column value in to xml
                xmlNodePtr pSigValClmn = xmlNewChild(pColWidthPtr, NULL, BAD_CAST DEF_Signal_Column, BAD_CAST strWidth.GetBuffer(strWidth.GetLength()));
                xmlAddChild(pColWidthPtr, pSigValClmn);
            }
        }
    }
    return S_OK;
}
// PTV XML

HRESULT CSignalWatch_CAN::SW_SetConfigData(const void* pbyConfigData)
{
    const BYTE* pbyTemp = (BYTE*)pbyConfigData;
    if ((pbyConfigData != NULL) && (m_pouSigWnd != NULL))
    {
        WINDOWPLACEMENT WndPlace;
        memcpy(&WndPlace, pbyConfigData, sizeof (WINDOWPLACEMENT));
        m_pouSigWnd->MoveWindow(&(WndPlace.rcNormalPosition), FALSE);
        pbyTemp += sizeof (WINDOWPLACEMENT);
        for (UINT i = 0; i < defSW_LIST_COLUMN_COUNT; i++)
        {
            m_pouSigWnd->m_omSignalList.SetColumnWidth(i, pbyTemp[i]);
        }
    }
    if(m_pouSigWnd != NULL)
    {
        //Signal watch window will move the List control in OnSize().
        //So the default values should be as followes.
        for (UINT i = 0; i < defSW_LIST_COLUMN_COUNT; i++)
        {
            RECT sClientRect;
            m_pouSigWnd->GetClientRect(&sClientRect);
            int ClientWidth = abs(sClientRect.left - sClientRect.right);
            m_pouSigWnd->m_omSignalList.SetColumnWidth(0, (int)(0.2 * ClientWidth));
            m_pouSigWnd->m_omSignalList.SetColumnWidth(1, (int)(0.2 * ClientWidth));
            m_pouSigWnd->m_omSignalList.SetColumnWidth(2, (int)(0.4 * ClientWidth));
            m_pouSigWnd->m_omSignalList.SetColumnWidth(3, (int)(0.2 * ClientWidth));
            m_pouSigWnd->m_omSignalList.MoveWindow(&sClientRect);
        }

        if(pbyConfigData == NULL)
        {
            m_pouSigWnd->ShowWindow(SW_HIDE);
        }
    }
    return S_OK;
}
//MVN
HRESULT CSignalWatch_CAN::SW_SetConfigData(xmlNodePtr pNode)
{
    INT nRetValue  = S_OK;
    if ((pNode != NULL) && (m_pouSigWnd != NULL))
    {
        WINDOWPLACEMENT WndPlace;
        while(pNode != NULL)
        {
            if ((!xmlStrcmp(pNode->name, (const xmlChar*)"Window_Position")))
            {
                nRetValue = xmlUtils::ParseWindowsPlacement(pNode, WndPlace);
                if(nRetValue == S_OK)
                {
                    m_pouSigWnd->MoveWindow(&(WndPlace.rcNormalPosition), FALSE);
                }
                break;
            }
            pNode = pNode->next;
        }
    }
    if(m_pouSigWnd != S_OK)
    {
        //Signal watch window will move the List control in OnSize().
        //So the default values should be as followes.
        for (UINT i = 0; i < defSW_LIST_COLUMN_COUNT; i++)
        {
            RECT sClientRect;
            m_pouSigWnd->GetClientRect(&sClientRect);
            int ClientWidth = abs(sClientRect.left - sClientRect.right);
            m_pouSigWnd->m_omSignalList.SetColumnWidth(0, (int)(0.2 * ClientWidth));
            m_pouSigWnd->m_omSignalList.SetColumnWidth(1, (int)(0.2 * ClientWidth));
            m_pouSigWnd->m_omSignalList.SetColumnWidth(2, (int)(0.4 * ClientWidth));
            m_pouSigWnd->m_omSignalList.SetColumnWidth(3, (int)(0.2 * ClientWidth));
            m_pouSigWnd->m_omSignalList.MoveWindow(&sClientRect);
        }
    }
    return S_OK;
}
INT CSignalWatch_CAN::nParseXMLColumn(xmlNodePtr pNode)
{
    INT nRetVal = S_OK;
    pNode = pNode->children;
    while(pNode != NULL)
    {
        if ((!xmlStrcmp(pNode->name, (const xmlChar*)"Message_Column")) == FALSE)
        {
            xmlChar* key = xmlNodeListGetString(pNode->doc, pNode->xmlChildrenNode, 1);
            if(NULL != key)
            {
                m_pouSigWnd->m_omSignalList.SetColumnWidth(0, atoi((char*)key));
                xmlFree(key);
            }
        }

        if ((!xmlStrcmp(pNode->name, (const xmlChar*)"Raw_Val_Column")) == FALSE)
        {
            xmlChar* key = xmlNodeListGetString(pNode->doc, pNode->xmlChildrenNode, 1);
            if(NULL != key)
            {
                m_pouSigWnd->m_omSignalList.SetColumnWidth(1, atoi((char*)key));
                xmlFree(key);
            }
        }

        if ((!xmlStrcmp(pNode->name, (const xmlChar*)"Physical_Val_Column")) == FALSE)
        {
            xmlChar* key = xmlNodeListGetString(pNode->doc, pNode->xmlChildrenNode, 1);
            if(NULL != key)
            {
                m_pouSigWnd->m_omSignalList.SetColumnWidth(2, atoi((char*)key));
                xmlFree(key);
            }
        }

        if ((!xmlStrcmp(pNode->name, (const xmlChar*)"Signal_Column")) == FALSE)
        {
            xmlChar* key = xmlNodeListGetString(pNode->doc, pNode->xmlChildrenNode, 1);
            if(NULL != key)
            {
                m_pouSigWnd->m_omSignalList.SetColumnWidth(3, atoi((char*)key));
                xmlFree(key);
            }
        }
        pNode = pNode->next;
    }
    return nRetVal;
}

//~MVN

/**
 * \req RS_18_23 Popup menu item 'Clear' (clears the signal watch window)
 */
HRESULT CSignalWatch_CAN::SW_ClearSigWatchWnd(void)
{
    if (m_pouSigWnd != NULL)
    {
        //m_pouSigWnd->PostMessage(WM_REMOVE_SIGNAL);

        //send the message without any extra value in LPARAM
        m_pouSigWnd->PostMessage(WM_REMOVE_SIGNAL,0,0);
    }
    return S_OK;
}

HRESULT CSignalWatch_CAN::SW_UpdateMsgInterpretObj(void* pvRefObj)
{
    CMsgInterpretation* RefObj = (CMsgInterpretation*) pvRefObj;
    EnterCriticalSection(&m_omCritSecSW);
    if (m_pMsgInterPretObj == NULL)
    {
        m_pMsgInterPretObj = new CMsgInterpretation;
    }
    RefObj->vCopy(m_pMsgInterPretObj);
    LeaveCriticalSection(&m_omCritSecSW);
    return S_OK;
}

BOOL CSignalWatch_CAN::SW_IsWindowVisible()
{
    BOOL bResult = FALSE;
    if (m_pouSigWnd != NULL)
    {
        bResult = m_pouSigWnd->IsWindowVisible();
    }
    return bResult;
}

HRESULT CSignalWatch_CAN::SW_SetDisplayMode(BOOL bHex)
{
    m_bHex = bHex;
    return S_OK;
}