#include "gbp.cpp"
#include <string>
#include <vector>

#define FILE_WARNING "/FILE/"

std::string printFullGBPList(bool = false);
std::string genericResponse();

/**
 * ##commandParse
 *
 * Uses the given arguments to choose a command to use
 *
 * Arguments:
 * * std::vector<std::string> args - Given arguments, separated into a vector
 */
std::string commandParse(std::vector<std::string> args)
{
	if (args[0] == "gbplist")
		return printFullGBPList(args[1] == "update" ? true : false);
	else if (args[0] == "hi")
		return genericResponse();
	else
		return "Invalid command!";
}

#define FILE_NAME "temp-GBP"
/**
 * printFullGBPList
 *
 * Returns a path to a file that contains a formatted list of gbp values
 *
 * Arguments:
 * * bool update - Should we fetch the latest GBP?
 */
std::string printFullGBPList(bool update)
{
	std::map<unsigned short int, std::pair<int, std::string>> gbp;
	if (update)
		gbp = fetchAndReadGBP();
	else
		gbp = readGBPIntoList();

	std::ofstream file;
	file.open("temp-GBP");
	std::string line;
	for (std::map<unsigned short int, std::pair<int, std::string>>::iterator it = gbp.begin(); it != gbp.end(); it++) {
		line = std::to_string(it->first) + ": " + it->second.second + " (" + std::to_string(it->second.first) + " GBP)\n";

		file << line;
	}
	file.close();

	std::string out = std::string(FILE_WARNING) + " " + std::string(FILE_NAME);
	return out;
}
#undef FILE_NAME

/**
 * ##genericResponse
 *
 * Gives a generic response.
 */
std::string genericResponse()
{
	return "Fuck you";
}
