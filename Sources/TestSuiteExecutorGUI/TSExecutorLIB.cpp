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
 * \file      TSExecutorLIB.cpp
 * \author    Venkatanarayana makam
 * \copyright Copyright (c) 2011, Robert Bosch Engineering and Business Solutions. All rights reserved.
 */
#include "TSExecutorGUI_stdafx.h"
#include "TSExecutorLIB.h"
#include "TSExecutorGUI_Extern.h"
#include "TSExecutorBase.h"
#include "include/XMLDefines.h"
#include "Utility/XMLUtils.h"
//#include "../Application/GettextBusmaster.h"
#include <Shlwapi.h>
#include "Utility\UtilFunctions.h"
#include "Utility\MultiLanguageSupport.h"

#define MSG_GET_CONFIGPATH  10000

//TODO::Move to Definition File
#define def_ID_TESTSUITE            -1
#define def_STR_TESTSUITENAME       "Unnamed Test Suite"

extern CTSExecutorBase* g_podTSExecutor;

/******************************************************************************
Function Name  :  CTSExecutorLIB
Input(s)       :  -
Output         :  -
Functionality  :  Constructor
Member of      :  CTSExecutorLIB
Friend of      :  -
Author(s)      :  Venkatanarayana Makam
Date Created   :  07/04/2011
Modifications  :
Code Tag       :
******************************************************************************/
CTSExecutorLIB::CTSExecutorLIB(void)
{
    m_omstrTestSuiteName = def_STR_TESTSUITENAME;
    m_ouTestSetupEntityList.RemoveAll();
    m_bTestSuiteStatus = FALSE;
    m_ompResultDisplayWnd = NULL;
}

/******************************************************************************
Function Name  :  SetResultDisplayWnd
Input(s)       :  CListCtrl* pWnd
Output         :  HRESULT
Functionality  :  Sets The Display Result Display list Control
Member of      :  CTSExecutorLIB
Friend of      :  -
Author(s)      :  Venkatanarayana Makam
Date Created   :  07/04/2011
Modifications  :
Code Tag       :
******************************************************************************/
HRESULT CTSExecutorLIB::SetResultDisplayWnd(CListCtrl* pWnd)
{
    m_ompResultDisplayWnd = pWnd;
    m_ompResultDisplayWnd->DeleteAllItems();
    if(g_podTSExecutor != NULL)
    {
        g_podTSExecutor->TSX_SetDisplayWnd(m_ompResultDisplayWnd);
    }
    return S_OK;
}

/******************************************************************************
Function Name  :  ~CTSExecutorLIB
Input(s)       :  -
Output         :  -
Functionality  :  Destructor
Member of      :  CTSExecutorLIB
Friend of      :  -
Author(s)      :  Venkatanarayana Makam
Date Created   :  07/04/2011
Modifications  :
Code Tag       :
******************************************************************************/
CTSExecutorLIB::~CTSExecutorLIB(void)
{
}

