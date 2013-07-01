

/**
 * \file      UDS_Protocol.h
 * \brief     Interface file for CUDS_Protocol class
 * \author    Sanchez A
 *
 * Interface file for CUDS_Protocol class
 */

#pragma once
#include "UDSMainWnd.h"
#include "DataTypes/UDS_DataTypes.h"
#include "include/struct_can.h"
#include "UDSWnd_Defines.h"
#include "UDS_Resource.h"


extern bool FWaitFlow;
extern int c_unPreviousTime;  //Deberia ir en otra clase para que sea más elegante
extern double c_dDiffTime;	//Deberia ir en otra clase para que sea más elegante
extern bool FWaitLongRespBSize;		// para indicar que debo esperar por el Flow control
extern int Counter_BSize;

typedef struct mstCanDataSpl : public STCANDATA
{
	__int64      m_nDeltime;

	mstCanDataSpl()
	{
		m_nDeltime = 0;
	}
} mSTCANDATASPL;

class CUDS_Protocol : public CWinApp
{
public:

    /**
     * Constructor
     */
	CUDS_Protocol();

	void  FormPadre();

	UINT SourceAddress;

	UINT TargetAddress;

	INT TypeofControl;
	UINT MsgID;
	UDS_INTERFACE fInterface ;
	int numberOfBytes;	// This variable will be used to set the number of bytes at the beginning of the row of bytes in the responseData section


	// Overrides
public:
	virtual BOOL InitInstance();
	void CUDS_Protocol::UpdateFields();
	TYPE_OF_FRAME CUDS_Protocol::getFrameType(unsigned char FirstByte);
	void CUDS_Protocol::StartTimer_Disable_Send();
	void CUDS_Protocol::KillTimer_Able_Send()  ;


    /**
	 * This function is called by EvaluateMessage to show in the Response Data section 
	 *	he bytes received in the bus as response of a request sent from the UDS tool.
     */

	void CUDS_Protocol::Show_ResponseData(unsigned char psMsg[], unsigned char Datalen , int posFirstByte);


	DECLARE_MESSAGE_MAP()
};