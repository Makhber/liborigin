/***************************************************************************
    File                 : OPJFile.h
    --------------------------------------------------------------------
    Copyright            : (C) 2005-2007 Stefan Gerlach
                           (C) 2007 by Alex Kargovsky, Ion Vasilief
    Email (use @ for *)  : kargovsky*yumr.phys.msu.su, ion_vasilief*yahoo.fr
    Description          : Origin project import class

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

/* Origin 7.5 column value display : see FORMAT */

#ifndef OPJFILE_H
#define OPJFILE_H

#include <string>
#include <vector>
#include "tree.hh"

using namespace std;

struct originData {
	int type; // 0 - double, 1 - string
	double d;
	string s;
	originData(double _d)
	:	d(_d)
	,	type(0)
	,	s("")
	{};
	originData(char* _s)
	:	s(_s)
	,	type(1)
	,	d(1.0e-307)
	{};
};

struct spreadColumn {
	string name;
	string type;
	int value_type;//Numeric = 0, Text = 1, Date = 2, Time = 3, Month = 4, Day = 5, Text&Numeric = 6
	int value_type_specification; //see above
	int significant_digits;
	int decimal_places;
	int numeric_display_type;//Default Decimal Digits=0, Decimal Places=1, Significant Digits=2
	string command;
	string comment;
	int width;
	int index;
	vector <originData> odata;
	spreadColumn(string _name="", int _index=0)
	:	name(_name)
	,	index(_index)
	,	command("")
	,	comment("")
	,	value_type(0)
	,	value_type_specification(0)
	,	significant_digits(6)
	,	decimal_places(6)
	,	width(8)
	,	numeric_display_type(0)
	{};
};

struct spreadSheet {
	string name;
	string label;
	int objectID;
	string parentFolder;
	int maxRows;
	bool bHidden;
	bool bLoose;
	bool bMultisheet;
	vector <spreadColumn> column;
	spreadSheet(string _name="")
	:	name(_name)
	,	label("")
	,	bHidden(false)
	,	bLoose(true)
	,	bMultisheet(false)
	{};
};

struct excel {
	string name;
	string label;
	int objectID;
	string parentFolder;
	int maxRows;
	bool bHidden;
	bool bLoose;
	vector <spreadSheet> sheet;
	excel(string _name="", string _label="", int _maxRows=0, bool _bHidden=false, bool _bLoose=true)
	:	name(_name)
	,	label(_label)
	,	maxRows(_maxRows)
	,	bHidden(_bHidden)
	,	bLoose(_bLoose)
	{};
};

struct matrix {
	string name;
	string label;
	int objectID;
	string parentFolder;
	int nr_rows;
	int nr_cols;
	int value_type_specification;
	int significant_digits;
	int decimal_places;
	int numeric_display_type;//Default Decimal Digits=0, Decimal Places=1, Significant Digits=2
	string command;
	int width;
	int index;
	vector <double> data;
	matrix(string _name="", int _index=0)
	:	name(_name)
	,	index(_index)
	,	command("")
	,	value_type_specification(0)
	,	significant_digits(6)
	,	decimal_places(6)
	,	width(8)
	,	numeric_display_type(0)
	{};
};

struct function {
	string name;
	int type;//Normal = 0, Polar = 1
	string formula;
	double begin;
	double end;
	int points;
	int index;
	function(string _name="", int _index=0)
	:	name(_name)
	,	index(_index)
	,	type(0)
	,	formula("")
	,	begin(0.0)
	,	end(0.0)
	,	points(0)
	{};
};

struct graphCurve {
	int type;
	string dataName;
	string xColName;
	string yColName;
	int line_color;
	int line_style;
	int line_connect;
	double line_width;

	bool fillarea;
	int fillarea_type;
	int fillarea_pattern;
	int fillarea_color;
	int fillarea_pattern_color;
	double fillarea_pattern_width;
	int fillarea_pattern_border_style;
	int fillarea_pattern_border_color;
	double fillarea_pattern_border_width;

