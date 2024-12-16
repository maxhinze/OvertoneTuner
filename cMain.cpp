#include "cMain.h"

wxBEGIN_EVENT_TABLE(cMain, wxFrame)
	EVT_TIMER(TIMER_ID, cMain::OnUpdateAudioDisplay)
//EVT_COMMAND(wxID_ANY, NEW_AUDIO_AVAILABLE, cMain::onUpdateAudioDisplay)
wxEND_EVENT_TABLE()





cMain::cMain() : wxFrame(nullptr, wxID_ANY, "OvertoneTuner")
{
	this->SetSizeHints(wxSize(800, 600), wxDefaultSize);
	this->SetBackgroundColour(wxSystemSettings::GetColour(wxSYS_COLOUR_WINDOW));

	m_SampleRate = 48000;
	m_BufferSize = 65536;

	// Init Scale
	myEqualScale = new cMusicScale(442.0);

	// Init FFT
	myFFT = new cMyFFT(m_SampleRate, m_BufferSize);
	myFFT->setWindow(FFT_WIN_HANNING);
	//Start Audio Recorder
	myRecorder = new cAudioRecorder((wxFrame*)this);
	myRecorder->setBufferSize(m_BufferSize);
	myRecorder->setTreshold(0.5);


	InitUI();
	//updateFFTtext();

	// Init Live Audio Graph
	m_grLiveAudio = new cGraph();
	m_grLiveAudio->SetPen(wxPen(wxColor(255, 0, 0), 1)); //red
	m_grLiveAudio->SetyLim(-1, 1);
	m_grLiveAudio->AppendData({0,0});
	m_grLiveAudio->AppendData({ (1.0/ m_SampleRate)* m_BufferSize,0 });

	m_grTriggerLevel = new cGraph();
	m_grTriggerLevel->SetPen(wxPen(wxColor(120, 120,120), 1)); //grey
	m_grTriggerLevel->SetyLim(-1, 1);
	m_grTriggerLevel->AppendData({ 0,0.5 });
	m_grTriggerLevel->AppendData({ (1.0 / m_SampleRate) * m_BufferSize,0.5 });


	m_plWaveformDisplay->AddGraph(m_grLiveAudio);
	m_plWaveformDisplay->AddGraph(m_grTriggerLevel);
	m_plWaveformDisplay->SetyTickCount(3);
	m_plWaveformDisplay->SetBackgroundColour(*wxWHITE);
	m_plWaveformDisplay->SetTickColour(*wxBLACK);
	m_plWaveformDisplay->SetyMinorTicksVisible(false);
	m_plWaveformDisplay->SetxMinorTicksVisible(false);
	
	m_plWaveformDisplay->UpdateGraph(0, m_grLiveAudio);


	// Init Live FFT Graph
	m_grLiveSpectrum = new cGraph();
	m_grLiveSpectrum->SetPen(wxPen(wxColor(255, 0, 0), 1)); //red
	m_grLiveSpectrum->SetyLim(0.001, 100);
	m_grLiveSpectrum->SetxLim(24, 24000);


	m_grLiveSpectrumAverage = new cGraph();
	m_grLiveSpectrumAverage->SetPen(wxPen(wxColor(0, 0, 255), 1)); //blue
	m_grLiveSpectrumAverage->SetyLim(0.001, 100);
	m_grLiveSpectrumAverage->SetxLim(24, 24000);



	m_plSpectrumDisplay->AddGraph(m_grLiveSpectrum);
	m_plSpectrumDisplay->AddGraph(m_grLiveSpectrumAverage);
	m_plSpectrumDisplay->SetBackgroundColour(*wxWHITE);
	m_plSpectrumDisplay->SetTickColour(*wxBLACK);
	
	m_plSpectrumDisplay->SetyMinorTicksVisible(false);
	m_plSpectrumDisplay->SetxMinorTicksVisible(true);
	m_plSpectrumDisplay->SetyLogAxis(true);
	m_plSpectrumDisplay->SetxLogAxis(true);
	
	m_plSpectrumDisplay->UpdateGraph(0, m_grLiveSpectrum);
	m_plSpectrumDisplay->UpdateGraph(1, m_grLiveSpectrumAverage);

	//this->Bind(NEW_AUDIO_AVAILABLE, &cMain::onUpdateAudioDisplay, this);

	myRecorder->start(m_SampleRate);

	m_timer = new wxTimer(this, TIMER_ID);
	m_timer->Start(10, wxTIMER_CONTINUOUS);
}


