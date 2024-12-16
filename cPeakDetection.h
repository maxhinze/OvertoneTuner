#pragma once

#include <vector>

struct Peak
{
	double value;
	unsigned int position;
};


class cPeakDetection
{
public:
	cPeakDetection();
	~cPeakDetection();

	void setOffset(double offset);
	void autoSetOffset(std::vector<double> &data);

	Peak findMaximum(std::vector<double> &data, unsigned int start, unsigned int end);

	std::vector<Peak> findPeaks(std::vector<double> &data);
	std::vector<double> getmovingAverage();

private:
	double m_offset; //const. offset
	unsigned int m_averageSize = 128;
	double m_peakLevel = 4;



	std::vector<Peak> m_peaks;

	std::vector<double> m_movingAverage;	
};

