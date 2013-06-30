/*
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

/**
 * \file      Filter_Datatypes.h
 * \brief     Definition file for filter data types.
 * \author    Ratnadip Choudhury
 * \copyright Copyright (c) 2011, Robert Bosch Engineering and Business Solutions. All rights reserved.
 *
 * Definition file for filter data types.
 */

#if !defined FILTER_DATATYPES_H__INCLUDED_
#define FILTER_DATATYPES_H__INCLUDED_
#include "Utility/XMLUtils.h"
#include "include/BaseDefs.h"
#include "include/Utils_macro.h"
#include "include/XMLDefines.h"


#define defFILTER_TYPE_SINGLE_ID            0
#define defFILTER_TYPE_ID_RANGE             1

// Basic information needed for filtering when the bus is FlexRay.
typedef struct tagSFRAMEINFO_BASIC_FLEXRAY
{
    DWORD        m_dwFrameID;
    TYPE_CHANNEL m_eChannel;
    EDIRECTION   m_eDrctn;
} SFRAMEINFO_BASIC_FLEXRAY;

// Basic information needed for filtering when the bus is CAN.
typedef struct tagSFRAMEINFO_BASIC_CAN
{
    DWORD        m_dwFrameID;
    TYPE_CHANNEL m_eChannel;
    EDIRECTION   m_eDrctn;
    BYTE         m_byIDType;
    BYTE         m_byMsgType;
} SFRAMEINFO_BASIC_CAN;

// Basic information needed for filtering when the bus is MCNet.
typedef struct tagSFRAMEINFO_BASIC_MCNET
{
    DWORD m_dwFrameID;
} SFRAMEINFO_BASIC_MCNET;

// Basic information needed for filtering when the bus is J1939.
typedef struct tagSFRAMEINFO_BASIC_J1939
{
    DWORD m_dwPGN;
    TYPE_CHANNEL m_eChannel; //KSS
    EDIRECTION   m_eDrctn;
} SFRAMEINFO_BASIC_J1939;

const int LENGTH_FILTERNAME = 128;

// The descriobes a filter name with type.
typedef struct tagFilterName
{
    char m_acFilterName[LENGTH_FILTERNAME];                     // Filter name
    BOOL  m_bFilterType;                    // Filter Type 0 - Stop, 1 for Pass

    tagFilterName();    // Standard constructor

    // To copy the source object. '=' operator overloaded.
    tagFilterName& operator=(const tagFilterName& RefObj);

    // To clear the current object.
    void vClear(void);

    // Called to get the filter object's data into a stream buffer.
    BYTE* pbGetConfigData(BYTE* pbTarget) const;

    // XML PTV
    void pbGetConfigData(xmlNodePtr pxmlNodePtr) const;
    void pbSetConfigData(xmlNodePtr pNodePtr, xmlDocPtr pDocPtr);
    // XML PTV
    // Called to retrieve a filter object's data from a byte stream and
    // initialise the current filter object with the retrieved data.
    BYTE* pbSetConfigData(BYTE* pbTarget);

    INT nSetXMLConfigData(xmlNodePtr pFilter);
    BOOL nFilterType(string strFilteType);

} SFILTERNAME, *PSFILTERNAME;

// Base class for an individual filter data structure.
typedef struct tagSFILTER
{
    UCHAR m_ucFilterType;   // 0 - Message ID and 1 - ID Range
    DWORD m_dwMsgIDFrom;    // From Message ID in case of range.
    // Msg ID in case of Single ID
    DWORD m_dwMsgIDTo;      // To Message ID incase of range.
    EDIRECTION m_eDrctn;    // Values are: DIR_RX, DIR_TX and DIR_ALL

    tagSFILTER();           // Standard constructor

    // To assign values from another object. The '=' operator overloaded.
    tagSFILTER& operator=(const tagSFILTER& RefObj);

    // To clear the current object.
    void vClear(void);

    // Query function to know if this filtering block is configured for the
    // message entry passed.
    virtual BOOL bDoesFrameOccur(const void* psCurrFrame) const = 0;

    // To get size of the filter block object in bytes.
    virtual UINT unGetSize(void) const;

    // Called to get the filter object's data into a stream buffer.
    virtual BYTE* pbGetConfigData(BYTE* pbTarget) const;

    // PTV XML
    virtual void pbGetConfigData(xmlNodePtr pxmlNodePtr) const;
    // PTV XML
    // Called to retrieve a filter object's data from a byte stream and
    // initialise the current filter object with the retrieved data.
    virtual BYTE* pbSetConfigData(BYTE* pbTarget);

    INT nSetXMLConfigData(xmlNodePtr pNodePtr);
    eDirection eGetMsgDirection(string strDirection);
} SFILTER, *PSFILTER;

