/*
 * ResourceManager.h
 *
 *  Created on: 22.07.2012
 *      Author: Felix
 */

#ifndef DG_RESOURCEMANAGER_H_
#define DG_RESOURCEMANAGER_H_

#include "../abstract/Singleton.h"

/**
 * Loads and manages all resources by providing Singleton access to
 * Thor ResourceManager.
 */
class ResourceManager : public thor::MultiResourceCache,
		public Singleton<ResourceManager> {
// Private functions.
private:
	friend class Singleton<ResourceManager>;
	ResourceManager() {
		setLoadingFailureStrategy(thor::Resources::ThrowException);
		setReleaseStrategy(thor::Resources::AutoRelease);
	};
};

#endif /* DG_RESOURCEMANAGER_H_ */
