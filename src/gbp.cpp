#include <string>
#include <fstream>
#include <bits/stdc++.h>
#include <Python.h>

/**
 * ##fetchLatestGBP
 *
 * Uses the python script located in /src/ to fetch the latest GBP and write it to a file
 */
void fetchLatestGBP()
{
	FILE* fp;

	Py_Initialize();
	fp = _Py_fopen("gbp-leaderboard.py", "r");
	PyRun_SimpleFile(fp, "gbp-leaderboard.py");
	Py_Finalize();
}

/**
 * ##readGBPIntoList
 *
 * Returns a map of containing the contents of the GBP file.
 * Formatted as <position (unsigned short int), <gbp amount (int), username (std::string)>>
 */
std::map<unsigned short int, std::pair<int, std::string>>readGBPIntoList()
{
	std::map<unsigned short int, std::pair<int, std::string>> GBP;
	std::ifstream file;
	file.open("../src/balances.txt");
	std::string line;
	int i = 1;
	while(getline(file, line)) {
		std::string username = line.substr(line.find(" "), line.rfind(" ") - line.find(" "));
		int tGBP = std::stoi(line.substr(line.rfind(" "), line.length() - line.rfind(" ")));

		GBP.insert({i, {tGBP, username}});
		i++;
	}
	file.close();

	return GBP;
}

/**
 * fetches latest GBP, and then reads it into a map
 */
std::map<unsigned short int, std::pair<int, std::string>>fetchAndReadGBP()
{
	fetchLatestGBP();
	return readGBPIntoList();
}
