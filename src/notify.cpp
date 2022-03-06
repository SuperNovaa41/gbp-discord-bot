#include <thread>
#include <fstream>
#include <iostream>
#include <vector>
#include "gbp.cpp"

void startScanning()
{
	while (1) {
		// read from the cached file contianing names to watch, if the gbp their position is > or < what is marked down, Ping the original recpiient that their locatoin has changed and then sleep for an hour, 

	}
}

void pingUsers()
{
	std::ifstream file;
	file.open("toPing.txt");
	if (!file) {
		file.close();
		return;
	}
	
	std::string line;
	std::string username;
	int gbpPos;
	std::string toPing;
	while(getline(file, line)) {
		username = line.substr(0, line.find(" "));
		gbpPos = std::stoi(line.substr(line.find(" ") + 1, line.length() - (line.find(" ") + 1)));
		toPing = line.substr(line.rfind(" "), line.length() - line.rfind(" "));

		std::map<unsigned short int, std::pair<int, std::string>> gbp = fetchAndReadGBP();

		if (gbp[gbpPos].second == username) {
			std::cout << "chungus" << std::endl;
		} else {
			
			// message
		}
	}
	
}