	int symbol_type;
	int symbol_color;
	int symbol_fill_color;
	double symbol_size;
	int symbol_thickness;
	int point_offset;
};

enum AxisPosition {Left = 0, Bottom = 1, Right = 2, Top = 3};

struct graphGrid {
	bool hidden;
	int color;
	int style;
	double width;
};

struct graphAxisFormat {
	bool hidden;
	int color;
	double thickness;
	double majorTickLength;
	int majorTicksType;
	int minorTicksType;
	int axisPosition;
	double axisPositionValue;
};

struct graphAxisTick {
	bool hidden;
	int color;
	int value_type;//Numeric = 0, Text = 1, Date = 2, Time = 3, Month = 4, Day = 5, Text&Numeric = 6
	int value_type_specification; 
	int decimal_places;
	int fontsize;
	string dataName;
	string colName;
	int rotation;
};

struct graphAxis {
	int pos;
	string label;
	double min;
	double max;
	double step;
	int majorTicks;
	int minorTicks;
	int scale;
	graphGrid majorGrid;
	graphGrid minorGrid;
	graphAxisFormat formatAxis[2];
	graphAxisTick tickAxis[2]; //bottom-top, left-right
};

struct rect {
	short left;
	short top;
	short right;
	short bottom;
	int height()
	{
		return bottom-top;
	};
	int width()
	{
		return right-left;
	};
	rect()
	{
	}
	rect(short width, short height)
	:	left(0)
	,	top(0)
	,	right(width)
	,	bottom(height)
	{
	}
};


struct text {
	string txt;
	rect clientRect;
	int color;
	int fontsize;
	int rotation;
	int tab;
	int border_type;
	int attach;

	text(string _txt="")
	:	txt(_txt)
	{};
};

struct rectangle {
	rect clientRect;
	int attach;
};

struct circle {
	rect clientRect;
	int attach;
};

struct lineVertex {
	int shape_type;
	double shape_width;
	double shape_length;
	double x;
	double y;
	lineVertex()
	:	shape_type(0)
	,	shape_width(0.0)
	,	shape_length(0.0)
	,	x(0.0)
	,	y(0.0)
	{}
};

struct line {
	rect clientRect;
	int color;
	int attach;
	double width;
	int line_style;
	lineVertex begin;
	lineVertex end;
};

struct bitmap {
	rect clientRect;
	int attach;
	unsigned long size;
	unsigned char* data;
	double left;
	double top;
	double width;
	double height;
};

struct metafile {
	rect clientRect;
	int attach;
};

struct graphLayer {
	rect clientRect;
	string legend;
	graphAxis xAxis;
	graphAxis yAxis;

	double histogram_bin;
	double histogram_begin;
	double histogram_end;

	vector<text> texts;
	vector<line> lines;
	vector<bitmap> bitmaps;
	vector<graphCurve> curve;
};

struct graph {
	string name;
	string label;
	int objectID;
	string parentFolder;
	bool bHidden;
	vector<graphLayer> layer;
	unsigned short width;
	unsigned short height;
	graph(string _name="")
	:	name(_name)
	,	label("")
	,	bHidden(false)
	{};
};

struct note {
	string name;
	string label;
	int objectID;
	string parentFolder;
	string text;
	note(string _name="")
	:	name(_name)
	{};
};

struct projectNode {
	int type; // 0 - object, 1 - folder
	string name;
	projectNode(string _name="", int _type=0)
	:	name(_name)
	,	type(_type)
	{};
};

class OPJFile
{
public:
	OPJFile(const char* filename);
	~OPJFile()
	{
		for(int g=0; g<GRAPH.size(); ++g)
			for(int l=0; l<GRAPH[g].layer.size(); ++l)
				for(int i=0; i<GRAPH[g].layer[l].bitmaps.size(); ++i)
					if(GRAPH[g].layer[l].bitmaps[i].size > 0)
						delete GRAPH[g].layer[l].bitmaps[i].data;
	}
	int Parse();
	double Version() const { return version/100.0; }		//!< get version of project file

