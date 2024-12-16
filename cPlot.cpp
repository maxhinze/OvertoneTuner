#include "cPlot.h"


BEGIN_EVENT_TABLE(cPlot, wxPanel)

 // catch paint events
	EVT_PAINT(cPlot::paintEvent)

END_EVENT_TABLE()




cPlot::cPlot(wxFrame* parent) :
	wxPanel(parent, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxFULL_REPAINT_ON_RESIZE)
{
	m_xTickCount = 11;
	m_yTickCount = 11;
	
	m_LeftMargin = 50;
	m_RightMargin = 20;
	m_TopMargin = 10;
	m_BottomMargin = 30;

	m_xMin = 0;
	m_xMax = 1;

	m_yMin = 0;
	m_yMax = 1;

	m_BackgroundColor = *wxBLACK;
	m_TickColor = *wxWHITE;
	m_GridColor = wxColour(150, 150, 150);

	SetxTickLabels();
	SetyTickLabels();
	this->SetDoubleBuffered(true); //to Avoid flickering
}


cPlot::~cPlot()
{
}

void cPlot::Update()
{
	GetMinMax();
	SetxTickLabels();
	SetyTickLabels();
	paintNow();
}

void cPlot::paintEvent(wxPaintEvent& evt)
{
	wxPaintDC dc(this);
	render(dc);
}

void cPlot::paintNow()
{
	wxClientDC dc(this);
	render(dc);
}

void cPlot::renderXGrid(wxDC& dc)
{
	int xSize, ySize;
	dc.GetSize(&xSize, &ySize);
	

	//Draw xGrid (linear)
	double xTickspacing = ((double)xSize - m_RightMargin - m_LeftMargin) / (m_xTickCount - 1.0); //calculate tickspacing
	for (int i = 0; i < m_xTickCount; i++)
	{
		int x = (double)m_LeftMargin + (double)i * xTickspacing;

		//Grid
		if (i > 0 && i < (m_xTickCount - 1))
		{
			dc.SetPen(wxPen(m_GridColor, 1, wxPENSTYLE_DOT));
		}
		else
		{
			dc.SetPen(wxPen(m_TickColor, 1, wxPENSTYLE_SOLID));
		}

		dc.DrawLine(x, m_TopMargin, x, ySize - m_BottomMargin);


		//minor Grid
		if (m_xMinorTicksVisible && i < (m_xTickCount - 1))
		{
			dc.SetPen(wxPen(m_GridColor, 1, wxPENSTYLE_DOT));
			int x_minor;
			for (int j = 1; j < 10; j++)
			{
				if (m_xLogAxis)
				{
					x_minor = x + xTickspacing * (log10(j+1) / log10(10));
					dc.DrawLine(x_minor, m_TopMargin, x_minor, ySize - m_BottomMargin);
				}
				else
				{
					x_minor = x + xTickspacing * (j / 10.0);
					dc.DrawLine(x_minor, m_TopMargin, x_minor, ySize - m_BottomMargin);
				}
			}
		}

	}
}

void cPlot::renderYGrid(wxDC& dc)
{
	int xSize, ySize;
	dc.GetSize(&xSize, &ySize);
	
	//Draw yGrid (linear)
	double yTickspacing = ((double)ySize - m_TopMargin - m_BottomMargin) / (m_yTickCount - 1.0); //calculate tickspacing
	for (int i = 0; i < m_yTickCount; i++)
	{
		int y = (double)ySize - (double)m_BottomMargin - i * yTickspacing;

		//Grid
		if (i > 0 && i < (m_yTickCount - 1))
		{
			dc.SetPen(wxPen(m_GridColor, 1, wxPENSTYLE_DOT));
		}
		else
		{
			dc.SetPen(wxPen(m_TickColor, 1, wxPENSTYLE_SOLID));
		}
		dc.DrawLine(m_LeftMargin, y, xSize - m_RightMargin, y);

		//minor Ticks
		if (m_yMinorTicksVisible && i < (m_yTickCount-1))
		{
			dc.SetPen(wxPen(m_GridColor, 1, wxPENSTYLE_DOT));
			int y_minor;
			for (int j = 1; j < 10; j++)
			{
				if (m_yLogAxis)
				{
					y_minor = y - yTickspacing * (log10(j+1) / log10(10));
					dc.DrawLine(m_LeftMargin, y_minor, xSize - m_RightMargin, y_minor);
				}
				else
				{
					y_minor = y - yTickspacing * (j / 10.0);
					dc.DrawLine(m_LeftMargin, y_minor, xSize - m_RightMargin, y_minor);
				}
			}
		}
	}
}

