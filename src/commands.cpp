#include "gbp.cpp"
#include <string>

#define FILE_WARNING "/FILE/"

#define FILE_NAME "temp-GBP"
std::string printFullGBPList(bool update = false)
{
	std::map<int, std::pair<int, std::string>> gbp;
	if (update)
		gbp = fetchAndReadGBP();
	else
		gbp = readGBPIntoList();

	std::ofstream file;
	file.open("temp-GBP");
	
	std::string line;
	for (std::map<int, std::pair<int, std::string>>::iterator it = gbp.begin(); it != gbp.end(); it++) {
		line = std::to_string(it->first) + ": " + it->second.second + " (" + std::to_string(it->second.first) + " GBP)\n";

		file << line;
	}
	file.close();

	std::string out;
	out.append(FILE_WARNING);
	out.append(" ");
	out.append(FILE_NAME);
	return out;
}
#undef FILE_NAME