struct SFILTER_CAN : public SFILTER
{
    BYTE         m_byIDType; //Extended or Standard
    BYTE         m_byMsgType;//RTR or NON RTR
    TYPE_CHANNEL m_eChannel;

    SFILTER_CAN();  // Standard constructor
    ~SFILTER_CAN(); // Destructor

    // To assign values from another object. The '=' operator overloaded.
    SFILTER_CAN& operator=(const SFILTER_CAN& RefObj);

    // To clear the current object.
    void vClear(void);

    // Query function to know if this filtering block is configured for the
    // message entry passed.
    BOOL bDoesFrameOccur(const void* psCurrFrame) const;

    // To get size of the filter block object in bytes.
    UINT unGetSize(void) const;

    // Called to get the filter object's data into a stream buffer.
    BYTE* pbGetConfigData(BYTE* pbTarget) const;

    // PTV XML
    void pbGetConfigData(xmlNodePtr pNodePtr) const;
    void pbSetConfigData(xmlNodePtr pNodePtr);
    // PTV XML
    // Called to retrieve a filter object's data from a byte stream and
    // initialise the current filter object with the retrieved data.
    BYTE* pbSetConfigData(BYTE* pbTarget);
    //MVN
    INT nSetXMLConfigData(xmlNodePtr pNodePtr);
    INT nGetIDType(string strIDType);
    INT nGetMsgType(string strMsgType);
    //~MVN
};
typedef SFILTER_CAN* PSFILTER_CAN;

// The below structure describes a filter block for FlexRay frames.
struct SFILTER_FLEXRAY : public SFILTER
{
    TYPE_CHANNEL m_eChannel;// Current channel

    SFILTER_FLEXRAY();  // Standard constructor
    ~SFILTER_FLEXRAY(); // Destructor

    // To assign values from another object. The '=' operator overloaded.
    SFILTER_FLEXRAY& operator=(const SFILTER_FLEXRAY& RefObj);

    // To clear the current object.
    void vClear(void);

    // Query function to know if this filtering block is configured for the
    // message entry passed.
    BOOL bDoesFrameOccur(const void* psCurrFrame) const;

    // To get size of the filter block object in bytes.
    UINT unGetSize(void) const;

    // Called to get the filter object's data into a stream buffer.
    BYTE* pbGetConfigData(BYTE* pbTarget) const;
    // XML PTV
    void pbGetConfigData(xmlNodePtr pNodePtr) const;
    // XML PTV

    // Called to retrieve a filter object's data from a byte stream and
    // initialise the current filter object with the retrieved data.
    BYTE* pbSetConfigData(BYTE* pbTarget);
};
typedef SFILTER_FLEXRAY* PSFILTER_FLEXRAY;

// The below structure describes a filter block for MCNet frames.
struct SFILTER_MCNET : public SFILTER
{
    SFILTER_MCNET();    // Standard constructor
    ~SFILTER_MCNET();   // Destructor

    // To assign values from another object. The '=' operator overloaded.
    SFILTER_MCNET& operator=(const SFILTER_MCNET& RefObj);

    // To clear the current object.
    void vClear(void);

    // Query function to know if this filtering block is configured for the
    // message entry passed.
    BOOL bDoesFrameOccur(const void* psCurrFrame) const;

    // To get size of the filter block object in bytes.
    UINT unGetSize(void) const;

    // Called to get the filter object's data into a stream buffer.
    BYTE* pbGetConfigData(BYTE* pbTarget) const;

    // PTV XML
    void pbGetConfigData(xmlNodePtr pxmlNodePtr) const;
    // PTV XML
    // Called to retrieve a filter object's data from a byte stream and
    // initialise the current filter object with the retrieved data.
    BYTE* pbSetConfigData(BYTE* pbTarget);
};
typedef SFILTER_MCNET* PSFILTER_MCNET;