void cPlot::renderXTicks(wxDC& dc)
{
	int xSize, ySize;
	dc.GetSize(&xSize, &ySize);

	wxPen TickPen = wxPen(m_TickColor, 1);
	dc.SetPen(wxPen(TickPen));

	//Draw xTicks
	double xTickspacing = ((double)xSize - m_RightMargin - m_LeftMargin) / (m_xTickCount - 1.0); //calculate tickspacing
	for (int i = 0; i < m_xTickCount; i++)
	{
		int x = (double)m_LeftMargin + (double)i * xTickspacing;

		//major Ticks
		dc.DrawLine(x, ySize - m_BottomMargin + 6, x, ySize - m_BottomMargin - 6);

		//minor Ticks
		if (m_xMinorTicksVisible && i < (m_xTickCount - 1))
		{
			int x_minor;
			for (int j = 1; j < 10; j++)
			{
				if (m_xLogAxis)
				{
					x_minor = x + xTickspacing * (log10(j+1) / log10(10));
					dc.DrawLine(x_minor, ySize - m_BottomMargin + 2, x_minor, ySize - m_BottomMargin - 2);
				}
				else
				{
					x_minor = x + xTickspacing * (j / 10.0);
					dc.DrawLine(x_minor, ySize - m_BottomMargin + 2, x_minor, ySize - m_BottomMargin - 2);
				}
			}
		}

		//TickLabel
		if (m_xTicks.size() > 0 && m_xTicks.size() <= m_xTickCount)
		{
			wxSize TextExtend = dc.GetTextExtent(m_xTicks[i]);
			dc.SetTextForeground(m_TickColor);
			dc.DrawText(m_xTicks[i], x - TextExtend.x / 2, ySize + TextExtend.y / 2 - m_BottomMargin);
		}
	}
}

void cPlot::renderYTicks(wxDC& dc)
{
	int xSize, ySize;
	dc.GetSize(&xSize, &ySize);

	wxPen TickPen = wxPen(m_TickColor, 1);
	dc.SetPen(wxPen(TickPen));

	//Draw yTicks (linear)
	double yTickspacing = ((double)ySize - m_TopMargin - m_BottomMargin) / (m_yTickCount - 1.0); //calculate tickspacing
	for (int i = 0; i < m_yTickCount; i++)
	{
		int y = (double)ySize - (double)m_BottomMargin - i * yTickspacing;

		//major Ticks
		dc.DrawLine(m_LeftMargin + 6, y, m_LeftMargin - 6, y);

		//minor Ticks
		if (m_yMinorTicksVisible && i < (m_yTickCount - 1))
		{
			int y_minor;
			for (int j = 1; j < 10; j++)
			{
				if (m_yLogAxis)
				{
					y_minor = y - yTickspacing*(log10(j+1)/log10(10));
					dc.DrawLine(m_LeftMargin + 2, y_minor, m_LeftMargin - 2, y_minor);
				}
				else
				{
					y_minor = y - yTickspacing * (j / 10.0);
					dc.DrawLine(m_LeftMargin + 2, y_minor, m_LeftMargin - 2, y_minor);
				}
			}
		}
		
		//TickLabel
		if (m_yTicks.size() > 0 && m_yTicks.size() <= m_yTickCount)
		{
			wxSize TextExtend = dc.GetTextExtent(m_yTicks[i]);
			dc.SetTextForeground(m_TickColor);
			dc.DrawText(m_yTicks[i], m_LeftMargin - TextExtend.x - 10, y - TextExtend.y / 2);
		}
	}
}

void cPlot::SetxTickLabels()
{
	m_xTicks.clear();

	double Tickvalue;

	for (int i = 0; i < m_xTickCount; i++)
	{
		if (m_xLogAxis == true)
		{
			// Log
			double LogMin = m_xMin / pow(10, round(log10(m_xMin)));
			Tickvalue = LogMin*pow(10, round(log10(m_xMin * pow(10, i))));
			m_xTicks.push_back(wxString::Format("%+.0e", Tickvalue));	//scientific Notation
		}
		else
		{
			// Linear
			Tickvalue = m_xMin + ((m_xMax - m_xMin) / (m_xTickCount - 1.0)) * i;
			m_xTicks.push_back(wxString::Format("%.2f", Tickvalue));  //float Notation
		}
	}
}

void cPlot::SetyTickLabels()
{
	m_yTicks.clear();

	double Tickvalue;

	for (int i = 0; i < m_yTickCount; i++)
	{
		if (m_yLogAxis == true)
		{
			// Log
			double LogMin = m_yMin/pow(10, round(log10(m_yMin)));
			Tickvalue = LogMin* pow(10, round(log10(m_yMin*pow(10,i))));
			m_yTicks.push_back(wxString::Format("%+.0e", Tickvalue));	//scientific Notation
		}
		else
		{
			// Linear
			Tickvalue = m_yMin + ((m_yMax - m_yMin) / (m_yTickCount - 1.0)) * i;
			m_yTicks.push_back(wxString::Format("%.2f", Tickvalue));  //float Notation
		}
	}
}

