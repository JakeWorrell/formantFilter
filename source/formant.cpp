//-------------------------------------------------------------------------------------------------------
// VST Plug-Ins SDK
// Version 2.4		$Date: 2006/11/13 09:08:27 $
//
// Category     : VST 2.x SDK Samples
// Filename     : again.cpp
// Created by   : Steinberg Media Technologies
// Description  : Stereo plugin which applies Gain [-oo, 0dB]
//
// © 2006, Steinberg Media Technologies, All Rights Reserved
//-------------------------------------------------------------------------------------------------------
#include "formant.h"
#include <math.h>
#include <algorithm>
using namespace std;
//-------------------------------------------------------------------------------------------------------
AudioEffect* createEffectInstance (audioMasterCallback audioMaster)
{
	return new AGain (audioMaster);
}

//-------------------------------------------------------------------------------------------------------
AGain::AGain (audioMasterCallback audioMaster)
: AudioEffectX (audioMaster, 1, 1)	// 1 program, 1 parameter only
{
	setNumInputs (2);		// stereo in
	setNumOutputs (2);		// stereo out
	setUniqueID ('Gain');	// identify
	canProcessReplacing ();	// supports replacing output
	canDoubleReplacing ();	// supports double precision processing

	fGain = 1.f;			// default to 0 dB
	low = 0;
	high = 0;
	peak = 0;
	band = 0;
	notch = 0;
	vst_strncpy (programName, "Default", kVstMaxProgNameLen);	// default program name
}

//-------------------------------------------------------------------------------------------------------
AGain::~AGain ()
{
	// nothing to do here
}

//-------------------------------------------------------------------------------------------------------
void AGain::setProgramName (char* name)
{
	vst_strncpy (programName, name, kVstMaxProgNameLen);
}

//-----------------------------------------------------------------------------------------
void AGain::getProgramName (char* name)
{
	vst_strncpy (name, programName, kVstMaxProgNameLen);
}

//-----------------------------------------------------------------------------------------
void AGain::setParameter (VstInt32 index, float value)
{
	fGain = value;
}

//-----------------------------------------------------------------------------------------
float AGain::getParameter (VstInt32 index)
{
	return fGain;
}

//-----------------------------------------------------------------------------------------
void AGain::getParameterName (VstInt32 index, char* label)
{
	vst_strncpy (label, "Gain", kVstMaxParamStrLen);
}

//-----------------------------------------------------------------------------------------
void AGain::getParameterDisplay (VstInt32 index, char* text)
{
	dB2string (fGain, text, kVstMaxParamStrLen);
}

//-----------------------------------------------------------------------------------------
void AGain::getParameterLabel (VstInt32 index, char* label)
{
	vst_strncpy (label, "dB", kVstMaxParamStrLen);
}

//------------------------------------------------------------------------
bool AGain::getEffectName (char* name)
{
	vst_strncpy (name, "Gain", kVstMaxEffectNameLen);
	return true;
}

//------------------------------------------------------------------------
bool AGain::getProductString (char* text)
{
	vst_strncpy (text, "Gain", kVstMaxProductStrLen);
	return true;
}

//------------------------------------------------------------------------
bool AGain::getVendorString (char* text)
{
	vst_strncpy (text, "Steinberg Media Technologies", kVstMaxVendorStrLen);
	return true;
}

//-----------------------------------------------------------------------------------------
VstInt32 AGain::getVendorVersion ()
{ 
	return 1000; 
}

//-----------------------------------------------------------------------------------------
void AGain::processReplacing (float** inputs, float** outputs, VstInt32 sampleFrames)
{
	float fc = fGain * 10000; //cutoff
	float fs =44100; //sampling freq
	float res =.9; 
	float freq   = 2.0*sin(3.14159265358979323846*min(0.25f, fc/(fs*2)));  // the fs*2 is because it's double sampled
	float damp   = min(2.0*(1.0 - pow(res, 0.25f)), min(2.0, 2.0/freq - freq*0.5));
	float drive = 0;

	float* in1  =  inputs[0];
    float* in2  =  inputs[1];
    float* out1 = outputs[0];
    float* out2 = outputs[1];

	float out=0;
	float in=0;
    while (--sampleFrames >= 0)
    {
		in=(*in1++);
		notch =  in - damp*band;
		low   = low + freq*band;
		high  = notch - low;
		band  = freq*high + band - drive*band*band*band;
		out   = 0.5*band;//(notch or low or high or band or peak);
		notch = in - damp*band;
		low   = low + freq*band;
		high  = notch - low;
		band  = freq*high + band - drive*band*band*band;
		out  += 0.5*band;//(same out as above);

        (*out1++) = out;
		
    }
}

//-----------------------------------------------------------------------------------------
void AGain::processDoubleReplacing (double** inputs, double** outputs, VstInt32 sampleFrames)
{
    /*
	commented out to avoid confusion
	double* in1  =  inputs[0];
    double* in2  =  inputs[1];
    double* out1 = outputs[0];
    double* out2 = outputs[1];
	double dGain = fGain;

    while (--sampleFrames >= 0)
    {
        (*out1++) = (*in1++) * dGain;
        (*out2++) = (*in2++) * dGain;
    }
	
	*/
}