/******************************************************************************
Function Name  :  SelectBus
Input(s)       :  eTYPE_BUS eCurrBus
Output         :  HRESULT
Functionality  :  Selects The Bus Type
Member of      :  CTSExecutorLIB
Friend of      :  -
Author(s)      :  Venkatanarayana Makam
Date Created   :  07/04/2011
Modifications  :
Code Tag       :
******************************************************************************/
HRESULT CTSExecutorLIB::SelectBus(eTYPE_BUS eCurrBus)
{
    m_eBusType = eCurrBus;
    return S_OK;
}
/******************************************************************************
Function Name  :  SetTestsuiteName
Input(s)       :  CString& omName
Output         :  HRESULT
Functionality  :  Set The TestSuite Name
Member of      :  CTSExecutorLIB
Friend of      :  -
Author(s)      :  Venkatanarayana Makam
Date Created   :  07/04/2011
Modifications  :
Code Tag       :
******************************************************************************/
HRESULT CTSExecutorLIB::SetTestsuiteName(CString& omName)
{
    m_omstrTestSuiteName = omName;
    return S_OK;
}
/******************************************************************************
Function Name  :  GetTestsuiteName
Input(s)       :  CString& omName
Output         :  HRESULT
Functionality  :  Returns The Test Suite Name
Member of      :  CTSExecutorLIB
Friend of      :  -
Author(s)      :  Venkatanarayana Makam
Date Created   :  07/04/2011
Modifications  :
Code Tag       :
******************************************************************************/
HRESULT CTSExecutorLIB::GetTestsuiteName(CString& omName)
{
    omName = m_omstrTestSuiteName;
    return S_OK;
}
/******************************************************************************
Function Name  :  AddTestSetup
Input(s)       :  CString omFilePath, DWORD& dwID
Output         :  HRESULT
Functionality  :  Adds The test Setup File
Member of      :  CTSExecutorLIB
Friend of      :  -
Author(s)      :  Venkatanarayana Makam
Date Created   :  07/04/2011
Modifications  :
Code Tag       :
******************************************************************************/
HRESULT CTSExecutorLIB::AddTestSetup(CString omFilePath, DWORD& dwID)
{
    //ERR_PATH_INCORRECT, ERR_FILE_INCORRECT - return from LoadFile
    //ERR_ALREADY_ADDED May Be
    //ERR_BUSTYPE_MISMATCH

    CTestSetupEntity ouTestSetup;
    HRESULT hResult = ouTestSetup.LoadFile(omFilePath);
    if(hResult == S_OK)
    {
        POSITION pos = m_ouTestSetupEntityList.AddTail(ouTestSetup);
        CTestSetupEntity& ouTempTestSetup = m_ouTestSetupEntityList.GetAt(pos);
        dwID = ouTempTestSetup.GetID();
        hResult = S_OK;
    }
    return hResult;
}
/******************************************************************************
Function Name  :  GetTestSetupName
Input(s)       :  DWORD dwID, CString& omName
Output         :  HRESULT
Functionality  :  Returns the test setup Name of the given ID
Member of      :  CTSExecutorLIB
Friend of      :  -
Author(s)      :  Venkatanarayana Makam
Date Created   :  07/04/2011
Modifications  :
Code Tag       :
******************************************************************************/
HRESULT CTSExecutorLIB::GetTestSetupName(DWORD dwID, CString& omName)
{
    BOOL bValidID = FALSE;
    INT nCount = (INT)m_ouTestSetupEntityList.GetCount();
    for(INT i = 0; i < nCount; i++)
    {
        POSITION pos = m_ouTestSetupEntityList.FindIndex(i);
        CTestSetupEntity& ouTempTestSetup = m_ouTestSetupEntityList.GetAt(pos);
        if(ouTempTestSetup.GetID() == dwID)
        {
            omName = ouTempTestSetup.m_omstrTestSetupTitle;
            bValidID = TRUE;
            break;
        }
    }
    if(bValidID == FALSE)
    {
        return ERR_WRONG_ID;
    }
    return S_OK;
}
/******************************************************************************
Function Name  :  UpdateTestSetup
Input(s)       :  DWORD dwID, CString omFilePath
Output         :  HRESULT
Functionality  :  Update the TestSetup Name of the given ID
Member of      :  CTSExecutorLIB
Friend of      :  -
Author(s)      :  Venkatanarayana Makam
Date Created   :  07/04/2011
Modifications  :
Code Tag       :
******************************************************************************/
HRESULT CTSExecutorLIB::UpdateTestSetup( DWORD dwID, CString omFilePath)
{
    BOOL bValidID = FALSE;
    INT nCount = (INT)m_ouTestSetupEntityList.GetCount();
    for(INT i = 0; i < nCount; i++)
    {
        POSITION pos = m_ouTestSetupEntityList.FindIndex(i);
        CTestSetupEntity& ouTempTestSetup = m_ouTestSetupEntityList.GetAt(pos);
        if(ouTempTestSetup.GetID() == dwID)
        {
            ouTempTestSetup.LoadFile(omFilePath);
            bValidID = TRUE;
            break;
        }
    }
    if(bValidID == FALSE)
    {
        return ERR_WRONG_ID;
    }
    return S_OK;
}
/******************************************************************************
Function Name  :  DeleteTestSetup
Input(s)       :  DWORD dwID
Output         :  HRESULT
Functionality  :  deletes the testsetup with the given dwID
Member of      :  CTSExecutorLIB
Friend of      :  -
Author(s)      :  Venkatanarayana Makam
Date Created   :  07/04/2011
Modifications  :
Code Tag       :
******************************************************************************/
HRESULT CTSExecutorLIB::DeleteTestSetup( DWORD dwID)
{
    BOOL bValidID = FALSE;
    INT nCount = (INT)m_ouTestSetupEntityList.GetCount();
    for(INT i = 0; i < nCount; i++)
    {
        POSITION pos = m_ouTestSetupEntityList.FindIndex(i);
        CTestSetupEntity& ouTempTestSetup = m_ouTestSetupEntityList.GetAt(pos);
        if(ouTempTestSetup.GetID() == dwID)
        {
            m_ouTestSetupEntityList.RemoveAt(pos);
            bValidID = TRUE;
            break;
        }
    }
    if(bValidID == FALSE)
    {
        return ERR_WRONG_ID;
    }
    return S_OK;
}

/******************************************************************************
Function Name  :  Update
Input(s)       :
Output         :  HRESULT
Functionality  :
Member of      :  CTSExecutorLIB
Friend of      :  -
Author(s)      :  Venkatanarayana Makam
Date Created   :  28/04/2011
Modifications  :
Code Tag       :
******************************************************************************/
HRESULT CTSExecutorLIB::Update( DWORD dwID)
{
    BOOL bValidID = FALSE;
    INT nCount = (INT)m_ouTestSetupEntityList.GetCount();
    for(INT i = 0; i < nCount; i++)
    {
        POSITION pos = m_ouTestSetupEntityList.FindIndex(i);
        CTestSetupEntity& ouTempTestSetup = m_ouTestSetupEntityList.GetAt(pos);
        if(ouTempTestSetup.GetID() == dwID)
        {
            ouTempTestSetup.UpdateTestSetup();
            bValidID = TRUE;
            break;
        }
    }
    if(bValidID == FALSE)
    {
        return ERR_WRONG_ID;
    }
    return S_OK;
}
/******************************************************************************
Function Name  :  EnableTestSetup
Input(s)       :  DWORD dwID, BOOL bEnable
Output         :  HRESULT
Functionality  :  Enables or disables the execution of given test setup
Member of      :  CTSExecutorLIB
Friend of      :  -
Author(s)      :  Venkatanarayana Makam
Date Created   :  07/04/2011
Modifications  :
Code Tag       :
******************************************************************************/
HRESULT CTSExecutorLIB::EnableTestSetup( DWORD dwID, BOOL bEnable)
{

    BOOL bValidID = FALSE;
    INT nCount = (INT)m_ouTestSetupEntityList.GetCount();
    for(INT i = 0; i < nCount; i++)
    {
        POSITION pos = m_ouTestSetupEntityList.FindIndex(i);
        CTestSetupEntity& ouTempTestSetup = m_ouTestSetupEntityList.GetAt(pos);
        if(ouTempTestSetup.GetID() == dwID)
        {
            ouTempTestSetup.vEnableEntity(bEnable);
            bValidID = TRUE;
            break;
        }
    }
    if(bValidID == FALSE)
    {
        return ERR_WRONG_ID;
    }
    return S_OK;
}
/******************************************************************************
Function Name  :  GetTestSetupEnableStatus
Input(s)       :  DWORD dwID, BOOL& bEnable
Output         :  HRESULT
Functionality  :  Returnd the Enable status of given test setup id
Member of      :  CTSExecutorLIB
Friend of      :  -
Author(s)      :  Venkatanarayana Makam
Date Created   :  07/04/2011
Modifications  :
Code Tag       :
******************************************************************************/
HRESULT CTSExecutorLIB::GetTestSetupEnableStatus(DWORD dwID, BOOL& bEnable)
{
    BOOL bValidID = FALSE;
    INT nCount = (INT)m_ouTestSetupEntityList.GetCount();
    for(INT i = 0; i < nCount; i++)
    {
        POSITION pos = m_ouTestSetupEntityList.FindIndex(i);
        CTestSetupEntity& ouTempTestSetup = m_ouTestSetupEntityList.GetAt(pos);
        if(ouTempTestSetup.GetID() == dwID)
        {
            bEnable = ouTempTestSetup.bGetEnableStatus();
            bValidID = TRUE;
            break;
        }
    }
    if(bValidID == FALSE)
    {
        return ERR_WRONG_ID;
    }
    return S_OK;
}

