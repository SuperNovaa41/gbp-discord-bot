#include "gbp.cpp"
#include <dpp/dpp.h>
#include <dpp/commandhandler.h>
#include <dpp/message.h>
#include <iostream>
#include <fstream>
#include <dpp/nlohmann/json.hpp>

using json = nlohmann::json;

#define TEMP_FILE_NAME "gbp.txt"
void sanitize_message(std::string &content, dpp::message &msg)
{
	if (content.length() < 2000)
		return;
	std::ofstream file(TEMP_FILE_NAME);
	for (int i = 0; i < content.length(); i++) {
		if (content[i] != '`')
			file << content[i];
	}
	file.close();
	content = "";
	msg.add_file(TEMP_FILE_NAME, dpp::utility::read_file(TEMP_FILE_NAME));
}
#undef TEMP_FILE_NAME

std::string get_copypasta(std::string msg, std::string in = "")
{
	json copypasta;
	std::ifstream copypasta_file("../copypasta.json");
	copypasta_file >> copypasta;

	if (copypasta.find(msg) == copypasta.end())
		return "";
	std::string out = copypasta[msg];
	
	int i;
	while ((i = out.find("%s")) != -1) {
		out.replace(i, 2, in);
	}
	return out;
}

void init_commands(dpp::commandhandler &command_handler)
{
	command_handler.add_command(
		"findname",
		{
			{"Username", dpp::param_info(dpp::pt_string, false, "The github usename to search for")}
		},
		[&command_handler](const std::string &command, const dpp::parameter_list_t &parameters, dpp::command_source src) {
			std::string username;
			if (!parameters.empty()) 
				username = std::get<std::string>(parameters[0].second);
			std::string response = find_username(username);
			dpp::message msg;
			sanitize_message(response, msg);
			msg.set_content(response);
			command_handler.reply(msg, src);
		},
		"Finds the github username, and their associated GBP value."
	);
	command_handler.add_command(
		"findgbp",
		{
			{"GBP Amount", dpp::param_info(dpp::pt_string, false, "Amount of GBP to search for")}
		},
		[&command_handler](const std::string &command, const dpp::parameter_list_t &parameters, dpp::command_source src) {
			int gbp;
			if (!parameters.empty())
				gbp = std::stoi(std::get<std::string>(parameters[0].second));
			std::string response = find_gbp(gbp);
			dpp::message msg;
			sanitize_message(response, msg);
			msg.set_content(response);
			command_handler.reply(msg, src);
		},
		"Finds the users with the amount of GBP provided."
	);
	command_handler.add_command(
		"findpos",
		{
			{"GBP Amount", dpp::param_info(dpp::pt_string, false, "Amount of GBP to search for")}
		},
		[&command_handler](const std::string &command, const dpp::parameter_list_t &parameters, dpp::command_source src) {
			int pos;
			if (!parameters.empty())
				pos = std::stoi(std::get<std::string>(parameters[0].second));
			std::string response = find_gbp_pos(pos);
			dpp::message msg;
			sanitize_message(response, msg);
			msg.set_content(response);
			command_handler.reply(msg, src);
		},
		"Finds the user at the specified spot on the leaderboard."
	);
	command_handler.add_command(
		"fetchgbp",
		{},
		[&command_handler](const std::string &command, const dpp::parameter_list_t &parameters, dpp::command_source src) {
			fetch_raw_gbp();
			command_handler.reply(dpp::message("Fetched latest GBP!"), src);
		},
		"Fetches the latest GBP."
	);
	command_handler.add_command(
		"copypasta",
		{
			{"message", dpp::param_info(dpp::pt_string, false, "The copypasta")},
			{"replace", dpp::param_info(dpp::pt_string, true, "The string to replace add to the copypasta template")}
		},
		[&command_handler](const std::string &command, const dpp::parameter_list_t &parameters, dpp::command_source src) {
			std::string copypasta_message;
			std::string to_replace;
			if (!parameters.empty()) {
				copypasta_message = std::get<std::string>(parameters[0].second);
				to_replace = std::get<std::string>(parameters[1].second);
			}
			std::string response = get_copypasta(copypasta_message, to_replace);
			dpp::message msg;
			sanitize_message(response, msg);
			msg.set_content(response);
			command_handler.reply(msg, src);
		}
	);
}


