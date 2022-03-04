#include <cstddef>
#include <dpp/appcommand.h>
#include <dpp/dispatcher.h>
#include <dpp/dpp.h>
#include <dpp/intents.h>
#include <dpp/message.h>
#include <dpp/once.h>
#include <dpp/queues.h>
#include <dpp/nlohmann/json.hpp>
#include <bits/stdc++.h>
#include <string>
#include "commands.cpp"

using json = nlohmann::json;

std::vector<std::string> separate_args(std::string args)
{
	std::vector<std::string> out;
	while (1) {
		if (args.find(" ") == -1) {
			out.push_back(args);
			break;
		} else {
			out.push_back(args.substr(0, args.find(" ")));
			args = args.substr(args.find(" ") + 1, args.length() - (args.find(" ") + 1));
		}
	}
	return out;
}

/**
 * ##hasCommand
 *
 * If the given message starts with the bots command BOT_COMMAND, return true
 *
 * Arguments: 
 * * dpp::message msg - The message in question
 */
bool hasCommand(dpp::message msg)
{
	json config;
	std::ifstream configFile("../config.json");
	configFile >> config;
	return msg.content.substr(0, (msg.content.find(" "))) == config["bot_command"];
}

dpp::message setMessage(unsigned int channel_id, std::string content)
{
	std::vector<std::string> messageArgs = separate_args(content);
	if (messageArgs[0] == std::string(FILE_WARNING)) {
		dpp::message msg = dpp::message(channel_id, "");
		msg.add_file("gbp-list.txt",  dpp::utility::read_file(content.substr(content.find(" ") + 1, content.length() - (content.find(" ") + 1))));
		return msg;
	} else {
		dpp::message msg = dpp::message(channel_id, content);
		return msg;
	}
}



/**
 * ##onMessage
 *
 * Handles message commands
 *
 * Arguments:
 * * dpp::cluster &bot - The discord bot
 * * dpp::message msg - The received message
 */
void onMessage(dpp::cluster &bot, dpp::message msg)
{
	if (!hasCommand(msg))
		return;

	int argIdx = msg.content.find(" ") + 1;
	std::string argument = msg.content.substr(argIdx, msg.content.length() - argIdx);

	std::vector<std::string> args = separate_args(argument);
	std::string msgContent = commandParse(args);
	std::cout << msgContent << std::endl;

	dpp::message toSend = setMessage(msg.channel_id, msgContent);

	bot.message_create(toSend);
}

int main()
{	
	
	/* Setup the bot **/	
	json config;
	std::ifstream configFile("../config.json");
	configFile >> config;

	std::string token = config["token"];
	dpp::cluster bot(token, dpp::i_default_intents | dpp::i_message_content);

	/* Start listening to commands */
	bot.on_log(dpp::utility::cout_logger());

	bot.on_message_create([&](const dpp::message_create_t &event) {
		if (event.msg.author.id != bot.me.id) // don't respond to own messages
			onMessage(bot, event.msg);
	});
	
	bot.start(false);

	return 0;
}
