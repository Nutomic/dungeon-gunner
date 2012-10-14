/*
 * Yaml.cpp
 *
 *  Created on: 06.10.2012
 *      Author: Felix
 */

#include "Yaml.h"

#include "../util/Log.h"

String Yaml::mFolder = "";

/**
 * Creates a readable object from a YAML file. The path must be relative to the directory
 * set in setFolder().
 */
Yaml::Yaml(const String& filename) :
		mFile(mFolder+filename) {
	if (mFile.fail()) {
		LOG_W("Failed to open YAML file: " << mFolder << filename);
	}
	YAML::Parser parser(mFile);
	parser.GetNextDocument(mNode);
}

Yaml::~Yaml() {
	mFile.close();
}

/**
 * Sets the folder where YAML files are placed. Is added in front of each file name. Allows
 * shorter strings as this does not have to be added everywhere.
 */
void
Yaml::setFolder(const String& folder) {
	mFolder = folder;
}
