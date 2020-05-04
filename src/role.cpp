#include "guild.h"

namespace discpp {
	Role::Role(snowflake role_id, discpp::Guild& guild) : DiscordObject(role_id) {
		/**
		 * @brief Constructs a discpp::Role object using a role id and a guild.
		 *
		 * This constructor searches the roles cache in the guild object to get the role object from.
		 *
		 * ```cpp
		 *      discpp::Role role(657246994997444614, guild);
		 * ```
		 *
		 * @param[in] role_id The role id.
		 * @param[in] guild The guild that has this role.
		 *
		 * @return discpp::Role, this is a constructor.
		 */

		std::unordered_map<snowflake, Role>::iterator it = guild.roles.find(role_id); 
		if (it != guild.roles.end()) {
			*this = it->second;
		}
	}

	Role::Role(rapidjson::Document& json) {
		/**
		 * @brief Constructs a discpp::Role object by parsing json.
		 *
		 * ```cpp
		 *      discpp::Role role(json);
		 * ```
		 *
		 * @param[in] json The json that makes up of role object.
		 *
		 * @return discpp::Role, this is a constructor.
		 */

		id = static_cast<snowflake>(json["id"].GetString());
		name = json["name"].GetString();
		color = json["color"].GetInt();
		hoist = json["hoist"].GetInt();
		position = json["position"].GetInt();
		rapidjson::Value::ConstMemberIterator itr = json.FindMember("permissions");
		if (itr != json.MemberEnd()) {
			permissions = Permissions(PermissionType::ROLE, json["permissions"].GetInt());
		}
		else {
			permissions = Permissions();
		}
		managed = json["manages"].GetBool();
		mentionable = json["mentionable"].GetBool();
	}
}