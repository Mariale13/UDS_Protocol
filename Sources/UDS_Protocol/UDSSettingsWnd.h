#pragma once


#include "UDS_Resource.h"
#include "UDS_Protocol.h"
#include "Utility/RadixEdit.h"
#include "DataTypes/UDS_DataTypes.h"

extern int numberOfTaken;
extern int initialByte;
extern int aux;					// Ayuda a controlar el index de i y a diferenciar entre el caso de Extended y Normal addressing
extern int STMin;				// Corresponds to the STMin received from the flowControlMessage
extern int BSize;
extern int SizeFC;
extern int P2_Time;
extern int P2_Time_Extended;
extern int S3_Client; 
extern int respID;   ////correspond to the exact ID of the message received
extern int RespMsgID;	 //Corresponds to the value put in the settingWnd if it's extended
extern bool fMsgSize;	// If TRUE the msg should have 8 bytes always 
extern bool	FCRespReq;	  // If TRUE TP will be ...0x3E 0x80 (No response required


// CUDSSettingsWnd dialog

class CUDSSettingsWnd : public CDialog
{
	DECLARE_DYNAMIC(CUDSSettingsWnd)

public:
	CUDSSettingsWnd(CWnd* pParent/* = NULL*/, CUDS_Protocol* pEngine);   // standard constructor
	virtual ~CUDSSettingsWnd();

	CComboBox  m_omStdDiag;
	CComboBox  m_omInterface;
	CComboBox m_omFCLength;
	CRadixEdit m_omReqCanID;
	CString StringReqCanID; 
	CRadixEdit m_omRespCanID;
	CString StringRespCanID; 
	CRadixEdit m_omReqBroadcAddress;
	CRadixEdit m_omRespBroadcAddress;
	CRadixEdit m_omReqBaseAddress;
	CRadixEdit m_omRespBaseAddress;	
	CButton m_omCheckMsgDLC;
	CButton m_omTPNoRespReq;
	CButton m_omFlowControl;
	CRadixEdit m_STMin; 
	CRadixEdit m_S3Client;
	CRadixEdit S3_Server;
	CRadixEdit P2TimeOut;
	CRadixEdit P2extended;
	CRadixEdit m_BSize;
	CString StringReqBaseAddress;
	CString StringRespBaseAddress;
	int m_nInterfaceIndex;
	UINT ResCanID;
	UINT ReqCanID;
	int Prev_InterfaceIndex;
	
	long TempReq_CanID;
	long TempResp_CanID;
	long TempResp_BaseAddress;
	long TempReq_BaseAddress;
	CString Temp_SSTMin;
	CString Temp_BSize;





	void vInitializeUDSSettingsfFields();
	void OnCbnSelchangeComboInterface();
	void CUDSSettingsWnd::OnBnOKPressed();
	void CUDSSettingsWnd::OnBnCancelPressed();

	void CUDSSettingsWnd::OnEnChangeSTmin();
	void CUDSSettingsWnd::OnSaveSTmin();

	void CUDSSettingsWnd::OnCbnSelchangeFCLength();
	void CUDSSettingsWnd::OnEnChangeBSize();
	//void CUDSSettingsWnd::OnChkbOnFlowC8();
	void CUDSSettingsWnd::OnEnChangeP2_TimeOut();
	void CUDSSettingsWnd::OnEnChangeP2_Extended();
	void CUDSSettingsWnd::OnEnChangeS3_Client();
	afx_msg  void CUDSSettingsWnd::OnCloseWnd();




	// Dialog Data
	enum { IDD = IDD_SETTINGS_UDS };

protected:
	virtual BOOL OnInitDialog();
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
};
