// DefConverterPage.cpp : implementation file
//


#include "DefConverterPage.h"
#include <strsafe.h>

// CDefConverterPage dialog

IMPLEMENT_DYNAMIC(CDefConverterPage, CPropertyPage)

CDefConverterPage::CDefConverterPage(INT nTabPosition)
    : CPropertyPage(CDefConverterPage::IDD)
    , m_omStrInputFilePath(_T(""))
    , m_omStrOutputFilePath(_T(""))
    , m_omstrConversionComment(_T(""))
    , m_omstrEditHelp(_T(""))
    , m_omstrLogFilePath(_T(""))
{
    m_nTabPosition = nTabPosition;
}

CDefConverterPage::~CDefConverterPage()
{
}

void CDefConverterPage::DoDataExchange(CDataExchange* pDX)
{
    CPropertyPage::DoDataExchange(pDX);
    DDX_Control(pDX, IDC_COMBO_CONVERSIONS, m_omComboConverterNames);
    DDX_Control(pDX, IDC_EDIT_INPUTFILEPATH, m_omEditInputPath);
    DDX_Control(pDX, IDC_EDIT_OUTPUTFILEPATH, m_omEditOutputPath);
    DDX_Text(pDX, IDC_EDIT_INPUTFILEPATH, m_omStrInputFilePath);
    DDX_Text(pDX, IDC_EDIT_OUTPUTFILEPATH, m_omStrOutputFilePath);
    DDX_Text(pDX, IDC_EDIT_COMMENT, m_omstrConversionComment);
    DDX_Text(pDX, IDC_EDIT_HELP, m_omstrEditHelp);
    DDV_MaxChars(pDX, m_omstrEditHelp, 1024);
}


BEGIN_MESSAGE_MAP(CDefConverterPage, CPropertyPage)
    ON_BN_CLICKED(IDC_BTN_INPUT, CDefConverterPage::OnBnClickedBtnInput)
    ON_BN_CLICKED(IDC_BTN_OUTPUT, CDefConverterPage::OnBnClickedBtnOutput)
    ON_BN_CLICKED(IDC_BTN_CONVERT, CDefConverterPage::OnBnClickedBtnConvert)
    ON_BN_CLICKED(IDC_BUTTON_VIEW_LOG, CDefConverterPage::OnBnClickedViewLog)
    ON_CBN_SELCHANGE(IDC_COMBO_CONVERSIONS, CDefConverterPage::OnCbnSelchangeComboConversions)
END_MESSAGE_MAP()


// CDefConverterPage message handlers
HRESULT CDefConverterPage::SetPluginManager(CPluginManagerBase* pouPluginManager)
{
    m_pouPluginManager = pouPluginManager;
    return S_FALSE;
}
HRESULT CDefConverterPage::LoadConverters()
{
    HRESULT hResult = S_FALSE;
    if(m_pouPluginManager != NULL)
    {
        INT nCount = m_pouPluginManager->m_ConverterList.GetCount();
        string strConverterName;
        for(INT i = 0; i < nCount; i++)
        {
            POSITION pos = m_pouPluginManager->m_ConverterList.FindIndex(i);
            ConverterInfo& ouConverterInfo = m_pouPluginManager->m_ConverterList.GetAt(pos);
            if(ouConverterInfo.m_pouConverter->bHaveOwnWindow() == FALSE)
            {
                ouConverterInfo.m_pouConverter->GettextBusmaster();
                ouConverterInfo.m_pouConverter->GetConverterName(strConverterName);
                INT nItem = m_omComboConverterNames.AddString(strConverterName.c_str());
                if(nItem >= 0)
                {
                    m_omComboConverterNames.SetItemData(nItem, i);
                }
            }
        }
        vAdjustWidthMessageComboBox();
        hResult = S_OK;
    }

    return hResult;
}

void CDefConverterPage::vAdjustWidthMessageComboBox()
{
    CString str;
    CSize sz;
    int dx = 0;
    TEXTMETRIC tm;
    CDC* pDC = m_omComboConverterNames.GetDC();
    CFont* pFont = m_omComboConverterNames.GetFont();
    // Select the listbox font, save the old font
    CFont* pOldFont = pDC->SelectObject(pFont);
    // Get the text metrics for avg char width
    pDC->GetTextMetrics(&tm);

    for (int i = 0; i < m_omComboConverterNames.GetCount(); i++)
    {
        m_omComboConverterNames.GetLBText(i, str);
        sz = pDC->GetTextExtent(str);
        sz = pDC->GetTextExtent(str);
        // Add the avg width to prevent clipping
        sz.cx += tm.tmAveCharWidth;

        if (sz.cx > dx)
        {
            dx = sz.cx;
        }
    }

    // Select the old font back into the DC
    pDC->SelectObject(pOldFont);
    m_omComboConverterNames.ReleaseDC(pDC);
    // Adjust the width for the vertical scroll bar and the left and right border.
    dx += ::GetSystemMetrics(SM_CXVSCROLL) + 2*::GetSystemMetrics(SM_CXEDGE);
    // Set the width of the list box so that every item is completely visible.
    m_omComboConverterNames.SetDroppedWidth(dx);
}

