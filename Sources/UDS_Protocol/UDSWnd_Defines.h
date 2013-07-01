#pragma once

#include "include/struct_can.h"

const int NO_OF_CHAR_IN_BYTE = 2; 

struct msTXMSGDATA
{
	UINT  m_unCount;              // Total array element in the point m_psTxMsg
	PSTCAN_MSG m_psTxMsg;         // pointer to array of structure for
								  // Transmitting  the data.
};

typedef msTXMSGDATA mSTXSELMSGDATA;
typedef msTXMSGDATA* mPSTXSELMSGDATA;

#define defDIV_FACT_FOR_SECOND              10000.0
#define defFORMAT_MSGID_DECIMAL "%d"
#define TIME_UNABLE_SEND_BUTTON				10				//Fijado por ahora
#define MASK_SA_ID_29Bits					0x3FF800	    
#define MASK_TA_ID_29Bits					0x7FF	    //Fijado por ahora
#define NEG_MASK_SA_ID_29Bits				0x1FC007FF	    //It's used tochange the SA from the Main Wnd
#define NEG_MASK_TA_ID_29Bits				0x1FFFF800	    //It's used tochange the TA from the Main Wnd
//#define NUM_BYTES_SHOWN_RESP                16
