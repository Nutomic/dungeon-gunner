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

#include "../abstract/Singleton.h"

template <class T>
class Singleton;

/**
 * Loads and manages all resources by providing Singleton access to Thor ResourceManager.
 */
class ResourceManager : public thor::MultiResourceCache, public Singleton<ResourceManager> {
// Private functions.
private:
	friend class Singleton<ResourceManager>;
	ResourceManager() = default;
};

#endif /* DG_RESOURCEMANAGER_H_ */