cMain::~cMain()
{
	m_timer->Stop();
	Disconnect(ID_BTN_QUIT, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler(cMain::OnBtnQuitClicked));
	Disconnect(ID_BTN_INPUT, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler(cMain::OnBtnInputSettingsClicked));
	Disconnect(ID_CB_V_AUDIO, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler(cMain::OnCbViewClicked));
	Disconnect(ID_CB_V_FFT, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler(cMain::OnCbViewClicked));
	Disconnect(ID_CB_V_FFT, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler(cMain::OnCbViewClicked));
}

void cMain::InitUI()
{
	// Assemble Menue Bar
	m_menubar = new wxMenuBar();
	m_menuMain = new wxMenu();

	// Main Menu
	wxMenuItem* m_btnNew;
	m_btnNew = new wxMenuItem(m_menuMain, ID_BTN_NEW, "Neu", "Neues Stimmprojekt starten.");
	m_menuMain->Append(m_btnNew);
	m_btnNew->Enable(false);

	wxMenuItem* m_btnOpen;
	m_btnOpen = new wxMenuItem(m_menuMain, ID_BTN_OPEN, "Öffnen", "Stimmprojekt öffnen.");
	m_menuMain->Append(m_btnOpen);
	m_btnOpen->Enable(false);

	wxMenuItem* m_btnSave;
	m_btnSave = new wxMenuItem(m_menuMain, ID_BTN_SAVE, "Speichern", "Stimmprojekt speichern.");
	m_menuMain->Append(m_btnSave);
	m_btnSave->Enable(false);

	m_menuMain->AppendSeparator();

	wxMenuItem* m_btnQuit;
	m_btnQuit = new wxMenuItem(m_menuMain, ID_BTN_QUIT, "Beenden", "Programm beenden.");
	m_menuMain->Append(m_btnQuit);
	Connect(ID_BTN_QUIT, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler(cMain::OnBtnQuitClicked));
	
	m_menubar->Append(m_menuMain, "Datei");


	// Settings Menu
	m_menuSettings = new wxMenu();
	wxMenuItem* m_btnInputDevice;
	m_btnInputDevice = new wxMenuItem(m_menuSettings, ID_BTN_INPUT, "Eingabegerät", "Audio-Eingabegerät auswählen.");
	m_menuSettings->Append(m_btnInputDevice);
	Connect(ID_BTN_INPUT, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler(cMain::OnBtnInputSettingsClicked));

	m_menubar->Append(m_menuSettings, "Einstellungen");

	//FFT Menue
	m_menuFFT = new wxMenu();
	wxMenuItem* m_menuFFTItem = new wxMenuItem(m_menuSettings, wxID_ANY, wxT("FFT"), wxEmptyString, wxITEM_NORMAL, m_menuFFT);
	m_menuFFTsize = new wxMenu();
	wxMenuItem* m_menuFFTsizeItem = new wxMenuItem(m_menuFFT, wxID_ANY, wxT("FFT Größe"), wxEmptyString, wxITEM_NORMAL, m_menuFFTsize);

	wxMenuItem* m_menuItemFFTSizeValues[10];

	// id 1110 to 1119
	// fft size 2048 to 1048576
	for (unsigned int i = 0; i < 10; i++)
	{
		m_menuItemFFTSizeValues[i] = new wxMenuItem(m_menuFFTsize, 1110 + i, wxString::Format("%d", (int)std::pow(2,i+11)), wxEmptyString, wxITEM_RADIO);
		m_menuFFTsize->Append(m_menuItemFFTSizeValues[i]);
		Connect(1110 + i, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler(cMain::OnRbFFTsizeClicked));
	}
	m_menuItemFFTSizeValues[5]->Check(true);	// std. FFt Size = 65536
	m_menuFFT->Append(m_menuFFTsizeItem);

	m_menuFFTwindow = new wxMenu();
	wxMenuItem* m_menuFFTwindowItem = new wxMenuItem(m_menuFFT, wxID_ANY, wxT("FFT Fenster"), wxEmptyString, wxITEM_NORMAL, m_menuFFTwindow);
	wxMenuItem* m_menueWinRect;
	m_menueWinRect = new wxMenuItem(m_menuFFTwindow, ID_RB_FFT_WIN_RECTANGLE, wxString(wxT("Rechteck")), wxEmptyString, wxITEM_RADIO);
	m_menuFFTwindow->Append(m_menueWinRect);
	Connect(ID_RB_FFT_WIN_RECTANGLE, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler(cMain::OnRbFFTwindowClicked));
	

	wxMenuItem* m_menueWinHan;
	m_menueWinHan = new wxMenuItem(m_menuFFTwindow, ID_RB_FFT_WIN_HANNING, wxString(wxT("Hanning")), wxEmptyString, wxITEM_RADIO);
	m_menuFFTwindow->Append(m_menueWinHan);
	m_menueWinHan->Check(true);
	Connect(ID_RB_FFT_WIN_HANNING, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler(cMain::OnRbFFTwindowClicked));

	wxMenuItem* m_menueWinHam;
	m_menueWinHam = new wxMenuItem(m_menuFFTwindow, ID_RB_FFT_WIN_HAMMING, wxString(wxT("Hamming")), wxEmptyString, wxITEM_RADIO);
	m_menuFFTwindow->Append(m_menueWinHam);
	Connect(ID_RB_FFT_WIN_HAMMING, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler(cMain::OnRbFFTwindowClicked));

	m_menuFFT->Append(m_menuFFTwindowItem);

	m_menuSettings->Append(m_menuFFTItem);

	// Visible Menu
	m_menuView = new wxMenu();
	m_cbVisibleLiveAudio = new wxMenuItem(m_menuView, ID_CB_V_AUDIO, "LiveAudio", "Live-Audio darstellen.", wxITEM_CHECK);
	m_menuView->Append(m_cbVisibleLiveAudio);
	m_cbVisibleLiveAudio->Check(true);
	Connect(ID_CB_V_AUDIO, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler(cMain::OnCbViewClicked));

	m_cbVisibleFFT = new wxMenuItem(m_menuView, ID_CB_V_FFT, "FFT", "FFT darstellen.", wxITEM_CHECK);
	m_menuView->Append(m_cbVisibleFFT);
	m_cbVisibleFFT->Check(true);
	Connect(ID_CB_V_FFT, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler(cMain::OnCbViewClicked));

	m_cbVGisibleWaterfall = new wxMenuItem(m_menuView, ID_CB_V_WATERFALL, "Wasserfall", "Wasserfall darstellen.", wxITEM_CHECK);
	m_menuView->Append(m_cbVGisibleWaterfall);
	m_cbVGisibleWaterfall->Enable(false);
	m_cbVGisibleWaterfall->Check(false);
	Connect(ID_CB_V_FFT, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler(cMain::OnCbViewClicked));

	m_menubar->Append(m_menuView, "Ansicht");


	// About Menu
	m_menuAbout = new wxMenu();
	m_menuAbout->Append(wxID_ABOUT, "Hilfe");
	m_menubar->Append(m_menuAbout, "Über");
	Connect(wxID_ABOUT, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler(cMain::OnAbout));

	this->SetMenuBar(m_menubar);


	//Main Screens
	m_MainSizer = new wxBoxSizer(wxVERTICAL);

	//////////////////////////////////////////////////////////////////////
	// Live Audio Screen												//
	//////////////////////////////////////////////////////////////////////
	m_sbLiveAudioSizer = new wxStaticBoxSizer(new wxStaticBox(this, wxID_ANY, "LiveAudio"), wxHORIZONTAL);
	m_plWaveformDisplay = new cPlot((wxFrame*)this);
	m_sbLiveAudioSizer->Add(m_plWaveformDisplay, 1, wxEXPAND);

	wxFlexGridSizer* fgSizer1;
	fgSizer1 = new wxFlexGridSizer(3, 1, 0, 0);
	fgSizer1->AddGrowableCol(0);
	fgSizer1->AddGrowableRow(1);
	fgSizer1->SetFlexibleDirection(wxBOTH);
	fgSizer1->SetNonFlexibleGrowMode(wxFLEX_GROWMODE_ALL);

	wxStaticText* m_staticText2;
	m_staticText2 = new wxStaticText(m_sbLiveAudioSizer->GetStaticBox(), wxID_ANY, wxT("Level"), wxDefaultPosition, wxDefaultSize, 0);
	m_staticText2->Wrap(-1);
	fgSizer1->Add(m_staticText2, 0, wxALL, 5);

	m_slTriggerLevel = new wxSlider(m_sbLiveAudioSizer->GetStaticBox(), wxID_ANY, 50, 0, 100, wxDefaultPosition, wxDefaultSize, wxSL_INVERSE | wxSL_LEFT | wxSL_VERTICAL);
	fgSizer1->Add(m_slTriggerLevel, 1, wxALL | wxEXPAND, 0);

	m_stTriggerLevel = new wxStaticText(m_sbLiveAudioSizer->GetStaticBox(), wxID_ANY, wxT(" 0.50"), wxDefaultPosition, wxDefaultSize, wxALIGN_LEFT);
	m_stTriggerLevel->Wrap(-1);
	fgSizer1->Add(m_stTriggerLevel, 0, wxALL | wxEXPAND, 5);


	m_sbLiveAudioSizer->Add(fgSizer1, 0, wxALL|wxEXPAND, 5);

	m_MainSizer->Add(m_sbLiveAudioSizer, 1, wxEXPAND, 5);

	//////////////////////////////////////////////////////////////////////
	// FFT Screen														//
	//////////////////////////////////////////////////////////////////////
	m_sbFFTSizer = new wxStaticBoxSizer(new wxStaticBox(this, wxID_ANY, "FFT"), wxVERTICAL);
	
	wxBoxSizer* bSizer2;
	bSizer2 = new wxBoxSizer(wxHORIZONTAL);
	/*
	wxStaticText* m_staticText4;
	m_staticText4 = new wxStaticText(m_sbFFTSizer->GetStaticBox(), wxID_ANY, wxT(" Größe: "), wxDefaultPosition, wxDefaultSize, 0);
	m_staticText4->Wrap(-1);
	bSizer2->Add(m_staticText4, 0, 0, 5);

	m_stFFTSize = new wxStaticText(m_sbFFTSizer->GetStaticBox(), wxID_ANY, wxT("-"), wxDefaultPosition, wxDefaultSize, 0);
	m_stFFTSize->Wrap(-1);
	bSizer2->Add(m_stFFTSize, 0, wxRIGHT, 5);

	wxStaticText* m_staticText5;
	m_staticText5 = new wxStaticText(m_sbFFTSizer->GetStaticBox(), wxID_ANY, wxT(" Auflösung:"), wxDefaultPosition, wxDefaultSize, 0);
	m_staticText5->Wrap(-1);
	bSizer2->Add(m_staticText5, 0, wxLEFT, 5);

	m_stFFTresolution = new wxStaticText(m_sbFFTSizer->GetStaticBox(), wxID_ANY, wxT("- Hz"), wxDefaultPosition, wxDefaultSize, 0);
	m_stFFTresolution->Wrap(-1);
	bSizer2->Add(m_stFFTresolution, 0, 0, 5);

	m_sbFFTSizer->Add(bSizer2, 0, wxALIGN_LEFT, 5);
	*/

	m_plSpectrumDisplay = new cPlot((wxFrame*)this);
	m_sbFFTSizer->Add(m_plSpectrumDisplay, 1, wxEXPAND);

	m_MainSizer->Add(m_sbFFTSizer, 1, wxEXPAND, 5);

	m_sbWaterfallSizer = new wxStaticBoxSizer(new wxStaticBox(this, wxID_ANY, "Waterfall"), wxVERTICAL);
	m_MainSizer->Add(m_sbWaterfallSizer, 1, wxEXPAND, 5);


	//////////////////////////////////////////////////////////////////////
	// Peak List Screen													//
	//////////////////////////////////////////////////////////////////////
	m_sbPeakListSizer = new wxStaticBoxSizer(new wxStaticBox(this, wxID_ANY, wxT("Peaks")), wxHORIZONTAL);
	
	wxBoxSizer* bSizer3;
	bSizer3 = new wxBoxSizer(wxVERTICAL);

	m_stNote = new wxStaticText(m_sbPeakListSizer->GetStaticBox(), wxID_ANY, wxT("-"), wxDefaultPosition, wxSize(-1, -1), 0);
	m_stNote->Wrap(-1);
	m_stNote->SetFont(wxFont(52, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_BOLD, false, wxEmptyString));
	
	bSizer3->Add(m_stNote, 0, wxALIGN_CENTER_HORIZONTAL, 5);

	m_stCent = new wxStaticText(m_sbPeakListSizer->GetStaticBox(), wxID_ANY, wxT("- cent"), wxDefaultPosition, wxDefaultSize, 0);
	m_stCent->Wrap(-1);
	m_stCent->SetFont(wxFont(20, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL, false, wxEmptyString));

	bSizer3->Add(m_stCent, 0, wxALL | wxALIGN_CENTER_HORIZONTAL, 5);

	m_sbPeakListSizer->Add(bSizer3, 1, wxEXPAND, 5);
	
	m_PeakList = new wxDataViewListCtrl(m_sbPeakListSizer->GetStaticBox(), wxID_ANY);
	m_PeakList->AppendTextColumn("Note");
	m_PeakList->AppendTextColumn("Frequenz");
	m_PeakList->AppendTextColumn("Cent");
	
	m_sbPeakListSizer->Add(m_PeakList, 1, wxEXPAND, 5);
	m_MainSizer->Add(m_sbPeakListSizer, 1, wxEXPAND, 5);


	LayoutDynamicWindows();


	this->SetSizer(m_MainSizer);
	this->Layout();
	m_statusBar = this->CreateStatusBar(1, wxSTB_SIZEGRIP, wxID_ANY);

	this->Centre(wxBOTH);
	
	// Connect Events
	m_slTriggerLevel->Bind(wxEVT_SLIDER, &cMain::OnTriggerLevelChanged, this);
}

