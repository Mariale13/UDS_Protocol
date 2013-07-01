// CUDSSettingsWnd.cpp : implementation file

#include "stdafx.h"
#include "UDSSettingsWnd.h"
#include "UDSMainWnd.h"	  
#include "UDS_Protocol.h"	  
#include "DataTypes/UDS_DataTypes.h"
#include "UDSWnd_Defines.h"

CUDS_Protocol* UdsProtocolPtr = NULL;
extern CUDSMainWnd* omMainWnd;
int SSTMin;



//________________________________________________________________________________________________________________________________________________________________
//________________________________________________________________________________________________________________________________________________________________

typedef struct
{
    UDS_INTERFACE   m_eType;
    CString         m_omTypeName;
    BOOL            m_bShow;
} INTERFACE_COMB;

#define SIZE_INTERFACE_COMB    4

INTERFACE_COMB sg_asInterface[SIZE_INTERFACE_COMB] =
{
    {INTERFACE_NORMAL_11,          " 11bits Normal",          TRUE},
    {INTERFACE_EXTENDED_11,        " 11bits Extended",        TRUE},
    {INTERFACE_NORMAL_ISO_29,      " 29bits Normal ISO",      TRUE},
	{INTERFACE_NORMAL_J1939_29,    " 29bits Normal J1939",    TRUE},


};

INTERFACE_COMB sg_asSupportedInterface[SIZE_INTERFACE_COMB] =
{
    {INTERFACE_NORMAL_11,          " 11bits Normal",          TRUE},
    {INTERFACE_EXTENDED_11,        " 11bits Extended",        TRUE},
    {INTERFACE_NORMAL_ISO_29,      " 29bits Normal ISO",      TRUE},
	{INTERFACE_NORMAL_J1939_29,    " 29bits Normal J1939",    TRUE},
};



void vPopulateInterfaceComboBox(INTERFACE_COMB sg_asInterface[], int nSize,

								CComboBox& omComboBox)
{
    omComboBox.ResetContent();
    for (int i = 0, nIncr = 0; i < nSize; i++)
    {
        if (sg_asInterface[i].m_bShow)
        {
            omComboBox.InsertString(nIncr++, sg_asInterface[i].m_omTypeName);
        }
    }
}
//________________________________________________________________________________________________________________________________________________________________
//________________________________________________________________________________________________________________________________________________________________

typedef struct
{
    DIAGNOSTIC_STANDARD   Diagnostic_eType;
    CString         m_omStandard;
    BOOL            mo_bShow;
} STANDARD_COMB;

STANDARD_COMB sg_asSupportedDiagStandard[SIZE_INTERFACE_COMB] =
{
    {STANDARD_UDS,          "UDS(ISO 14229)",  TRUE},
    //{STANDARD_KWP2000,      "KPW2000",        TRUE},
};

void vPopulateDiagnosticStandardComboBox(STANDARD_COMB sg_asInterface[], int nSize,
                              CComboBox& omComboBox)
{
    omComboBox.ResetContent();
    for (int i = 0, nIncr = 0; i < nSize; i++)
    {
        if (sg_asInterface[i].mo_bShow)
        {
            omComboBox.InsertString(nIncr++, sg_asInterface[i].m_omStandard);
        }
    }
}

//________________________________________________________________________________________________________________________________________________________________
//________________________________________________________________________________________________________________________________________________________________


void vPopulateFCLength(CComboBox& omComboBox)
{
	omComboBox.ResetContent();
	omComboBox.InsertString(0,"3");
	omComboBox.InsertString(1,"8");   //We just have two options for the length of Flowcontrol
}
//________________________________________________________________________________________________________________________________________________________________
//________________________________________________________________________________________________________________________________________________________________

	 
// CUDSSettingsWnd dialog

IMPLEMENT_DYNAMIC(CUDSSettingsWnd, CDialog)

CUDSSettingsWnd::CUDSSettingsWnd(CWnd* pParent /*=NULL*/,CUDS_Protocol* pEngine)
	: CDialog(CUDSSettingsWnd::IDD, pParent)
{
   UdsProtocolPtr = pEngine;

  
}

CUDSSettingsWnd::~CUDSSettingsWnd()
{
}

