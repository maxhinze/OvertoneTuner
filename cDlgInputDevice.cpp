#include "cDlgInputDevice.h"

wxBEGIN_EVENT_TABLE(cDlgInputDevice, wxDialog)
	//EVT_BUTTON(wxID_OK, OnBtnOKClicked)
wxEND_EVENT_TABLE()





cDlgInputDevice::cDlgInputDevice(std::vector<std::string> &AudioDevices, std::string currentDevice)
	: wxDialog(NULL, -1, "Eingabegerät", wxDefaultPosition, wxSize(400,200))
{

	//Assemble the Dialog
	wxBoxSizer* bSizer1;
	bSizer1 = new wxBoxSizer(wxVERTICAL);

	wxStaticBoxSizer* sbSizer1;
	sbSizer1 = new wxStaticBoxSizer(new wxStaticBox(this, wxID_ANY, wxT("Auswahl des Eingabegeräts")), wxVERTICAL);

	m_listInputDevices = new wxListBox(sbSizer1->GetStaticBox(), wxID_ANY);
	sbSizer1->Add(m_listInputDevices, 1, wxALL | wxEXPAND, 5);

	wxFlexGridSizer* fgSizer1;
	fgSizer1 = new wxFlexGridSizer(2, 3, 0, 0);
	fgSizer1->SetFlexibleDirection(wxBOTH);
	fgSizer1->SetNonFlexibleGrowMode(wxFLEX_GROWMODE_SPECIFIED);

	wxStaticText* m_staticText1;
	m_staticText1 = new wxStaticText(sbSizer1->GetStaticBox(), wxID_ANY, wxT("Abtastrate:"), wxDefaultPosition, wxDefaultSize, 0);
	m_staticText1->Wrap(-1);
	fgSizer1->Add(m_staticText1, 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);

	m_cbSampleRate = new wxComboBox(sbSizer1->GetStaticBox(), wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0, NULL, wxCB_READONLY);
	m_cbSampleRate->Append(wxT("8000"));
	m_cbSampleRate->Append(wxT("11025"));
	m_cbSampleRate->Append(wxT("16000"));
	m_cbSampleRate->Append(wxT("22050"));
	m_cbSampleRate->Append(wxT("32000"));
	m_cbSampleRate->Append(wxT("44100"));
	m_cbSampleRate->Append(wxT("48000"));
	m_cbSampleRate->Append(wxT("96000"));
	m_cbSampleRate->Append(wxT("176400"));
	m_cbSampleRate->Append(wxT("192000"));
	m_cbSampleRate->Append(wxT("352800"));
	m_cbSampleRate->Append(wxT("384000"));
	m_cbSampleRate->SetSelection(6);
	fgSizer1->Add(m_cbSampleRate, 0, wxALIGN_CENTER_VERTICAL, 5);

	wxStaticText * m_staticText2;
	m_staticText2 = new wxStaticText(sbSizer1->GetStaticBox(), wxID_ANY, wxT("Hz"), wxDefaultPosition, wxDefaultSize, wxALIGN_CENTER_HORIZONTAL);
	m_staticText2->Wrap(-1);
	fgSizer1->Add(m_staticText2, 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);

	wxStaticText * m_staticText3;
	m_staticText3 = new wxStaticText(sbSizer1->GetStaticBox(), wxID_ANY, wxT("max. Frequenz:"), wxDefaultPosition, wxDefaultSize, 0);
	m_staticText3->Wrap(-1);
	fgSizer1->Add(m_staticText3, 0, wxALL | wxALIGN_CENTER_VERTICAL, 5);

	m_stMaxFrequency = new wxStaticText(sbSizer1->GetStaticBox(), wxID_ANY, wxT("24000"), wxDefaultPosition, wxDefaultSize, 0);
	m_stMaxFrequency->Wrap(-1);
	fgSizer1->Add(m_stMaxFrequency, 0, wxALL | wxALIGN_RIGHT | wxALIGN_CENTER_VERTICAL, 5);

	wxStaticText * m_staticText4;
	m_staticText4 = new wxStaticText(sbSizer1->GetStaticBox(), wxID_ANY, wxT("Hz"), wxDefaultPosition, wxDefaultSize, wxALIGN_CENTER_HORIZONTAL);
	m_staticText4->Wrap(-1);
	fgSizer1->Add(m_staticText4, 0, wxALL | wxALIGN_CENTER_VERTICAL, 5);


	sbSizer1->Add(fgSizer1, 1, wxEXPAND, 5);

	bSizer1->Add(sbSizer1, 1, wxEXPAND|wxALL, 5);

	m_btnOK = new wxButton(this, wxID_OK, wxT("Ok"));
	bSizer1->Add(m_btnOK, 0, wxALL | wxALIGN_CENTER_HORIZONTAL, 5);


	m_cbSampleRate->Bind(wxEVT_COMMAND_COMBOBOX_SELECTED, &cDlgInputDevice::OnSamplerateChanged, this);
	m_btnOK->Bind(wxEVT_COMMAND_BUTTON_CLICKED, &cDlgInputDevice::OnBtnOKClicked, this);


	this->SetSizer(bSizer1);
	this->Layout();

	this->Centre(wxBOTH);

	//Populate Device-List with available Devices
	for (auto AudioDevice : AudioDevices)
	{
		sAvailableDevices.push_back(AudioDevice);
		m_listInputDevices->AppendString(wxString(AudioDevice));
	}
	m_listInputDevices->SetStringSelection(wxString(currentDevice));

	ShowModal();

	//Destroy();
}


cDlgInputDevice::~cDlgInputDevice()
{
}

void cDlgInputDevice::OnSamplerateChanged(wxCommandEvent& event)
{
	//get Value from Combobox
	wxString str = m_cbSampleRate->GetStringSelection();
	SampleRate = wxAtoi(str);

	//calculate max. Frequency and set Label
	int maxFreq = SampleRate / 2;
	m_stMaxFrequency->SetLabel(wxString::Format("%d", maxFreq));

	event.Skip();
}

void cDlgInputDevice::OnBtnOKClicked(wxCommandEvent& event)
{
	//get Selected InputDevice and close the Dialog
	int Selection = m_listInputDevices->GetSelection();
	//sDeviceName = m_listInputDevices->GetStringSelection();
	sDeviceName = sAvailableDevices[Selection];
	EndModal(wxID_OK);
}

std::string cDlgInputDevice::getDeviceName()
{
	return sDeviceName;
}

int cDlgInputDevice::getSampleRate()
{
	return SampleRate;
}