void cMain::OnBtnQuitClicked(wxCommandEvent& event)
{
	m_timer->Stop();
	Close(true);
}

void cMain::OnBtnInputSettingsClicked(wxCommandEvent& event)
{
	//get availbable Audio Devices
	std::vector<std::string> availableRecordingDevices = myRecorder->getAvailableDevices();
	std::string currentDevice = myRecorder->getDevice();

	//loadAvailable Input devices in Selector Dialog
	cDlgInputDevice* InputDeviceDialog = new cDlgInputDevice(availableRecordingDevices, currentDevice);

	if (InputDeviceDialog->ShowModal() == wxID_OK)
	{
		currentDevice = InputDeviceDialog->getDeviceName();
		myRecorder->setDevice(currentDevice);
		m_SampleRate = InputDeviceDialog->getSampleRate();

		myRecorder->stop();
		myRecorder->start(m_SampleRate);

		//get Value from Slider and scale from 0-100 to 0-1.0
		float trigLevel = m_slTriggerLevel->GetValue() / 100.0;

		//redraw Line on Graph
		drawTriggerLine(trigLevel);

	}
	InputDeviceDialog->Destroy();

	event.Skip();
}

void cMain::OnCbViewClicked(wxCommandEvent& event)
{
	LayoutDynamicWindows();
	event.Skip();
}