// The below structure describes a filter block for J1939 frames.
struct SFILTER_J1939 : public SFILTER
{
    SFILTER_J1939();    // Standard constructor
    ~SFILTER_J1939();   // Destructor

    // To assign values from another object. The '=' operator overloaded.
    SFILTER_J1939& operator=(const SFILTER_J1939& RefObj);

    // To clear the current object.
    void vClear(void);

    // Query function to know if this filtering block is configured for the
    // message entry passed.
    BOOL bDoesFrameOccur(const void* psCurrFrame) const;

    // To get size of the filter block object in bytes.
    UINT unGetSize(void) const;

    // Called to get the filter object's data into a stream buffer.
    BYTE* pbGetConfigData(BYTE* pbTarget) const;
    // PTV XML
    void pbGetConfigData(xmlNodePtr pNodePtr) const;
    // PTV XML

    // Called to retrieve a filter object's data from a byte stream and
    // initialise the current filter object with the retrieved data.
    BYTE* pbSetConfigData(BYTE* pbTarget);
};
typedef SFILTER_J1939* PSFILTER_J1939;

// This below structure defines a filtering block.
typedef struct tagFilterSet
{
    SFILTERNAME m_sFilterName;  // Filter name and type
    BOOL        m_bEnabled;     // Enable status of the filter set
    ETYPE_BUS   m_eCurrBus;     // The type of bus
    USHORT      m_ushFilters;   // Number of filters
    PSFILTER    m_psFilterInfo; // Filter informations

    tagFilterSet();             // Standard constructor
    ~tagFilterSet();            // Destructor

    // To clear the current filter set.
    void vClear(void);

    // To clone a source filter set.
    bool bClone(const tagFilterSet& RefObj);

    // To get size of the filter block set in bytes.
    UINT unGetSize(void) const;

    // Called to get the filter block set's data into a stream buffer.
    BYTE* pbGetConfigData(BYTE* pbTarget) const;

    // XML PTV
    void pbGetConfigData(xmlNodePtr pNodePtr) const;

    void pbSetConfigData(xmlNodePtr pNodePtr, xmlDocPtr pDocPtr, bool& Result);
    // XML PTV
    // Called to retrieve a filter block set's data from a byte stream and
    // initialise the current filter object with the retrieved data.
    BYTE* pbSetConfigData(BYTE* pbTarget, bool& Result);
    int nSetXMLConfigData( ETYPE_BUS eBus, xmlNodePtr pFilter);

    static tagFilterSet* psGetFilterSetPointer(tagFilterSet* psSet, UINT Count, char* acFilterName);

} SFILTERSET, *PSFILTERSET;

// This structure defines a set of filters along with the sufficient entities
// to apply this for filtering process. So the necessary member functions.
template <typename SFRAMEINFO_BASIC_BUS>
struct SFILTERAPPLIED
{
    BOOL                m_bEnabled;         // Enable flag of current filter
    USHORT              m_ushTotal;         // Total number of filter blocks.
    PSFILTERSET         m_psFilters;        // The filter set dynamic array.

    SFILTERAPPLIED();       // Standard constructor
    ~SFILTERAPPLIED();      // Destructor

    // To clone a source filter object.
    bool bClone(const SFILTERAPPLIED<SFRAMEINFO_BASIC_BUS>& Source);

    // To clear the filter object
    void vClear(void);

    // Query function that tells if the filter object will block the frame.
    BOOL bToBeBlocked(const SFRAMEINFO_BASIC_BUS& sCurrFrame) const;

    // To get the size (in bytes) of the object.
    UINT unGetSize(void) const;

    // Called to get the filter object's data into a stream buffer.
    BYTE* pbGetConfigData(BYTE* pbTarget) const;

    // PTV XML
    void pbGetConfigData(xmlNodePtr pNodePtr) const;
    void pbGetConfigFilterData(xmlNodePtr pNodePtr) const;
    // PTV XML
    // Called to retrieve a filter object's data from a byte stream and
    // initialise the current filter object with the retrieved data.
    BYTE* pbSetConfigData(BYTE* pbTarget, bool& Result);

    // PTV XML
    void pbSetConfigData(SFILTERAPPLIED& pFilterAppliedCAN, xmlNodeSetPtr pNodeSet,xmlDocPtr pdocptr, bool& Result);
    // PTV XML

