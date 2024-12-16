#pragma once

#include "wx/wx.h"
#include <wx/dcbuffer.h>
#include "wx/string.h"
#include "cGraph.h"
#include <vector>
#include <string>

// some useful events
	/*
	 void mouseMoved(wxMouseEvent& event);
	 void mouseDown(wxMouseEvent& event);
	 void mouseWheelMoved(wxMouseEvent& event);
	 void mouseReleased(wxMouseEvent& event);
	 void rightClick(wxMouseEvent& event);
	 void mouseLeftWindow(wxMouseEvent& event);
	 void keyPressed(wxKeyEvent& event);
	 void keyReleased(wxKeyEvent& event);
	 */


class cPlot : public wxPanel
{
public:
	cPlot(wxFrame* parent);
	~cPlot();

	void Update();

	void paintEvent(wxPaintEvent& evt);
	void paintNow();
	
	void renderXGrid(wxDC& dc);
	void renderYGrid(wxDC& dc);

	void renderXTicks(wxDC& dc);
	void renderYTicks(wxDC& dc);

	void SetxTickLabels();	//No Argument--> calculate xLabels
	void SetyTickLabels();
	
	void SetxTickCount(int n);
	void SetyTickCount(int n);

	void SetRightMargin(int Margin);
	void SetLeftMargin(int Margin);
	void SetTopMargin(int Margin);
	void SetBottomMargin(int Margin);

	void SetxGridVisible(bool Visible);
	void SetyGridVisible(bool Visible);

	void SetxTicksVisible(bool Visible);
	void SetyTicksVisible(bool Visible);
	void SetxMinorTicksVisible(bool Visible);
	void SetyMinorTicksVisible(bool Visible);

	void SetBackgroundColour(wxColour colour);
	void SetTickColour(wxColour colour);
	void SetGridColour(wxColour colour);

	void SetxLogAxis(bool LogAxis);
	void SetyLogAxis(bool LogAxis);

	void render(wxDC& dc);

	void GetMinMax();	//Get min and maximum x and y values of the vector

	void AddGraph(cGraph *Graph);


	void UpdateGraph(int index, cGraph* Graph);


private:
	int m_xTickCount;
	int m_yTickCount;

	int m_RightMargin;
	int m_LeftMargin;
	int m_TopMargin;
	int m_BottomMargin;

	double m_xMin;
	double m_xMax;
	double m_yMin;
	double m_yMax;

	bool m_xLogAxis = false;
	bool m_yLogAxis = false;
	
	bool m_xTicksVisible = true;
	bool m_yTicksVisible = true;

	bool m_xMinorTicksVisible = true;
	bool m_yMinorTicksVisible = true;

	bool m_xGridVisible = true;
	bool m_yGridVisible = true;

	wxColour m_BackgroundColor;
	wxColour m_TickColor;
	wxColour m_GridColor;

	std::vector<wxString> m_xTicks;
	std::vector<wxString> m_yTicks;

	std::vector<cGraph> m_graphs;

	DECLARE_EVENT_TABLE()

};