void CUDSSettingsWnd::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBO_STAND_DIAG, m_omStdDiag);

	DDX_Control(pDX, IDC_COMBO_INTERFACE, m_omInterface);
	DDX_Control(pDX, IDC_COMBO_FC_LENGTH, m_omFCLength);
	DDX_Control(pDX, IDC_SIZE_MSG, m_omCheckMsgDLC);  	
	DDX_Control(pDX, IDC_REQ_CAN_ID, m_omReqCanID);
	DDX_Text(pDX, IDC_REQ_CAN_ID, StringReqCanID );

	DDX_Control(pDX, IDC_RES_CAN_ID, m_omRespCanID);
	DDX_Text(pDX, IDC_RES_CAN_ID, StringRespCanID );
	//DDX_Control(pDX, IDC_REQ_BROADC_ADDRESS, m_omReqBroadcAddress);
	//DDX_Control(pDX, IDC_RES_BROADC_ADDRESS, m_omRespBroadcAddress);

	DDX_Control(pDX, IDC_REQ_BASE_ADRRESS, m_omReqBaseAddress);
	DDX_Text(pDX, IDC_REQ_BASE_ADRRESS, StringReqBaseAddress );
	
	DDX_Control(pDX, IDC_RES_BASE_ADRRESS, m_omRespBaseAddress);   
	DDX_Text(pDX, IDC_RES_BASE_ADRRESS, StringRespBaseAddress );
	
	DDX_Control(pDX, IDC_STmin, m_STMin); 
	DDX_Text(pDX, IDC_STmin, Temp_SSTMin);

	DDX_Control(pDX, IDC_BSize, m_BSize); 
	DDX_Text(pDX, IDC_BSize, Temp_BSize);

	//DDX_Control(pDX, IDC_CHECK_FC8, m_omFlowControl);

	DDX_Control(pDX, IDC_S3_Client, m_S3Client);  // Aún no tengo una variable que guarde estos valores
	DDX_Control(pDX, IDC_S3_Server, S3_Server);	 // Una vez que use los valores para algo en ese momento creo las variables para utilizarla
												 // Hasta ahora sólo los inicializo

	DDX_Control(pDX, IDC_P2_TimeOut, P2TimeOut);
	DDX_Control(pDX, IDC_P2_Extended, P2extended);

	DDX_Control(pDX,IDC_TP_NORESP_REQ, m_omTPNoRespReq);

}


BEGIN_MESSAGE_MAP(CUDSSettingsWnd, CDialog)

	ON_CBN_SELCHANGE(IDC_COMBO_INTERFACE, OnCbnSelchangeComboInterface)
	//ON_EN_UPDATE(IDC_P2_TimeOut, OnEnChangeP2_TimeOut)
	ON_EN_UPDATE(IDC_P2_Extended, OnEnChangeP2_Extended)
	ON_BN_CLICKED(IDC_OKButton, OnBnOKPressed)
	ON_BN_CLICKED(ID_CANCEL, OnBnCancelPressed)
	ON_EN_UPDATE( IDC_STmin, OnEnChangeSTmin) 

	
	//ON_WM_CLOSE()

END_MESSAGE_MAP()	

//________________________________________________________________________________________________________________________________________________________________
//________________________________________________________________________________________________________________________________________________________________

void CUDSSettingsWnd::OnEnChangeBSize(){
	UpdateData();							 // función para actualizar los datos
	BSize = m_BSize.lGetValue();
	if   ( !Temp_BSize.GetLength()){		 //in the case that the BSize Box is empty when OK botton has been pressed 
		Temp_BSize = "0"; 
		UpdateData(false);
	}
}

//________________________________________________________________________________________________________________________________________________________________
//________________________________________________________________________________________________________________________________________________________________

void CUDSSettingsWnd::OnCbnSelchangeFCLength(){
	switch(m_omFCLength.GetCurSel() ){
		case 0:
		{
			SizeFC=3+aux;
		}
		break;
		case 1:
			{
			SizeFC=8;
			}
		break;
		default:
			//do nothing
		break; 			
	}
}

//________________________________________________________________________________________________________________________________________________________________
//________________________________________________________________________________________________________________________________________________________________

