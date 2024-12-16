#include "cPeakDetection.h"



cPeakDetection::cPeakDetection()
{
	m_offset = 0;
}


cPeakDetection::~cPeakDetection()
{
}

void cPeakDetection::setOffset(double offset)
{
	m_offset = offset;
}

void cPeakDetection::autoSetOffset(std::vector<double> &data)
{
	double sum = 0;

	for (unsigned int i = 0; i < data.size(); i++)
	{
		sum += data[i];
	}
	sum = sum / data.size();

	m_offset = sum;
}

Peak cPeakDetection::findMaximum(std::vector<double> &data, unsigned int start, unsigned int end)
{
	Peak peak;
	peak.position = 0;
	peak.value = 0;

	for (unsigned int i = start; i < end; i++)
	{
		if (data[i] > peak.value)
		{
			peak.value = data[i];
			peak.position = i;
		}
	}

	return peak;
}

std::vector<Peak> cPeakDetection::findPeaks(std::vector<double> &data)
{
	std::vector<char> peakArea;

	std::vector<Peak> peaks;

	m_movingAverage.clear();

	for (unsigned int i = 0; i < m_averageSize; i++)
	{
		peakArea.push_back(false);
	}


	for (unsigned int i = (m_averageSize - 1); i < (data.size() - m_averageSize); i++)
	{
		double sumLow = 0;
		double sumHigh = 0;

		for (unsigned int n = 1; n < m_averageSize; n++)
		{
			sumLow += data[i - n];
			sumHigh += data[i + n];
		}

		sumLow = sumLow / m_averageSize;
		sumHigh = sumHigh / m_averageSize;

		m_movingAverage.push_back((sumLow * sumLow) / 2.0);

		if (data[i] > (sumLow * m_peakLevel) && data[i] > (sumHigh * m_peakLevel))
		{
			peakArea.push_back(true);
		}
		else
		{
			peakArea.push_back(false);
		}
	}

	for (unsigned int i = 0; i < m_averageSize -1; i++)
	{
		peakArea.push_back(false);
	}


	// find edges
	bool oldEdge = 0;
	unsigned int peakStart = 0;		// Temporary

	for (unsigned int i = 1; i < data.size(); i++)
	{
		if (oldEdge == 0) //search rising edges
		{
			if (peakArea[i] == true && peakArea[i - 1] == false)
			{
				peakStart = i;
				oldEdge = 1;
			}
		}
		else    // search falling edges
		{
			if (peakArea[i] == false && peakArea[i - 1] == true)
			{
				//find Peak in Area
				peaks.push_back(findMaximum(data, peakStart, i));

				oldEdge = 0;
			}
		}
	}

	return peaks;
}

std::vector<double> cPeakDetection::getmovingAverage()
{
	return m_movingAverage;
}