	const tree<projectNode>* project() const { return &projectTree; }
	//spreadsheet properties
	int numSpreads() const { return SPREADSHEET.size(); }			//!< get number of spreadsheets
	const char *spreadName(int s) const { return SPREADSHEET[s].name.c_str(); }	//!< get name of spreadsheet s
	const char *spreadParentFolder(int s) const { return SPREADSHEET[s].parentFolder.c_str(); }	//!< get parent folder of spreadsheet s
	bool spreadHidden(int s) const { return SPREADSHEET[s].bHidden; }	//!< is spreadsheet s hidden
	bool spreadLoose(int s) const { return SPREADSHEET[s].bLoose; }	//!< is spreadsheet s loose
	const char *spreadLabel(int s) const { return SPREADSHEET[s].label.c_str(); }	//!< get label of spreadsheet s
	int numCols(int s) const { return SPREADSHEET[s].column.size(); }		//!< get number of columns of spreadsheet s
	int numRows(int s,int c) const { return SPREADSHEET[s].column[c].odata.size(); }	//!< get number of rows of column c of spreadsheet s
	int maxRows(int s) const { return SPREADSHEET[s].maxRows; }		//!< get maximum number of rows of spreadsheet s
	//spreadsheet's column properties
	const char *colName(int s, int c) const { return SPREADSHEET[s].column[c].name.c_str(); }	//!< get name of column c of spreadsheet s
	const char *colType(int s, int c) const { return SPREADSHEET[s].column[c].type.c_str(); }	//!< get type of column c of spreadsheet s
	const char *colCommand(int s, int c) const { return SPREADSHEET[s].column[c].command.c_str(); }	//!< get command of column c of spreadsheet s
	const char *colComment(int s, int c) const { return SPREADSHEET[s].column[c].comment.c_str(); }	//!< get comment of column c of spreadsheet s
	int colValueType(int s, int c) const { return SPREADSHEET[s].column[c].value_type; }	//!< get value type of column c of spreadsheet s
	int colValueTypeSpec(int s, int c) const { return SPREADSHEET[s].column[c].value_type_specification; }	//!< get value type specification of column c of spreadsheet s
	int colSignificantDigits(int s, int c) const { return SPREADSHEET[s].column[c].significant_digits; }	//!< get significant digits of column c of spreadsheet s
	int colDecPlaces(int s, int c) const { return SPREADSHEET[s].column[c].decimal_places; }	//!< get decimal places of column c of spreadsheet s
	int colNumDisplayType(int s, int c) const { return SPREADSHEET[s].column[c].numeric_display_type; }	//!< get numeric display type of column c of spreadsheet s
	int colWidth(int s, int c) const { return SPREADSHEET[s].column[c].width; }	//!< get width of column c of spreadsheet s
	void* oData(int s, int c, int r, bool alwaysDouble=false) const {
		if(alwaysDouble)
			return (void*)&SPREADSHEET[s].column[c].odata[r].d;
		if(SPREADSHEET[s].column[c].odata[r].type==0)
			return (void*)&SPREADSHEET[s].column[c].odata[r].d;
		else
			return (void*)SPREADSHEET[s].column[c].odata[r].s.c_str();
	}	//!< get data of column c/row r of spreadsheet s

	//matrix properties
	int numMatrices() const { return MATRIX.size(); }			//!< get number of matrices
	const char *matrixName(int s) const { return MATRIX[s].name.c_str(); }	//!< get name of matrix s
	const char *matrixParentFolder(int s) const { return MATRIX[s].parentFolder.c_str(); }	//!< get parent folder of matrix s
	const char *matrixLabel(int s) const { return MATRIX[s].label.c_str(); }	//!< get label of matrix s
	int numMartixCols(int s) const { return MATRIX[s].nr_cols; }		//!< get number of columns of matrix s
	int numMartixRows(int s) const { return MATRIX[s].nr_rows; }	//!< get number of rows of matrix s
	const char *matrixFormula(int s) const { return MATRIX[s].command.c_str(); }	//!< get formula of matrix s
	int matrixValueTypeSpec(int s) const { return MATRIX[s].value_type_specification; }	//!< get value type specification of matrix s
	int matrixSignificantDigits(int s) const { return MATRIX[s].significant_digits; }	//!< get significant digits of matrix s
	int matrixDecPlaces(int s) const { return MATRIX[s].decimal_places; }	//!< get decimal places of matrix s
	int matrixNumDisplayType(int s) const { return MATRIX[s].numeric_display_type; }	//!< get numeric display type of matrix s
	int matrixWidth(int s) const { return MATRIX[s].width; }	//!< get width of matrix s
	double matrixData(int s, int c, int r) const { return MATRIX[s].data[r*MATRIX[s].nr_cols+c]; }	//!< get data of row r of column c of matrix s

