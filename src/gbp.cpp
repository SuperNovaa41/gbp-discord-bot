#include <curl/curl.h>
#include <curl/easy.h>
#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <map>
#include <algorithm>

#define URL "https://raw.githubusercontent.com/tgstation/tgstation/gbp-balances/.github/gbp-balances.toml"
#define FILE_NAME "gbp" 

void fetch_raw_gbp()
{
	CURL *curl;
	curl = curl_easy_init();
	if (curl) {
		curl_easy_setopt(curl, CURLOPT_URL, URL);

		FILE* GBP = fopen(FILE_NAME, "w");
		curl_easy_setopt(curl, CURLOPT_WRITEDATA, GBP); 

		curl_easy_perform(curl);
		curl_easy_cleanup(curl);
		fclose(GBP);
	}
}

std::vector<std::string> parse_raw_gbp()
{
		
	std::ifstream file;
	file.open(FILE_NAME);
	
	if (!file) { // if the file doesnt exist, lets force it
		fetch_raw_gbp();
		file.open(FILE_NAME);
	}
	
	std::vector<std::string> out;
	std::string line;
	while (getline(file, line)) {
		if (line[0] >= '0' && line[0] <= '9')
			out.push_back(line);
	}

	return out;
}

std::vector<std::pair<short int, std::string>> fetch_gbp_info()
{
	std::vector<std::string> in = parse_raw_gbp();	
	std::vector<std::pair<short int, std::string>> out;
	for (std::vector<std::string>::iterator it = in.begin(); it != in.end(); it++) {
		std::string line = (*it);
		std::string username = line.substr(line.find("#") + 2, line.length() - (line.find("#") + 1));
		int gbp = std::stoi(line.substr(line.find("=") + 2, line.find("#") - 1 -  line.find("#")));
		
		std::pair<short int, std::string> info = {gbp, username};
		out.push_back(info);
	}
	return out;
}

std::map<unsigned short int, std::pair<short int, std::string>> fetch_formatted_gbp()
{
	std::vector<std::pair<short int, std::string>> in = fetch_gbp_info();
	std::sort(in.begin(), in.end(),
			[](std::pair<short int, std::string> a, std::pair<short int, std::string> b) { // lambda expression to reverse the compare method
				return (a.first > b.first);
			}	
		);

	std::map<unsigned short int, std::pair<short int, std::string>> out;
	for (int i = 1; i <= in.size(); i++)
		out[i] = in[i-1];


	return out;
}
#undef URL
#undef FILE_NAME



/*
 * Commands
 */

std::string find_gbp_pos(int num_to_find)
{
	std::map<unsigned short int, std::pair<short int, std::string>> gbp = fetch_formatted_gbp();	
	std::map<unsigned short int, std::pair<short int, std::string>>::iterator it = gbp.find(num_to_find);
	if (it == gbp.end())
		return "Could not find user at that position!";
	return "```#" + std::to_string(num_to_find) + ": " + gbp[num_to_find].second+ " (" + std::to_string(gbp[num_to_find].first) + " GBP)```\n";
}

std::string find_gbp(int gbp_to_find)
{
	std::string out = "```";
	std::map<unsigned short int, std::pair<short int, std::string>> gbp = fetch_formatted_gbp();
	for (std::map<unsigned short int, std::pair<short int, std::string>>::iterator it = gbp.begin(); it != gbp.end(); it++) {
		if ((*it).second.first == gbp_to_find)
			out += "#" + std::to_string((*it).first) +": " + (*it).second.second + " (" + std::to_string((*it).second.first) + " GBP)\n";
	}
	out += "```";
	if (out == "``````")
		return "Could not find any user with that amount of GBP!";
	return out;
}

std::string find_username(std::string username)
{
	std::string out = "```";
	std::map<unsigned short int, std::pair<short int, std::string>> gbp = fetch_formatted_gbp();
	for (std::map<unsigned short int, std::pair<short int, std::string>>::iterator it = gbp.begin(); it != gbp.end(); it++) {
		if ((*it).second.second.find(username) != -1)
			out += "#" + std::to_string((*it).first) +": " + (*it).second.second + " (" + std::to_string((*it).second.first) + " GBP)\n";
	}
	out += "```";
	if (out == "``````")
		return "Could not find any user named " + username + "!";
	return out;
}
