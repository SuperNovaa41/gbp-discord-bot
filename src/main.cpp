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
	std::cout << "Command received!\n";


	int argIdx = msg.content.find(" ") + 1;
	std::string argument = msg.content.substr(argIdx, msg.content.length() - argIdx);

	std::string msgContent = "";
	if (argument == "gbp")
		msgContent = printFullGBPList();

	dpp::message toSend = dpp::message(msg.channel_id, msgContent); 

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
