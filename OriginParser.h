/***************************************************************************
    File                 : OriginParser.h
    --------------------------------------------------------------------
    Copyright            : (C) 2008 Alex Kargovsky (kargovsky@yumr.phys.msu.su)
    Copyright            : (C) 2017 Stefan Gerlach (stefan.gerlach@uni.kn)
    Description          : Origin file parser base class

 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *  This program is free software; you can redistribute it and/or modify   *
 *  it under the terms of the GNU General Public License as published by   *
 *  the Free Software Foundation; either version 2 of the License, or      *
 *  (at your option) any later version.                                    *
 *                                                                         *
 *  This program is distributed in the hope that it will be useful,        *
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of         *
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the          *
 *  GNU General Public License for more details.                           *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the Free Software           *
 *   Foundation, Inc., 51 Franklin Street, Fifth Floor,                    *
 *   Boston, MA  02110-1301  USA                                           *
 *                                                                         *
 ***************************************************************************/

#ifndef ORIGIN_PARSER_H
#define ORIGIN_PARSER_H

#include "OriginObj.h"
#include "tree.h"

#ifdef GENERATE_CODE_FOR_LOG
#define LOG_PRINT(logfile, ...)                                                                    \
    {                                                                                              \
        fprintf(logfile, __VA_ARGS__);                                                             \
    }
#else // !GENERATE_CODE_FOR_LOG
#define LOG_PRINT(logfile, ...) {};
#endif

class OriginParser
{
public:
    virtual ~OriginParser() = default;
    virtual bool parse() = 0;

    std::vector<Origin::SpreadSheet>::difference_type
    findSpreadByName(const std::string &name) const;
    std::vector<Origin::Matrix>::difference_type findMatrixByName(const std::string &name) const;
    std::vector<Origin::Function>::difference_type
    findFunctionByName(const std::string &name) const;
    std::vector<Origin::Excel>::difference_type findExcelByName(const std::string &name) const;

protected:
    std::vector<Origin::SpreadColumn>::difference_type
    findSpreadColumnByName(std::vector<Origin::SpreadSheet>::size_type spread,
                           const std::string &name) const;
    std::vector<Origin::SpreadColumn>::difference_type
    findExcelColumnByName(std::vector<Origin::Excel>::size_type excel,
                          std::vector<Origin::SpreadSheet>::size_type sheet,
                          const std::string &name) const;
    std::pair<std::string, std::string> findDataByIndex(unsigned int index) const;
    std::pair<Origin::ProjectNode::NodeType, std::string>
    findObjectByIndex(unsigned int index) const;
    std::pair<Origin::ProjectNode::NodeType, Origin::Window>
    findWindowObjectByIndex(unsigned int index) const;
    void convertSpreadToExcel(std::vector<Origin::SpreadSheet>::size_type spread);

    int findColumnByName(int spread, const std::string &name);

private:
    bool iequals(const std::string &, const std::string &,
                 const std::locale & = std::locale()) const;

public:
    std::vector<Origin::SpreadColumn> datasets;
    std::vector<Origin::SpreadSheet> spreadSheets;
    std::vector<Origin::Matrix> matrixes;
    std::vector<Origin::Excel> excels;
    std::vector<Origin::Function> functions;
    std::vector<Origin::Graph> graphs;
    std::vector<Origin::Note> notes;
    tree<Origin::ProjectNode> projectTree;
    std::string resultsLog;
    unsigned int windowsCount;
    unsigned int fileVersion, buildVersion;
};

OriginParser *createOriginAnyParser(const std::string &fileName);

#endif // ORIGIN_PARSER_H