void cPlot::SetxTickCount(int n)
{
	m_xTickCount = n;
	SetxTickLabels();
}

void cPlot::SetyTickCount(int n)
{
	m_yTickCount = n;
	SetyTickLabels();
}

void cPlot::SetRightMargin(int Margin)
{
	m_RightMargin = Margin;
}

void cPlot::SetLeftMargin(int Margin)
{
	m_LeftMargin = Margin;
}

void cPlot::SetTopMargin(int Margin)
{
	m_TopMargin = Margin;
}

void cPlot::SetBottomMargin(int Margin)
{
	m_BottomMargin = Margin;
}

void cPlot::SetxGridVisible(bool Visible)
{
	m_xGridVisible = Visible;
}

void cPlot::SetyGridVisible(bool Visible)
{
	m_yGridVisible = Visible;
}

void cPlot::SetxTicksVisible(bool Visible)
{
	m_xTicksVisible = Visible;
}

void cPlot::SetyTicksVisible(bool Visible)
{
	m_yTicksVisible = Visible;
}

void cPlot::SetxMinorTicksVisible(bool Visible)
{
	m_xMinorTicksVisible = Visible;
}

void cPlot::SetyMinorTicksVisible(bool Visible)
{
	m_yMinorTicksVisible = Visible;
}

void cPlot::SetBackgroundColour(wxColour colour)
{
	m_BackgroundColor = colour;
}

void cPlot::SetTickColour(wxColour colour)
{
	m_TickColor = colour;
}

void cPlot::SetGridColour(wxColour colour)
{
	m_GridColor = colour;
}

void cPlot::SetxLogAxis(bool LogAxis)
{
	m_xLogAxis = LogAxis;
	if (m_xLogAxis) {
		// calculate Tick-Count
		m_xTickCount = (int)(log10(m_xMax) + abs(log10(m_xMin))) + 1;
		SetxTickLabels();
	}
}

void cPlot::SetyLogAxis(bool LogAxis)
{
	m_yLogAxis = LogAxis;


	if (m_yLogAxis) {
		// calculate Tick-Count
		m_yTickCount = (int)(log10(m_yMax) + abs(log10(m_yMin))) + 1;
		SetyTickLabels();
	}
}


void cPlot::render(wxDC& dc)
{
	dc.SetBackground(wxBrush(m_BackgroundColor, wxBRUSHSTYLE_SOLID));
	dc.Clear();

	int xSize, ySize;
	dc.GetSize(&xSize, &ySize);
	
	//draw grid
	if(m_xGridVisible) renderXGrid(dc);
	if(m_yGridVisible) renderYGrid(dc);
	
	//draw ticks
	if (m_xTicksVisible) renderXTicks(dc);
	if (m_yTicksVisible) renderYTicks(dc);

	//draw graphs
	dc.SetClippingRegion(m_LeftMargin, m_TopMargin, xSize - m_RightMargin-m_LeftMargin, ySize - m_BottomMargin-m_TopMargin);
	for (auto graph : m_graphs)
	{
		graph.SetxLim(m_xMin, m_xMax);
		graph.SetyLim(m_yMin, m_yMax);

		graph.render(dc, m_LeftMargin, m_TopMargin, xSize - m_RightMargin, ySize - m_BottomMargin,m_xLogAxis,m_yLogAxis);
	}
}

void cPlot::GetMinMax()
{
	// set same x and ylimit for all graphs
	//double m_xMin, m_xMax, m_yMin, m_yMax;
	m_graphs[0].GetxLim(m_xMin, m_xMax);
	m_graphs[0].GetyLim(m_yMin, m_yMax);

	for (unsigned int i = 1; i < m_graphs.size(); i++)
	{
		double Max, Min;
		m_graphs[i].GetxLim(Min, Max);
		if (Min < m_xMin) m_xMin = Min;
		if (Max > m_xMax) m_xMax = Max;

		m_graphs[i].GetyLim(Min, Max);
		if (Min < m_yMin) m_yMin = Min;
		if (Max > m_yMax) m_yMax = Max;
	}
}

void cPlot::AddGraph(cGraph *Graph)
{
	m_graphs.push_back(*Graph);

	GetMinMax();
	SetxTickLabels();
	SetyTickLabels();
}

void cPlot::UpdateGraph(int index, cGraph* Graph)
{
	if (m_graphs.size() > index && index >= 0)
	{
		m_graphs[index] = *Graph;
		Update();
	}
}