    int nGetFiltersFromName(PSFILTERSET& sFilterDest, CStringArray& omStrNames);

    //MVN
    int nSetXMLConfigData(xmlDocPtr& pDocPtr);
    int nGetFiltersFromName(SFILTERAPPLIED& sFilterDest, CStringArray& omStrNames);
    int GetFilterNameIndex(string strName);
    //~MVN
};

/******************************************************************************
  Function Name    :  SFILTERAPPLIED
  Input(s)         :  -
  Output           :  -
  Functionality    :  Standard constructor
  Member of        :  SFILTERAPPLIED
  Friend of        :  -
  Author(s)        :  Ratnadip Choudhury
  Date Created     :  1.12.2009
  Modification date:
  Modification By  :
******************************************************************************/
template <typename SFRAMEINFO_BASIC_BUS>
SFILTERAPPLIED<SFRAMEINFO_BASIC_BUS>::SFILTERAPPLIED()
{
    m_ushTotal = 0;
    m_bEnabled = FALSE;
    m_psFilters = NULL;
}

/******************************************************************************
  Function Name    :  ~SFILTERAPPLIED
  Input(s)         :  -
  Output           :  -
  Functionality    :  Destructor
  Member of        :  SFILTERAPPLIED
  Friend of        :  -
  Author(s)        :  Ratnadip Choudhury
  Date Created     :  1.12.2009
  Modification date:
  Modification By  :
******************************************************************************/
template <typename SFRAMEINFO_BASIC_BUS>
SFILTERAPPLIED<SFRAMEINFO_BASIC_BUS>::~SFILTERAPPLIED()
{
    vClear();
}

/******************************************************************************
  Function Name    :  bClone
  Input(s)         :  Source - The source object.
  Output           :  true if cloning is successful, else false.
  Functionality    :  Called to clone a filter object to copy its contents
                      into the current object.
  Member of        :  SFILTERAPPLIED
  Friend of        :  -
  Author(s)        :  Ratnadip Choudhury
  Date Created     :  1.12.2009
  Modification date:
  Modification By  :
******************************************************************************/
template <typename SFRAMEINFO_BASIC_BUS>
bool SFILTERAPPLIED<SFRAMEINFO_BASIC_BUS>::bClone(
    const SFILTERAPPLIED<SFRAMEINFO_BASIC_BUS>& Source)
{
    // Overloading of '=' operator isn't done because in case of unavailability
    // of heap (although highly unlikely), this copying operation will fail.

    vClear();

    bool bResult = false;

    if (Source.m_ushTotal > 0)
    {
        m_psFilters = new SFILTERSET[Source.m_ushTotal];
        if (NULL != m_psFilters)
        {
            bResult = true;
            for (int i = 0; (i < Source.m_ushTotal) && bResult; i++)
            {
                bResult = m_psFilters[i].bClone(Source.m_psFilters[i]);
            }
            if (!bResult) // If the cloning operation above is unsuccessful,
            {
                // clear all the buffer and the current object.
                vClear();
            }
        }
    }
    else
    {
        bResult = true;
    }

    if (bResult)
    {
        m_bEnabled = Source.m_bEnabled;
        m_ushTotal = Source.m_ushTotal;
    }

    return bResult;
}

/******************************************************************************
  Function Name    :  vClear
  Input(s)         :  void
  Output           :  void
  Functionality    :  Called to clear the current object.
  Member of        :  SFILTERAPPLIED
  Friend of        :  -
  Author(s)        :  Ratnadip Choudhury
  Date Created     :  1.12.2009
  Modification date:
  Modification By  :
******************************************************************************/
template <typename SFRAMEINFO_BASIC_BUS>
void SFILTERAPPLIED<SFRAMEINFO_BASIC_BUS>::vClear(void)
{
    m_ushTotal = 0;
    m_bEnabled = FALSE;

    if (NULL != m_psFilters)
    {
        delete[] m_psFilters;
        m_psFilters = NULL;
    }
}