void CUDSSettingsWnd::OnEnChangeSTmin(){
	UpdateData();  // función para actualizar los datos
	if (Temp_SSTMin.GetLength()){
		int STmin_Value = _tcstol(Temp_SSTMin, L'\0', 16); 
		if ( STmin_Value >= 0xFA){
			m_STMin.vSetValue(0xF);
			return  ;
		}else if( ( STmin_Value&0x80) && ( STmin_Value<0xF1 ) ) m_STMin.vSetValue(0); 
	}
}	
//________________________________________________________________________________________________________________________________________________________________
//________________________________________________________________________________________________________________________________________________________________

void CUDSSettingsWnd::OnSaveSTmin(){
	UpdateData();  // función para actualizar los datos
	SSTMin = m_STMin.lGetValue();
	if   ( !Temp_SSTMin.GetLength()){		 //in the case that the STMin Box is empty when OK botton has been pressed 
		Temp_SSTMin = "0"; 
		UpdateData(false);
	}
}

//________________________________________________________________________________________________________________________________________________________________
//________________________________________________________________________________________________________________________________________________________________

void CUDSSettingsWnd::OnEnChangeP2_TimeOut(){
	UpdateData();  // función para actualizar los datos
	P2_Time = P2TimeOut.lGetValue();
}	  
//________________________________________________________________________________________________________________________________________________________________
//________________________________________________________________________________________________________________________________________________________________

void CUDSSettingsWnd::OnEnChangeP2_Extended(){
	UpdateData();  // función para actualizar los datos
	P2_Time_Extended =  P2extended.lGetValue();
}

//________________________________________________________________________________________________________________________________________________________________
//________________________________________________________________________________________________________________________________________________________________

void CUDSSettingsWnd::OnEnChangeS3_Client(){
	UpdateData();  // función para actualizar los datos
	S3_Client = m_S3Client.lGetValue();   
}


//________________________________________________________________________________________________________________________________________________________________
//________________________________________________________________________________________________________________________________________________________________

BOOL CUDSSettingsWnd::OnInitDialog(){
	CDialog::OnInitDialog();
	vInitializeUDSSettingsfFields();
	return TRUE;
}

//________________________________________________________________________________________________________________________________________________________________
//________________________________________________________________________________________________________________________________________________________________

void CUDSSettingsWnd::vInitializeUDSSettingsfFields(){

	m_omReqCanID.vSetBase(BASE_HEXADECIMAL);
	m_omRespCanID.vSetBase(BASE_HEXADECIMAL);   //Pendiente asunto de como avisar que es Hexa
	
	m_omReqCanID. LimitText(3);
   	m_omRespCanID. LimitText(3);  

	m_omReqCanID.vSetSigned(false);
	m_omRespCanID.vSetSigned(false);

	m_omReqBroadcAddress.vSetBase(BASE_HEXADECIMAL);
	m_omReqBroadcAddress.vSetSigned(false); // Verificar por que a TRUE? 
	m_omReqBroadcAddress.LimitText(3); //Esa correcto verdad! 

	m_omRespBroadcAddress.vSetBase(BASE_HEXADECIMAL);
	m_omRespBroadcAddress.vSetSigned(false); // Verificar por ue a TRUE? 
	m_omRespBroadcAddress. LimitText(3); //Esa correcto verdad! 

	m_omReqBaseAddress.vSetBase(BASE_HEXADECIMAL);
	m_omReqBaseAddress.vSetSigned(false); // Verificar por ue a TRUE? 
	m_omReqBaseAddress. LimitText(3); //Esa correcto verdad! 

	m_omRespBaseAddress.vSetBase(BASE_HEXADECIMAL);
	m_omRespBaseAddress.vSetSigned(false); // Verificar por ue a TRUE? 
	m_omRespBaseAddress.LimitText(3); //Esa correcto verdad! 

	m_STMin.vSetBase(BASE_HEXADECIMAL);
	m_STMin.LimitText(2);
	m_STMin.vSetSigned(false);
	m_STMin.vSetValue(0);

	m_BSize.vSetBase(BASE_DECIMAL);
	m_BSize.vSetSigned(false);
	m_BSize.vSetValue(0);

	m_S3Client.vSetBase( BASE_DECIMAL);
	m_S3Client.vSetSigned(false);
	m_S3Client.vSetValue(2000);

	S3_Server.vSetBase(	BASE_DECIMAL);
	S3_Server.vSetSigned(false);
	S3_Server.vSetValue(5000);

	P2TimeOut.vSetBase(BASE_DECIMAL);
	P2TimeOut.vSetSigned(false);
	P2TimeOut.vSetValue(250);
	P2TimeOut.LimitText(5);


	P2extended.vSetBase(BASE_DECIMAL);
	P2extended.vSetSigned(false);
	P2extended.vSetValue(2000);

	vPopulateInterfaceComboBox(sg_asInterface, SIZE_INTERFACE_COMB, m_omInterface);
	vPopulateDiagnosticStandardComboBox(sg_asSupportedDiagStandard,1, m_omStdDiag);	  //Changed to modify only the Diagnostic Standard box
    vPopulateFCLength(m_omFCLength);
	m_omFCLength.SetCurSel(0);
	m_omStdDiag.SetCurSel(0);		// Hace falta una función como "OnCbnSelchangeComboInterface()" que haga algo con la opción seleccionada en el Diagnostic Standard 
	m_omInterface.SetCurSel(0);

	OnCbnSelchangeComboInterface();
	Prev_InterfaceIndex = 0;
	OnBnOKPressed();

	//m_omRespBaseAddress.vSetValue( TempResp_BaseAddress);	 // Initialize of this values
	//m_omReqBaseAddress.vSetValue(TempReq_BaseAddress);


}

