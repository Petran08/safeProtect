#pragma once
#include <string>

class effect
{
public:
	int id;
	std::string eff;
	float val;
	float dur;
	float activated;
	bool active;

	//overloading the == function(can be customized)
	//really cool trick for comparing two objects of the same class(and a lot cleaner)
	bool operator==(const effect& other)const {
		return eff == other.eff && val == other.val && dur == other.dur && id == other.id;
	}

	bool operator!=(const effect& other)const {
		return !(*this == other);
	}
};

