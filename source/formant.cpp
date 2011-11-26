//-------------------------------------------------------------------------------------------------------
// VST Plug-Ins SDK
// Version 2.4		$Date: 2006/11/13 09:08:27 $
//
// Category     : VST 2.x SDK Samples
// Filename     : FormantPlugin.cpp
// Created by   : Steinberg Media Technologies
// Description  : Stereo plugin which applies Gain [-oo, 0dB]
//
// © 2006, Steinberg Media Technologies, All Rights Reserved
//-------------------------------------------------------------------------------------------------------
#include "StateVariableFilter.h"
#include "VstParameter.h"
#include "formant.h"

enum {pPosition, pVowelA};
//-------------------------------------------------------------------------------------------------------
AudioEffect* createEffectInstance (audioMasterCallback audioMaster)
{
	return new FormantPlugin (audioMaster);
}

//-------------------------------------------------------------------------------------------------------
FormantPlugin::FormantPlugin (audioMasterCallback audioMaster)
: AudioEffectX (audioMaster, 1,2)	// 1 program, 1 parameter only
{
	setNumInputs (2);		// stereo in
	setNumOutputs (2);		// stereo out
	setUniqueID ('ijnh');	// identify
	canProcessReplacing ();	// supports replacing output
	canDoubleReplacing ();	// supports double precision processing

	params[pPosition] = new VstParameter("Position", 1.f);
	params[pVowelA] = new VstParameter("VowelA", 1.f);

	filters[0]= new StateVariableFilter(800);
	filters[1]= new StateVariableFilter(1150);
	filters[2]= new StateVariableFilter(800);
	filters[3]= new StateVariableFilter(1150);

	vst_strncpy (programName, "Default", kVstMaxProgNameLen);	// default program name
}

//-------------------------------------------------------------------------------------------------------
FormantPlugin::~FormantPlugin ()
{
	// nothing to do here
}

//-------------------------------------------------------------------------------------------------------
void FormantPlugin::setProgramName (char* name)
{
	vst_strncpy (programName, name, kVstMaxProgNameLen);
}

//-----------------------------------------------------------------------------------------
void FormantPlugin::getProgramName (char* name)
{
	vst_strncpy (name, programName, kVstMaxProgNameLen);
}

//-----------------------------------------------------------------------------------------
void FormantPlugin::setParameter (VstInt32 index, float value)
{
	params[index]->setValue(value);
	filters[0]->setCutoff(350*(1-value)+800*value);
	filters[1]->setCutoff(2000*(1-value)+1150*value);
	filters[2]->setCutoff(350*(1-value)+800*value);
	filters[3]->setCutoff(2000*(1-value)+1150*value);
}

//-----------------------------------------------------------------------------------------
float FormantPlugin::getParameter (VstInt32 index)
{
	return params[index]->getValue();
}

//-----------------------------------------------------------------------------------------
void FormantPlugin::getParameterName (VstInt32 index, char* label)
{
	vst_strncpy (label, params[index]->getNameChar(), kVstMaxParamStrLen);
}

//-----------------------------------------------------------------------------------------
void FormantPlugin::getParameterDisplay (VstInt32 index, char* text)
{
	dB2string (params[index]->getValue(), text, kVstMaxParamStrLen);
}

//-----------------------------------------------------------------------------------------
void FormantPlugin::getParameterLabel (VstInt32 index, char* label)
{
	vst_strncpy (label, "%", kVstMaxParamStrLen);
}

//------------------------------------------------------------------------
bool FormantPlugin::getEffectName (char* name)
{
	vst_strncpy (name, "Gain", kVstMaxEffectNameLen);
	return true;
}

//------------------------------------------------------------------------
bool FormantPlugin::getProductString (char* text)
{
	vst_strncpy (text, "Gain", kVstMaxProductStrLen);
	return true;
}

//------------------------------------------------------------------------
bool FormantPlugin::getVendorString (char* text)
{
	vst_strncpy (text, "Steinberg Media Technologies", kVstMaxVendorStrLen);
	return true;
}

//-----------------------------------------------------------------------------------------
VstInt32 FormantPlugin::getVendorVersion ()
{ 
	return 1000; 
}

//-----------------------------------------------------------------------------------------
void FormantPlugin::processReplacing (float** inputs, float** outputs, VstInt32 sampleFrames)
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
        outA = filters[0]->process(inA);
		outA += filters[1]->process(inA);
		outB = filters[2]->process(inB);
		outB += filters[3]->process(inB);

		(*out1++) = outA;
		(*out2++) = outB;
    }
}

//-----------------------------------------------------------------------------------------
void FormantPlugin::processDoubleReplacing (double** inputs, double** outputs, VstInt32 sampleFrames)
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