	//function properties
	int numFunctions() const { return FUNCTION.size(); }			//!< get number of functions
	int functionIndex(const char* s) const { return compareFunctionnames(s); }	//!< get name of function s
	const char *functionName(int s) const { return FUNCTION[s].name.c_str(); }	//!< get name of function s
	int functionType(int s) const { return FUNCTION[s].type; }		//!< get type of function s
	double functionBegin(int s) const { return FUNCTION[s].begin; }	//!< get begin of interval of function s
	double functionEnd(int s) const { return FUNCTION[s].end; }	//!< get end of interval of function s
	int functionPoints(int s) const { return FUNCTION[s].points; }	//!< get number of points in interval of function s
	const char *functionFormula(int s) const { return FUNCTION[s].formula.c_str(); }	//!< get formula of function s

	//graph properties
	enum Color {Black=0, Red=1, Green=2, Blue=3, Cyan=4, Magenta=5, Yellow=6, DarkYellow=7, Navy=8,
		Purple=9, Wine=10, Olive=11, DarkCyan=12, Royal=13, Orange=14, Violet=15, Pink=16, White=17,
		LightGray=18, Gray=19, LTYellow=20, LTCyan=21, LTMagenta=22, DarkGray=23, Custom=255};

	enum Plot {Line=200, Scatter=201, LineSymbol=202, Column=203, Area=204, HiLoClose=205, Box=206,
		ColumnFloat=207, Vector=208, PlotDot=209, Wall3D=210, Ribbon3D=211, Bar3D=212, ColumnStack=213,
		AreaStack=214, Bar=215, BarStack=216, FlowVector=218, Histogram=219, MatrixImage=220, Pie=225,
		Contour=226, Unknown=230, ErrorBar=231, TextPlot=232, XErrorBar=233, SurfaceColorMap=236,
		SurfaceColorFill=237, SurfaceWireframe=238, SurfaceBars=239, Line3D=240, Text3D=241, Mesh3D=242,
		XYZTriangular=245, LineSeries=246, YErrorBar=254, XYErrorBar=255, GraphScatter3D=0x8AF0,
		GraphTrajectory3D=0x8AF1, Polar=0x00020000, SmithChart=0x00040000, FillArea=0x00800000};

	enum LineStyle {Solid=0, Dash=1, Dot=2, DashDot=3, DashDotDot=4, ShortDash=5, ShortDot=6, ShortDashDot=7};

	enum LineConnect {NoLine=0, Straight=1, TwoPointSegment=2, ThreePointSegment=3, BSpline=8, Spline=9, StepHorizontal=11, StepVertical=12, StepHCenter=13, StepVCenter=14, Bezier=15};

	enum Scale {Linear=0, Log10=1, Probability=2, Probit=3, Reciprocal=4, OffsetReciprocal=5, Logit=6, Ln=7, Log2=8};

	enum ValueType {Numeric=0, Text=1, Time=2, Date=3,  Month=4, Day=5, ColumnHeading=6, TickIndexedDataset=7, TextNumeric=9, Categorical=10};

	enum BorderType {BlackLine=0, Shadow=1, DarkMarble=2, WhiteOut=3, BlackOut=4, None=-1};

	enum Attach {Frame=0, Page=1, Scale=2};

