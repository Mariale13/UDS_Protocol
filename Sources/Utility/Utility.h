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
 * \file      Utility.h
 * \brief     This file contains utility function declarations
 * \authors   Ratnadip Choudhury, Anish Kumar, Pradeep Kadoor
 * \copyright Copyright (c) 2011, Robert Bosch Engineering and Business Solutions. All rights reserved.
 *
 * This file contains utility function declarations
 */

#pragma once

#include <cstringt.h>
#include <atlstr.h>

#define UNUSED_DELIBERATELY(X)     X

#define INITIALISE_DATA(Data)   memset(&Data, 0, sizeof(Data))
#define INITIALISE_ARRAY(Array) memset(Array, 0, sizeof(Array))

bool BSTR_2_PCHAR(BSTR bstrSrcStr, char acTargetStr[], int nTargetStrLen);
BOOL WORD_2_CHAR(char acCharDat[], int nCharDatLen,
                 WORD awWordDat[], int nWordDatLen);
BOOL CHAR_2_WORD(WORD awWordDat[], int nWordDatLen,
                 char acCharDat[], int nCharDatLen);
void FilterOut_NonHexChar(char acSource[]);
int CalculateGCF(int anNumbers[], int Length);

// Top mask unwanted bit of the data
void vRemoveUnwantedBits(__int64& n64rData, int nLength);
void vExtendSignBit( __int64& n64Val, int nSize);
//bool bGetSystemErrorString(HRESULT hResult, CHAR acErrStr[256]);
bool bGetSystemErrorString(HRESULT hResult, char acErrStr[256]);
//bool bGetSystemErrorString(CHAR acErrStr[256]);

//// To copy the text into the clipboard
//BOOL CopyTextToClipboard(LPCTSTR lpctstrText, HWND hWND = NULL);

// To copy the text into the clipboard
BOOL CopyTextToClipboard(LPSTR lpstrText, HWND hWND = NULL);

__int64 nConvertStringToInt(LPCTSTR omStrHexNo);

// Top mask unwanted bit of the data
void s_vRemoveUnwantedBits(__int64& n64rData, int nLength);
void s_vRemoveUnwantedBits(UINT64& n64rData, int nLength);
void s_vExtendSignBit( __int64& n64Val, int nSize);

// To conver string to __int64. The string value could be in Hex or dec
BOOL bConvertStringToInt64( CString omStrHexNo,    // String
                            __int64& n64Value,     // Value
                            int nBase);
//Physical to value
unsigned int unConvertPhysicalToRaw(double dMaxVal, double dMinVal, double dOffset, double dScalFactor, unsigned long long int& unMaxVal, unsigned long long int& unMinVal);
int nConvertPhysicalToRaw(double dMaxVal, double dMinVal, double dOffset, double dScalFactor, long long int& nMaxVal, long long int& nMinVal);

//Raw to Physical value
unsigned int unConvertRawToPhysical(unsigned long long int unMaxRawVal, unsigned long long int unMinRawVal, double dOffset, double dScalFactor, double& dMaxPhyVal, double& dMinPhyVal);
int nConvertRawToPhysical(long long int unMaxRawVal, long long int unMinRawVal, double dOffset, double dScalFactor, double& dMaxPhyVal, double& dMinPhyVal);

//Helper function
unsigned long long int unRoundOffdoubleValue(double dValue);

//to get the time elapsed
__int64 gnGetCpuClocks();