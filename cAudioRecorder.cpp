#include "cAudioRecorder.h"



cAudioRecorder::cAudioRecorder(wxFrame* parent)
{
	setProcessingInterval(sf::seconds(0.01f)); //SetProcessingIntervall to 10ms
	m_parent = parent;
}


cAudioRecorder::~cAudioRecorder()
{
	stop();
}

void cAudioRecorder::setBufferSize(int Size)
{
	m_BufferSize = Size;
}

int cAudioRecorder::getBufferSize()
{
	return m_BufferSize;
}


void cAudioRecorder::setTreshold(float treshold)
{
	m_Treshold = treshold;
}

bool cAudioRecorder::newSamplesAvailable()
{
	return m_newSamplesAvailable;
}

std::vector<double> cAudioRecorder::getSamples()
{
	std::vector<double> tempSamples = m_samples;
	m_samples.clear();
	m_newSamplesAvailable = false;
	return  tempSamples;
}


bool cAudioRecorder::onStart()
{
	m_samples.clear();
	return true;
}

bool cAudioRecorder::onProcessSamples(const sf::Int16* samples, std::size_t sampleCount)
{

	unsigned int triggerPoint = 0;

	unsigned int PretriggerPoint = m_Pretrigger * m_BufferSize; //calculate PretriggerPoint

	if (m_trigger == false)
	{
		for (unsigned int i = 0; i < sampleCount; i++)
		{
			//Normalize
			float Sample = samples[i] / 32768.0;

			m_preTriggerBuffer.push(Sample);

			if (m_preTriggerBuffer.size() > PretriggerPoint)
			{
				m_preTriggerBuffer.pop();
			}

			// Schwellwertüberprüfung
			if (abs(Sample) >= m_Treshold)
			{
				while (m_preTriggerBuffer.size() > 0)
				{
					//load Samples in Buffer
					m_samples.push_back(m_preTriggerBuffer.front());
					m_preTriggerBuffer.pop();
				}
				m_trigger = true;
				triggerPoint = i;
				break;
			}
		}
	}

	if (m_trigger == true && m_newSamplesAvailable == false)
	{
		for (unsigned int i = triggerPoint; i < sampleCount; i++)
		{
			if (m_samples.size() < m_BufferSize)
			{
				//Normalize
				float Sample = samples[i] / 32768.0;

				//load Samples in Buffer
				m_samples.push_back(Sample);
			}
			else
			{
				m_trigger = false;
				m_newSamplesAvailable = true;
			}
		}
	}
	return true;
}

void cAudioRecorder::onStop()
{
}


