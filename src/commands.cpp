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
std::string korolCopypasta(std::string username);
std::string mothblocksCopypasta();

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
	} else if (args[0] == "findpos") {
		if (args.size() >= 2)
			return findNum(std::stoi(args[1]));
	} else if (args[0] == "findname") {
		if (args.size() >= 2)
			return findName(args[1]);
	} else if (args[0] == "findgbp") { 
		if (args.size() >= 2)
			return findGBP(std::stoi(args[1]));
	} else if (args[0] == "fetchgbp") {
		fetchLatestGBP();
		return "Fetched latest GBP!";
	} else if (args[0] == "korol") {
		if (args.size() >= 2)
			return korolCopypasta(args[1]);
	} else if (args[0] == "mothblocks") {
		return mothblocksCopypasta();
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
	std::string out = "```";
	std::map<unsigned short int, std::pair<int, std::string>> gbp = readGBPIntoList();
	for (std::map<unsigned short int, std::pair<int, std::string>>::iterator it = gbp.begin(); it != gbp.end(); it++) {
		if (it->second.second.find(user) != -1) {
			out += "#" + std::to_string(it->first) + ": " + it->second.second + " (" + std::to_string(it->second.first) + " GBP)\n";
		} 	
	}
	out += "```";

	if (out == "``````")
		return "User not found!";
	return out;
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
	out += "----> Prints out every user with (number) GBP.\n";
	out += "findname (username):\n";
	out += "----> Prints out the user (username) and their GBP.\n";
	out += "```\n";
	return out;
}

std::string korolCopypasta(std::string username)
{
	return username + " is well known for murderboning, valid hunting, and power gaming. I especially see them during my timeslot where I am often one of only two or three admins, the EST mornings. During this, regardless of whether or not I'm playing or adminning, I will see " + username + " at the core of the conflict. If they're an antag, then a lot of people are dying/dead. If not, then they are usually one of the biggest threats to the antagonists. It was to absolutely no one's surprise when " + username + " killed virtually everyone during their round in question following the tradition of another killing spree, while only talking to complain about something or to vent their minor irritation in deadchat between revivals as a changeling. This was at both the station and the evac shuttle.";
}

std::string mothblocksCopypasta() 
{
	std::string out = "\
		His blant disregard for normal tg servers.\n\
		1) He consistently tells new admin candidates not to play, so how will they know more about a current playerbase if their only interaction is hovering around as a ghost. Why do you think nanites died as they did.\n\
		2)his use of back channels to ensure that notes stick the way that they do, if someone disagree and proves that they are correct in a way. Tough shit, he already communicated with headmins about it before they even communicated to the player.\n\
		Aka no reason for headmin ruling.\n\
		3) MSO and server specific playerbases say one thing and they guy with his metagroup still force \"mrp\" policies and code that conflict.\n\
		4)consistently banning Europeans out of discord code channel for bitching about it.\n\
		5)saying that \"playerbase has to make their responsibility of knowing the pipeline changes for github.\" Which I, myself couldn't get a simple poll to see how players actually take the time to look at forums or github. (We both know its real low on that bell curve.)";
	return out;
}