	int numGraphs() const { return GRAPH.size(); }			//!< get number of graphs
	const char *graphName(int s) const { return GRAPH[s].name.c_str(); }	//!< get name of graph s
	const char *graphParentFolder(int s) const { return GRAPH[s].parentFolder.c_str(); }	//!< get parent folder of graph s
	const char *graphLabel(int s) const { return GRAPH[s].label.c_str(); }	//!< get name of graph s
	bool graphHidden(int s) const { return GRAPH[s].bHidden; }	//!< is graph s hidden
	rect graphRect(int s) const { return rect(GRAPH[s].width, GRAPH[s].height); }		//!< get rectangle of graph s
	int numLayers(int s) const { return GRAPH[s].layer.size(); }			//!< get number of layers of graph s
	rect layerRect(int s, int l) const { return GRAPH[s].layer[l].clientRect; }		//!< get rectangle of layer l of graph s
	const char *layerXAxisTitle(int s, int l) const { return GRAPH[s].layer[l].xAxis.label.c_str(); }		//!< get label of X-axis of layer l of graph s
	const char *layerYAxisTitle(int s, int l) const { return GRAPH[s].layer[l].yAxis.label.c_str(); }		//!< get label of Y-axis of layer l of graph s
	const char *layerLegend(int s, int l) const { return GRAPH[s].layer[l].legend.c_str(); }		//!< get legend of layer l of graph s
	vector<text> layerTexts(int s, int l) const {	return GRAPH[s].layer[l].texts; } //!< get texts of layer l of graph s
	vector<line> layerLines(int s, int l) const {	return GRAPH[s].layer[l].lines; } //!< get lines of layer l of graph s
	vector<bitmap> layerBitmaps(int s, int l) const {	return GRAPH[s].layer[l].bitmaps; } //!< get bitmaps of layer l of graph s
	vector<double> layerXRange(int s, int l) const {
		vector<double> range;
		range.push_back(GRAPH[s].layer[l].xAxis.min);
		range.push_back(GRAPH[s].layer[l].xAxis.max);
		range.push_back(GRAPH[s].layer[l].xAxis.step);
		return range;
	} //!< get X-range of layer l of graph s
	vector<double> layerYRange(int s, int l) const {
		vector<double> range;
		range.push_back(GRAPH[s].layer[l].yAxis.min);
		range.push_back(GRAPH[s].layer[l].yAxis.max);
		range.push_back(GRAPH[s].layer[l].yAxis.step);
		return range;
	} //!< get Y-range of layer l of graph s
	vector<int> layerXTicks(int s, int l) const {
		vector<int> tick;
		tick.push_back(GRAPH[s].layer[l].xAxis.majorTicks);
		tick.push_back(GRAPH[s].layer[l].xAxis.minorTicks);
		return tick;
	} //!< get X-axis ticks of layer l of graph s
	vector<int> layerYTicks(int s, int l) const {
		vector<int> tick;
		tick.push_back(GRAPH[s].layer[l].yAxis.majorTicks);
		tick.push_back(GRAPH[s].layer[l].yAxis.minorTicks);
		return tick;
	} //!< get Y-axis ticks of layer l of graph s
	vector<graphGrid> layerGrid(int s, int l) const {
		vector<graphGrid> grid;
		grid.push_back(GRAPH[s].layer[l].xAxis.majorGrid);
		grid.push_back(GRAPH[s].layer[l].xAxis.minorGrid);
		grid.push_back(GRAPH[s].layer[l].yAxis.majorGrid);
		grid.push_back(GRAPH[s].layer[l].yAxis.minorGrid);
		return grid;
	} //!< get grid of layer l of graph s
	vector<graphAxisFormat> layerAxisFormat(int s, int l) const {
		vector<graphAxisFormat> format;
		format.push_back(GRAPH[s].layer[l].yAxis.formatAxis[0]); //bottom
		format.push_back(GRAPH[s].layer[l].yAxis.formatAxis[1]); //top
		format.push_back(GRAPH[s].layer[l].xAxis.formatAxis[0]); //left
		format.push_back(GRAPH[s].layer[l].xAxis.formatAxis[1]); //right
		return format;
	} //!< get title and format of axes of layer l of graph s
	vector<graphAxisTick> layerAxisTickLabels(int s, int l) const {
		vector<graphAxisTick> tick;
		tick.push_back(GRAPH[s].layer[l].yAxis.tickAxis[0]); //bottom
		tick.push_back(GRAPH[s].layer[l].yAxis.tickAxis[1]); //top
		tick.push_back(GRAPH[s].layer[l].xAxis.tickAxis[0]); //left
		tick.push_back(GRAPH[s].layer[l].xAxis.tickAxis[1]); //right
		return tick;
	} //!< get tick labels of axes of layer l of graph s
	vector<double> layerHistogram(int s, int l) const {
		vector<double> range;
		range.push_back(GRAPH[s].layer[l].histogram_bin);
		range.push_back(GRAPH[s].layer[l].histogram_begin);
		range.push_back(GRAPH[s].layer[l].histogram_end);
		return range;
	} //!< get histogram bin of layer l of graph s
	int layerXScale(int s, int l) const { return GRAPH[s].layer[l].xAxis.scale; }		//!< get scale of X-axis of layer l of graph s
	int layerYScale(int s, int l) const { return GRAPH[s].layer[l].yAxis.scale; }		//!< get scale of Y-axis of layer l of graph s
	int numCurves(int s, int l) const { return GRAPH[s].layer[l].curve.size(); }			//!< get number of curves of layer l of graph s
	const char *curveDataName(int s, int l, int c) const { return GRAPH[s].layer[l].curve[c].dataName.c_str(); }	//!< get data source name of curve c of layer l of graph s
	const char *curveXColName(int s, int l, int c) const { return GRAPH[s].layer[l].curve[c].xColName.c_str(); }	//!< get X-column name of curve c of layer l of graph s
	const char *curveYColName(int s, int l, int c) const { return GRAPH[s].layer[l].curve[c].yColName.c_str(); }	//!< get Y-column name of curve c of layer l of graph s
	int curveType(int s, int l, int c) const { return GRAPH[s].layer[l].curve[c].type; }	//!< get type of curve c of layer l of graph s
	int curveLineStyle(int s, int l, int c) const { return GRAPH[s].layer[l].curve[c].line_style; }	//!< get line style of curve c of layer l of graph s
	int curveLineColor(int s, int l, int c) const { return GRAPH[s].layer[l].curve[c].line_color; }	//!< get line color of curve c of layer l of graph s
	int curveLineConnect(int s, int l, int c) const { return GRAPH[s].layer[l].curve[c].line_connect; }	//!< get line connect of curve c of layer l of graph s
	double curveLineWidth(int s, int l, int c) const { return GRAPH[s].layer[l].curve[c].line_width; }	//!< get line width of curve c of layer l of graph s

