#include "DIL_Interface_stdafx.h"
#include "Include/BaseDefs.h"
//#include "UDS_Protocol/UDS_Extern.h"
//#include "UDS_Protocol/Uds_Protocol.h"
#include "DILI_UDS_Protocol.h"

//__declspec(dllimport) HRESULT DIL_UDS_ShowWnd(void);

/**
 * Constructor
 */
DILI_UDS_Protocol::DILI_UDS_Protocol(void)
{
}

/**
 * Destructor
 */
DILI_UDS_Protocol::~DILI_UDS_Protocol(void)
{
}

BOOL DILI_UDS_Protocol::InitInstance(void)
{
    return TRUE;
}


HRESULT DILI_UDS_Protocol::DisplayUdsMainWnd(HWND hParent)     //Esta funci�n llamar� a una funci�n definida dentro del proyecto UDS_Protocol
{	
	HRESULT hResult = S_OK;
	return hResult;
	//return DIL_UDS_ShowWnd(hParent);		//Hace Falta colocar par�meteros


	//! /Aqui tengo que retornar otra funci�n que estar� definida en el .cpp UDS_Manager project
//Check-- La funci�n que se retorne aqu� debe estar declarada en el Extern del proyecto
	//La funci�n que se retorne aqui no ser� declarada en el DILI_.... .h
	//La funci�n retornada no debe estar declarada tambien en el Manager.h del proyecto
	
}