#pragma once
class StateVariableFilter
{
public:
	StateVariableFilter(int cutoff);
	~StateVariableFilter(void);

	float process(float in);

	void setCutoff(int cutoff);
private:
	float fGain;
	float low;
	float high;
	float peak;
	float band;
	float notch;

	float fc; //cutoff
	float fs; //sampling freq
	float res; 
	float freq; 
	float damp;
	float drive;
};

