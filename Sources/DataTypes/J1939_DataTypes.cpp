/******************************************************************************
  Project       :  Auto-SAT_Tools
  FileName      :  DataTypes_J1939.cpp
  Description   :
  $Log:   X:/Archive/Sources/DataTypes/J1939_DataTypes.cpv  $

      Rev 1.5   15 Apr 2011 18:55:06   rac2kor
   Inserted RBEI Copyright information text into the file header.

      Rev 1.4   04 Mar 2011 17:30:38   CANMNTTM
   Updated to solve the issues in J1939 MsgWnd:
   1. Sorting issues in J1939 Msg Wnd.
   2. Problem with J1939 messages with DLC > 150.

      Rev 1.3   18 Feb 2011 12:06:46   CANMNTTM
   Updated to suite multiple message windows for buses.

      Rev 1.2   14 Dec 2010 17:52:16   CANMNTTM
   Added a member function vSetPGN for STRUCT_29_BIT_ID.


      Rev 1.1   13 Dec 2010 21:21:46   rac2kor
   1. Added a data member for message type in SFORMATTEDATA_J1939
   2. Added a helper function to get the associated text for a type.

      Rev 1.0   06 Dec 2010 18:34:32   rac2kor


  Author(s)     :  Pradeep Kadoor
  Date Created  :  23/11/2010
  Modified By   :
  Copyright (c) 2011, Robert Bosch Engineering and Business Solutions.  All rights reserved.
******************************************************************************/
#include "Datatypes_stdafx.h"
#include "Include/BaseDefs.h"
#include "Include/Utils_Macro.h"
#include "J1939_DataTypes.h"
#include "application/hashdefines.h"
#include "application/Common.h"

int STJ1939_MSG::m_nSortField = 0;
int STJ1939_MSG::m_nMFactor = 1;

/* strcut STRUCT_29_BIT_ID*/
UINT32 tagSTRUCT_29_BIT_ID::unGetPGN(void)
{
    UINT32 unPGN = m_uPGN.m_unPGN & 0x3FFFF;
    if (m_uPGN.m_sPGN.m_byPDU_Format < 240)
    {
        unPGN = m_uPGN.m_unPGN & 0x3FF00;
    }
    return unPGN;
}

void tagSTRUCT_29_BIT_ID::vSetPGN(UINT32 unPGN)
{
    UNION_PGN uPGN;
    uPGN.m_unPGN = unPGN & 0x3FFFF; //Mask unecessary bits.
    m_uPGN.m_unPGN = m_uPGN.m_unPGN & 0xFFFC0000;   //Mask PGN bits

    BYTE byPduFormat = (BYTE)(unPGN >> 8);
    if  (byPduFormat < 240)
    {
        m_uPGN.m_unPGN |= (unPGN & 0x3FF00);
    }
    else
    {
        m_uPGN.m_unPGN |= unPGN;
    }
}

/* Constructor*/
tagSTJ1939_MSG::tagSTJ1939_MSG()
{
    memset(&m_sMsgProperties, 0, sizeof(STJ1939_MSG_PROPERTIES));
    m_unDLC = 0;
    m_pbyData = NULL;
}

/* Destructor */
tagSTJ1939_MSG::~tagSTJ1939_MSG()
{
    memset(&m_sMsgProperties, 0, sizeof(STJ1939_MSG_PROPERTIES));
    m_unDLC = 0;
    DELETE_ARRAY(m_pbyData);
}

/* returns the total size of the message.*/
UINT tagSTJ1939_MSG::unGetSize(void) const
{
    return (sizeof(STJ1939_MSG_PROPERTIES) + sizeof (UINT) + (sizeof (BYTE) * m_unDLC) );
}
/* Renders the BYTE stream of the total message. */
void tagSTJ1939_MSG::vGetDataStream(BYTE* pbyData) const
{
    COPY_DATA(pbyData, &m_sMsgProperties, sizeof(STJ1939_MSG_PROPERTIES));
    COPY_DATA(pbyData, &m_unDLC, sizeof (UINT));
    COPY_DATA(pbyData, m_pbyData, (sizeof (BYTE) * m_unDLC));
}

void tagSTJ1939_MSG::vSetDataStream(BYTE* pbyData)
{
    COPY_DATA_2(&m_sMsgProperties, pbyData,  sizeof(STJ1939_MSG_PROPERTIES));
    UINT unTempDLC = 0;
    COPY_DATA_2(&unTempDLC, pbyData, sizeof (UINT));
    if (/*unTempDLC > m_unDLC*/ m_pbyData == NULL)
    {
        //DELETE_ARRAY(m_pbyData);
        m_pbyData = new BYTE[MAX_DATA_LEN_J1939];
    }
    m_unDLC = unTempDLC;
    COPY_DATA_2(m_pbyData, pbyData, (sizeof (BYTE) * m_unDLC));
}
void tagSTJ1939_MSG::vInitialize(int nSize)
{
    if (m_unDLC < (UINT)nSize)
    {
        DELETE_ARRAY(m_pbyData);
        m_pbyData = new BYTE[nSize];
    }
    m_unDLC = nSize;
}

