#pragma once
#include <string>
class VstParameter
{
public:
	VstParameter(std::string name, float val );
	~VstParameter(void);
	float getValue();
	void setValue(float val);

	std::string getName();
	const char* getNameChar();

	void setName(std::string name);
private:
	float value;
	std::string name;
};

