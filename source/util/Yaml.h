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

#include "String.h"

/**
 * Interface to a YAML file.
 */
class Yaml {
// Public functions.
public:
	Yaml(const String& filename);

	static void setFolder(const String& folder);
	/**
	 * Gets a value of a specified type by key. Throws exception if key not found.
	 *
	 * @param key The string by which to select the return value.
	 * @tparam T The type of the return value.
	 * @return The value of the specified key.
	 */
	template <typename T>
	T get(const String& key) const {
		T tmp;
		mNode[key] >> tmp;
		return tmp;
	};

// Private variables.
private:
	static String mFolder;
	YAML::Node mNode;
};

#endif /* DG_YAML_H_ */