	bool curveIsFilledArea(int s, int l, int c) const { return GRAPH[s].layer[l].curve[c].fillarea; }	//!< get is filled area of curve c of layer l of graph s
	int curveFillAreaColor(int s, int l, int c) const { return GRAPH[s].layer[l].curve[c].fillarea_color; }	//!< get area fillcolor of curve c of layer l of graph s
	int curveFillPattern(int s, int l, int c) const { return GRAPH[s].layer[l].curve[c].fillarea_pattern; }	//!< get fill pattern of curve c of layer l of graph s
	int curveFillPatternColor(int s, int l, int c) const { return GRAPH[s].layer[l].curve[c].fillarea_pattern_color; }	//!< get fill pattern color of curve c of layer l of graph s
	double curveFillPatternWidth(int s, int l, int c) const { return GRAPH[s].layer[l].curve[c].fillarea_pattern_width; }	//!< get fill pattern line width of curve c of layer l of graph s
	int curveFillPatternBorderStyle(int s, int l, int c) const { return GRAPH[s].layer[l].curve[c].fillarea_pattern_border_style; }	//!< get fill pattern border style of curve c of layer l of graph s
	int curveFillPatternBorderColor(int s, int l, int c) const { return GRAPH[s].layer[l].curve[c].fillarea_pattern_border_color; }	//!< get fill pattern border color of curve c of layer l of graph s
	double curveFillPatternBorderWidth(int s, int l, int c) const { return GRAPH[s].layer[l].curve[c].fillarea_pattern_border_width; }	//!< get fill pattern border line width of curve c of layer l of graph s

