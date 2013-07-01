#include "UDS_NegRespMng.h"


#define SIZE_NEG_RESP_STRUCT    39

CUDS_Protocol* UdsProtocolPtr2 = NULL;


typedef struct
{
    INT 			MsgValue;
    CString         m_omTypeResp;
} NEGATIVE_RESP_STRUCT;


NEGATIVE_RESP_STRUCT NegativeResponseStruct[SIZE_NEG_RESP_STRUCT] =
{
	{0x10,        "Neg Response - General Reject"},
	{0x11,        "Neg Response - Service Not Supported"},
	{0x12,        "Neg Response - SubFunction Not Supported"},
	{0x13,        "Neg Response - Incorrect Message Length Or Invalid Format"},				   
	{0x21,        "Neg Response - Busy Repeat Request"},				   
	{0x22,        "Neg Response - Conditions Not Correct"},				   
	{0x24,        "Neg Response - Request Sequence Error"},				   
	{0x31,        "Neg Response - Request Out Of Range"},				   
	{0x33,        "Neg Response - Security Access Denied"},				   
	{0x35,        "Neg Response - Invalid Key"},				   
	{0x36,        "Neg Response - Exceed Number Of Attempts"},				   
	{0x37,        "Neg Response - Required Time Delay Not Expired"},				   
	{0x70,        "Neg Response - Upload Download NotAccepted"},				   
	{0x71,        "Neg Response - Transfer Data Suspended"},				   
	{0x72,        "Neg Response - General Programming Failure"},				   
	{0x73,        "Neg Response - Wrong Block Sequence Counter"},				   
	{0x78,		  "Neg Response - Response Pending" },
	{0x7E,        "Neg Response - SubFunction Not Supported In Active Session"},				   
	{0x7F,        "Neg Response - Service Not Supported In Active Session"},				   
	{0x81,        "Neg Response - Rpm Too High"},			
	{0x82,        "Neg Response - Rpm Too Low"},				   
	{0x83,        "Neg Response - Engine Is Running"},				   
	{0x84,        "Neg Response - Engine Is Not Running"},				   
	{0x85,        "Neg Response - Engine RunTime TooLow"},				   
	{0x86,        "Neg Response - Temperature Too High"},				   
	{0x87,        "Neg Response - Temperature Too Low"},				   
	{0x88,        "Neg Response - Vehicle Speed Too High"},				   
	{0x89,        "Neg Response - Vehicle Speed Too Low"},				   
	{0x8A,        "Neg Response - Throttle/Pedal Too High"},				   
	{0x8B,        "Neg Response - Throttle/Pedal Too Low"},				   
	{0x8C,        "Neg Response - Transmission Range In Neutral"},				   
	{0x8D,        "Neg Response - Transmission Range In Gear"},				   
	{0x8F,        "Neg Response - Brake Switch(es)NotClosed (Brake Pedal not pressed or not applied)"},				   
	{0x90,        "Neg Response - Shifter Lever Not In Park "},				   
	{0x91,        "Neg Response - Torque Converter Clutch Locked"},				   
	{0x92,        "Neg Response - Voltage Too High"},	
	{0x93,        "Neg Response - voltageTooLow"},				   
	{10,	   "Positive Response"},		//It has to be always declared here 
	{4,        "Negative Response"},
};	


CUDS_NegRespMng::CUDS_NegRespMng(CUDS_Protocol* pEngine)
{  
   UdsProtocolPtr2 = pEngine;
}

CUDS_NegRespMng::~CUDS_NegRespMng(void)
{
}

CString CUDS_NegRespMng::VerifyNegResponse(unsigned char value){

	for (int i_counter = 0; i_counter < SIZE_NEG_RESP_STRUCT ; i_counter++){
		if (NegativeResponseStruct[i_counter].MsgValue == value){
			return NegativeResponseStruct[i_counter].m_omTypeResp;
		}	
	}  
	return NegativeResponseStruct[SIZE_NEG_RESP_STRUCT-1].m_omTypeResp;
}

CString CUDS_NegRespMng::evaluateResp(unsigned char arrayMsg[],int Byte2Init ){			

	// Byte2Init indicates which is the first byte to read from the message	 to evaluate if it's a possitive or a negative Response 
	// Sometimes this value is Initialbyte but in te case of a long response is Initialbyte+1 

	unsigned char prueba = arrayMsg[Byte2Init+2];
	int TempValue = strtol(CurrentService, NULL, 16);
	if (arrayMsg[Byte2Init+1] == 0x7F) {
		if (arrayMsg[Byte2Init+2] ==TempValue /*CurrentService*/){
			// Kill the timer   and able SendButton
			UdsProtocolPtr2->KillTimer_Able_Send();

			if( arrayMsg[Byte2Init+3] == 0x78){
				FSending = TRUE;          // Continue showing the bytes 
				UdsProtocolPtr2->StartTimer_Disable_Send();  // Start the timer again and disable SEND button 
				Lear = NUM_BYTES_SHOWN_RESP-2;
			}
			return VerifyNegResponse(arrayMsg[Byte2Init+3]);			 // arrayMsg[Byte2Init+3] will indicate wich message'll be shown in the RespDescription section 

			// If its equal to 0x78 restart everything
		}else{
			FDontShow = TRUE;		// It's a negResponse but not for my Msg
		}   
		// If it isn't the current Service I shouldn't do anything 
	} else if(arrayMsg[Byte2Init+1] ==  TempValue+0x40){
		UdsProtocolPtr2->KillTimer_Able_Send();
		return NegativeResponseStruct[SIZE_NEG_RESP_STRUCT-2].m_omTypeResp;	  // The Positive resp message will always be at the end of the list 
		//Write Possitive Response in the RespDescription  section
	}  else	 FDontShow = TRUE;		// If it isn't a NegResp/PosResptomyMsg
									// Then it shouldn't be shown

	return VerifyNegResponse(arrayMsg[Byte2Init+3]); 
}