void cMain::OnRbFFTsizeClicked(wxCommandEvent& event)
{
	if (event.GetId() >= 1110 && event.GetId() <= 1119)
	{
		// id 1110 to 1119
		// fft size 2048 to 1048576
		int id = event.GetId();
		unsigned int fftsize = (unsigned int)std::pow(2, 11 + (id - 1110));

		m_timer->Stop();
		myRecorder->stop();

		m_BufferSize = fftsize;
		myRecorder->setBufferSize(m_BufferSize);
		delete myFFT;
		myFFT = new cMyFFT(m_SampleRate, m_BufferSize);

		myRecorder->start(m_SampleRate);
		m_timer->Start(10, wxTIMER_CONTINUOUS);
	}
}

void cMain::OnRbFFTwindowClicked(wxCommandEvent& event)
{
	switch (event.GetId())
	{
	case ID_RB_FFT_WIN_RECTANGLE:
		myFFT->setWindow(FFT_WIN_RECTANGLE);
		break;
	case ID_RB_FFT_WIN_HANNING:
		myFFT->setWindow(FFT_WIN_HANNING);
		break;
	case ID_RB_FFT_WIN_HAMMING:
		myFFT->setWindow(FFT_WIN_HAMMING);
		break;
	default:
		break;
	}
}

