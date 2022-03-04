#include <string>
#include <fstream>
#include <bits/stdc++.h>
#include <Python.h>

void fetchLatestGBP()
{
	FILE* fp;

	Py_Initialize();
	fp = _Py_fopen("gbp-leaderboard.py", "r");
	PyRun_SimpleFile(fp, "gbp-leaderboard.py");
	Py_Finalize();
}

std::map<int, std::string>readGBPIntoList()
{
	std::map<int, std::string> GBP;

	std::ifstream file;
	file.open("balances.txt");
	std::string line;
	while(getline(file, line)) {
		std::string username = line.substr(line.find(" "), line.rfind(" ") - line.find(" "));
		int tGBP = std::stoi(line.substr(line.rfind(" "), line.length() - line.rfind(" ")));

		GBP.insert({tGBP, username});
	}
	return GBP;
}

std::map<int, std::string>fetchAndReadGBP()
{
	fetchLatestGBP();
	return readGBPIntoList();
}