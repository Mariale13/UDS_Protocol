#pragma once
#include "afxwin.h"
#include "UDSWnd_Defines.h"
#include "UDS_Protocol.h"	  
#include "UDS_Resource.h"

extern int Lear; 
extern int initialByte;
extern CString CurrentService;
extern bool FSending;
extern bool FDontShow;		 // if it's TRUE the message should not be sent
							// This variable is used to check if the response corresponds to my request



class CUDS_NegRespMng 
{
public:

	CUDS_NegRespMng(CUDS_Protocol* pEngine);
	~CUDS_NegRespMng(void);
	CString VerifyNegResponse(unsigned char value);
	CString evaluateResp(unsigned char arrayMsg[],int Byte2Init); 

};
