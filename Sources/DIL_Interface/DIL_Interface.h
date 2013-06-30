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
 * \file      DIL_Interface/DIL_Interface.h
 * \brief     Definition file for CDILApp class.
 * \author    Pradeep Kadoor
 * \copyright Copyright (c) 2011, Robert Bosch Engineering and Business Solutions. All rights reserved.
 *
 * Definition file for CDILApp class.
 */
#pragma once

// CDILApp
// See FrameProcessor.cpp for the implementation of this class
//
#ifndef __AFXWIN_H__
#error include 'stdafx.h' before including this file for PCH
#endif

class CDILApp : public CWinApp
{
    // Overrides
public:
    CDILApp();
    ~CDILApp();

    // To be used by the read thread
    virtual BOOL InitInstance();

    DECLARE_MESSAGE_MAP();
    virtual int ExitInstance();
};