/******************************************************************************
Function Name  :  RepositionTestSetup
Input(s)       :  DWORD dwID, DWORD dwIDPreceding
Output         :  HRESULT
Functionality  :
Member of      :  CTSExecutorLIB
Friend of      :  -
Author(s)      :  Venkatanarayana Makam
Date Created   :  07/04/2011
Modifications  :
Code Tag       :  CS044
******************************************************************************/
HRESULT CTSExecutorLIB::RepositionTestSetup( DWORD dwID, DWORD dwIDPreceding)
{
    INT nCount = (INT)m_ouTestSetupEntityList.GetCount();
    POSITION pos;
    BOOL bIdFound = FALSE;
    CTestSetupEntity ouCurrentTestSetup;
    for(INT i = 0; i < nCount; i++)
    {
        pos = m_ouTestSetupEntityList.FindIndex(i);
        CTestSetupEntity& ouTestSetupEntity = m_ouTestSetupEntityList.GetAt(pos);
        if(ouTestSetupEntity.GetID() == dwID)
        {
            ouCurrentTestSetup = m_ouTestSetupEntityList.GetAt(pos);
            m_ouTestSetupEntityList.RemoveAt(pos);
            bIdFound = TRUE;
            break;
        }
    }
    if(bIdFound == FALSE)
    {
        return ERR_WRONG_ID;
    }

    bIdFound = FALSE;
    nCount = (INT)m_ouTestSetupEntityList.GetCount();
    if(dwIDPreceding == def_ID_TESTSUITE)
    {
        m_ouTestSetupEntityList.AddHead(ouCurrentTestSetup);
        bIdFound = TRUE;
    }
    else
    {
        for(INT i = 0; i < nCount; i++)
        {
            pos = m_ouTestSetupEntityList.FindIndex(i);
            CTestSetupEntity& ouTestSetupEntity = m_ouTestSetupEntityList.GetAt(pos);
            if(ouTestSetupEntity.GetID() == dwIDPreceding)
            {
                m_ouTestSetupEntityList.InsertAfter(pos, ouCurrentTestSetup);
                bIdFound = TRUE;
                break;
            }
        }
    }
    if(bIdFound == FALSE)
    {
        return ERR_WRONG_ID_REF;
    }
    return S_OK;
}

/******************************************************************************
Function Name  :  GetTestcaseCount
Input(s)       :  DWORD dwID, UINT& unTotal
Output         :  HRESULT
Functionality  :  Returns the testcase count
Member of      :  CTSExecutorLIB
Friend of      :  -
Author(s)      :  Venkatanarayana Makam
Date Created   :  07/04/2011
Modifications  :
Code Tag       :
******************************************************************************/
HRESULT CTSExecutorLIB::GetTestcaseCount( DWORD dwID, UINT& unTotal)
{
    BOOL bValidID = FALSE;
    unTotal = (UINT)-1;
    INT nCount = (INT)m_ouTestSetupEntityList.GetCount();
    for(INT i = 0; i < nCount; i++)
    {
        POSITION pos = m_ouTestSetupEntityList.FindIndex(i);
        CTestSetupEntity& ouTempTestSetup = m_ouTestSetupEntityList.GetAt(pos);
        if(ouTempTestSetup.GetID() == dwID)
        {
            unTotal = ouTempTestSetup.GetSubEntryCount();
            bValidID = TRUE;
            break;
        }
    }
    if(bValidID == FALSE)
    {
        return ERR_WRONG_ID;
    }
    return S_OK;
}

/******************************************************************************
Function Name  :  GetTestcaseCount
Input(s)       :
Output         :  HRESULT
Functionality  :
Member of      :  CTSExecutorLIB
Friend of      :  -
Author(s)      :  Venkatanarayana Makam
Date Created   :  07/04/2011
Modifications  :
Code Tag       :
******************************************************************************/
HRESULT CTSExecutorLIB::GetTestcaseCount( INT nIndex, UINT& unTotal)
{
    BOOL bValidID = FALSE;
    unTotal = (UINT)-1;
    POSITION pos = m_ouTestSetupEntityList.FindIndex(nIndex);
    if(pos != NULL)
    {
        CTestSetupEntity& ouTempTestSetup = m_ouTestSetupEntityList.GetAt(pos);
        unTotal = ouTempTestSetup.GetSubEntryCount();
        bValidID = TRUE;
    }
    if(bValidID == FALSE)
    {
        return ERR_WRONG_ID;
    }
    return S_OK;
}

/******************************************************************************
Function Name  :  GetTestCaseInfo
Input(s)       :  DWORD dwID, UINT unIndex, CBaseEntityTA** pTCInfo
Output         :  HRESULT
Functionality  :  Returns the testcase Info
Member of      :  CTSExecutorLIB
Friend of      :  -
Author(s)      :  Venkatanarayana Makam
Date Created   :  07/04/2011
Modifications  :
Code Tag       :
******************************************************************************/
HRESULT CTSExecutorLIB::GetTestCaseInfo( DWORD dwID, UINT unIndex, CBaseEntityTA** pTCInfo)
{
    BOOL bValidID = FALSE;
    HRESULT hResult = ERR_WRONG_ID;
    INT nCount = (INT)m_ouTestSetupEntityList.GetCount();

    for(INT i = 0; i < nCount; i++)
    {
        POSITION pos = m_ouTestSetupEntityList.FindIndex(i);
        CTestSetupEntity& ouTempTestSetup = m_ouTestSetupEntityList.GetAt(pos);
        if(ouTempTestSetup.GetID() == dwID)
        {
            hResult = S_OK;
            if(ouTempTestSetup.GetSubEntityObj(unIndex, pTCInfo) != S_OK)
            {
                hResult = ERR_WRONG_INDEX;
                break;
            }
        }
    }
    return hResult;
}

