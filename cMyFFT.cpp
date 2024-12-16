#include "cMyFFT.h"


cMyFFT::cMyFFT(int SampleRate, int numSamples)
{
	m_SampleRate = SampleRate;
	m_NumSamples = numSamples;

	m_maxFrequency = m_SampleRate/2.0;
	m_Resolution = m_SampleRate/ (double)m_NumSamples;

	setWindow(FFT_WIN_RECTANGLE);
}

cMyFFT::~cMyFFT()
{
}

// Cooley–Tukey myFFT (in-place, divide-and-conquer)
// Higher memory requirements and redundancy although more intuitive
void cMyFFT::fft(CArray& x)
{
	const unsigned int N = x.size();
	if (N <= 1) return;

	// divide
	CArray even = x[std::slice(0, N / 2, 2)];
	CArray  odd = x[std::slice(1, N / 2, 2)];

	// conquer
	fft(even);
	fft(odd);

	// combine
	for (unsigned int k = 0; k < N / 2; k++)
	{
		Complex t = std::polar(1.0, -2 * M_PI * k / N) * odd[k];
		x[k] = even[k] + t;
		x[k + N / 2] = even[k] - t;
	}
}

void cMyFFT::setWindow(FFTWindow window)
{
	m_window.clear();
	unsigned int size = m_NumSamples;

	for (unsigned int i = 0; i < size; i++)
	{
		switch (window)
		{
		case FFT_WIN_RECTANGLE:
			m_window.push_back(1.0);
			break;

		case FFT_WIN_HANNING:
			m_window.push_back(0.5 * (1.0 - cos(2 * M_PI * i / ((double)size - 1))));
			break;

		case FFT_WIN_HAMMING:
			m_window.push_back(0.54 - 0.46 * cos(2 * M_PI * i / ((double)size - 1)));
			break;

		default:
			break;
		}
	}
}

void cMyFFT::FFT(std::vector<double>& samples, std::vector<double> &spectrum)
{
	CArray bin;
	unsigned int size = m_NumSamples;

	bin.resize(size);

	spectrum.clear();

	for (unsigned int i = 0; i < size; i++)
	{
		// windowing
		double sample = samples[i] * m_window[i];

		bin[i] = Complex(sample, 0);	//save in complex Array
	}

	//calculate the FFT
	fft(bin);

	//Save the (real) Result in spectrum Vector
	for (unsigned int i = 0; i < (size / 2); i++)
	{
		spectrum.push_back(abs(bin[i]));
	}
}

double cMyFFT::getMaxFrequency()
{
	return m_maxFrequency;
}

double cMyFFT::getResulution()
{
	return m_Resolution;
}