void cMain::OnAbout(wxCommandEvent& event)
{
	wxAboutDialogInfo info;
	info.SetName("Overtone Tuner");
	info.SetVersion("0.0.1 Alpha");
	info.SetDescription("Programm zum Stimmen von Obertönen bei Instrumenten. Entwickelt speziell für das Stimmen von Malletinstrumenten.\r\n"
						"Für Fehler im Programm wird keine Haftung übernommen. Auftretende Fehler und Bugs können gerne über die unten \r\n"
						"gezeigte Website mitgeteilt werden.");
	info.SetCopyright("Copyright (c) 2020 Max Hinze");
	info.SetWebSite("http://www.maxhinze.de");
	wxAboutBox(info);
}


void cMain::OnUpdateAudioDisplay(wxTimerEvent& event)
{

	if (myRecorder->newSamplesAvailable())
	{
		//////////////////////////////////////////////////////////////////////
		// get AudioData													//
		//////////////////////////////////////////////////////////////////////
		std::vector<double> Samples;
		Samples =  myRecorder->getSamples();

		//put them in a propper Vector for Plotting
		std::vector<vector2d> Points;
		Points.reserve(Samples.size());

		double SampleRate = 1.0 / myRecorder->getSampleRate();
		for (int i = 0; i < Samples.size(); i++)
		{
			vector2d Point;
			Point.x = i * SampleRate;
			Point.y = Samples[i];
			Points.push_back(Point);
		}

		//update Graph
		m_grLiveAudio->SetData(Points);
		m_grLiveAudio->SetyLim(-1, 1);
		m_grLiveAudio->SetxLim(0, SampleRate * Samples.size());
		m_plWaveformDisplay->UpdateGraph(0, m_grLiveAudio);

		float value;

		//get Value from Slider and scale from 0-100 to 0-1.0
		value = m_slTriggerLevel->GetValue() / 100.0;
		drawTriggerLine(value);

		//////////////////////////////////////////////////////////////////////
		// Calculate FFT													//
		//////////////////////////////////////////////////////////////////////
		std::vector<double> SpectrumBins;
		SpectrumBins.reserve(Samples.size() / 2);
		myFFT->FFT(Samples, SpectrumBins);

		Points.clear();
		double resolution = myFFT->getResulution();
		double minFrequency = 1.0 / ((1.0/m_SampleRate) * m_BufferSize  );

		for (int i = 0; i < SpectrumBins.size(); i++)
		{
			vector2d Point;
			Point.x = i * resolution;// -10.0;
			Point.y = SpectrumBins[i];
			Points.push_back(Point);
		}

		//update FFT Graph
		m_grLiveSpectrum->SetData(Points);
		m_grLiveSpectrum->SetxLim(1, 100000);
		m_plSpectrumDisplay->UpdateGraph(0, m_grLiveSpectrum);


		//////////////////////////////////////////////////////////////////////
		// Find Peaks														//
		//////////////////////////////////////////////////////////////////////
		cPeakDetection peakDetector;
		std::vector<Peak> peaks;


		peakDetector.autoSetOffset(SpectrumBins);
		peaks = peakDetector.findPeaks(SpectrumBins);
		
		/*
		std::vector<double> SpectrumAverageBins = peakDetector.getmovingAverage();
		Points.clear();
		for (int i = 0; i < SpectrumBins.size()-255; i++)
		{
			vector2d Point;
			Point.x = (i+64) * resolution;// -10.0;
			Point.y = SpectrumAverageBins[i]* 4.0;
			Points.push_back(Point);
		}
		m_grLiveSpectrumAverage->SetData(Points);
		m_grLiveSpectrumAverage->SetxLim(1, 100000);
		m_plSpectrumDisplay->UpdateGraph(1, m_grLiveSpectrumAverage);
		*/

		m_PeakList->DeleteAllItems();
		wxVector<wxVariant> data;

		for (unsigned int i = 0; i<peaks.size(); i++)
		{
			double Frequency = peaks[i].position * resolution;

			key PeakKey = myEqualScale->findNearesNote(Frequency);
			double cent = myEqualScale->calcCent(PeakKey, Frequency);

			data.clear();
			data.push_back(wxVariant(wxString::Format("%s", PeakKey.name)));
			data.push_back(wxVariant(wxString::Format("%4.2f Hz", Frequency)));
			data.push_back(wxVariant(wxString::Format("%+2.2f Cent", cent)));
			m_PeakList->AppendItem(data);

			if (i == 0)
			{
				if (abs(cent) < 5.0)
				{
					m_stNote->SetForegroundColour(wxColour(  0,255, 0));
				}
				else
				{
					m_stNote->SetForegroundColour(wxColour(255,  0, 0));
				}

				m_stNote->SetLabel(wxString::Format("%s", PeakKey.name));
				m_stCent->SetLabel(wxString::Format("%+2.2f Cent", cent));
			}
		}
	}
	event.Skip();
}