/******************************************************************************
  Function Name    :  bToBeBlocked
  Input(s)         :  sCurrFrame - The current frame entry
  Output           :  TRUE if this is to be blocked.
  Functionality    :  This function tells if the filter object will block the
                      frame entry passed as argument.
  Member of        :  SFILTERAPPLIED
  Friend of        :  -
  Author(s)        :  Ratnadip Choudhury
  Date Created     :  1.12.2009
  Modification date:
  Modification By  :
******************************************************************************/
template <typename SFRAMEINFO_BASIC_BUS>
BOOL SFILTERAPPLIED<SFRAMEINFO_BASIC_BUS>::bToBeBlocked(const SFRAMEINFO_BASIC_BUS& sCurrFrame) const
{
    // If filtering is not enabled, no filtering out takes place.
    if (m_bEnabled == FALSE)
    {
        return FALSE;
    }

    /* Truth table for Filter Type, occurrence of the frame in the filtering
    block and decision on final filtering action */
    /* Pass    Frame found   To block
         T          T           F
         F          T           T
         T          F           T
         F          F           F   */

    BOOL bToBlock = FALSE;

    BOOL bToContinue = TRUE;
    for (USHORT i = 0; (i < m_ushTotal) && bToContinue; i++)
    {
        PSFILTERSET psCurrFilterBlk = m_psFilters + i;

        if (FALSE == psCurrFilterBlk->m_bEnabled)
        {
            continue;
        }

        bToBlock = psCurrFilterBlk->m_sFilterName.m_bFilterType;
        USHORT ushFilters = psCurrFilterBlk->m_ushFilters;

        for (USHORT j = 0; (j < ushFilters) && bToContinue; j++)
        {
            switch (psCurrFilterBlk->m_eCurrBus)
            {
                case CAN:
                {
                    PSFILTER psCurrFilter = (((SFILTER_CAN*) psCurrFilterBlk->m_psFilterInfo) + j);
                    if (psCurrFilter->bDoesFrameOccur((void*) &sCurrFrame))
                    {
                        bToContinue = FALSE;
                        bToBlock = !bToBlock;
                    }
                }
                break;
            }
        }
    }
    return bToBlock;
}

/******************************************************************************
  Function Name    :  unGetSize
  Input(s)         :  void
  Output           :  UINT
  Functionality    :  Returns size of the object in bytes
  Member of        :  SFILTERAPPLIED
  Friend of        :  -
  Author(s)        :  Ratnadip Choudhury
  Date Created     :  1.12.2009
  Modification date:
  Modification By  :
******************************************************************************/
template <typename SFRAMEINFO_BASIC_BUS>
UINT SFILTERAPPLIED<SFRAMEINFO_BASIC_BUS>::unGetSize(void) const
{
    //version
    UINT Result = sizeof (BYTE) + sizeof(m_bEnabled) + sizeof(m_ushTotal);

    for (USHORT i = 0; i < m_ushTotal; i++)
    {
        Result += (m_psFilters + i)->unGetSize();
    }

    return Result;
}

/******************************************************************************
  Function Name    :  pbGetConfigData
  Input(s)         :  pbSource - Target stream to copy data into.
  Output           :  Address of the next available byte to write data.
  Functionality    :  This copies current object data into the target byte stream
  Member of        :  SFILTERAPPLIED
  Friend of        :  -
  Author(s)        :  Ratnadip Choudhury
  Date Created     :  1.12.2009
  Modification date:
  Modification By  :
******************************************************************************/
template <typename SFRAMEINFO_BASIC_BUS>
BYTE* SFILTERAPPLIED<SFRAMEINFO_BASIC_BUS>::pbGetConfigData(BYTE* pbTarget) const
{
    BYTE* pbTStream = pbTarget;

    BYTE byVersion = 0x1;

    COPY_DATA(pbTStream, &byVersion, sizeof(BYTE));
    COPY_DATA(pbTStream, &m_bEnabled, sizeof(m_bEnabled));
    COPY_DATA(pbTStream, &m_ushTotal, sizeof(m_ushTotal));
    for (USHORT i = 0; i < m_ushTotal; i++)
    {
        pbTStream = m_psFilters[i].pbGetConfigData(pbTStream);
    }

    return pbTStream;
}

