#pragma once

#include "wx/wx.h"
#include <iostream>
#include <string>
#include <vector>
#include "cAudioRecorder.h"


class cDlgInputDevice : public wxDialog
{
public:
	cDlgInputDevice(std::vector<std::string>& AudioDevices, std::string currentDevice);
	~cDlgInputDevice();
	std::string getDeviceName();
	int getSampleRate();

protected:
	wxListBox* m_listInputDevices;
	wxComboBox* m_cbSampleRate;
	wxStaticText* m_stMaxFrequency;
	wxButton* m_btnOK;

private:

	void OnSamplerateChanged(wxCommandEvent& event);
	void OnBtnOKClicked(wxCommandEvent& event);
	
	std::vector<std::string> sAvailableDevices;
	std::string sDeviceName;

	unsigned int SampleRate = 48000;

	DECLARE_EVENT_TABLE()
};

