/*
 * OriginAnyParser.h
 *
 * Copyright 2017 Miquel Garriga <gbmiquel@gmail.com>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 * Parser for all versions. Based mainly on Origin750Parser.h
 */
#ifndef ORIGIN_ANY_PARSER_H
#define ORIGIN_ANY_PARSER_H

#include "OriginParser.h"
#include "endianfstream.h"

#include <string>
#include <cmath> // for floor()

using namespace Origin;
using namespace endianfstream;

class OriginAnyParser : public OriginParser
{
public:
    explicit OriginAnyParser(const std::string &fileName);
    bool parse() override;

protected:
    unsigned int readObjectSize();
    std::string readObjectAsString(unsigned int);
    void readFileVersion();
    void readGlobalHeader();
    bool readDataSetElement();
    bool readWindowElement();
    bool readLayerElement();
    unsigned int readAnnotationList();
    bool readAnnotationElement();
    bool readCurveElement();
    bool readAxisBreakElement();
    bool readAxisParameterElement(unsigned int);
    bool readParameterElement();
    bool readNoteElement();
    void readProjectTree();
    unsigned int readFolderTree(tree<ProjectNode>::iterator, unsigned int);
    void readProjectLeaf(tree<ProjectNode>::iterator);
    void readAttachmentList();
    bool getColumnInfoAndData(const std::string &, unsigned int, const std::string &, unsigned int);
    void getMatrixValues(const std::string &, unsigned int, short, char, char,
                         std::vector<Origin::Matrix>::difference_type);
    void getWindowProperties(Origin::Window &, const std::string &, unsigned int);
    void getLayerProperties(const std::string &, unsigned int);
    Origin::Color getColor(const std::string &);
    void getAnnotationProperties(const std::string &, unsigned int, const std::string &,
                                 unsigned int, const std::string &, unsigned int,
                                 const std::string &, unsigned int);
    void getCurveProperties(const std::string &, unsigned int, const std::string &, unsigned int);
    void getAxisBreakProperties(const std::string &, unsigned int);
    void getAxisParameterProperties(const std::string &, unsigned int, int);
    void getNoteProperties(const std::string &, unsigned int, const std::string &, unsigned int,
                           const std::string &, unsigned int);
    void getColorMap(ColorMap &, const std::string &, unsigned int);
    void getZcolorsMap(ColorMap &, const std::string &, unsigned int);
    void getProjectLeafProperties(tree<ProjectNode>::iterator, const std::string &, unsigned int);
    void getProjectFolderProperties(tree<ProjectNode>::iterator, const std::string &, unsigned int);
    void outputProjectTree(std::ostream &);

    inline time_t doubleToPosixTime(double jdt)
    {
        /* 2440587.5 is julian date for the unixtime epoch */
        return (time_t)floor((jdt - 2440587) * 86400. + 0.5);
    }

    iendianfstream file;
    FILE *logfile;

    std::streamsize d_file_size;
    std::streamoff curpos;
    unsigned int objectIndex, parseError;
    std::vector<Origin::SpreadSheet>::difference_type ispread;
    std::vector<Origin::Matrix>::difference_type imatrix;
    std::vector<Origin::Excel>::difference_type iexcel;
    int igraph, ilayer;
};

#endif // ORIGIN_ANY_PARSER_H