template <typename SFRAMEINFO_BASIC_BUS>
void SFILTERAPPLIED<SFRAMEINFO_BASIC_BUS>::pbGetConfigFilterData(xmlNodePtr pNodePtr) const
{
    BYTE byVersion = 0x1;

    for (USHORT i = 0; i < m_ushTotal; i++)
    {
        CString strFilterName = m_psFilters[i].m_sFilterName.m_acFilterName;

        xmlNodePtr pFilterPtr = xmlNewChild(pNodePtr, NULL, BAD_CAST DEF_FILTER, BAD_CAST strFilterName.GetBufferSetLength(strFilterName.GetLength()));
        xmlAddChild(pNodePtr, pFilterPtr);
    }

    //return pbTStream;
}
template <typename SFRAMEINFO_BASIC_BUS>
void SFILTERAPPLIED<SFRAMEINFO_BASIC_BUS>::pbGetConfigData(xmlNodePtr pNodePtr) const
{
    BYTE byVersion = 0x1;

    xmlNodePtr pFilterTag;

    xmlNodePtr pNodeCanFilterPtr = NULL;
    xmlNodePtr pNodeMCNEtFilterPtr = NULL;
    xmlNodePtr pNodeJ1939FilterPtr = NULL;
    xmlNodePtr pNodeFLEXRAYFilterPtr = NULL;
    xmlNodePtr pNodeLINFilterPtr = NULL;
    xmlNodePtr pNodeMOSTFilterPtr = NULL;

    for (USHORT i = 0; i < m_ushTotal; i++)
    {
        //// Adding Filter to the xml
        //xmlNodePtr pNodeFilterPtr = xmlNewNode(NULL, BAD_CAST  DEF_FILTER);
        //xmlAddChild(pNodePtr, pNodeFilterPtr);

        if(m_psFilters[i].m_eCurrBus == MCNET)
        {
            if(pNodeMCNEtFilterPtr == NULL)
            {
                pNodeMCNEtFilterPtr = xmlNewNode(NULL, BAD_CAST  DEF_MCNETFILTERS);
                xmlAddChild(pNodePtr, pNodeMCNEtFilterPtr);
            }

            m_psFilters[i].pbGetConfigData(pNodeMCNEtFilterPtr);
        }

        if(m_psFilters[i].m_eCurrBus == J1939)
        {
            if(pNodeJ1939FilterPtr == NULL)
            {
                pNodeJ1939FilterPtr = xmlNewNode(NULL, BAD_CAST  DEF_J1939FILTERS);
                xmlAddChild(pNodePtr, pNodeJ1939FilterPtr);
            }

            m_psFilters[i].pbGetConfigData(pNodeJ1939FilterPtr);
        }

        if(m_psFilters[i].m_eCurrBus == FLEXRAY)
        {
            if(pNodeFLEXRAYFilterPtr == NULL)
            {
                pNodeFLEXRAYFilterPtr = xmlNewNode(NULL, BAD_CAST  DEF_FLEXRAYFILTERS);
                xmlAddChild(pNodePtr, pNodeFLEXRAYFilterPtr);
            }

            m_psFilters[i].pbGetConfigData(pNodeFLEXRAYFilterPtr);
        }

        if(m_psFilters[i].m_eCurrBus == LIN)
        {
            if(pNodeLINFilterPtr == NULL)
            {
                pNodeLINFilterPtr = xmlNewNode(NULL, BAD_CAST  DEF_LINFILTERS);
                xmlAddChild(pNodePtr, pNodeLINFilterPtr);
            }

            m_psFilters[i].pbGetConfigData(pNodeLINFilterPtr);
        }

        if(m_psFilters[i].m_eCurrBus == MOST)
        {
            if(pNodeMOSTFilterPtr == NULL)
            {
                pNodeMOSTFilterPtr = xmlNewNode(NULL, BAD_CAST  DEF_MOSTFILTERS);
                xmlAddChild(pNodePtr, pNodeMOSTFilterPtr);
            }

            m_psFilters[i].pbGetConfigData(pNodeMOSTFilterPtr);
        }

        if(m_psFilters[i].m_eCurrBus == CAN)
        {
            if(pNodeCanFilterPtr == NULL)
            {
                pNodeCanFilterPtr = xmlNewNode(NULL, BAD_CAST  DEF_CANFILTERS);
                xmlAddChild(pNodePtr, pNodeCanFilterPtr);
            }

            m_psFilters[i].pbGetConfigData(pNodeCanFilterPtr);
        }
    }

    //return pbTStream;
}

