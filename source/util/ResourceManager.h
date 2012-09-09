/*
 * ResourceManager.h
 *
 *  Created on: 22.07.2012
 *      Author: Felix
 */

#ifndef DG_RESOURCEMANAGER_H_
#define DG_RESOURCEMANAGER_H_

#include <SFML/Graphics.hpp>

#include <Thor/Resources.hpp>

#include "Singleton.h"

/**
 * Loads and manages all resources by providing Singleton access to Thor ResourceManager.
 */
class ResourceManager : public thor::MultiResourceCache, public Singleton<ResourceManager> {
private:
	friend class Singleton<ResourceManager>;
	ResourceManager() = default;
};

#endif /* DG_RESOURCEMANAGER_H_ */
