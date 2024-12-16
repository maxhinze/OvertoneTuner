#pragma once

#include "wx/wx.h"
#include <vector>
#include <string>

struct vector2d
{
	double x;
	double y;
};

class cGraph
{
public:
	cGraph();
	cGraph(std::vector<vector2d>& data, wxPen Pen);

	void SetData(std::vector<vector2d>& data);

	void AppendData(vector2d data);
	void AppendData(std::vector<vector2d>& data);

	void RollAppendData(vector2d data);
	void RollAppendData(std::vector<vector2d>& data);

	void SetPen(wxPen Pen);

	void GetxLim(double &xMin, double &xMax);
	void GetyLim(double &yMin, double &yMax);

	void SetxLim(double xMin, double xMax);
	void SetyLim(double yMin, double yMax);

	int getSize();

	void render(wxDC& dc, int xPos, int yPos, int xSize, int ySize, bool logx, bool logy);

	~cGraph();

private:

	// transforms Data from World into Space Coordinates
	int trans(int ScreenStart, int ScreenEnd, double WorldStart, double WorldEnd, double Value);
	int logtrans(int ScreenStart, int ScreenEnd, double WorldStart, double WorldEnd, double Value);

	//Get min ánd maximum x and y values of the vector
	void GetMinMax(std::vector<vector2d> &data);


private:

	// todo: Log und Lin Achse

	//minimum and maximum Values of the Values
	double m_xmin, m_xmax, m_ymin, m_ymax;
	
	// Data Container (x and y Value)
	std::vector<vector2d> m_data;

	// Data Container Size
	int m_datasize;

	wxPen m_Pen;
};

