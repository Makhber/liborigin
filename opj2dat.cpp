/***************************************************************************
    File                 : opj2dat.cpp
    --------------------------------------------------------------------
    Copyright            : (C) 2008 Stefan Gerlach
                           (C) 2017 Miquel Garriga
    Email (use @ for *)  : stefan.gerlach*uni-konstanz.de
    Description          : Origin project converter

 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *  This program is free software; you can redistribute it and/or modify   *
 *  it under the terms of the GNU General Public License as published by   *
 *  the Free Software Foundation; either version 3 of the License, or      *
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

#include "OriginFile.h"
#include <cmath>
#include <fstream>
#include <sstream>
#include <string>

int main(int argc, char *argv[])
{
    if (argc < 2) {
        std::cout << "Usage : ./opj2dat [--check-only] <file.opj>" << std::endl;
        return -1;
    }

    std::cout << "opj2dat " << liboriginVersionString()
              << ", Copyright (C) 2008 Stefan Gerlach, 2017 Miquel Garriga" << std::endl;

    if (std::string(argv[1]) == "-v")
        return 0;

    bool write_spreads = true;
    if ((argc > 2) && (std::string(argv[1]) == "--check-only"))
        write_spreads = false;

    std::string inputfile = argv[argc - 1];
    OriginFile opj(inputfile);
    int status = opj.parse();
    std::cout << "Parsing status = " << status << std::endl;
    if (!status)
        return -1;
    std::cout << "OPJ PROJECT \"" << inputfile.c_str() << "\" VERSION = " << opj.version()
              << std::endl;

    std::cout << "number of datasets     = " << opj.datasetCount() << std::endl;
    std::cout << "number of spreadsheets = " << opj.spreadCount() << std::endl;
    std::cout << "number of matrixes     = " << opj.matrixCount() << std::endl;
    std::cout << "number of excels       = " << opj.excelCount() << std::endl;
    std::cout << "number of functions    = " << opj.functionCount() << std::endl;
    std::cout << "number of graphs       = " << opj.graphCount() << std::endl;
    std::cout << "number of notes        = " << opj.noteCount() << std::endl;

    for (unsigned int s = 0; s < opj.spreadCount(); s++) {
        Origin::SpreadSheet spread = opj.spread(s);
        size_t columnCount = spread.columns.size();
        std::cout << "Spreadsheet " << (s + 1) << std::endl;
        std::cout << " Name: " << spread.name.c_str() << std::endl;
        std::cout << " Label: " << spread.label.c_str() << std::endl;
        std::cout << "	Columns: " << columnCount << std::endl;
        for (size_t j = 0; j < columnCount; j++) {
            Origin::SpreadColumn column = spread.columns[j];
            std::cout << "	Column " << (j + 1) << " : " << column.name.c_str()
                      << " / type : " << column.type << ", rows : " << spread.maxRows << std::endl;
        }

        if (write_spreads) {
            std::ostringstream sfilename;
            sfilename << inputfile.c_str() << "." << (s + 1) << ".dat";
            std::cout << "saved to " << sfilename.str() << std::endl;

            std::ofstream outf;
            outf.open(sfilename.str().c_str(), std::ios_base::out);
            if (!outf.good()) {
                std::cout << "Could not open " << sfilename.str() << std::endl;
                return -1;
            }
            // header
            for (size_t j = 0; j < columnCount; j++) {
                outf << spread.columns[j].name.c_str() << "; ";
                std::cout << spread.columns[j].name.c_str();
            }
            outf << std::endl;
            std::cout << std::endl << " Data: " << std::endl;
            // data
            for (int i = 0; i < (int)spread.maxRows; i++) {
                for (size_t j = 0; j < columnCount; j++) {
                    if (i < (int)spread.columns[j].data.size()) {
                        Origin::variant value(spread.columns[j].data[i]);
                        double v = 0.;
                        if (value.type() == Origin::variant::V_DOUBLE) {
                            v = value.as_double();
                            if (v != _ONAN) {
                                outf << v << "; ";
                            } else {
                                outf << nan("NaN") << "; ";
                            }
                        }
                        if (value.type() == Origin::variant::V_STRING) {
                            outf << value.as_string() << "; ";
                        }
                    } else {
                        outf << "; ";
                    }
                }
                outf << std::endl;
            }
            outf.close();
        }
    }
    return 0;
}
