#pragma once
#include <nlohmann/json.hpp>
namespace apistandard {
	struct status {
		bool up;
	};
	void to_json(nlohmann::json& j, const status& s);
	void from_json(const nlohmann::json& j, status& s);
}