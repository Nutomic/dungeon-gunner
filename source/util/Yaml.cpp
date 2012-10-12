/*
 * Yaml.cpp
 *
 *  Created on: 06.10.2012
 *      Author: Felix
 */

#include "Yaml.h"

String Yaml::mFolder = "";

/**
 * Creates a readable object from a YAML file. The path must be relative to the directory
 * set in setFolder().
 */
Yaml::Yaml(const String& filename) {
	std::ifstream file(mFolder+filename);
	YAML::Parser parser(file);
	parser.GetNextDocument(mNode);
}

/**
 * Sets the folder where YAML files are placed. Is added in front of each file name. Allows
 * shorter strings as this does not have to be added everywhere.
 */
void
Yaml::setFolder(const String& folder) {
	mFolder = folder;
}