/******************************************************************************
Function Name  :  EnableTestCase
Input(s)       :  DWORD dwID, UINT unIndex, BOOL bEnable
Output         :  HRESULT
Functionality  :  Enables or disable the Testcase with index unIndex in
                  test setup with id dwID
Member of      :  CTSExecutorLIB
Friend of      :  -
Author(s)      :  Venkatanarayana Makam
Date Created   :  07/04/2011
Modifications  :
Code Tag       :
******************************************************************************/
HRESULT CTSExecutorLIB::EnableTestCase( DWORD dwID, UINT unIndex, BOOL bEnable)
{
    BOOL bValidID = FALSE;
    HRESULT hResult = ERR_WRONG_ID;
    INT nCount = (INT)m_ouTestSetupEntityList.GetCount();
    CBaseEntityTA* pTCInfo;
    for(int i = 0; i < nCount; i++)
    {
        POSITION pos = m_ouTestSetupEntityList.FindIndex(i);
        CTestSetupEntity& ouTempTestSetup = m_ouTestSetupEntityList.GetAt(pos);
        if(ouTempTestSetup.GetID() == dwID)
        {
            hResult = S_OK;
            if(ouTempTestSetup.GetSubEntityObj(unIndex, &pTCInfo) == S_OK)
            {
                pTCInfo->vEnableEntity(bEnable);
                break;
            }
            else
            {
                hResult = ERR_WRONG_INDEX;
                break;
            }
        }
    }
    return hResult;
}

/******************************************************************************
Function Name  :  GetTestSetupCount
Input(s)       :  INT& nCount
Output         :  HRESULT
Functionality  :  returns the testsetup count
Member of      :  CTSExecutorLIB
Friend of      :  -
Author(s)      :  Venkatanarayana Makam
Date Created   :  07/04/2011
Modifications  :
Code Tag       :
******************************************************************************/
HRESULT CTSExecutorLIB::GetTestSetupCount(INT& nCount)
{
    nCount = (INT)m_ouTestSetupEntityList.GetCount();
    return S_OK;
}

/******************************************************************************
Function Name  :  GetTestSetupInfo
Input(s)       :  INT nIndex, STestSetupInfo& sTSInfo
Output         :  HRESULT
Functionality  :  return the testsetup info
Member of      :  CTSExecutorLIB
Friend of      :  -
Author(s)      :  Venkatanarayana Makam
Date Created   :  07/04/2011
Modifications  :
Code Tag       :
******************************************************************************/
HRESULT CTSExecutorLIB::GetTestSetupInfo(INT nIndex, STestSetupInfo& sTSInfo)
{
    POSITION pos = m_ouTestSetupEntityList.FindIndex(nIndex);
    if(pos != NULL)
    {
        CTestSetupEntity& ouTestSetupEntity = m_ouTestSetupEntityList.GetAt(pos);
        sTSInfo.m_dwID = ouTestSetupEntity.GetID();
        sTSInfo.m_bEnable = ouTestSetupEntity.bGetEnableStatus();
        sTSInfo.m_omstrName = ouTestSetupEntity.m_omstrCurrentTSFile;
        sTSInfo.m_nTCCount = ouTestSetupEntity.GetSubEntryCount();
        return S_OK;
    }
    return ERR_WRONG_INDEX;
}

/******************************************************************************
Function Name  :  GetConfigurationData
Input(s)       :  BYTE*& pDesBuffer, UINT& unBuffSize
Output         :  HRESULT
Functionality  :
Member of      :  CTSExecutorLIB
Friend of      :  -
Author(s)      :  Venkatanarayana Makam
Date Created   :  07/04/2011
Modifications  :
Code Tag       :
******************************************************************************/
HRESULT CTSExecutorLIB::GetConfigurationData(BYTE*& pDesBuffer, UINT& unBuffSize)
{
    INT nFileCount = (INT)m_ouTestSetupEntityList.GetCount();
    unBuffSize = sizeof(UINT) + sizeof(BOOL)+sizeof(char)*MAX_PATH; //Number of TestSetupFiles + Status of TestSuite+TestSuite Name

    for(INT i = 0; i < nFileCount; i++)
    {
        UINT unCount;
        GetTestcaseCount(i, unCount);
        //TestSuite Name + File Path + Selection Status + Number of TestCases + Testcases Status
        unBuffSize += (sizeof(char)*MAX_PATH + sizeof(char)*MAX_PATH) + sizeof(BOOL) + sizeof(UINT) + (unCount*sizeof(BOOL));
    }

    pDesBuffer = new BYTE[unBuffSize];

    if (pDesBuffer != NULL)
    {
        BYTE* pbyTemp = pDesBuffer;
        //TestSuite name
        char acTestSuiteName[MAX_PATH] = {_T('\0')};
        strcpy_s(acTestSuiteName, MAX_PATH, m_omstrTestSuiteName);
        COPY_DATA(pbyTemp, acTestSuiteName, (sizeof (char) * MAX_PATH));
        //TestSuite Status
        COPY_DATA(pbyTemp, &m_bTestSuiteStatus, sizeof(BOOL));

        //File Count
        COPY_DATA(pbyTemp, &nFileCount, sizeof(INT));


        for(INT i = 0; i < nFileCount; i++)
        {
            UINT unCount;
            POSITION pos = m_ouTestSetupEntityList.FindIndex(i);
            CTestSetupEntity& ouTestSetupEntity = m_ouTestSetupEntityList.GetAt(pos);

            //File Path
            char acName[MAX_PATH] = {_T('\0')};
            strcpy_s(acName, MAX_PATH, ouTestSetupEntity.m_omstrCurrentTSFile.GetBuffer(MAX_PATH));
            COPY_DATA(pbyTemp, acName, (sizeof (char) * MAX_PATH));
            //Selection Status
            BOOL bStatus = ouTestSetupEntity.bGetEnableStatus();
            COPY_DATA(pbyTemp, &bStatus, sizeof(BOOL));

            //TestCase Count
            ouTestSetupEntity.GetSubEntryCount(unCount);
            COPY_DATA(pbyTemp, &unCount, sizeof(UINT));

            //TestCase Selection Status
            for(UINT j=0; j<unCount; j++)
            {
                CBaseEntityTA* pTCEntity;
                ouTestSetupEntity.GetSubEntityObj(j, &pTCEntity);
                bStatus = FALSE;
                if(pTCEntity != NULL)
                {
                    bStatus = pTCEntity->bGetEnableStatus();
                }
                COPY_DATA(pbyTemp, &bStatus, sizeof(BOOL));
            }
            //File Path + Selection Status + Number of TestCases + Testcases Status
        }
    }
    return S_OK;
}

