/*
 * Yaml.h
 *
 *  Created on: 06.10.2012
 *      Author: Felix
 */

#ifndef DG_YAML_H_
#define DG_YAML_H_

#include <fstream>

#include <yaml-cpp/yaml.h>

#include "../types/String.h"
#include "../types/Vector.h"

/**
 * Interface to a YAML file.
 */
class Yaml {
// Public functions.
public:
	Yaml(const String& filename);
	~Yaml();

	String getFilename() const;

	static void setFolder(const String& folder);

	template <typename T>
	T get(const String& key, const T& defaultValue) const;

// Private variables.
private:
	static String mFolder;

	String mFilename;
	std::ifstream mFile;
	YAML::Node mNode;
};

/**
 * Stream output operators to specialize Yaml::get for other types.
 */
namespace {
	void operator>>(const YAML::Node& node, Vector2i& vector) {
		node[0] >> vector.x;
		node[1] >> vector.y;
	}

	void operator>>(const YAML::Node& node, Vector2f& vector) {
		node[0] >> vector.x;
		node[1] >> vector.y;
	}
};

/**
 * Gets a value of a specified type by key. Throws exception if key not found.
 *
 * @param key The string by which to select the return value.
 * @tparam T The type of the return value.
 * @return The value of the specified key.
 */
template <typename T>
T Yaml::get(const String& key, const T& defaultValue) const {
	if (const YAML::Node* node = mNode.FindValue(key)) {
		T value;
		*node >> value;
		return value;
	}
	else {
		return defaultValue;
	}
};

#endif /* DG_YAML_H_ */