void cMain::OnTriggerLevelChanged(wxCommandEvent& event)
{
	float value;

	//get Value from Slider and scale from 0-100 to 0-1.0
	value = m_slTriggerLevel->GetValue() / 100.0;
		
	//setValue in SliderText
	m_stTriggerLevel->SetLabel(wxString::Format(" %1.2f", value));

	// set Value as new Trigger-Level
	myRecorder->setTreshold(value);

	//redraw Line on Graph
	drawTriggerLine(value);
	event.Skip();
}

void cMain::LayoutDynamicWindows()
{
	//Check the Checkboxes, add Panel if set to Visible
	if (m_cbVisibleLiveAudio->IsChecked() )
		m_MainSizer->Show(m_sbLiveAudioSizer);
	else
		m_MainSizer->Hide(m_sbLiveAudioSizer);

	if (m_cbVisibleFFT->IsChecked() )
		m_MainSizer->Show(m_sbFFTSizer);
	else
		m_MainSizer->Hide(m_sbFFTSizer);
	
	if (m_cbVGisibleWaterfall->IsChecked() )
		m_MainSizer->Show(m_sbWaterfallSizer);
	else
		m_MainSizer->Hide(m_sbWaterfallSizer);
	
	this->Layout();
}

void cMain::drawTriggerLine(float level)
{
	m_grTriggerLevel->RollAppendData({ 0,level });
	m_grTriggerLevel->RollAppendData({ (1.0 / m_SampleRate) * m_BufferSize, level });
	m_plWaveformDisplay->UpdateGraph(1, m_grTriggerLevel);
}

void cMain::updateFFTtext()
{
	m_stFFTSize->SetLabel(wxString::Format(" %d   ", m_BufferSize));
	m_stFFTresolution->SetLabel(wxString::Format(" %f Hz", myFFT->getResulution()));
}