//________________________________________________________________________________________________________________________________________________________________
//________________________________________________________________________________________________________________________________________________________________

void CUDSSettingsWnd::OnCbnSelchangeComboInterface()
{
	//Set From address field readonly
	//m_omFromEdit.SetReadOnly(TRUE);

	m_nInterfaceIndex = m_omInterface.GetCurSel();
	//UdsProtocolPtr->fInterface = sg_asSupportedInterface[m_nInterfaceIndex].m_eType;  //Lo coloco acá para modificar cosas en el MainWnd 
	switch (sg_asSupportedInterface[m_nInterfaceIndex].m_eType)
	{
	case INTERFACE_NORMAL_11:
		{
			m_omReqCanID.vSetValue(0x700);		  
			m_omRespCanID.vSetValue(0x700);
			StringRespCanID= "700";
			StringReqCanID= "700";
			m_omReqCanID. LimitText(3);
   			m_omRespCanID. LimitText(3);  

			m_omReqCanID.SetReadOnly(FALSE);		  
			m_omRespCanID.SetReadOnly(FALSE);

			m_omReqBaseAddress.SetReadOnly(TRUE);
			m_omRespBaseAddress.SetReadOnly(TRUE);

			m_omRespBroadcAddress.SetReadOnly(TRUE);
			m_omReqBroadcAddress.SetReadOnly(TRUE);	

			StringReqBaseAddress = " ";
			StringRespBaseAddress = " ";

			m_omCheckMsgDLC.SetCheck(BST_CHECKED);
			m_omFCLength.SetCurSel(1);
		}
		break;
	case INTERFACE_EXTENDED_11:
		{
			m_omReqCanID.SetReadOnly(TRUE);
			m_omRespCanID.SetReadOnly(TRUE);

			m_omReqBaseAddress.vSetValue(0x600);
			m_omRespBaseAddress.vSetValue(0x600);

			StringReqBaseAddress = "600";
			StringRespBaseAddress = "600";

			m_omReqBaseAddress.SetReadOnly(FALSE);
			m_omRespBaseAddress.SetReadOnly(FALSE);

			m_omRespBroadcAddress.SetReadOnly(TRUE);
			m_omReqBroadcAddress.SetReadOnly(TRUE);

			StringRespCanID= " ";
			StringReqCanID= " ";

			m_omCheckMsgDLC.SetCheck(BST_UNCHECKED);
			m_omFCLength.SetCurSel(0);

		}
		break;
	case INTERFACE_NORMAL_ISO_29:
		{
			m_omReqCanID. LimitText(8);
   			m_omRespCanID. LimitText(8);  
			m_omReqCanID.vSetValue(0x1BC00000);		  
			m_omRespCanID.vSetValue(0x1BC00000);
			StringRespCanID= "1BC00000";
			StringReqCanID= "1BC00000";

			m_omReqCanID.SetReadOnly(FALSE);		  
			m_omRespCanID.SetReadOnly(FALSE);

			m_omReqBaseAddress.SetReadOnly(TRUE);
			m_omRespBaseAddress.SetReadOnly(TRUE);

			m_omRespBroadcAddress.SetReadOnly(TRUE);
			m_omReqBroadcAddress.SetReadOnly(TRUE);	

			StringReqBaseAddress = " ";
			StringRespBaseAddress = " ";

			//m_omCheckMsgDLC.SetCheck(BST_CHECKED);
			//m_omFCLength.SetCurSel(1);
		}
		break;
		case INTERFACE_NORMAL_J1939_29:
		{
			m_omReqCanID. LimitText(8);
   			m_omRespCanID. LimitText(8);  
			m_omReqCanID.vSetValue(0x1BDA0000);		  
			m_omRespCanID.vSetValue(0x1BDA0000);
			StringRespCanID= "1BDA0000";
			StringReqCanID= "1BDA0000";

			m_omReqCanID.SetReadOnly(FALSE);		  
			m_omRespCanID.SetReadOnly(FALSE);

			m_omReqBaseAddress.SetReadOnly(TRUE);
			m_omRespBaseAddress.SetReadOnly(TRUE);

			m_omRespBroadcAddress.SetReadOnly(TRUE);
			m_omReqBroadcAddress.SetReadOnly(TRUE);	

			StringReqBaseAddress = " ";
			StringRespBaseAddress = " ";

			//m_omCheckMsgDLC.SetCheck(BST_CHECKED);
			//m_omFCLength.SetCurSel(1);
		}
		break;

	}
	//OnChkbOnFlowC8();		// Para actalizar el tamaño del Flow Control
	UpdateData(false);  // función para actualizar los datos 
}