/******************************************************************************
  Function Name    :  pbSetConfigData
  Input(s)         :  pbSource - Source stream to copy data from.
                      Result - [out] parameter. true if operation is successful.
  Output           :  Address of the byte next to the already read block.
  Functionality    :  This copies an applied filter block from a byte stream
                      and initialises the current object.
  Member of        :  SFILTERAPPLIED
  Friend of        :  -
  Author(s)        :  Ratnadip Choudhury
  Date Created     :  1.12.2009
  Modification date:
  Modification By  :
******************************************************************************/
template <typename SFRAMEINFO_BASIC_BUS>
BYTE* SFILTERAPPLIED<SFRAMEINFO_BASIC_BUS>::pbSetConfigData(BYTE* pbSource, bool& Result)
{
    vClear();

    Result = true;
    BYTE* pbSStream = pbSource;
    BYTE byVersion = 0;
    COPY_DATA_2(&byVersion, pbSStream, sizeof(BYTE));
    COPY_DATA_2(&m_bEnabled, pbSStream, sizeof(m_bEnabled));
    COPY_DATA_2(&m_ushTotal, pbSStream, sizeof(m_ushTotal));

    if (m_ushTotal > 0)
    {
        m_psFilters = new SFILTERSET[m_ushTotal];
        if (NULL != m_psFilters)
        {
            for (USHORT i = 0; (i < m_ushTotal) && Result; i++)
            {
                pbSStream = m_psFilters[i].pbSetConfigData(pbSStream, Result);
            }
        }
        else
        {
            Result = false;
        }
    }

    if (false == Result)
    {
        vClear();
    }

    return pbSStream;
}

//MVN

// PTV XML
template <typename SFRAMEINFO_BASIC_BUS>
void SFILTERAPPLIED<SFRAMEINFO_BASIC_BUS>::pbSetConfigData(SFILTERAPPLIED& pFilterAppliedCAN, xmlNodeSetPtr pNodeSetPtr,xmlDocPtr pdocptr, bool& Result)
{
    vClear();

    Result = true;
    //BYTE* pbSStream = pbSource;
    //BYTE byVersion = 0;
    //COPY_DATA_2(&byVersion, pbSStream, sizeof(BYTE));
    //COPY_DATA_2(&m_bEnabled, pbSStream, sizeof(m_bEnabled));
    //COPY_DATA_2(&m_ushTotal, pbSStream, sizeof(m_ushTotal));

    m_ushTotal = pNodeSetPtr->nodeNr;
    CStringArray omStrFilters;


    for(INT nIndex = 0; nIndex < m_ushTotal; nIndex++)
    {
        xmlNodePtr pNodePtr = pNodeSetPtr->nodeTab[nIndex];

        if ((!xmlStrcmp(pNodePtr->name, (const xmlChar*)"Filter")))
        {
            xmlChar* key = xmlNodeListGetString(pNodePtr->doc, pNodePtr->xmlChildrenNode, 1);
            if(NULL != key)
            {
                omStrFilters.Add((char*)key);
                xmlFree(key);
            }
        }
    }

    if( omStrFilters.GetSize() > 0 && pdocptr != NULL)
    {
        SFILTERAPPLIED_CAN sFilterApplied;
        if( sFilterApplied.nSetXMLConfigData(pdocptr) == S_OK)
        {
            /*for(INT nI = 0; nI < omStrFilters.GetSize(); nI++)
            {
                CString strFilterName = m_psFilters[nI].m_sFilterName.m_acFilterName;

                for(INT nIFilterInfo = 0; nIFilterInfo < m_psFilters[nI].m_psFilterInfo->unGetSize(); nIFilterInfo++)
                {
                    m_psFilters[nI].m_psFilterInfo[nIFilterInfo].m_dwMsgIDFrom;
                }
            }*/
            sFilterApplied.nGetFiltersFromName(pFilterAppliedCAN, omStrFilters);
        }
    }

    /*m_ushTotal = pNodeSetPtr->nodeNr;

    if (m_ushTotal > 0)
    {
        m_psFilters = new SFILTERSET[m_ushTotal];
        if (NULL != m_psFilters)
        {
            for (USHORT i = 0; (i < m_ushTotal) && Result; i++)
            {
                m_psFilters[i].pbSetConfigData(pNodeSetPtr->nodeTab[i],pdocptr, Result);
            }
        }
        else
        {
            Result = false;
        }
    }

    if (false == Result)
    {
        vClear();
    }*/
}
// PTV XML
template <typename SFRAMEINFO_BASIC_BUS>
int SFILTERAPPLIED<SFRAMEINFO_BASIC_BUS>::nGetFiltersFromName(SFILTERAPPLIED& sFilterDest, CStringArray& omStrFilters)
{
    int nRealFilters = 0;
    for(int i = 0; i < omStrFilters.GetSize(); i++)
    {
        int nIndex = GetFilterNameIndex((LPCSTR)omStrFilters.GetAt(i));
        if(nIndex >= 0)
        {
            nRealFilters++;
        }
        else
        {
            omStrFilters.RemoveAt(i);
        }
    }
    sFilterDest.m_bEnabled = m_bEnabled;
    sFilterDest.m_ushTotal = omStrFilters.GetSize();
    sFilterDest.m_psFilters = new SFILTERSET[sFilterDest.m_ushTotal];

    for(int i = 0; i < omStrFilters.GetSize(); i++)
    {
        int nIndex = GetFilterNameIndex((LPCSTR)omStrFilters.GetAt(i));
        if(nIndex >= 0)
        {
            sFilterDest.m_psFilters[i].bClone(m_psFilters[nIndex]);
        }
    }
    return S_OK;
}
template <typename SFRAMEINFO_BASIC_BUS>
int SFILTERAPPLIED<SFRAMEINFO_BASIC_BUS>::GetFilterNameIndex(string strName)
{
    int nIndex = -1;
    for(int i =0 ; i < m_ushTotal; i++)
    {
        if(strName == m_psFilters[i].m_sFilterName.m_acFilterName)
        {
            nIndex = i;
            break;
        }
    }
    return nIndex;
}

