#include "gbp.cpp"
#include <string>

std::string printFullGBPList(bool update = false)
{
	std::map<int, std::pair<int, std::string>> gbp;
	if (update)
		gbp = fetchAndReadGBP();
	else
		gbp = readGBPIntoList();

	std::string msg = "";
	for (std::map<int, std::pair<int, std::string>>::iterator it = gbp.begin(); it != gbp.end(); it++) {
		msg += std::to_string(it->first) + ": " + it->second.second + "(" + std::to_string(it->second.first) + " GBP)\n";

	}
	return msg;
}

