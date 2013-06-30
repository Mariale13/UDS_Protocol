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
 * \file      KeyPanel_Datatypes.h
 * \brief     Defines data type class for key panel
 * \author    Pradeep Kadoor
 * \copyright Copyright (c) 2011, Robert Bosch Engineering and Business Solutions. All rights reserved.
 *
 * Defines data type class for key panel
 */


#if !defined KEYPANEL_DATATYPES__H_INCLUDED_
#define KEYPANEL_DATATYPES__H_INCLUDED_
#include "afxtempl.h"
struct tagKeyPanelEntry
{
    CString m_omDllPath;
    CString m_omNodeName;
    CString m_omFuncName;
    CString m_omRemarks;

    BOOL operator==(const tagKeyPanelEntry& RefObj)const
    {
        return ((m_omDllPath == RefObj.m_omDllPath) && (m_omFuncName == RefObj.m_omFuncName));
    }
};
typedef tagKeyPanelEntry SKEYPANELENTRY;
/* Key panel entries */
typedef CList <SKEYPANELENTRY, SKEYPANELENTRY&> CKeyPanelEntryList;

#endif //KEYPANEL_DATATYPES__H_INCLUDED_