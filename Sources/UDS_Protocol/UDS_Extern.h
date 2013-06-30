#pragma once

//#include "PSDI_CAN/MsgContainer_CAN.h"
#include "UDS_Protocol.h"
#include "include/struct_can.h"



#if defined USAGEMODE
#undef USAGEMODE
#endif

#if defined USAGE_EXPORT
#define USAGEMODE   __declspec(dllexport)
#else
#define USAGEMODE   __declspec(dllimport)
#endif

#ifdef __cplusplus
extern "C" {  // only need to export C interface if used by C++ source code
#endif



    USAGEMODE HRESULT UDS_Initialise();
	//__declspec(dllexport) HRESULT DIL_UDS_ShowWnd(void); //Funci�n Para mostrar Main Window 
	
	USAGEMODE HRESULT DIL_UDS_ShowWnd(HWND hParent,int TotalChannels);//; //Funci�n Para mostrar Main Window 
	//USAGEMODE HRESULT Show_ResponseData(unsigned char psMsg[], unsigned char Datalen); 
	USAGEMODE HRESULT EvaluateMessage(unsigned char psMsg[],  unsigned char Datalen, UINT MessageID );
	USAGEMODE HRESULT UpdateChannelUDS(HWND hParent);
	USAGEMODE HRESULT DIL_UDS_ShowSettingWnd(HWND hParent);
	USAGEMODE HRESULT TX_vSetDILInterfacePtrUDS(void* ptrDILIntrf);

	//int respID;


#ifdef __cplusplus
}
#endif



