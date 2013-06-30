#pragma once

#include "UDS_Resource.h"
#include "UDS_Protocol.h"
#include "Utility/RadixEdit.h"
#include "UDSWnd_Defines.h"
#include "DataTypes/UDS_DataTypes.h"
#include "DataTypes/MsgBufFSE.h"

//#include "include/struct_can.h"
//#include "Application/Struct.h"
//int P2_Time=250;
//#include "UDSWnd_Defines.h"
extern int numberOfTaken;
extern int initialByte;
extern int aux;					// Ayuda a controlar el index de i y a diferenciar entre el caso de Extended y Normal addressing
extern int STMin;				// Corresponds to the STMin received from the flowControlMessage
extern int SSTMin;
extern int BSize;
extern int BSizE;
extern int SizeFC;
extern int P2_Time;
extern int P2_Time_Extended;
extern CString Bytes_to_Show;
extern bool FSending;
extern int RespMsgID;
extern int respID;   //correspond to the exact ID of the message received
extern bool fMsgSize;	// If TRUE the msg should have 8 bytes always 
extern bool	FCRespReq;	 // If TRUE TP will be ...0x3E 0x80 (No response required
extern int Lear; 
extern CString CurrentService;
extern UINT_PTR m_nTimer;


class CUDSMainWnd : public CDialog
{
	DECLARE_DYNAMIC(CUDSMainWnd)
public:
	CUDSMainWnd(UINT SA, UINT TA , UDS_INTERFACE fInterface,UINT CanId, CWnd* pParent =NULL );   // standard constructor
	virtual ~CUDSMainWnd();

	CString DatatoSend;			// It will containt all the remaining bytes that has to be sent in long requests
	CRadixEdit m_omBytes;
	CComboBox m_omComboChannelUDS;
	CRadixEdit m_omSourceAddress;
	CRadixEdit m_omTargetAddress;
	CRadixEdit m_omCanID;
	CRadixEdit m_omEditDLC;
	UINT m_stringEditDLC;
	CRadixEdit  m_omEditMsgData;
	CString m_omMsgDataEdit;
	CString  m_abDatas; 
	CButton m_omCheckTP;

	CString m_omDiagService;  
	CRadixEdit m_omDataResponse;
	CButton m_omSendButton;

	UINT TotalLength;
	int ConsecutiveFrame;				 // It has the first Byte of the consecutive messages 0x20 , 0x21, 0x22, 0x23 

	static CUDSMainWnd* m_spodInstance;
	UINT SourceAddress;
	UINT TargetAddress;
	UDS_INTERFACE fInterface;
	UINT CanID;
	int MsgID; 
	CString StringCanID; 

   	mPSTXSELMSGDATA psTxCanMsgUds;
	int TotalChannel;

	//UINT_PTR m_nTimer;

	//CWnd* pWndd;

	CFont m_Font;

	// Declaración de funciones 
	(void)vInitializeUDSfFields();
	(void)setValue();		
	(void)CUDSMainWnd::OnBnClickedSendUD();   // Aún noc que parámetro ha de devolver
	(void)CUDSMainWnd::OnBnClickedTesterPresent(); // Aún noc que parámetro ha de devolver

	(void)CUDSMainWnd::OnEnChangeData();  //	Actualiza el DLC
	(void) CUDSMainWnd::OnEnChangeSA();
	(void) CUDSMainWnd::OnEnChangeTA();
	(void) CUDSMainWnd::SendSimpleDiagnosticMessage(void/*mPSTXSELMSGDATA psTxCanMsgUds*/);
	(int) CUDSMainWnd::SendFirstFrame(/*UINT TotalLength ,*/CString omByteStr, unsigned char abByteArr[],mPSTXSELMSGDATA psTxCanMsgUds, UDS_INTERFACE FInterface);
	(void) CUDSMainWnd::SendContinuosFrames(/*UINT TotalLength ,*//*CString omByteStr,*/ unsigned char abByteArr[],mPSTXSELMSGDATA psTxCanMsgUds, UDS_INTERFACE FInterface);
	
	(void) CUDSMainWnd::PrepareDiagnosticMessage(CString omByteStr,mPSTXSELMSGDATA psTxCanMsgUds, unsigned char abByteArr[], UINT ByteArrLen /*, UDS_INTERFACE FInterface,UINT targetAddress*/ );
	void CUDSMainWnd::PrepareFlowControl(void);


	CWnd* pomGetTxMsgViewPointers() const;

	void vSetDILInterfacePtr(void* ptrDILIntrf);
	void* pGetDILInterfacePtr();
	void vUpdateChannelIDInfo();
	static CUDSMainWnd* s_podGetUdsMsgManager();
	 afx_msg void OnTimer(UINT_PTR nIDEvent);
	 afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);

	 CString initialEval(CString Data2Send );



 //   static BOOL KillTimer(UINT_PTR nIDEvent);

	UINT StartTimer(/*LPVOID nada*/);
	
private:
	//CUdsMsgBlocksView* m_pomUDSMsgBlocksView;
	CWnd* pomGetBlocksViewPointer() const;
	// To Get Parent Window Pointer
	CWnd* pomGetParentWindow() const;

	(void) CUDSMainWnd::CalculateDiffTime(void);			//esta función debería estar implementada en otra clase
	int  CUDSMainWnd::nCalculateCurrTime(BOOL bFromDIL);
	/*static*/ 

	// Dialog Data
	enum { IDD = IDM_UDS };

protected:
	virtual BOOL OnInitDialog();
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	static UINT OnSendSelectedMsg(LPVOID pParam);


	//static  UINT_PTR SetTimer(UINT_PTR nIDEvent, UINT nElapse,void (CALLBACK* lpfnTimer)(HWND, UINT, UINT_PTR, DWORD));
	//static void OnTimer(UINT nIDEvent);
 //   static BOOL KillTimer(UINT_PTR nIDEvent);



	DECLARE_MESSAGE_MAP()
};
