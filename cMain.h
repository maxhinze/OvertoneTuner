#pragma once

#include "wx/wx.h"
#include "wx/dataview.h"
#include "wx/statline.h"
#include <wx/aboutdlg.h>

#include "cDlgInputDevice.h"
#include "cPlot.h"
#include "cMyFFT.h"
#include "cMusicScale.h"
#include "cPeakDetection.h"


#define TIMER_ID 8128

enum MenuButtons
{
	ID_BTN_NEW = 1001,
	ID_BTN_OPEN = 1002,
	ID_BTN_SAVE = 1003,
	ID_BTN_QUIT = 1004,
	ID_BTN_INPUT = 1101,

	ID_RB_FFT_WIN_RECTANGLE=1120,
	ID_RB_FFT_WIN_HANNING =	1121,
	ID_RB_FFT_WIN_HAMMING =	1122,

	ID_CB_V_AUDIO = 1201,
	ID_CB_V_FFT = 1202,
	ID_CB_V_WATERFALL = 1203,
};

class cMain : public wxFrame
{
public:
	cMain();
	~cMain();
	void InitUI();


protected:
	wxMenuBar* m_menubar;
	wxMenu* m_menuMain;
	wxMenu* m_menuSettings;
	wxMenu* m_menuFFT;
	wxMenu* m_menuFFTsize;
	wxMenu* m_menuFFTwindow;
	wxMenu* m_menuView;
	wxMenu* m_menuAbout;
	wxStatusBar* m_statusBar;

	wxMenuItem* m_cbVisibleLiveAudio;
	wxMenuItem* m_cbVisibleFFT;
	wxMenuItem* m_cbVGisibleWaterfall;


	wxBoxSizer* m_MainSizer;

	wxStaticBoxSizer* m_sbLiveAudioSizer;	//Box for drawing live Audio Data
	wxSlider* m_slTriggerLevel;
	wxStaticText* m_stTriggerLevel;
	cPlot* m_plWaveformDisplay;
	cGraph* m_grLiveAudio;
	cGraph* m_grTriggerLevel;

	wxStaticBoxSizer* m_sbFFTSizer;			//Box for drawing FFT-Analysis
	cPlot* m_plSpectrumDisplay;
	cGraph* m_grLiveSpectrum;
	cGraph* m_grLiveSpectrumAverage;
	wxStaticText* m_stFFTSize;
	wxStaticText* m_stFFTresolution;

	wxStaticBoxSizer* m_sbWaterfallSizer;	//Box for drawing Waterfall-Analysis

	wxStaticBoxSizer* m_sbPeakListSizer;	// Box for Peak-List
	wxStaticText* m_stNote;
	wxStaticText* m_stCent;
	wxDataViewListCtrl* m_PeakList;


	
	

	
	

private:
	void OnBtnQuitClicked(wxCommandEvent& event);
	void OnBtnInputSettingsClicked(wxCommandEvent& event);
	
	void OnCbViewClicked(wxCommandEvent& event);
	void OnRbFFTsizeClicked(wxCommandEvent& event);
	void OnRbFFTwindowClicked(wxCommandEvent& event);
	void OnAbout(wxCommandEvent& event);

	void OnUpdateAudioDisplay(wxTimerEvent& event);
	void OnTriggerLevelChanged(wxCommandEvent& event);


	void LayoutDynamicWindows();

	void drawTriggerLine(float level);
	void updateFFTtext();

	cAudioRecorder* myRecorder;
	cMyFFT* myFFT;
	cMusicScale* myEqualScale;


	wxTimer* m_timer;

	unsigned int m_SampleRate;
	unsigned int m_BufferSize;
	

	DECLARE_EVENT_TABLE()
};

