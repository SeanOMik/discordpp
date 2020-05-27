#include "emoji.h"
#include "guild.h"

namespace discpp {
	Emoji::Emoji(const std::string& name, const snowflake& id) : name(EscapeString(name)), id(id) {
		/**
		 * @brief Constructs a discpp::Emoji object with a name and id.
		 *
		 * ```cpp
		 *      discpp::Emoji emoji("no_x", 657246994997444614);
		 * ```
		 *
		 * @param[in] name The name of the emoji
		 * @param[in] id The id of the emoji
		 *
		 * @return discpp::Emoji, this is a constructor.
		 */
	}

	Emoji::Emoji(const discpp::Guild& guild, const snowflake& id) : id(id) {
		/**
		 * @brief Constructs a discpp::Emoji object using a guild object and id.
		 *
		 * This constructor searches the emoji cache in the guild object to get an emoji object.
		 *
		 * ```cpp
		 *      discpp::Emoji emoji(guild, 657246994997444614);
		 * ```
		 *
		 * @param[in] guild The guild that has this emoji.
		 * @param[in] id The id of the emoji.
		 *
		 * @return discpp::Emoji, this is a constructor.
		 */

		auto it = guild.emojis.find(id);
		if (it != guild.emojis.end()) {
			*this = it->second;
		}
	}

	Emoji::Emoji(rapidjson::Document& json) {
		/**
		 * @brief Constructs a discpp::Emoji object by parsing json.
		 *
		 * ```cpp
		 *      discpp::Emoji emoji(json);
		 * ```
		 *
		 * @param[in] json The json that makes up of emoji object.
		 *
		 * @return discpp::Emoji, this is a constructor.
		 */

		id = GetIDSafely(json, "id");
		name = GetDataSafely<std::string>(json, "name");
		if (ContainsNotNull(json, "roles")) {
			for (auto& role : json["roles"].GetArray()) {
				rapidjson::Document role_json;
				role_json.CopyFrom(role, role_json.GetAllocator());
				roles.emplace_back(SnowflakeFromString(role.GetString()));
			}
		}
		if (ContainsNotNull(json, "user")) {
			rapidjson::Document user_json;
			user_json.CopyFrom(json["user"], user_json.GetAllocator());
			creator = discpp::User(user_json);
		}
		require_colons = GetDataSafely<bool>(json, "require_colons");
        managed = GetDataSafely<bool>(json, "managed");
        animated = GetDataSafely<bool>(json, "animated");
	}

	Emoji::Emoji(const std::wstring& w_unicode) : unicode(w_unicode) {
		/**
		 * @brief Constructs a discpp::Emoji object with a std::wstring unicode representation.
		 *
		 * ```cpp
		 *      discpp::Emoji emoji( (std::wstring) L"\u0030");
		 * ```
		 *
		 * @param[in] w_unicode The std::wstring unicode representation of this emoji.
		 *
		 * @return discpp::Emoji, this is a constructor.
		 */
	}

    Emoji::Emoji(const std::string& s_unicode) {
        /**
         * @brief Constructs a discpp::Emoji object with a std::string unicode representation.
         *
         * ```cpp
         *      discpp::Emoji emoji( (std::string) "\u0030");
         * ```
         *
         * @param[in] s_unicode The std::string unicode representation of this emoji.
         *
         * @return discpp::Emoji, this is a constructor.
         */

#ifdef WIN32
        wchar_t thick_emoji[MAX_PATH];
        if (!MultiByteToWideChar(CP_ACP, WC_COMPOSITECHECK, s_unicode.c_str(), -1, thick_emoji, MAX_PATH)) {
            throw std::runtime_error("Failed to convert emoji to string!");
        } else {
            std::cout << "Just processed: " << thick_emoji << std::endl;
            this->unicode = thick_emoji;
        }
#else
        auto converter = std::wstring_convert<std::codecvt_utf8<wchar_t>>();
        this->unicode = converter.from_bytes(s_unicode);
#endif
    }
}