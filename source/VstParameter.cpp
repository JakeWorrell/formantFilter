#include "VstParameter.h"


VstParameter::VstParameter(std::string name, float val )
{
	setName(name);
	setValue(val);
}


VstParameter::~VstParameter(void)
{
}

float VstParameter::getValue()
{
	return value;
}

void VstParameter::setValue(float value)
{
	this->value=value;
}

std::string VstParameter::getName()
{
	return name;
}

const char* VstParameter::getNameChar()
{
	const char* tmp = getName().c_str();
	return tmp;
}

void  VstParameter::setName(std::string name)
{
	this->name = name;
}