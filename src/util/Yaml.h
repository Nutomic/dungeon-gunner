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

#include "Log.h"
#include "Vector.h"

/**
 * Interface to a YAML file.
 */
class Yaml {
public:
	explicit Yaml(const std::string& filename);

	static void setFolder(const std::string& folder);

	template <typename T>
	T get(const std::string& key, const T& defaultValue) const;

private:
	static std::string mFolder;
	std::string mFilename;
	YAML::Node mNode;
};

/**
 * Template specializations to convert yaml parameters into custom types.
 */
namespace YAML {
template<>
struct convert<Vector2f> {
	static bool decode(const Node& node, Vector2f& rhs) {
		if(!node.IsSequence() || node.size() != 2)
			return false;

		rhs.x = node[0].as<float>();
		rhs.y = node[1].as<float>();
		return true;
	}
};

template<>
struct convert<Vector2i> {
	static bool decode(const Node& node, Vector2i& rhs) {
		if(!node.IsSequence() || node.size() != 2)
			return false;

		rhs.x = node[0].as<int>();
		rhs.y = node[1].as<int>();
		return true;
	}
};
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
    if(mNode[key])
    	return mNode[key].as<T>();
    else
    	return defaultValue;
};

#endif /* DG_YAML_H_ */
