#ifndef DISCPP_MESSAGE_H
#define DISCPP_MESSAGE_H

#include "discord_object.h"
#include "channel.h"
#include "user.h"
#include "member.h"
#include "guild.h"
#include "reaction.h"
#include "role.h"
#include "embed_builder.h"
#include "attachment.h"

namespace discpp {
	enum class GetReactionsMethod : int {
		BEFORE_USER,
		AFTER_USER
	};

	struct MessageActivity {
		enum class ActivityType : int {
			NONE = 0,
			JOIN = 1,
			SPECTATE = 2,
			LISTEN = 3,
			JOIN_REQUEST = 5
		};

		ActivityType type;
		std::string party_id;

		MessageActivity() = default;
		MessageActivity(rapidjson::Document& json) {
			type = static_cast<ActivityType>(json["type"].GetInt());
			party_id = GetDataSafely<std::string>(json, "party_id");
		}
	};

	struct MessageApplication : public DiscordObject {
		//snowflake id;
		std::string cover_image;
		std::string description;
		std::string icon;
		std::string name;

		MessageApplication() = default;
		MessageApplication(rapidjson::Document& json) {
			id = SnowflakeFromString(json["id"].GetString());
			cover_image = GetDataSafely<std::string>(json, "cover_image");
			description = json["description"].GetString();
			icon = json["icon"].GetString();
			name = json["name"].GetString();
		}
	};

	struct MessageReference {
		snowflake message_id;
		snowflake channel_id;
		snowflake guild_id;

		MessageReference() = default;
		MessageReference(rapidjson::Document& json) {
			message_id = GetIDSafely(json, "message_id");
			channel_id = SnowflakeFromString(json["channel_id"].GetString());
			guild_id = GetIDSafely(json, "guild_id");
		}
	};

	class Message : public DiscordObject {
	public:
	    class ChannelMention : public DiscordObject {
	    public:
            ChannelMention(rapidjson::Document& json) {
                id = SnowflakeFromString(json["id"].GetString());
                guild_id = SnowflakeFromString(json["id"].GetString());
                type = static_cast<discpp::ChannelType>(json["type"].GetInt());
                name = json["name"].GetString();
            }

            discpp::snowflake guild_id;
            discpp::ChannelType type;
            std::string name;
	    };

		Message() = default;
		Message(const snowflake& id);
		Message(const snowflake& message_id, const snowflake& channel_id);
		Message(rapidjson::Document& json);

		void AddReaction(const discpp::Emoji& emoji);
		void RemoveBotReaction(const discpp::Emoji& emoji);
		void RemoveReaction(const discpp::User& user, const discpp::Emoji& emoji);
        std::unordered_map<discpp::snowflake, discpp::User> GetReactorsOfEmoji(const discpp::Emoji& emoji, const int& amount);
		std::unordered_map<discpp::snowflake, discpp::User> GetReactorsOfEmoji(const discpp::Emoji& emoji, const discpp::User& user, const GetReactionsMethod& method);
		void ClearReactions();
		discpp::Message EditMessage(const std::string& text);
		discpp::Message EditMessage(discpp::EmbedBuilder& embed);
		discpp::Message EditMessage(const int& flags);
		void DeleteMessage();
		inline void PinMessage();
		inline void UnpinMessage();
        inline bool IsTTS();
        inline bool MentionsEveryone();
        inline bool IsPinned();

        discpp::Channel channel;
        std::shared_ptr<discpp::Guild> guild;
        std::shared_ptr<discpp::User> author;
		std::string content;
		std::string timestamp; // TODO: Convert to iso8601Time
		std::string edited_timestamp; // TODO: Convert to iso8601Time
		std::unordered_map<discpp::snowflake, discpp::User> mentions;
		std::vector<discpp::snowflake> mentioned_roles;
        std::unordered_map<discpp::snowflake, ChannelMention> mention_channels;
		std::vector<discpp::Attachment> attachments;
		std::vector<discpp::EmbedBuilder> embeds;
		std::vector<discpp::Reaction> reactions;
		snowflake webhook_id;
		int type;
		discpp::MessageActivity activity;
		discpp::MessageApplication application;
		discpp::MessageReference message_reference;
		int flags;
	private:
	    char bit_flags; /**< For internal use only. */
	};
}

#endif