HRESULT CTSExecutorLIB::GetConfigurationData(xmlNodePtr pxmlNodePtr)
{
    const char* omcVarChar ;

    //<Test_Suite_Name />

    omcVarChar = m_omstrTestSuiteName.GetBuffer(MAX_PATH);

    xmlNodePtr pTSName = xmlNewChild(pxmlNodePtr, NULL, BAD_CAST DEF_TS_NAME, BAD_CAST omcVarChar);
    xmlAddChild(pxmlNodePtr, pTSName);

    // <IsEnable />
    CString csIsEnabled;
    csIsEnabled.Format("%d", m_bTestSuiteStatus);
    omcVarChar = csIsEnabled;
    xmlNodePtr pIsEnabled = xmlNewChild(pxmlNodePtr, NULL, BAD_CAST DEF_IS_ENABLE, BAD_CAST omcVarChar);
    xmlAddChild(pxmlNodePtr, pIsEnabled);

    INT nFileCount = (INT)m_ouTestSetupEntityList.GetCount();

    for(int iCnt = 0; iCnt < nFileCount; iCnt++)
    {
        //<TEST_SUITE>
        xmlNodePtr pTestSuite = xmlNewNode(NULL, BAD_CAST DEF_TEST_SUITE);
        xmlAddChild(pxmlNodePtr, pTestSuite);

        //<Test_Suite_Name />
        POSITION pos = m_ouTestSetupEntityList.FindIndex(iCnt);
        CTestSetupEntity& ouTestSetupEntity = m_ouTestSetupEntityList.GetAt(pos);

        string omPath, omStrConfigFolder;
        char configPath[MAX_PATH];
        AfxGetMainWnd()->SendMessage(MSG_GET_CONFIGPATH, (WPARAM)configPath, 0);
        CUtilFunctions::nGetBaseFolder(configPath, omStrConfigFolder );
        CUtilFunctions::MakeRelativePath(omStrConfigFolder.c_str(), (char*)ouTestSetupEntity.m_omstrCurrentTSFile.GetBuffer(MAX_PATH), omPath);
        omcVarChar = omPath.c_str();

        xmlNodePtr pFilePath = xmlNewChild(pTestSuite, NULL, BAD_CAST DEF_FILE_PATH, BAD_CAST omcVarChar);
        xmlAddChild(pTestSuite, pFilePath);

        //<IsEnable />
        CString csIsEnabled;
        csIsEnabled.Format("%d", ouTestSetupEntity.bGetEnableStatus());
        omcVarChar = csIsEnabled;
        xmlNodePtr pIsEnabled = xmlNewChild(pTestSuite, NULL, BAD_CAST DEF_IS_ENABLE, BAD_CAST omcVarChar);
        xmlAddChild(pTestSuite, pIsEnabled);

        //<Testcases_Selected>
        xmlNodePtr pTestCasesSel = xmlNewNode(NULL, BAD_CAST DEF_TEST_CASES_SEL);
        xmlAddChild(pTestSuite, pTestCasesSel);

        // <Index>1</Index>
        CString csIndex;
        bool    bStatus;
        UINT    unCount;
        ouTestSetupEntity.GetSubEntryCount(unCount);
        for(UINT j=0; j<unCount; j++)
        {
            CBaseEntityTA* pTCEntity;
            ouTestSetupEntity.GetSubEntityObj(j, &pTCEntity);
            bStatus = FALSE;
            if(pTCEntity != NULL)
            {
                bStatus = pTCEntity->bGetEnableStatus();
            }
            if( bStatus == true )
            {
                csIndex.Format("%d", j);
                omcVarChar = csIndex;
                xmlNodePtr pIndex = xmlNewChild(pTestCasesSel, NULL, BAD_CAST DEF_INDEX, BAD_CAST omcVarChar);
                xmlAddChild(pTestCasesSel, pIndex);
            }
        }
    }
    return true;
}
/******************************************************************************
Function Name  :  SetConfigurationData
Input(s)       :  BYTE*& pDesBuffer, UINT& unBuffSize
Output         :  HRESULT
Functionality  :
Member of      :  CTSExecutorLIB
Friend of      :  -
Author(s)      :  Venkatanarayana Makam
Date Created   :  07/04/2011
Modifications  :
Code Tag       :
******************************************************************************/
HRESULT CTSExecutorLIB::SetConfigurationData(BYTE* pSrcBuffer, UINT /*unBuffSize*/)
{

    INT nFileCount = (INT)m_ouTestSetupEntityList.GetCount();

    if (pSrcBuffer != NULL)
    {
        BYTE* pbyTemp = pSrcBuffer;
        //File Path
        char acTestSuiteName[MAX_PATH] = {_T('\0')};
        COPY_DATA_2(acTestSuiteName, pbyTemp, (sizeof (char) * MAX_PATH));
        m_omstrTestSuiteName = acTestSuiteName;

        //TestSuite Status
        COPY_DATA_2(&m_bTestSuiteStatus, pbyTemp, sizeof(BOOL));

        //File Count
        COPY_DATA_2(&nFileCount, pbyTemp, sizeof(INT));
        INT nFileIndex = 0;
        for(INT i = 0; i < nFileCount; i++)
        {
            UINT unCount, unConfigCount;

            //File Path
            CString omstrFileName;
            DWORD dwID;
            char acName[MAX_PATH] = {_T('\0')};
            COPY_DATA_2(acName, pbyTemp, (sizeof (char) * MAX_PATH));
            omstrFileName = acName;
            if(AddTestSetup(omstrFileName, dwID) == S_OK)
            {
                //Selection Status
                BOOL bStatus;
                COPY_DATA_2(&bStatus, pbyTemp, sizeof(BOOL));
                POSITION pos = m_ouTestSetupEntityList.FindIndex(nFileIndex++);
                if(pos != NULL)
                {
                    CTestSetupEntity& ouTestSetupEntity = m_ouTestSetupEntityList.GetAt(pos);
                    ouTestSetupEntity.vEnableEntity(bStatus);

                    //TestCase Count
                    COPY_DATA_2(&unConfigCount, pbyTemp, sizeof(UINT));
                    ouTestSetupEntity.GetSubEntryCount(unCount);
                    if(unConfigCount != unCount)
                    {
                        //File Has Modified;
                    }

                    //TestCase Selection Status
                    for(UINT j=0; j<unCount; j++)
                    {
                        CBaseEntityTA* pTCEntity;
                        COPY_DATA_2(&bStatus, pbyTemp, sizeof(BOOL));
                        ouTestSetupEntity.GetSubEntityObj(j, &pTCEntity);
                        if(pTCEntity != NULL)
                        {
                            pTCEntity->vEnableEntity(bStatus);
                        }
                    }
                }
            }
            else
            {
                UINT unConfigCount;
                BOOL bStatus;
                COPY_DATA_2(&bStatus, pbyTemp, sizeof(BOOL));
                COPY_DATA_2(&unConfigCount, pbyTemp, sizeof(UINT));
                BOOL* bTempData = new BOOL[unConfigCount];
                COPY_DATA_2(bTempData, pbyTemp, unConfigCount*sizeof(BOOL));
                delete []bTempData;
            }
            //File Path + Selection Status + Number of TestCases + Testcases Status
        }
    }
    return S_OK;
}

