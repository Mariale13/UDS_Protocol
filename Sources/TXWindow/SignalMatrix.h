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
 * \file      SignalMatrix.h
 * \brief     Interface file for CSignalMatrix class
 * \author    Raja N
 * \copyright Copyright (c) 2011, Robert Bosch Engineering and Business Solutions. All rights reserved.
 *
 * Interface file for CSignalMatrix class
 */


#if !defined(AFX_SIGNALMATRIX_H__C23EEDF9_B670_410A_9E4C_FE24C9492F1C__INCLUDED_)
#define AFX_SIGNALMATRIX_H__C23EEDF9_B670_410A_9E4C_FE24C9492F1C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// SignalMatrix.h : header file
//

#define MAX_SIGNALS 64

typedef enum { GRAYED, HIGHLIGHT, NO_HIGHLIGHT } USER_BGTYPE;

class CSignalMatrix : public CStatic
{

public:
    // Construction
    CSignalMatrix();
    CSignalMatrix(int nMessageLength);

    // To set Message Length
    void vSetMessageLength(UINT unMsgLength);
    //Sets the values to be displayed in the Signal Matrix
    // This is Bit Array or Boolean Array
    void vSetValue(UINT* punValues);
    // Set the values using Byte Array.
    void vSetByteValue(UCHAR* punValues);

    //Sets unCount of cells starting from unFrom as Hiighlighted
    void vSetHighlight(const BYTE* pbySigMask, UINT unArrayLen);
    //Resets all the values in the cells to '0'
    void vResetValues();


    // Attributes
public:

    // Overrides
    // ClassWizard generated virtual function overrides
    //{{AFX_VIRTUAL(CSignalMatrix)
    //}}AFX_VIRTUAL

    // Implementation
public:
    virtual ~CSignalMatrix();

    // Generated message map functions
protected:
    //{{AFX_MSG(CSignalMatrix)
    afx_msg void OnPaint();
    afx_msg BOOL OnEraseBkgnd(CDC* pDC);
    //}}AFX_MSG

    DECLARE_MESSAGE_MAP()
private:
    // To initialise highlight bits
    void vResetHighlight();
    //Highlight brush (dark blue).
    CBrush* m_pHighlightBrush;
    //No-Hilite brush (white).
    CBrush* m_pNoHighlightBrush;
    // Disabled color Gray
    CBrush* m_pDisabledBrush;
    //Array to hold the highlight status for each cell.
    USER_BGTYPE m_aunHighlight[MAX_SIGNALS];
    //Length of the current message.
    UINT m_unMessageLength;
    // Signal data
    BOOL m_abSignalData[MAX_SIGNALS];
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SIGNALMATRIX_H__C23EEDF9_B670_410A_9E4C_FE24C9492F1C__INCLUDED_)
