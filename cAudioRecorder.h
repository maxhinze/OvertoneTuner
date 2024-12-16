#pragma once

#include "wx/wx.h"
#include "SFML/Audio.hpp"
#include <queue>

//wxDECLARE_EVENT(NEW_AUDIO_AVAILABLE, wxCommandEvent);
//wxDEFINE_EVENT(NEW_AUDIO_AVAILABLE, wxCommandEvent);

class cAudioRecorder : public sf::SoundRecorder
{
public:
	cAudioRecorder(wxFrame *parent);
	~cAudioRecorder();

	void setBufferSize(int Size);
	int getBufferSize();
	void setTreshold(float treshold);
	bool newSamplesAvailable();


	std::vector<double> getSamples();

private:
	virtual bool onStart();
	virtual bool onProcessSamples(const sf::Int16* samples, std::size_t sampleCount);
	virtual void onStop();

private:
	float m_Treshold = 0;		//Schwellwert
	int m_BufferSize = 65536;   //Buffersize (important for FFT)

	bool m_trigger = false;		//Trigger if Threshold is reached
	bool m_newSamplesAvailable = false;


	float m_Pretrigger = 0.1;	//pretrigger = 10%

	std::vector<double> m_samples;

	std::queue<double> m_preTriggerBuffer;

	wxFrame *m_parent;
};

