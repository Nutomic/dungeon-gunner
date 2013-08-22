/*
 * Yaml.cpp
 *
 *  Created on: 06.10.2012
 *      Author: Felix
 */

#include "Yaml.h"

#include "../util/Log.h"

std::string Yaml::mFolder = "";

/**
 * Creates a readable object from a YAML file. The path must be relative to the directory
 * set in setFolder().
 */
Yaml::Yaml(const std::string& filename) :
		mFilename(mFolder+filename),
		mFile(mFilename) {
	if (mFile.fail())
		LOG_W("Failed to open YAML file: " << mFolder << filename);
	YAML::Parser parser(mFile);
	parser.GetNextDocument(mNode);
}

Yaml::~Yaml() {
	mFile.close();
}

/**
 * Return path and name of the file opened in this object.
 */
std::string
Yaml::getFilename() const {
	return mFilename;
}

/**
 * Sets the folder where YAML files are placed. Is added in front of each file name. Allows
 * shorter strings as this does not have to be added everywhere.
 */
void
Yaml::setFolder(const std::string& folder) {
	mFolder = folder;
}
