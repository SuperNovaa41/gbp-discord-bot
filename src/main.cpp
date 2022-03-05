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

#define BOT_COMMAND "!gbp"

/**
 * ##separateArgs
 *
 * Returns a vector of each word in the given input, split by " "
 * 
 * Arguments:
 * * std::string args - The string to split up
 */

std::vector<std::string> separateArgs(std::string args)
{
	std::vector<std::string> out;
	while (1) {
		if (args.find(" ") == -1) {
			out.push_back(args);
			break;
		}
		out.push_back(args.substr(0, args.find(" ")));
		args = args.substr(args.find(" ") + 1, args.length() - (args.find(" ") + 1));
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
	return msg.content.substr(0, (msg.content.find(" "))) == std::string(BOT_COMMAND);
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

	/* Parse the command */
	std::vector<std::string> args = separateArgs(argument);
	std::string msgContent = commandParse(args);

	/* Here we check if we should embed a file, or just send a message */
	std::vector<std::string> messageArgs = separateArgs(msgContent);
	dpp::message toSend;
	if (messageArgs[0] == std::string(FILE_WARNING)) {
		toSend = dpp::message(msg.channel_id, "");
		toSend.add_file(messageArgs[2],  dpp::utility::read_file(messageArgs[1]));
	} else 
		toSend = dpp::message(msg.channel_id, msgContent);
	
	/* Send the message */
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
	initCommands();

	bot.on_message_create([&](const dpp::message_create_t &event) {
		if (event.msg.author.id != bot.me.id) // don't respond to own messages
			onMessage(bot, event.msg);
	});
	
	bot.start(false);

	return 0;
}
