#include "StateVariableFilter.h"
#include <math.h>
#include <algorithm>
using namespace std;

StateVariableFilter::StateVariableFilter(int cutoff)
{
	low   = 0;
	high  = 0;
	peak  = 0;
	band  = 0;
	notch = 0;
	fs    = 44100; //sampling freq
	res   =.9; 
	setCutoff(cutoff);
}
/**
 * based on http://www.musicdsp.org/archive.php?classid=3#92
 * removed drive though
 */
float StateVariableFilter::process(float input)
{
	if (input == 0){
		return 0;
	}

	float out = 0;

	notch =  input - damp*band;
	low   = low + freq*band;
	high  = notch - low;
	band  = freq*high + band;
	out   = 0.5*band;//(notch or low or high or band or peak);
	notch = input - damp*band;
	low   = low + freq*band;
	high  = notch - low;
	band  = freq*high + band;
	out  += 0.5*band;//(same out as above);
	return out;
}

StateVariableFilter::~StateVariableFilter(void)
{
}

void StateVariableFilter::setCutoff(int cutoff)
{
	fc = cutoff;
	freq  = 2.0*sin(3.14159265358979323846*min(0.25f, fc/(fs*2)));  // the fs*2 is because it's double sampled
	damp  = min(2.0*(1.0 - pow(res, 0.25f)), min(2.0, 2.0/freq - freq*0.5));
}
