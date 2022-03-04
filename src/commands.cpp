#include "gbp.cpp"
#include <string>
#include <unordered_map>
#include <vector>
#include <algorithm>

#define FILE_WARNING "/FILE/"
#define EMBED_WARNING "/EMBED/"

std::string printFullGBPList(bool = false);
std::string genericResponse();
std::string helpMessage();
std::string findNum(int pos);
std::string findName(std::string user);
std::string findGBP(int gbpinp);

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
	if (args[0] == "gbplist") {
		if (args.size() >= 2)
			return printFullGBPList(args[1] == "update" ? true : false);
		else
			return printFullGBPList();
	} else if (args[0] == "hi") {
		return genericResponse();
	} else if (args[0] == "help") {
		return helpMessage();
	} else if (args[0] == "findpos" ) {
		if (args.size() >= 2)
			return findNum(std::stoi(args[1]));
	} else if (args[0] == "findname") {
		if (args.size() >= 2)
			return findName(args[1]);
	} else if (args[0] == "findgbp") {
		if (args.size() >= 2)
			return findGBP(std::stoi(args[1]));
	} 
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
 * ##findNum
 *
 * Returns the user + GBP at the given position.
 *
 * Arguments:
 * * int pos - The position to find.
 */
std::string findNum(int pos)
{
	std::map<unsigned short int, std::pair<int, std::string>> gbp = readGBPIntoList();
	std::string out = "```#" + std::to_string(pos) + ": " + gbp[pos].second + " (" + std::to_string(gbp[pos].first) + " GBP)```\n"; 
	return out;
}

/**
 * ##findName
 *
 * Finds the given user, and returns a string with their GBP.
 *
 * Arguments:
 * * std::string user - The username to search for.
 */
std::string findName(std::string user)
{
	std::map<unsigned short int, std::pair<int, std::string>> gbp = readGBPIntoList();
	for (std::map<unsigned short int, std::pair<int, std::string>>::iterator it = gbp.begin(); it != gbp.end(); it++) {
		if (it->second.second.find(user) != -1) {
			return "```#" + std::to_string(it->first) + ": " + it->second.second + " (" + std::to_string(it->second.first) + " GBP)```\n";
		} 	
	}
	return "User not found!";
}

/**
 * ##findGBP
 *
 * Returns a string containing every user with the given gbp.
 *
 * Arguments:
 * * int gbpinp - The GBP to find.
 */
std::string findGBP(int gbpinp)
{
	std::string out = "```";
	std::map<unsigned short int, std::pair<int, std::string>> gbp = readGBPIntoList();
	for (std::map<unsigned short int, std::pair<int, std::string>>::iterator it = gbp.begin(); it != gbp.end(); it++)
		if (it->second.first == gbpinp)
			out += "#" + std::to_string(it->first) + ": " + it->second.second + " (" + std::to_string(it->second.first) + " GBP)\n";
	out += "```";
	if (out == "``````")
		return "No users found with that GBP!";
	return out;
}

/**
 * ##genericResponse
 *
 * Gives a generic response.
 */
std::string genericResponse()
{
	return "Fuck you";
}

/** 
 * ##helpMessage
 *
 * Return the list of commands.
 */
std::string helpMessage()
{
	std::string out;
	out += "```\n";
	out += "Available Commands: \n";
	out += "gbplist (update):\n";
	out += "----> Print out the current GBP leaderboard. Add update to fetch the latest page.\n";
	out += "findpos (number):\n";
	out += "----> Prints out the user at number (number) in the GBP leaderboard.\n";
	out += "findgbp (number):\n";
	out += "----> Prints out every use with (number) GBP.\n";
	out += "findname (username):\n";
	out += "----> Prints out the user (username) and their GBP.\n";
	out += "```\n";
	return out;
}
