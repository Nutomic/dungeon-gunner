/*
 * Yaml.h
 *
 *  Created on: 30.03.2013
 *      Author: Felix
 */ 

#include <SFML/System.hpp>

#include "Log.h"

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
}

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
		const YAML::Node* node = mNode.FindValue(key);
		T value;
		*node >> value;
		return value;
	}
	catch (YAML::InvalidScalar&) {
		LOG_W("Failed to get key " << key << " from " << mFolder << mFilename);
		return defaultValue;
	}
};