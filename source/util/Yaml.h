/*
 * Yaml.h
 *
 *  Created on: 06.10.2012
 *      Author: Felix
 */

#ifndef DG_YAML_H_
#define DG_YAML_H_

#include <string>
#include <fstream>

#include <yaml-cpp/yaml.h>

#include <SFML/System.hpp>

#include "Log.h"

/**
 * Interface to a YAML file.
 */
class Yaml {
public:
	explicit Yaml(const std::string& filename);
	~Yaml();

	std::string getFilename() const;

	static void setFolder(const std::string& folder);

	template <typename T>
	T get(const std::string& key, const T& defaultValue) const;

private:
	static std::string mFolder;

	std::string mFilename;
	std::ifstream mFile;
	YAML::Node mNode;
};

/**
 * Stream output operators to specialize Yaml::get for other types.
 * Error handling is done in Yaml::get.
 */
namespace {
	void operator>>(const YAML::Node& node, sf::Vector2i& vector) {
		node[0] >> vector.x;
		node[1] >> vector.y;
	}

	void operator>>(const YAML::Node& node, sf::Vector2f& vector) {
		node[0] >> vector.x;
		node[1] >> vector.y;
	}
};

/**
 * Gets a value of a specified type by key. Returns default value on error.
 *
 * @param key The string by which to select the return value.
 * @tparam T The type of the return value.
 * @return The value of the specified key.
 */
template <typename T>
T Yaml::get(const std::string& key, const T& defaultValue) const {
	try {
		if (const YAML::Node* node = mNode.FindValue(key)) {
			T value;
			*node >> value;
			return value;
		}
		else
			return defaultValue;
	}
	catch(YAML::Exception&) {
		return defaultValue;
	}
};

namespace YAML_KEY {
	// Sprite
	const std::string SIZE = "size";
	const std::string RADIUS = "radius";
	const std::string TEXTURE = "texture";

	// Character
	const std::string HEALTH = "health";
	const std::string SPEED = "speed";
	const std::string WEAPON = "weapon";

	// Bullet
	const std::string DAMAGE = "damage";

	// Weapon
	const std::string BULLET = "bullet";
	const std::string INTERVAL = "interval";
	const std::string AUTOMATIC = "automatic";
}

namespace YAML_DEFAULT {
	const int HEALTH = 100;
	const float SPEED = 100;
	const std::string WEAPON = "weapon.yaml";
	const int DAMAGE = 10;
	const int INTERVAL = 250;
	const std::string BULLET = "bullet.yaml";
	const bool AUTOMATIC = false;
}

#endif /* DG_YAML_H_ */