	int curveSymbolType(int s, int l, int c) const { return GRAPH[s].layer[l].curve[c].symbol_type; }	//!< get symbol type of curve c of layer l of graph s
	int curveSymbolColor(int s, int l, int c) const { return GRAPH[s].layer[l].curve[c].symbol_color; }	//!< get symbol color of curve c of layer l of graph s
	int curveSymbolFillColor(int s, int l, int c) const { return GRAPH[s].layer[l].curve[c].symbol_fill_color; }	//!< get symbol fill color of curve c of layer l of graph s
	double curveSymbolSize(int s, int l, int c) const { return GRAPH[s].layer[l].curve[c].symbol_size; }	//!< get symbol size of curve c of layer l of graph s
	int curveSymbolThickness(int s, int l, int c) const { return GRAPH[s].layer[l].curve[c].symbol_thickness; }	//!< get symbol thickness of curve c of layer l of graph s

	//note
	int numNotes() const { return NOTE.size(); }			//!< get number of notes
	const char *noteName(int n) const { return NOTE[n].name.c_str(); }	//!< get name of note n
	const char *noteParentFolder(int n) const { return NOTE[n].parentFolder.c_str(); }	//!< get parent folder of note n
	const char *noteLabel(int n) const { return NOTE[n].label.c_str(); }	//!< get label of note n
	const char *noteText(int n) const { return NOTE[n].text.c_str(); }	//!< get text of note n

	const char* resultsLogString() const { return resultsLog.c_str();}		//!< get Results Log

private:
	bool IsBigEndian();
	void ByteSwap(unsigned char * b, int n);
	int ParseFormatOld();
	int ParseFormatNew();
	int compareSpreadnames(char *sname) const;				//!< returns matching spread index
	int compareExcelnames(char *sname) const;				//!< returns matching excel index
	int compareColumnnames(int spread, char *sname) const;	//!< returns matching column index
	int compareExcelColumnnames(int excel, int sheet, char *sname) const;  //!< returns matching column index
	int compareMatrixnames(char *sname) const;				//!< returns matching matrix index
	int compareFunctionnames(const char *sname) const;				//!< returns matching function index
	vector<string> findDataByIndex(int index) const;
	string findObjectByIndex(int index, string folder);
	void readSpreadInfo(FILE *fopj, FILE *fdebug);
	void readExcelInfo(FILE *f, FILE *debug);
	void readMatrixInfo(FILE *fopj, FILE *fdebug);
	void readGraphInfo(FILE *fopj, FILE *fdebug);
	void readGraphGridInfo(graphGrid &grid, FILE *fopj, int pos);
	void readGraphAxisFormatInfo(graphAxisFormat &format, FILE *fopj, int pos);
	void readGraphAxisTickLabelsInfo(graphAxisTick &tick, FILE *fopj, int pos);
	void readProjectTree(FILE *f, FILE *debug);
	void readProjectTreeFolder(FILE *f, FILE *debug, tree<projectNode>::iterator parent);
	void skipObjectInfo(FILE *fopj, FILE *fdebug);
	void setColName(int spread);		//!< set default column name starting from spreadsheet spread
	void convertSpreadToExcel(int spread);
	const char* filename;			//!< project file name
	int version;				//!< project version
	int dataIndex;
	int objectIndex;
	string resultsLog;
	vector <spreadSheet> SPREADSHEET;
	vector <matrix> MATRIX;
	vector <excel> EXCEL;
	vector <function> FUNCTION;
	vector <graph> GRAPH;
	vector <note> NOTE;
	tree <projectNode> projectTree;
};

#endif // OPJFILE_H
