#include "gbp.cpp"
#include <string>
#include <vector>

#define FILE_WARNING "/FILE/"

std::string printFullGBPList(bool = false);
std::string genericResponse();

std::string commandParse(std::vector<std::string> args)
{
	if (args[0] == "gbplist")
		return printFullGBPList();
	else if (args[0] == "hi")
		return genericResponse();
	else
		return "Invalid command!";
}


#define FILE_NAME "temp-GBP"
std::string printFullGBPList(bool update)
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

	std::string out = std::string(FILE_WARNING) + " " + std::string(FILE_NAME);
	return out;
}
#undef FILE_NAME

std::string genericResponse()
{
	return "Fuck you";
}
