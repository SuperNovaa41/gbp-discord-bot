#include "gbp.cpp"

std::string printFullGBPList(bool update = false)
{
	std::map<int, std::string> gbp;
	if (update)
		gbp = fetchAndReadGBP();
	else
		gbp = readGBPIntoList();

	std::string msg;
	int i = 1;
	for (std::map<int, std::string>::iterator it = gbp.end(); it != gbp.begin(); it--) {
		msg += "#[i] [it->second], GBP: [it->first]\n"; 
		i++;
	}
	return msg;
}