HRESULT CTSExecutorLIB::SetConfigurationData(xmlNodePtr pXmlNode)
{
    xmlXPathObjectPtr pObjectPtr = NULL;
    xmlNodePtr pTempNode;
    //Test Suite Name
    m_omstrTestSuiteName = def_STR_TESTSUITENAME;

    pObjectPtr = xmlUtils::pGetChildNodes(pXmlNode, (xmlChar*)"Test_Suite_Name");
    if( NULL != pObjectPtr)
    {
        pTempNode = pObjectPtr->nodesetval->nodeTab[0];
        m_omstrTestSuiteName = (char*)xmlNodeListGetString(pTempNode->doc, pTempNode->children, 1);

        xmlXPathFreeObject(pObjectPtr);
        pObjectPtr = NULL;
    }
    //Test Suite Enable
    pObjectPtr = xmlUtils::pGetChildNodes(pXmlNode, (xmlChar*)"IsEnable");
    m_bTestSuiteStatus = TRUE;
    if( NULL != pObjectPtr)
    {
        pTempNode = pObjectPtr->nodesetval->nodeTab[0];
        char* pchKey = (char*)xmlNodeListGetString(pTempNode->doc, pTempNode->children, 1);
        if(pchKey != NULL)
        {
            m_bTestSuiteStatus = xmlUtils::bGetBooleanValue(pchKey);
            xmlFree(pchKey);
        }
        xmlXPathFreeObject(pObjectPtr);
        pObjectPtr = NULL;
    }

    //Test Suite FileCount
    int nFileCount = 0;
    pObjectPtr = xmlUtils::pGetChildNodes(pXmlNode, (xmlChar*)"TEST_SUITE");

    if( NULL != pObjectPtr)
    {
        nFileCount = pObjectPtr->nodesetval->nodeNr;
        INT nFileIndex = 0;
        for( int i = 0; i < nFileCount; i++ )
        {
            sTestSuiteConfigInfo sConfigInfo;
            if ( S_OK == nParseTestSuite(pObjectPtr->nodesetval->nodeTab[i], sConfigInfo) )
            {
                DWORD dwID;
                UINT unCount, unConfigCount;
                if(AddTestSetup(sConfigInfo.m_strFileName.c_str(), dwID) == S_OK)
                {
                    //Selection Status
                    BOOL bStatus;
                    POSITION pos = m_ouTestSetupEntityList.FindIndex(nFileIndex++);
                    if(pos != NULL)
                    {
                        CTestSetupEntity& ouTestSetupEntity = m_ouTestSetupEntityList.GetAt(pos);
                        ouTestSetupEntity.vEnableEntity(sConfigInfo.m_bEnable);

                        //TestCase Count
                        unConfigCount = sConfigInfo.m_nListSelctedCases.size();
                        ouTestSetupEntity.GetSubEntryCount(unCount);

                        if( unConfigCount == 0)
                        {
                            for(UINT j=0; j<unCount; j++)
                            {
                                CBaseEntityTA* pTCEntity;
                                ouTestSetupEntity.GetSubEntityObj(j, &pTCEntity);
                                if(pTCEntity != NULL)
                                {
                                    pTCEntity->vEnableEntity(FALSE);
                                }
                            }
                        }
                        else
                        {
                            //TestCase Selection Status
                            list<int>::iterator listIterator = sConfigInfo.m_nListSelctedCases.begin();

                            for(listIterator = sConfigInfo.m_nListSelctedCases.begin();
                                    listIterator != sConfigInfo.m_nListSelctedCases.end(); listIterator++)
                            {
                                CBaseEntityTA* pTCEntity = NULL;
                                int nIndex = *listIterator;
                                HRESULT hValue = ouTestSetupEntity.GetSubEntityObj(nIndex, &pTCEntity);
                                if(pTCEntity != NULL && hValue == S_OK)
                                {
                                    pTCEntity->vEnableEntity(TRUE);
                                }
                            }
                        }
                    }
                }
            }
        }
    }
    return S_OK;
}

