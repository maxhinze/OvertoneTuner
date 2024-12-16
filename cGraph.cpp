#include "cGraph.h"



cGraph::cGraph()
{
	m_xmin = 0;
	m_xmax = 0;
	m_ymin = 0;
	m_ymax = 0;
	m_datasize = 0;
	m_Pen = wxPen(wxColor(255, 0, 0), 1);
}

cGraph::cGraph(std::vector<vector2d>& data, wxPen Pen)
{
	SetData(data);
	SetPen(Pen);
}

void cGraph::SetData(std::vector<vector2d>& data)
{
	//delete old data
	m_data.clear();
	m_data.shrink_to_fit();

	m_data = data;
	m_datasize = m_data.size();
	GetMinMax(m_data);
}

void cGraph::AppendData(vector2d data)
{
	m_data.push_back(data);

	if (data.x < m_xmin) m_xmin = data.x;
	if (data.x > m_xmax) m_xmax = data.x;
	if (data.y < m_ymin) m_ymin = data.y;
	if (data.y > m_ymax) m_ymax = data.y;

	m_datasize = m_data.size();
}

void cGraph::AppendData(std::vector<vector2d>& data)
{
	GetMinMax(data);
	m_data.insert(m_data.end(), data.begin(), data.end()); // merge the two Vectors
	m_datasize = m_data.size();
}

void cGraph::RollAppendData(vector2d data)
{
	m_data.push_back(data);
	m_data.erase(m_data.begin());

	m_datasize = m_data.size();

	GetMinMax(m_data);
}

void cGraph::RollAppendData(std::vector<vector2d>& data)
{
	m_data.insert(m_data.end(), data.begin(), data.end()); //merge the two Vectors
	for (int i = data.size(); i > 0; i--)
	{
		m_data.pop_back();
	}

	m_datasize = m_data.size();
}

void cGraph::SetPen(wxPen Pen)
{
	m_Pen = Pen;
}

void cGraph::GetxLim(double& xMin, double& xMax)
{
	xMin = m_xmin;
	xMax = m_xmax;
}

void cGraph::GetyLim(double& yMin, double& yMax)
{
	yMin = m_ymin;
	yMax = m_ymax;
}

void cGraph::SetxLim(double xMin, double xMax)
{
	m_xmin = xMin;
	m_xmax = xMax;
}

void cGraph::SetyLim(double yMin, double yMax)
{
	m_ymin = yMin;
	m_ymax = yMax;
}

int cGraph::getSize()
{
	return m_datasize;
}


void cGraph::render(wxDC& dc, int xPos, int yPos, int xSize, int ySize, bool logx, bool logy)
{
	//transform Data into Screen Coordinates
	wxPoint* GraphPoints = new wxPoint[m_datasize];

	wxPoint lastScreenPoint = {99999999,99999999};

	int size = 0;


	for (int i = 0; i < m_datasize; i++)
	{
		wxPoint ScreenPoint;

		if (logy){
			ScreenPoint.y = logtrans(yPos, ySize, m_ymax, m_ymin, m_data[i].y);
		}else{
			ScreenPoint.y = trans(yPos, ySize, m_ymax, m_ymin, m_data[i].y);
		}

		if (logx) {
			ScreenPoint.x = logtrans(xPos, xSize, m_xmin, m_xmax, m_data[i].x);
		}
		else {
			ScreenPoint.x = trans(xPos, xSize, m_xmin, m_xmax, m_data[i].x);
		}

		// to reduce the Time to Draw, only Draw the Points, where the Pixel has changed
		if (abs(lastScreenPoint.x - ScreenPoint.x) >= 1 || abs(lastScreenPoint.y - ScreenPoint.y) >= 1)
		{
			GraphPoints[size] = ScreenPoint;
			size++;
		}
		lastScreenPoint = ScreenPoint;
	}

	dc.SetPen(m_Pen);
	dc.DrawLines(size, GraphPoints);

	delete[] GraphPoints;
}

cGraph::~cGraph()
{
}

int cGraph::trans(int ScreenStart, int ScreenEnd, double WorldStart, double WorldEnd, double Value)
{
	return ( (int) ((ScreenEnd - ScreenStart) / (WorldEnd - WorldStart) * (Value - WorldStart) + ScreenStart) );
}

int cGraph::logtrans(int ScreenStart, int ScreenEnd, double WorldStart, double WorldEnd, double Value)
{
	return ((int)((ScreenEnd - ScreenStart) / (log10(WorldEnd) - log10(WorldStart)) * (log10(Value) - log10(WorldStart)) + ScreenStart));
}

void cGraph::GetMinMax(std::vector<vector2d>& data)
{
	//get maximum x and y Value of the Data
	m_xmin = data[0].x;
	m_ymin = data[0].y;
	m_xmax = data[0].x;
	m_ymax = data[0].y;
	for (int i = 0; i < data.size(); i++)
	{
		if (data[i].x < m_xmin) m_xmin = data[i].x;
		if (data[i].x > m_xmax) m_xmax = data[i].x;
		if (data[i].y < m_ymin) m_ymin = data[i].y;
		if (data[i].y > m_ymax) m_ymax = data[i].y;
	}
}