//________________________________________________________________________________________________________________________________________________________________
//________________________________________________________________________________________________________________________________________________________________

void CUDSSettingsWnd::OnBnOKPressed(){

	UpdateData();  // función para actualizar los datos 
	Prev_InterfaceIndex = m_nInterfaceIndex;
	UdsProtocolPtr->fInterface = sg_asSupportedInterface[m_nInterfaceIndex].m_eType;  //Lo coloco acá para modificar cosas en el MainWnd 
	switch (UdsProtocolPtr->fInterface){ 
		case INTERFACE_NORMAL_11:			//In this case I'm changing only the CANID
			{
				numberOfTaken=12;	
				initialByte=0;
				aux = 0; 

				TempReq_CanID = m_omReqCanID.lGetValue();
				TempResp_CanID = m_omRespCanID.lGetValue();

				if ( StringReqCanID.GetLength() == 3  && TempReq_CanID >= 0x01 && TempReq_CanID <= 0x7FF){
					ReqCanID = strtoul(StringReqCanID.Right(2), NULL, 16);
					UdsProtocolPtr->SourceAddress = ReqCanID;
					UdsProtocolPtr->MsgID = strtoul(StringReqCanID.Right(3), NULL, 16);	  
				}else {				   // Isn't a valid CAN ID 
					UdsProtocolPtr->SourceAddress = 0x00;
					UdsProtocolPtr->MsgID = 0x000;
					UdsProtocolPtr->TargetAddress = 00;
				}	

				if ( StringRespCanID.GetLength() == 3 && TempResp_CanID >= 0x01 && TempResp_CanID <= 0x7FF){
					ReqCanID = strtoul(StringRespCanID.Right(2), NULL, 16);
					UdsProtocolPtr->TargetAddress = ReqCanID;
					respID = strtoul(StringRespCanID , NULL, 16);   // the StringRespCanID is updated only if the CanID is correct
					// Deberia guardar este valor en alguna parte para 
					//luego verificar el flow control
				}else {							// Isn't a valid CAN ID 
					UdsProtocolPtr->SourceAddress = 0x00;
					UdsProtocolPtr->MsgID = 0x000;
					UdsProtocolPtr->TargetAddress = 00;
				}	
				//UdsProtocolPtr->UpdateFields();   Esto debe hacerse al final!! 
			}
			break;
		case INTERFACE_EXTENDED_11:
			{
				numberOfTaken = 10;
				initialByte =1;
				aux = 1;
				TempResp_BaseAddress = m_omRespBaseAddress.lGetValue();
				TempReq_BaseAddress = m_omReqBaseAddress.lGetValue();

				if ( StringReqBaseAddress.GetLength()== 3 && TempReq_BaseAddress>=0x100 && TempResp_BaseAddress<=0xF00){
					UdsProtocolPtr->MsgID = strtoul(StringReqBaseAddress.Left(3), NULL, 16);
				}else{
					UdsProtocolPtr->MsgID = 0x000; 
					//UdsProtocolPtr->fInterface = INTERFACE_NORMAL_11;
				}

				if ( StringRespBaseAddress.GetLength()== 3 && TempResp_BaseAddress>=0x100 && TempReq_BaseAddress<=0xF00){
					//UdsProtocolPtr->MsgID = strtoul(StringReqBaseAddress.Left(3), NULL, 16);
					// Aqui no se que debo hacer con este valor
					RespMsgID= strtoul( StringRespBaseAddress, NULL, 16);  
				}else{
					UdsProtocolPtr->MsgID = 0x000;
					//UdsProtocolPtr->fInterface = INTERFACE_NORMAL_11;
				}
			}
			break;
		case INTERFACE_NORMAL_ISO_29:			//In this case I'm changing only the CANID
			{
				numberOfTaken=12;	
				initialByte=0;
				aux = 0; 

				TempReq_CanID = m_omReqCanID.lGetValue();
				TempResp_CanID = m_omRespCanID.lGetValue();

				if ( StringReqCanID.GetLength() == 8  && /*TempReq_CanID >= 0x01 && */TempReq_CanID <= 0x1FFFFFFF){
					// Por ahora está incorrecto y debo pasarlo a bit y tomar los valores 
					ReqCanID = strtoul(StringReqCanID, NULL, 16);
					UdsProtocolPtr->SourceAddress = (ReqCanID & MASK_SA_ID_29Bits)>>11;
					UdsProtocolPtr->MsgID = strtoul(StringReqCanID.Right(8), NULL, 16);	
					UdsProtocolPtr->TargetAddress = (ReqCanID & MASK_TA_ID_29Bits);
				}else {				   // Isn't a valid CAN ID 
					UdsProtocolPtr->SourceAddress = 0x00;
					UdsProtocolPtr->MsgID = 0x000;
					UdsProtocolPtr->TargetAddress = 00;
				}	

				if ( StringRespCanID.GetLength() == 8 && /*TempResp_CanID >= 0x01 &&*/ TempResp_CanID <= 0x1FFFFFFF){
					//ReqCanID = strtoul(StringRespCanID.Right(2), NULL, 16);
					//UdsProtocolPtr->TargetAddress = ReqCanID;
					respID = strtoul(StringRespCanID , NULL, 16);   // the StringRespCanID is updated only if the CanID is correct
				}else {							// Isn't a valid CAN ID 
					UdsProtocolPtr->SourceAddress = 0x00;
					UdsProtocolPtr->MsgID = 0x000;
					UdsProtocolPtr->TargetAddress = 00;
				}	
			}
			break;
		case INTERFACE_NORMAL_J1939_29:			//In this case I'm changing only the CANID
			{
				numberOfTaken=12;	
				initialByte=0;
				aux = 0; 

				TempReq_CanID = m_omReqCanID.lGetValue();
				TempResp_CanID = m_omRespCanID.lGetValue();

				if ( StringReqCanID.GetLength() == 8  && TempReq_CanID <= 0x1FFFFFFF){
					// Por ahora está incorrecto y debo pasarlo a bit y tomar los valores 
					ReqCanID = strtoul(StringReqCanID, NULL, 16);
					UdsProtocolPtr->TargetAddress = (ReqCanID & 0xFF00)>>8;
					UdsProtocolPtr->MsgID = strtoul(StringReqCanID.Right(8), NULL, 16);	
					UdsProtocolPtr->SourceAddress = (ReqCanID & 0xFF);
				}else {				   // Isn't a valid CAN ID 

					UdsProtocolPtr->SourceAddress = 0x00;
					UdsProtocolPtr->MsgID = 0x000;
					UdsProtocolPtr->TargetAddress = 00;
				}	

				if ( StringRespCanID.GetLength() == 8 && /*TempResp_CanID >= 0x01 &&*/ TempResp_CanID <= 0x1FFFFFFF){

					respID = strtoul(StringRespCanID , NULL, 16);   // the StringRespCanID is updated only if the CanID is correct
				}else {							// Isn't a valid CAN ID 
					UdsProtocolPtr->SourceAddress = 0x00;
					UdsProtocolPtr->MsgID = 0x000;
					UdsProtocolPtr->TargetAddress = 00;
				}	
			}
			break;


	}

	//To verify the legth of the simple messages   
	fMsgSize = m_omCheckMsgDLC.GetCheck();  
	FCRespReq = m_omTPNoRespReq.GetCheck();
	//if(m_omCheckMsgDLC.GetCheck()){ 
	//	fMsgSize =TRUE;
	//} else { 
	//	fMsgSize = FALSE;
	//}	
	OnEnChangeP2_TimeOut();
	UdsProtocolPtr->UpdateFields();
	OnCbnSelchangeFCLength();
	OnSaveSTmin();
	OnEnChangeBSize();
	OnEnChangeS3_Client();
}