int CTSExecutorLIB::nParseTestSuite(xmlNodePtr pXmlNode, sTestSuiteConfigInfo& sConfigInfo)
{
    xmlXPathObjectPtr pObjectPtr = NULL;
    xmlNodePtr pTempNode;
    int nRetValue = S_OK;

    //Test Setup File Path
    pObjectPtr = xmlUtils::pGetChildNodes(pXmlNode, (xmlChar*)"File_Path");
    if( NULL != pObjectPtr)
    {
        pTempNode = pObjectPtr->nodesetval->nodeTab[0];
        char* key = (char*)xmlNodeListGetString(pTempNode->doc, pTempNode->children, 1);

        if( NULL != key)
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
                sConfigInfo.m_strFileName = chAbsPath;
            }
            else
            {
                sConfigInfo.m_strFileName = (char*)key;
            }
            xmlFree(key);
        }

        xmlXPathFreeObject(pObjectPtr);
        pObjectPtr = NULL;
    }
    else
    {
        nRetValue = S_FALSE;
    }

    //Test Setup File Enable
    if( S_OK == nRetValue )
    {
        pObjectPtr = xmlUtils::pGetChildNodes(pXmlNode, (xmlChar*)"IsEnable");
        sConfigInfo.m_bEnable = TRUE;
        if( NULL != pObjectPtr)
        {
            pTempNode = pObjectPtr->nodesetval->nodeTab[0];
            sConfigInfo.m_bEnable = xmlUtils::bGetBooleanValue((char*)xmlNodeListGetString(pTempNode->doc, pTempNode->children, 1));
            xmlXPathFreeObject(pObjectPtr);
            pObjectPtr = NULL;
        }

        pObjectPtr = xmlUtils::pGetChildNodes(pXmlNode, (xmlChar*)"Testcases_Selected/Index");
        if( NULL != pObjectPtr)
        {
            int nSelectedCount = pObjectPtr->nodesetval->nodeNr;
            for( int i =0; i < nSelectedCount; i++ )
            {
                char* pchKey = (char*)xmlNodeListGetString( pXmlNode->doc, pObjectPtr->nodesetval->nodeTab[i]->children, 1);
                int nValue = strtol(pchKey, NULL, 10);
                xmlFree(pchKey);
                sConfigInfo.m_nListSelctedCases.push_back(nValue);
            }
            xmlXPathFreeObject(pObjectPtr);
            pObjectPtr = NULL;
        }

    }
    return nRetValue;
}
/******************************************************************************
Function Name  :  EnableItem
Input(s)       :  DWORD dwID, BOOL& bEnable
Output         :  HRESULT
Functionality  :  Enable or disables the entity
Member of      :  CTSExecutorLIB
Friend of      :  -
Author(s)      :  Venkatanarayana Makam
Date Created   :  07/04/2011
Modifications  :
Code Tag       :
******************************************************************************/
HRESULT CTSExecutorLIB::EnableItem(DWORD dwID, BOOL& bEnable)
{
    HRESULT hResult = ERR_WRONG_ID;
    if(dwID == def_ID_TESTSUITE)
    {
        m_bTestSuiteStatus = bEnable;
        hResult= S_OK;
    }
    else
    {
        INT nCount = (INT)m_ouTestSetupEntityList.GetCount();
        for(INT i=0; i<nCount; i++)
        {
            POSITION pos = m_ouTestSetupEntityList.FindIndex(i);
            CTestSetupEntity& ouTempTestSetup = m_ouTestSetupEntityList.GetAt(pos);
            CBaseEntityTA* pEntity;
            ouTempTestSetup.SearchEntityObject(dwID, &pEntity);
            if(pEntity != NULL)
            {
                pEntity->vEnableEntity(bEnable);
                hResult= S_OK;
            }
        }
    }
    return  hResult;
}
/******************************************************************************
Function Name  :  RemoveAllItems
Input(s)       :  -
Output         :  HRESULT
Functionality  :
Member of      :  CTSExecutorLIB
Friend of      :  -
Author(s)      :  Venkatanarayana Makam
Date Created   :  07/04/2011
Modifications  :
Code Tag       :
******************************************************************************/
HRESULT CTSExecutorLIB::RemoveAllItems(void)
{
    m_omstrTestSuiteName = def_STR_TESTSUITENAME;
    INT nCount;
    nCount = (INT)m_ouTestSetupEntityList.GetCount();
    for(INT i=0; i<nCount; i++)
    {
        POSITION pos = m_ouTestSetupEntityList.FindIndex(i);
        CTestSetupEntity& ouTempTestSetup = m_ouTestSetupEntityList.GetAt(pos);
        ouTempTestSetup.vInitialise();
    }
    m_ouTestSetupEntityList.RemoveAll();
    return S_OK;
}

/******************************************************************************
Function Name  :  Execute
Input(s)       :  -
Output         :  HRESULT
Functionality  :  Executes the TestSuite
Member of      :  CTSExecutorLIB
Friend of      :  -
Author(s)      :  Venkatanarayana Makam
Date Created   :  07/04/2011
Modifications  :
Code Tag       :
******************************************************************************/
HRESULT CTSExecutorLIB::Execute( /*PFCALLBACKRESULTTC pfResultTC*/)
{
    HRESULT hResult = S_OK;
    INT nTSCount = (INT)m_ouTestSetupEntityList.GetCount();
    for(INT nTSIndex=0; nTSIndex<nTSCount; nTSIndex++)
    {
        POSITION pos = m_ouTestSetupEntityList.FindIndex(nTSIndex);
        CTestSetupEntity& ouTestSetupEntity = m_ouTestSetupEntityList.GetAt(pos);
        if(ouTestSetupEntity.bGetEnableStatus() == TRUE)
        {
            bExecuteTestSetup(ouTestSetupEntity);
        }
    }
    return hResult;
}

