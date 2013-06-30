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
 * \file      projConfigManager.h
 * \brief     Contains project configuration manager class definition.
 * \authors   Anish Kumar, Ratnadip Choudhury
 * \copyright Copyright (c) 2011, Robert Bosch Engineering and Business Solutions. All rights reserved.
 *
 * Contains project configuration manager class definition.
 */

#pragma once

typedef map <string, CProjConfig> PROJECTMAP;

class CProjConfigManager
{

private:

    PROJECTMAP m_MapOfProject;

    bool GetProjectConfig(string ProjectName, CProjConfig*& ProjConfig);

public:

    // Constructor and destructor
    CProjConfigManager();
    virtual ~CProjConfigManager();

    // Getters
    int GetProjectCount();
    int GetProjectList(list<string>& ProjectList);
    bool GetProjectData(string ProjectName, PROJECTDATA& ProjData);
    int GetSectionCount(string ProjectName);
    int GetSectionList(string ProjectName, list<string>& SectionList);
    bool GetSectionData(string ProjectName, string SectionName, SECTIONDATA& Sectiondata);

    // Setters
    void AddModifyProjDetail(const PROJECTDATA& ProjData);
    bool AddModifySection(string ProjectName, const SECTIONDATA& SectionData);
    void DeleteProjectTable(string ProjectName);
    void DeleteAllProjectTable(void);
};
