#include "gbp.cpp"
#include <string>
#include <unordered_map>
#include <vector>
#include <algorithm>

#define FILE_WARNING "/FILE/"
#define EMBED_WARNING "/EMBED/"

std::string fetchGBP();
std::string printFullGBPList();
std::string genericResponse();
std::string helpMessage();

std::string copypasta(std::string, std::string);
std::string findName(std::string);

std::string findGBP(int);
std::string findNum(int);

typedef std::string (*vFunc)(void);
typedef std::string (*iFunc)(int);
typedef std::string (*sFunc)(std::string);
typedef std::string (*ssFunc)(std::string, std::string);

std::map<std::string, vFunc>noArgs;
std::map<std::string, iFunc>intArgs;
std::map<std::string, sFunc>stringArgs;
std::map<std::string, ssFunc>twoStringArgs;

/* Initializes all the commands */
void initCommands()
{
	noArgs.emplace("hi", &genericResponse);
	noArgs.emplace("help", &helpMessage);
	noArgs.emplace("gbplist", &printFullGBPList);
	noArgs.emplace("fetchgbp", &fetchGBP);

	intArgs.emplace("findpos", &findNum);
	intArgs.emplace("findgbp", &findGBP);

	stringArgs.emplace("findname", &findName);

	twoStringArgs.emplace("copypasta", &copypasta);
}

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
	auto i1 = noArgs.find(args[0]);
	if (i1 != noArgs.end()) {
		return (*i1->second)();
	}
	auto i2 = intArgs.find(args[0]);
	if (i2 != intArgs.end()) {
		if (args.size() >= 2)
			return (*i2->second)(std::stoi(args[1]));
	}
	auto i3 = stringArgs.find(args[0]);
	if (i3 != stringArgs.end()) {
		if (args.size() >= 2)
			return (*i3->second)(args[1]);
	}
	auto i4 = twoStringArgs.find(args[0]);
	if (i4 != twoStringArgs.end()) {
		if (args.size() == 2)
			return (*i4->second)(args[1], "");
		else if (args.size() >= 3)
			return (*i4->second)(args[1], args[2]);
	}
	return "Invalid command!";
}

#define FILE_NAME "temp-GBP"
/**
 * printFullGBPList
 *
 * Returns a path to a file that contains a formatted list of gbp values
 */
std::string printFullGBPList()
{
	std::map<unsigned short int, std::pair<int, std::string>> gbp;
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
 * ##fetchGBP
 *
 * Fetches the latest GBP
 */
std::string fetchGBP()
{
	fetchLatestGBP();
	return "Fetched latest GBP!";
}

/**
 * ##findGBP
 *
 * Returns a string containing every user with the given gbp.
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

std::string copypasta(std::string msg, std::string username)
{
	std::string out;

	if (msg == "korol") {
		out = username + " is well known for murderboning, valid hunting, and power gaming. I especially see them during my timeslot where I am often one of only two or three admins, the EST mornings. During this, regardless of whether or not I'm playing or adminning, I will see " + username + " at the core of the conflict. If they're an antag, then a lot of people are dying/dead. If not, then they are usually one of the biggest threats to the antagonists. It was to absolutely no one's surprise when " + username + " killed virtually everyone during their round in question following the tradition of another killing spree, while only talking to complain about something or to vent their minor irritation in deadchat between revivals as a changeling. This was at both the station and the evac shuttle.";
	} else if (msg == "mothblocks") {
		out = "\
		His blant disregard for normal tg servers.\n\
		1) He consistently tells new admin candidates not to play, so how will they know more about a current playerbase if their only interaction is hovering around as a ghost. Why do you think nanites died as they did.\n\
		2)his use of back channels to ensure that notes stick the way that they do, if someone disagree and proves that they are correct in a way. Tough shit, he already communicated with headmins about it before they even communicated to the player.\n\
		Aka no reason for headmin ruling.\n\
		3) MSO and server specific playerbases say one thing and they guy with his metagroup still force \"mrp\" policies and code that conflict.\n\
		4)consistently banning Europeans out of discord code channel for bitching about it.\n\
		5)saying that \"playerbase has to make their responsibility of knowing the pipeline changes for github.\" Which I, myself couldn't get a simple poll to see how players actually take the time to look at forums or github. (We both know its real low on that bell curve.)";
	} else if (msg == "shotgun") {
		out = "I used the shotgun. You know why? Cause the shot gun doesn't miss: and unlike the shitty hybrid taser it stops a criminal in their tracks in two hits. Bang: bang: and they're fucking done. I use four shots just to make damn sure. Because: once again: I'm not there to coddle a bunca criminal scum sucking f------: I'm there to 1) Survive the fucking round. 2) Guard the armory. So you can absolutely get fucked. If I get unbanned: which I won't: you can guarantee I will continue to use the shotgun to apprehend criminals. Because it's quick: clean and effective as fuck. Why in the seven hells would I fuck around with the disabler shots: which take half a clip just to bring someone down: or with the tazer bolts which are slow as balls: impossible to aim and do about next to jack shit: fuck all. The shotgun is the superior law enforcement weapon. Because it stops crime. And it stops crime by reducing the number of criminals roaming the fucking halls.";
	} else if (msg == "meow") {
		out = "Wowwwww, you meow like a cat! That means you are one, right? Shut the fuck up. If you really want to be put on a leash and treated like a domestic animal then that's called a fetish, not \"quirky\" or \"cute\". What part of you seriously thinks that any part of acting like a felin establishes a reputation of appreciation? Is it your lack of any defining aspect of personality that urges you to restort to shitty representations of cats to create an illusion of meaning in your worthless life? Wearing \"car ears\" in the shape of headbands further notes the complete absence of human attribution to your false sense of personality, such as intelligence or charisma in any form or shape. Where do you think this mindset's gonna lead you? You think you're funny, random, quirky even? What makes you think that acting like a fucking cat will make a goddamn hyena laugh? I, personally, feel extremely sympathetic towards you as your only escape from the worthless thing you call your existence is to pretend to be an animal. But it's not a worthy choice to assert this horrifying fact as a dominant trait, mainly because personality traits require an initial personality to lay their foundation on. You're not worthy of anybody's time, so go fuck off, \"cat-girl\".";
	} else if (msg == "cobb") {
		out = "SNIFF SNIFF is that feet I smell? Said Ryan Cobb, as his boner swelled. \"Feet, feet, feet with sweat!\" He chanted loud, like a threat. Fast and nimble, quiet as night, Ryan cobb did say with delight: \"YES I SEE THEM, TASTY FEET, SWEATY LARGE AND READY TO EAT!\" He sucked the toes and slurped them down as with horror did watch the clown! He slobbered, swallowed, and sucked with grace as did horror befall our face! The Stun baton swang fast and fleet, so did signal ryan's defeat! But if you see him in the halls, bald head shining off the walls, keep your shoes on, laces knit, lest ryan catch you, and your forced to submit.";
	}

	return out;
}
