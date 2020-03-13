#ifndef DISCORDPP_MESSAGE_REACTION_REMOVE_EVENT_H
#define DISCORDPP_MESSAGE_REACTION_REMOVE_EVENT_H

#include "../event.h"
#include "../message.h"
#include "../emoji.h"
#include "../user.h"

#include <nlohmann/json.hpp>

namespace discord {
	class MessageReactionRemoveEvent : public Event {
	public:
		inline MessageReactionRemoveEvent(nlohmann::json json) : message(discord::Message(json["message_id"].get<snowflake>())) {
			message.channel = discord::Channel(json["channel_id"].get<snowflake>());

			if (json.contains("guild_id")) {
				message.guild = discord::Guild(json["guild_id"].get<snowflake>());
				message.channel.guild_id = json["guild_id"].get<snowflake>();
			}

			emoji = discord::Emoji(json["emoji"]);
			user = discord::User(json["user_id"].get<snowflake>());
		}

		inline MessageReactionRemoveEvent(discord::Message message, discord::Emoji emoji, discord::User user) : message(message), emoji(emoji), user(user) {}

		discord::Message message;
		discord::Emoji emoji;
		discord::User user;
	};
}

#endif