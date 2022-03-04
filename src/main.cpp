#include <dpp/appcommand.h>
#include <dpp/dispatcher.h>
#include <dpp/dpp.h>
#include <dpp/intents.h>
#include <dpp/message.h>
#include <dpp/once.h>
#include <dpp/queues.h>
#include <string>
#include "gbp.h"


//TODO: put these into a gitignored text file :))))))))
const std::string BOT_TOKEN = "OTQ5MTIxNDU2MTY2NTM5Mjk2.YiFwPA.Hrj5LxqQuApFZfNuSUBVXe3kYEQ";
const std::string BOT_COMMAND = "!gbp";


const dpp::snowflake GUILD_ID = 865347537287249980; 

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
	return msg.content.substr(0, (msg.content.find(" "))) == BOT_COMMAND;
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

	int argIdx = msg.content.find(" ");
	std::string argument = msg.content.substr(argIdx, msg.content.length() - argIdx);
	dpp::message toSend = dpp::message(msg.channel_id, argument); 

	bot.message_create(toSend);
}

int main()
{
	//Setup bot 
	dpp::cluster bot(BOT_TOKEN, dpp::i_default_intents | dpp::i_message_content);

	bot.on_log(dpp::utility::cout_logger());

	bot.on_message_create([&](const dpp::message_create_t &event) {
		if (event.msg.author.id != bot.me.id) // don't respond to own messages
			onMessage(bot, event.msg);
	});
	
	bot.start(false);

	return 0;
}
