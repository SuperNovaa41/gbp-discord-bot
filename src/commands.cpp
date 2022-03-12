#include "gbp.cpp"
#include <dpp/dpp.h>
#include <dpp/commandhandler.h>
#include <dpp/message.h>
#include <iostream>

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
			command_handler.reply(dpp::message(response), src);
		},
		"Finds the github username, and their associated GBP value.",
		865347537287249980
	);


	std::cout << "Command added!\n";
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
			command_handler.reply(dpp::message(response), src);
		},
		"Finds the users with the amount of GBP provided.",
		865347537287249980
	);

	std::cout << "Command added!\n";
	command_handler.add_command(
		"findpos",
		{
			{"GBP Amount", dpp::param_info(dpp::pt_string, false, "Amount of GBP to search for")}
		},
		[&command_handler](const std::string &command, const dpp::parameter_list_t &parameters, dpp::command_source src) {
			int pos;
			if (!parameters.empty())
				pos= std::stoi(std::get<std::string>(parameters[0].second));
			std::string response = find_gbp_pos(pos);
			command_handler.reply(dpp::message(response), src);
		},
		"Finds the users with the amount of GBP provided.",
		865347537287249980
	);

	std::cout << "Command added!\n";
	command_handler.add_command(
		"fetchgbp",
		{
			{"Test", dpp::param_info(dpp::pt_string, true, "test")}
		},
		[&command_handler](const std::string &command, const dpp::parameter_list_t &parameters, dpp::command_source src) {
			fetch_raw_gbp();
			command_handler.reply(dpp::message("Fetched latest GBP!"), src);
		},
		"Fetches the latest GBP.",
		865347537287249980
	);

	std::cout << "Command added!\n";
}