tagSTJ1939_MSG& tagSTJ1939_MSG::operator=(const tagSTJ1939_MSG& RefObj)
{
    m_sMsgProperties = RefObj.m_sMsgProperties;
    if (m_unDLC < RefObj.m_unDLC)
    {
        delete[] m_pbyData;
        m_pbyData = new BYTE[RefObj.m_unDLC];
    }
    memcpy(m_pbyData, RefObj.m_pbyData, RefObj.m_unDLC);
    m_unDLC = RefObj.m_unDLC;
    return *this;
}

void tagSTJ1939_MSG::vSetSortField(int nField)
{
    m_nSortField = nField;
}

void tagSTJ1939_MSG::vSetSortAscending(bool bAscending)
{
    m_nMFactor = bAscending ? 1 : -1;
};

int tagSTJ1939_MSG::DoCompareIndiv(const void* pEntry1, const void* pEntry2)
{
    int Result = 0;

    STJ1939_MSG* pJ1939Msg1 = (STJ1939_MSG*) pEntry1;
    STJ1939_MSG* pJ1939Msg2 = (STJ1939_MSG*) pEntry2;

    switch (m_nSortField)
    {
        case 8: // Sort by DEST
        {
            Result = (UINT) (pJ1939Msg1->m_sMsgProperties.m_uExtendedID.m_s29BitId.m_uPGN.m_sPGN.m_byPDU_Specific -
                             pJ1939Msg2->m_sMsgProperties.m_uExtendedID.m_s29BitId.m_uPGN.m_sPGN.m_byPDU_Specific);
            Result *= m_nMFactor;
            if (Result != 0)
            {
                break;
            }
        }
        case 7: // Sort by SRC
        {
            Result = (UINT) (pJ1939Msg1->m_sMsgProperties.m_uExtendedID.m_s29BitId.m_bySrcAddress -
                             pJ1939Msg2->m_sMsgProperties.m_uExtendedID.m_s29BitId.m_bySrcAddress);
            Result *= m_nMFactor;
            if (Result != 0)
            {
                break;
            }
        }
        case 5: // Sort by message name
        {
            CString str1, str2;
            AfxGetMainWnd()->SendMessage(WM_GET_PGN_NAME_FROM_CODE, (WPARAM)pJ1939Msg1->m_sMsgProperties.m_uExtendedID.m_s29BitId.unGetPGN(), (LPARAM)&str1);
            AfxGetMainWnd()->SendMessage(WM_GET_PGN_NAME_FROM_CODE, (WPARAM)pJ1939Msg2->m_sMsgProperties.m_uExtendedID.m_s29BitId.unGetPGN(), (LPARAM)&str2);

            Result = (int) (str1.CompareNoCase(str2));
            Result *= m_nMFactor;

            if (Result != 0)
            {
                break;
            }
        }
        case 4: // Sort by PGN
        {
            Result = (UINT) (pJ1939Msg1->m_sMsgProperties.m_uExtendedID.m_s29BitId.unGetPGN() -
                             pJ1939Msg2->m_sMsgProperties.m_uExtendedID.m_s29BitId.unGetPGN());
            Result *= m_nMFactor;
            if (Result != 0)
            {
                break;
            }
        }
        case 3: // Sort by CANID
        {
            Result = (UINT) (pJ1939Msg1->m_sMsgProperties.m_uExtendedID.m_unExtID -
                             pJ1939Msg2->m_sMsgProperties.m_uExtendedID.m_unExtID);
            Result *= m_nMFactor;
            if (Result != 0)
            {
                break;
            }
        }
        case 2: // Sort by channel
        {
            Result = (int) (pJ1939Msg1->m_sMsgProperties.m_byChannel - pJ1939Msg2->m_sMsgProperties.m_byChannel);
            Result *= m_nMFactor;
            if (Result != 0)
            {
                break;
            }
        }
        case 1: // Sort by time stamp
        {
            Result = (int) (pJ1939Msg1->m_sMsgProperties.m_un64TimeStamp -
                            pJ1939Msg2->m_sMsgProperties.m_un64TimeStamp);
            Result *= m_nMFactor;
        }
        break;
        default:
        {
            ASSERT(FALSE);
        }
        break;
    }
    return Result;
}

__int64 tagSTJ1939_MSG::GetSlotID(STJ1939_MSG& pouJ1939Msg)
{
    // Form message to get message index in the CMap
    UINT nMsgID = MAKE_RX_TX_MESSAGE(
                      pouJ1939Msg.m_sMsgProperties.m_uExtendedID.m_s29BitId.unGetPGN(),
                      IS_RX_MESSAGE(pouJ1939Msg.m_sMsgProperties.m_eDirection));

    // For extended message
    nMsgID = MAKE_EXTENDED_MESSAGE_TYPE(nMsgID);
    // Apply Channel Information
    __int64 n64MapIndex = MAKE_CHANNEL_SPECIFIC_MESSAGE( nMsgID,
                          pouJ1939Msg.m_sMsgProperties.m_byChannel);

    n64MapIndex = MAKE_SOURCE_SPECIFIC_MESSAGE( n64MapIndex,
                  pouJ1939Msg.m_sMsgProperties.m_uExtendedID.m_s29BitId.m_bySrcAddress);

    n64MapIndex = MAKE_DEST_SPECIFIC_MESSAGE( n64MapIndex,
                  pouJ1939Msg.m_sMsgProperties.m_uExtendedID.m_s29BitId.m_uPGN.m_sPGN.m_byPDU_Specific);

    n64MapIndex = MAKE_TYPE_SPECIFIC_MESSAGE( n64MapIndex,
                  pouJ1939Msg.m_sMsgProperties.m_eType);

    return n64MapIndex;
};


