#include <dpp/dispatcher.h>
#include <dpp/dpp.h>
#include <dpp/nlohmann/json.hpp>
#include <fstream>
#include "commands.cpp"

using json = nlohmann::json;

int main()
{
	json config;
	std::ifstream config_file("../config.json");
	config_file >> config;
	std::string token = config["token"];
	dpp::cluster bot(token, dpp::i_default_intents | dpp::i_message_content);

	bot.on_log(dpp::utility::cout_logger());
	
	dpp::commandhandler command_handler(&bot);
	command_handler.add_prefix("!gbp ");

	init_commands(command_handler);
	
	bot.start(false);
	return 0;
}