//________________________________________________________________________________________________________________________________________________________________
//________________________________________________________________________________________________________________________________________________________________

void CUDSSettingsWnd::OnBnCancelPressed(){

	//UpdateData();  // función para actualizar los datos 
	UdsProtocolPtr->fInterface = sg_asSupportedInterface[Prev_InterfaceIndex].m_eType;  //Lo coloco acá para modificar cosas en el MainWnd 
	m_omInterface.SetCurSel(Prev_InterfaceIndex);
	switch (UdsProtocolPtr->fInterface){ 
		case INTERFACE_NORMAL_11:			//In this case I'm changing only the CANID
			{
				numberOfTaken=12;	
				initialByte=0;
				aux = 0; 
				StringReqBaseAddress = " ";
				StringRespBaseAddress = " ";
				UpdateData(false);  // función para actualizar los datos 
				m_omReqCanID.vSetValue(TempReq_CanID);
				m_omRespCanID.vSetValue(TempResp_CanID);

				//m_omRespBaseAddress.vSetValue( TempResp_BaseAddress);
				//m_omReqBaseAddress.vSetValue(TempReq_BaseAddress);
				m_omReqCanID.SetReadOnly(FALSE);		  
				m_omRespCanID.SetReadOnly(FALSE);

				m_omReqBaseAddress.SetReadOnly(TRUE);
				m_omRespBaseAddress.SetReadOnly(TRUE);

				//m_omRespBroadcAddress.SetReadOnly(TRUE);
				//m_omReqBroadcAddress.SetReadOnly(TRUE);	

				m_omCheckMsgDLC.SetCheck(BST_CHECKED);
				m_omFCLength.SetCurSel(1);
		}
			break;
		case INTERFACE_EXTENDED_11:
			{
				numberOfTaken = 10;
				initialByte =1;
				aux = 1;
				StringRespCanID= " ";
				StringReqCanID= " ";
				UpdateData(false);  // función para actualizar los datos 
				m_omRespBaseAddress.vSetValue( TempResp_BaseAddress);
				m_omReqBaseAddress.vSetValue(TempReq_BaseAddress);
				//m_omReqCanID.vSetValue(TempReq_CanID);
				//m_omRespCanID.vSetValue(TempResp_CanID);


				m_omReqCanID.SetReadOnly(TRUE);
				m_omRespCanID.SetReadOnly(TRUE);

				m_omReqBaseAddress.SetReadOnly(FALSE);
				m_omRespBaseAddress.SetReadOnly(FALSE);

				//m_omRespBroadcAddress.SetReadOnly(TRUE);
				//m_omReqBroadcAddress.SetReadOnly(TRUE);

				m_omCheckMsgDLC.SetCheck(BST_UNCHECKED);
				m_omFCLength.SetCurSel(0);

			}
			break;
	}

	//To verify the legth of the simple messages   

	if (fMsgSize) m_omCheckMsgDLC.SetCheck( BST_CHECKED); 
	//fMsgSize = m_omCheckMsgDLC.GetCheck();  

	FCRespReq = m_omTPNoRespReq.GetCheck();
	P2TimeOut.vSetValue( P2_Time);	// To save the previous P2Time Value

	if(SizeFC==8){
		m_omFCLength.SetCurSel(1);
	}else{
		m_omFCLength.SetCurSel(0);	
	}

	m_STMin.vSetValue(SSTMin);
	//OnEnChangeSTmin();

	m_BSize.vSetValue(BSize);

	//OnEnChangeBSize();
	m_S3Client.vSetValue(S3_Client);


	this->ShowWindow(SW_HIDE);

}


//________________________________________________________________________________________________________________________________________________________________
//________________________________________________________________________________________________________________________________________________________________
