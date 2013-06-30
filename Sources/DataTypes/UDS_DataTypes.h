
#include "Utility\MultiLanguageSupport.h"
#include "include/struct_can.h"
#pragma once
#include <locale.h>


typedef enum UDS_INTERFACE
{	
	INTERFACE_NORMAL_11,
    INTERFACE_EXTENDED_11,
	INTERFACE_NORMAL_ISO_29,
};

typedef enum DIAGNOSTIC_STANDARD
{	
	STANDARD_UDS,
    STANDARD_KWP2000,
};

typedef enum TYPE_OF_FRAME
{	
	LONG_RESPONSE,
    CONSECUTIVE_FRAME,
	SIMPLE_RESPONSE,
	FLOW_CONTROL,
};