BOOL CDefConverterPage::OnInitDialog()
{
    CPropertyPage::OnInitDialog();
    // TODO:  Add extra initialization here
    LoadConverters();
    m_omComboConverterNames.SetCurSel(0);
    OnCbnSelchangeComboConversions();

    GetDlgItem(IDC_BUTTON_VIEW_LOG)->EnableWindow(FALSE);
    return TRUE;  // return TRUE unless you set the focus to a control
}

void CDefConverterPage::OnBnClickedBtnInput()
{
    // TODO: Add your control notification handler code here
    string strFileExt;
    string strFileFilters;
    INT nSelected = GetConverterPos();
    if( (nSelected != -1) && (nSelected < m_pouPluginManager->m_ConverterList.GetCount()))
    {
        POSITION pos = m_pouPluginManager->m_ConverterList.FindIndex(nSelected);
        ConverterInfo& ouConverterInfo = m_pouPluginManager->m_ConverterList.GetAt(pos);
        if( ouConverterInfo.m_pouConverter != NULL)
        {
            ouConverterInfo.m_pouConverter->GettextBusmaster();
            ouConverterInfo.m_pouConverter->GetInputFileFilters(strFileExt, strFileFilters);
        }
        CFileDialog fileDlg( TRUE,     // Open File dialog
                             strFileExt.c_str(),     // Default Extension,
                             NULL,     // Initial file name.
                             OFN_OVERWRITEPROMPT|OFN_FILEMUSTEXIST|
                             OFN_PATHMUSTEXIST,
                             strFileFilters.c_str(),
                             this );
        INT_PTR nRetVal = fileDlg.DoModal();
        if(IDOK == nRetVal)
        {
            m_omEditInputPath.SetWindowText(fileDlg.GetPathName());
            CString omStrOutputFile = fileDlg.GetPathName();
            INT nIndex = omStrOutputFile.ReverseFind('.');
            if ( nIndex >= 0)
            {
                omStrOutputFile = omStrOutputFile.Left(nIndex);
                ouConverterInfo.m_pouConverter->GetOutputFileFilters(strFileExt, strFileFilters);
                omStrOutputFile += ".";
                omStrOutputFile += strFileExt.c_str();
                m_omEditOutputPath.SetWindowText(omStrOutputFile);
            }
            GetDlgItem(IDC_EDIT_COMMENT)->SetWindowText("");
            GetDlgItem(IDC_BUTTON_VIEW_LOG)->EnableWindow(FALSE);
        }
    }

}

void CDefConverterPage::OnBnClickedBtnOutput()
{
    string strFileExt;
    string strFileFilters;
    INT nSelected = GetConverterPos();
    if( (nSelected != -1) && (nSelected < m_pouPluginManager->m_ConverterList.GetCount()))
    {
        POSITION pos = m_pouPluginManager->m_ConverterList.FindIndex(nSelected);
        ConverterInfo& ouConverterInfo = m_pouPluginManager->m_ConverterList.GetAt(pos);
        if( ouConverterInfo.m_pouConverter != NULL)
        {
            ouConverterInfo.m_pouConverter->GettextBusmaster();
            ouConverterInfo.m_pouConverter->GetOutputFileFilters(strFileExt, strFileFilters);
        }
    }
    CFileDialog fileDlg( TRUE,     // Open File dialog
                         strFileExt.c_str(),     // Default Extension,
                         NULL,     // Initial file name.
                         OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,
                         strFileFilters.c_str(),
                         this );
    INT_PTR nRetVal = fileDlg.DoModal();

    if(IDOK == nRetVal)
    {
        m_omEditOutputPath.SetWindowText(fileDlg.GetPathName());
        GetDlgItem(IDC_EDIT_COMMENT)->SetWindowText("");
        GetDlgItem(IDC_BUTTON_VIEW_LOG)->EnableWindow(FALSE);
    }
}

void CDefConverterPage::OnBnClickedViewLog()
{
    //Open a .txt file with it's default editor
    if ( !m_omstrLogFilePath.IsEmpty() )
    {
        //ShellExecute(NULL, "open", m_omStrOutputFilePath, NULL, NULL, SW_SHOWNORMAL);
        HINSTANCE hInst = ShellExecute( 0,
                                        "open",                                  // Operation to perform
                                        "notepad.exe",                           // Application name
                                        m_omstrLogFilePath.GetBuffer(MAX_PATH),  // Additional parameters
                                        0,                                       // Default directory
                                        SW_SHOW);
    }
}

