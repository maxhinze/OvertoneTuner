#pragma once
#define _USE_MATH_DEFINES

#include <complex>
#include <valarray>
#include <vector>
#include <cmath>

typedef std::complex<double> Complex;
typedef std::valarray<Complex> CArray;

enum FFTWindow
{
	FFT_WIN_RECTANGLE,
	FFT_WIN_HANNING,
	FFT_WIN_HAMMING
};


class cMyFFT
{
public:
	cMyFFT(int SampleRate, int numSamples);
	~cMyFFT();

	void setWindow(FFTWindow window);
	void FFT(std::vector<double> &samples, std::vector<double> &spectrum);

	double getMaxFrequency();
	double getResulution();

protected:
	void fft(CArray& x);

private:

	int m_SampleRate;
	int m_NumSamples;
	double m_maxFrequency;
	double m_Resolution;

	std::vector<float> m_window;
};