void tagSTJ1939_MSG::vClear(void)
{
    m_sMsgProperties.m_eType = MSG_TYPE_NONE;
    m_unDLC = 0;
    DELETE_ARRAY(m_pbyData);
}

/* Starts tagFormattedData_J1939 related codes */
tagFormattedData_J1939::tagFormattedData_J1939()
{
    _tcsnset(m_acTimeSys,   L'\0', LEN_STR_TIMESTAMP_J1939);
    _tcsnset(m_acTimeRel,   L'\0', LEN_STR_TIMESTAMP_J1939);
    _tcsnset(m_acTimeAbs,   L'\0', LEN_STR_TIMESTAMP_J1939);

    _tcsnset(m_acMsgType,   L'\0', LEN_STR_TYPE_J1939     );
    _tcsnset(m_acChannel,   L'\0', LEN_STR_CHANNEL_J1939  );

    _tcsnset(m_acPGNHex,    L'\0', LEN_STR_PGN_J1939      );
    _tcsnset(m_acPGNDec,    L'\0', LEN_STR_PGN_J1939      );
    _tcsnset(m_acMsgName,   L'\0', LEN_STR_NAME_J1939     );
    _tcsnset(m_acSenderName,L'\0', LEN_STR_SENDNODE_J1939 );

    _tcsnset(m_acSrcHex,    L'\0', LEN_STR_SRC_J1939      );
    _tcsnset(m_acSrcDec,    L'\0', LEN_STR_SRC_J1939      );
    _tcsnset(m_acDestHex,   L'\0', LEN_STR_DEST_J1939     );
    _tcsnset(m_acDestDec,   L'\0', LEN_STR_DEST_J1939     );


    _tcsnset(m_acPriority,  L'\0', LEN_STR_PRIO_J1939     );
    _tcsnset(m_acMsgDir,    L'\0', LEN_STR_DIR_J1939      );
    _tcsnset(m_acDataLen,   L'\0', LEN_STR_DLC_J1939      );

    m_acMsgDir[LEN_STR_DIR_J1939 - 2] = L'x'; // It will be either Tx or Rx

    m_pcDataHex = NULL;
    m_pcDataDec = NULL;
}

tagFormattedData_J1939::~tagFormattedData_J1939()
{
    DELETE_ARRAY(m_pcDataHex);
    DELETE_ARRAY(m_pcDataDec);
}
/* Ends tagFormattedData_J1939 related codes */


// Helper functions start
void GetMessageTypeStr(EJ1939_MSG_TYPE eType, char acResult[LEN_STR_TYPE_J1939])
{
    switch (eType)
    {
        case MSG_TYPE_NONE:
            _tcscpy(acResult, "NONE");
            break;
        case MSG_TYPE_COMMAND:
            _tcscpy(acResult, "CMD");
            break;
        case MSG_TYPE_REQUEST:
            _tcscpy(acResult, "RQST");
            break;
        case MSG_TYPE_DATA:
            _tcscpy(acResult, "DATA");
            break;
        case MSG_TYPE_BROADCAST:
            _tcscpy(acResult, "BROADCAST");
            break;
        case MSG_TYPE_ACKNOWLEDGEMENT:
            _tcscpy(acResult, "ACK");
            break;
        case MSG_TYPE_GROUP_FUNCTIONS:
            _tcscpy(acResult, "GRP_FUNC");
            break;

        case MSG_TYPE_NM_ACL:
            _tcscpy(acResult, "ACL");
            break;
        case MSG_TYPE_NM_RQST_ACL:
            _tcscpy(acResult, "RQST_ACL");
            break;
        case MSG_TYPE_NM_CMD_ADDRESS:
            _tcscpy(acResult, "CA");
            break;
        case MSG_TYPE_NM_TPCM_BAM:
            _tcscpy(acResult, "BAM");
            break;
        case MSG_TYPE_NM_TPCM_RTS:
            _tcscpy(acResult, "RTS");
            break;
        case MSG_TYPE_NM_TPCM_CTS:
            _tcscpy(acResult, "CTS");
            break;
        case MSG_TYPE_NM_TPCM_EOM_ACK:
            _tcscpy(acResult, "EOM");
            break;
        case MSG_TYPE_NM_TPCM_CON_ABORT:
            _tcscpy(acResult, "CON_ABORT");
            break;
        case MSG_TYPE_NM_TPDT:
            _tcscpy(acResult, "TPDT");
            break;
        default:
            ASSERT(FALSE);
    }
}

// Helper functions end