template <typename SFRAMEINFO_BASIC_BUS>
int SFILTERAPPLIED<SFRAMEINFO_BASIC_BUS>::nSetXMLConfigData(xmlDocPtr& pDocPtr)
{
    int nRetval = S_OK;
    vClear();
    //BYTE* pbSStream = pbSource;
    //BYTE byVersion = 0;
    //COPY_DATA_2(&byVersion, pbSStream, sizeof(BYTE));
    //COPY_DATA_2(&m_bEnabled, pbSStream, sizeof(m_bEnabled));
    //COPY_DATA_2(&m_ushTotal, pbSStream, sizeof(m_ushTotal));


    xmlNodeSetPtr pNodeSet;
    xmlXPathObjectPtr pPathObject;

    xmlChar* pchPath = (xmlChar*)"//BUSMASTER_CONFIGURATION/Module_Configuration/CAN_Filters/Filter";
    pPathObject = xmlUtils:: pGetNodes(pDocPtr, pchPath);
    if( NULL != pPathObject )
    {
        pNodeSet = pPathObject->nodesetval;
        if(NULL != pNodeSet)
        {
            m_ushTotal = pNodeSet->nodeNr;
            if (m_ushTotal > 0)
            {
                m_psFilters = new SFILTERSET[m_ushTotal];
                if (NULL != m_psFilters)
                {
                    for (USHORT i = 0; i < m_ushTotal; i++)
                    {
                        m_psFilters[i].nSetXMLConfigData(CAN, pNodeSet->nodeTab[i]);
                    }
                }
                else
                {
                    nRetval = S_FALSE;
                }
            }
        }
        else
        {
            nRetval = S_FALSE;
        }
        xmlXPathFreeObject (pPathObject);
    }
    else
    {
        nRetval = S_FALSE;
    }



    if (S_FALSE == nRetval)
    {
        vClear();
    }

    return nRetval;
}
//~MVN
// For CAN
typedef SFILTERAPPLIED<tagSFRAMEINFO_BASIC_CAN> SFILTERAPPLIED_CAN;

// For FlexRay
typedef SFILTERAPPLIED<tagSFRAMEINFO_BASIC_FLEXRAY> SFILTERAPPLIED_FLEXRAY;

// For MCNet
typedef SFILTERAPPLIED<tagSFRAMEINFO_BASIC_MCNET> SFILTERAPPLIED_MCNET;

// For J1939
typedef SFILTERAPPLIED<tagSFRAMEINFO_BASIC_J1939> SFILTERAPPLIED_J1939;

#endif // FILTER_DATATYPES_H__INCLUDED_
