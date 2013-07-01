#pragma once


#include "UDS_Resource.h"
#include "UDS_Protocol.h"
#include "Utility/RadixEdit.h"
#include "DataTypes/UDS_DataTypes.h"

/**  It contains the number of Data bytes that can be sent in the current message */ 
extern int numberOfTaken;

extern int initialByte;
/** It's used to control the index i according to if it's working on extended or normal addressing  */
extern int aux_Finterface;	
/** Corresponds to the STMin received from the flowControlMessage */
extern int STMin;

extern int BSize;

extern int SizeFC;
extern int P2_Time;
extern int P2_Time_Extended;
extern int S3_Client; 
/** it corresponds to the exact ID of the message received    */ 
extern int respID;   

/** It Corresponds to the value put in the settingWnd if it's extended	 */ 
extern int RespMsgID;	 
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




	/**
	* This function is called to initialize the settings Window
	*/
	void vInitializeUDSSettingsfFields();

	/**
	* This function is called by the framework when the user selects an option in the 
	* interface combo box. 
	*/
	void OnCbnSelchangeComboInterface();
	/**
	\brief It saves all the settings settled previously in the settings window
	* This function is called by the framework when the user press the OK buttom in the
	* Settings Window to save all the changes made  previously. 
	*/
	void CUDSSettingsWnd::OnBnOKPressed();

	/**
	\brief It closes the settings window without saving the changes
	*/
	void CUDSSettingsWnd::OnBnCancelPressed();

	void CUDSSettingsWnd::OnEnChangeSTmin();
	void CUDSSettingsWnd::OnSaveSTmin();

	/**
	\brief It takes the value of the Flow Control Length combo box 
	* This function is called when the user press the OK button and it saves the new value
	* of  FCLength
	*/
	void CUDSSettingsWnd::OnCbnSelchangeFCLength();

	/**
	\brief It takes the value of the parameter Bsize
	* This function is called when the user press the OK button and it saves the new value
	* of the ISO TP parameter BSize 
	*/
	void CUDSSettingsWnd::OnEnChangeBSize();
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
