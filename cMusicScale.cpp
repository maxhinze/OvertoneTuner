#include "cMusicScale.h"



cMusicScale::cMusicScale(double pitch)
{
	m_concertPitch = pitch;

	double higherpitch = pow(2.0, (1.0 / 12.0));
	double lowerpitch = 1.0 / pow(2.0, (1.0 / 12.0));

	double frequency = m_concertPitch * pow(lowerpitch, 48);  // multiply concertPitch with lowerPitch-Factor^48 to get the Frequency of the Lowest Note (A0)

	unsigned int Note = A;		//First Note
	unsigned int oktave = 0;	//First Oktave


	// fill keys Array with all values
	for (unsigned int i = 0; i < 88; i++)
	{
		m_keys[i].midi = i + 21; // A0 has Midi-Number 21

		switch (Note)
		{
		case C:
			m_keys[i].name = "C";
			break;
		case Des:
			m_keys[i].name = "DES";
			break;
		case D:
			m_keys[i].name = "D";
			break;
		case Es:
			m_keys[i].name = "ES";
			break;
		case E:
			m_keys[i].name = "E";
			break;
		case F:
			m_keys[i].name = "F";
			break;
		case Ges:
			m_keys[i].name = "GES";
			break;
		case G:
			m_keys[i].name = "G";
			break;
		case As:
			m_keys[i].name = "AS";
			break;
		case A:
			m_keys[i].name = "A";
			break;
		case B:
			m_keys[i].name = "B";
			break;
		case H:
			m_keys[i].name = "H";
			break;
		default:
			m_keys[i].name = "";
			break;
		}
		m_keys[i].name = m_keys[i].name + std::to_string(oktave);


		//m_keys[i].name = NoteNames[(unsigned int)Note] + std::to_string(oktave);
		m_keys[i].oktave = oktave;
		m_keys[i].frequency = frequency;


		if (Note == C || Note == D || Note == E || Note == F || Note == G || Note == A || Note == H) // white keys
		{
			m_keys[i].color = white;
		}
		else
		{
			m_keys[i].color = black;
		}

		Note++;
		if (Note > 11)
		{
			Note = C;
			oktave++;
		}
		frequency *= higherpitch;
	}
}


cMusicScale::~cMusicScale()
{
}

void cMusicScale::pressKey(int midi, bool state)
{
	for (char i = 0; i < 88; i++)
	{
		if (m_keys[i].midi == midi)
		{
			m_keys[i].isPressed = state;
		}
	}
}

key cMusicScale::findNearesNote(double frequency)
{
	key KeyOfInterest;

	unsigned int min_pos = 0;
	double min_diff = 9999999;

	for (unsigned int i = 0; i < 88; i++)
	{
		double difference = abs(m_keys[i].frequency - frequency);

		if (difference < min_diff)
		{
			min_diff = difference;
			min_pos = i;
		}
	}

	KeyOfInterest = m_keys[min_pos];
	return KeyOfInterest;
}

double cMusicScale::calcCent(key Note, double frequency)
{
	double cent = log2(frequency / Note.frequency) * 1200.0;
	return  cent;
}
