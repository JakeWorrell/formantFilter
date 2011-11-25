//-------------------------------------------------------------------------------------------------------
// VST Plug-Ins SDK
// Version 2.4		$Date: 2006/11/13 09:08:27 $
//
// Category     : VST 2.x SDK Samples
// Filename     : again.cpp
// Created by   : Steinberg Media Technologies
// Description  : Stereo plugin which applies Gain [-oo, 0dB]
//
// � 2006, Steinberg Media Technologies, All Rights Reserved
//-------------------------------------------------------------------------------------------------------
#include "StateVariableFilter.h"
#include "formant.h"
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

	filter[0]= new StateVariableFilter(800);
	filter[1]= new StateVariableFilter(1150);
	filter[2]= new StateVariableFilter(800);
	filter[3]= new StateVariableFilter(1150);

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
	filter[0]->setCutoff(350*(1-value)+800*value);
	filter[1]->setCutoff(2000*(1-value)+1150*value);
	filter[2]->setCutoff(350*(1-value)+800*value);
	filter[3]->setCutoff(2000*(1-value)+1150*value);
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
	float* in1  =  inputs[0];
    float* in2  =  inputs[1];
    float* out1 = outputs[0];
    float* out2 = outputs[1];

	float outA=0;
	float outB=0;
	float inA=0;
	float inB=0;

    while (--sampleFrames >= 0)
    {
		inA=(*in1++);
		inB=(*in2++);
        outA = filter[0]->process(inA);
		outA += filter[1]->process(inA);
		outB = filter[2]->process(inB);
		outB += filter[3]->process(inB);

		(*out1++) = outA;
		(*out2++) = outB;
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