/******************************************************************************
Function Name  :  bExecuteTestSetup
Input(s)       :  CTestSetupEntity& ouTestSetupEntity
Output         :  BOOL
Functionality  :  Executes the TestSetup
Member of      :  CTSExecutorLIB
Friend of      :  -
Author(s)      :  Venkatanarayana Makam
Date Created   :  07/04/2011
Modifications  :
Code Tag       :
******************************************************************************/
BOOL CTSExecutorLIB::bExecuteTestSetup(CTestSetupEntity& ouTestSetupEntity)
{
    CBaseEntityTA* pouTestCase;
    UINT unTCCount;
    //Initialise Result Information
    ouTestSetupEntity.GetSubEntryCount(unTCCount);
    ouTestSetupEntity.GetHeaderData(m_ouResult.m_ouHeaderInfo);
    ouTestSetupEntity.m_ouDataBaseManager.bFillDataStructureFromDatabaseFile(m_ouResult.m_ouHeaderInfo.m_omDatabasePath);
    m_ouResult.m_omStrTestSetupFile = ouTestSetupEntity.m_omstrTestSetupTitle;
    m_ouResult.m_odTcResultList.RemoveAll();
    for(UINT nTCIndex=0; nTCIndex<unTCCount; ++nTCIndex)
    {

        ouTestSetupEntity.GetSubEntityObj(nTCIndex, &pouTestCase);
        if(pouTestCase->bGetEnableStatus() == TRUE)
        {
            //Start Result Update
            CTestCaseData ouTestCaseData;
            pouTestCase->GetEntityData(TEST_CASE, &ouTestCaseData);

            CResultTc ouTestCaseResult;
            ouTestCaseResult.m_omTestCase = ouTestCaseData.m_omTitle;
            ouTestCaseResult.m_omTestDescripter = "";
            GetLocalTime(&ouTestCaseResult.m_sStartTime);
            //Now Execute;
            ouTestCaseResult.m_eResult = SUCCESS;
            BOOL bResult = bExecuteTestCase(pouTestCase, ouTestCaseResult);
            if(bResult == FALSE)
            {
                //TODO:: ProperHandling Required
                ouTestCaseResult.m_eResult = ERRORS;
            }
            GetLocalTime(&ouTestCaseResult.m_sEndTime);
            m_ouResult.m_odTcResultList.AddTail(ouTestCaseResult);
        }
    }
    m_ouResult.GenerateReport();
    return TRUE;
}

/******************************************************************************
Function Name  :  bExecuteTestCase
Input(s)       :  CBaseEntityTA* pTCEntity, CResultTc& ouTestCaseResult
Output         :  BOOL
Functionality  :  Executes the TestCase
Member of      :  CTSExecutorLIB
Friend of      :  -
Author(s)      :  Venkatanarayana Makam
Date Created   :  07/04/2011
Modifications  :
Code Tag       :  CS041
******************************************************************************/
BOOL CTSExecutorLIB::bExecuteTestCase(CBaseEntityTA* pTCEntity, CResultTc& ouTestCaseResult)
{
    if(g_podTSExecutor == NULL)
    {
        return NULL;
    }
    CBaseEntityTA* pEntity;
    CString omStrTilte;
    CString omStrID;
    eACTION_EXCP eExp;
    CString omStrDisplayMsg;
    BOOL bResult = TRUE;
    ouTestCaseResult.m_ouVerifyList.RemoveAll();

    ((CTestCaseEntity*)pTCEntity)->GetTestCaseDetails(omStrTilte, omStrID, eExp);
    int nCurrentRow = m_ompResultDisplayWnd->GetItemCount();
    m_ompResultDisplayWnd->InsertItem(nCurrentRow, omStrTilte);

    UINT unCount;
    pTCEntity->GetSubEntryCount(unCount);
    for(UINT unIndex = 0; unIndex < unCount; unIndex++)
    {
        nCurrentRow = m_ompResultDisplayWnd->GetItemCount();
        m_ompResultDisplayWnd->InsertItem(nCurrentRow, "");
        pTCEntity->GetSubEntityObj(unIndex, &pEntity);
        switch(pEntity->GetEntityType())
        {
            case SEND:
            {
                m_ompResultDisplayWnd->SetItemText(nCurrentRow, 1, _("Sending Messages Started"));
                g_podTSExecutor->TSX_SendMessage(pEntity);
            }
            break;
            case VERIFY:
            {
                m_ompResultDisplayWnd->SetItemText(nCurrentRow, 1, _("Verifying Started"));
                CResultVerify ouVerifyResult;
                if(g_podTSExecutor->TSX_VerifyMessage(pEntity, ouVerifyResult) == S_FALSE)
                {
                    bResult = FALSE;
                }
                ouTestCaseResult.m_ouVerifyList.AddTail(ouVerifyResult);
            }
            break;
            case WAIT:
            {
                CString omStrTemp;
                CWaitEntityData ouWaitData;
                pEntity->GetEntityData(WAIT, &ouWaitData);
                omStrTemp.Format(_("Waiting %d msec for %s"), ouWaitData.m_ushDuration, ouWaitData.m_omPurpose);
                m_ompResultDisplayWnd->SetItemText(nCurrentRow, 1, omStrTemp);
                Sleep(ouWaitData.m_ushDuration);
            }
            break;
            case VERIFYRESPONSE:
            {
                m_ompResultDisplayWnd->SetItemText(nCurrentRow, 1, _("VerifyRequest Started"));
                CResultVerify ouVerifyResult;
                if(g_podTSExecutor->TSX_VerifyResponse(pEntity, ouVerifyResult) == S_FALSE)
                {
                    bResult = FALSE;
                }
                ouTestCaseResult.m_ouVerifyList.AddTail(ouVerifyResult);
            }
            break;
            default:
                break;
        }
        if(bResult == FALSE && eExp == EXIT)
        {
            break;
        }
    }
    return bResult;
}
void CTSExecutorLIB::vSetVersionInfo(CString& omStrVersionInfo)
{
    m_ouResult.m_omStrVersion = omStrVersionInfo;
}