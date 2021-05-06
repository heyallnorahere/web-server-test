#include "status.h"
namespace apistandard {
	void to_json(nlohmann::json& j, const status& s) {
		j["up"] = s.up;
	}
	void from_json(const nlohmann::json& j, status& s) {
		j["up"].get_to(s.up);
	}
}