void CDefConverterPage::OnBnClickedBtnConvert()
{
    // TODO: Add your control notification handler code here
    INT nSelected = GetConverterPos();
    m_omstrLogFilePath.Empty();

    if( (nSelected >= 0 ) && (nSelected < m_pouPluginManager->m_ConverterList.GetCount()))
    {
        POSITION pos = m_pouPluginManager->m_ConverterList.FindIndex(nSelected);
        ConverterInfo& ouConverterInfo = m_pouPluginManager->m_ConverterList.GetAt(pos);
        if( ouConverterInfo.m_pouConverter != NULL)
        {
            UpdateData();
            string strInputFilePath  = (LPCTSTR) m_omStrInputFilePath;
            string strOutputFilePath = (LPCTSTR) m_omStrOutputFilePath;

            ouConverterInfo.m_pouConverter->GettextBusmaster();

            //StringCbCopy(strInputFilePath, sizeof(strInputFilePath), m_omStrInputFilePath);
            //StringCbCopy(strOutputFilePath, sizeof(strOutputFilePath), m_omStrOutputFilePath);
            if( S_OK == ValidateFileExtensions(m_omStrInputFilePath, m_omStrOutputFilePath, ouConverterInfo.m_pouConverter))
            {
                HRESULT hResult = ouConverterInfo.m_pouConverter->ConvertFile(strInputFilePath, strOutputFilePath);
                string conversionComment;
                ouConverterInfo.m_pouConverter->GetLastConversionStatus(hResult, conversionComment);
                m_omstrConversionComment = conversionComment.c_str();

                /* If a log file is created */
                if ( m_omstrConversionComment.Compare(_T("Conversion completed with warnings.")) == 0 )
                {
                    m_omstrLogFilePath = m_omStrOutputFilePath;

                    m_omstrLogFilePath.Replace(_T(".dbf"), _T(".log"));

                    GetDlgItem(IDC_BUTTON_VIEW_LOG)->EnableWindow();
                }
                else
                {
                    GetDlgItem(IDC_BUTTON_VIEW_LOG)->EnableWindow(FALSE);
                }

                UpdateData(FALSE);
            }
            else
            {
                MessageBox(_("Invalid Input/Output Files"), _("Error"), MB_OK|MB_ICONERROR);
            }
        }
    }
}

void CDefConverterPage::OnCbnSelchangeComboConversions()
{
    // TODO: Add your control notification handler code here
    m_omStrInputFilePath = _T("");
    m_omStrOutputFilePath = _T("");
    m_omstrConversionComment = _T("");
    string m_omstrConversionName;
    INT nSelectedItemIndex = GetConverterPos();

    GetDlgItem(IDC_BUTTON_VIEW_LOG)->EnableWindow(FALSE);

    if( nSelectedItemIndex >= 0 )
    {
        string chHelpText;
        string chConversionName;
        POSITION pos = m_pouPluginManager->m_ConverterList.FindIndex(nSelectedItemIndex);
        ConverterInfo& ouConverterInfo = m_pouPluginManager->m_ConverterList.GetAt(pos);
        ouConverterInfo.m_pouConverter->GettextBusmaster();
        ouConverterInfo.m_pouConverter->GetHelpText(chHelpText);

        m_omstrEditHelp = chHelpText.c_str();
    }
    UpdateData(FALSE);
}



INT CDefConverterPage::GetConverterPos()
{
    INT nComboBoxIndex = m_omComboConverterNames.GetCurSel();
    INT nListIndex = m_omComboConverterNames.GetItemData(nComboBoxIndex);
    if( (nListIndex >= 0 ) && (nListIndex < m_pouPluginManager->m_ConverterList.GetCount()))
    {
        return nListIndex;
    }
    else
    {
        return -1;
    }
}

HRESULT CDefConverterPage::ValidateFileExtensions(CString& m_omStrInputFilePath, CString& m_omStrOutputFilePath, CBaseConverter*& m_pouConverter)
{
    CString omStrExtension;
    CString omStrFileExtension;
    string strFilter1, strFilter2;
    HRESULT hResult = S_OK;
    if(m_pouConverter == NULL)
    {
        return S_FALSE;
    }

    INT nIndex = m_omStrInputFilePath.ReverseFind('.');
    if( nIndex >= 0 )
    {
        omStrExtension = m_omStrInputFilePath.Right(m_omStrInputFilePath.GetLength() - nIndex-1);
        omStrExtension.MakeLower();
        m_pouConverter->GetInputFileFilters(strFilter1, strFilter2);
        omStrFileExtension = strFilter1.c_str();
        omStrFileExtension.MakeLower();
        if(omStrExtension == strFilter1.c_str())
        {
            nIndex = m_omStrOutputFilePath.ReverseFind('.');
            if( nIndex >= 0 )
            {
                omStrExtension = m_omStrOutputFilePath.Right(m_omStrOutputFilePath.GetLength() - nIndex-1);
                omStrExtension.MakeLower();
                m_pouConverter->GetOutputFileFilters(strFilter1, strFilter2);
                omStrFileExtension = strFilter1.c_str();
                omStrFileExtension.MakeLower();
                if(omStrExtension != strFilter1.c_str())
                {
                    hResult = S_FALSE;
                }
            }
            else
            {
                hResult = S_FALSE;
            }
        }
        else
        {
            hResult = S_FALSE;
        }
    }
    else
    {
        hResult = S_FALSE;
    }
    return hResult;
}