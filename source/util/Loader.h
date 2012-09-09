/*
 * Loader.h
 *
 *  Created on: 13.08.2012
 *      Author: Felix
 */

#ifndef DG_LOADER_H_
#define DG_LOADER_H_

#include <string>
#include <map>
#include <memory>

#include <SFML/System.hpp>

#include <Thor/Resources.hpp>

#include "Singleton.h"

/**
 * This class allows to set default resource folders and subfolders, which means that these
 * folders only have to be set once, and not be included in the creation of every single
 * resource key.
 *
 * If the general resource folder or a specific resource folder is not set, the current
 * directory is searched.
 *
 * R is any resource that can be loaded with Thor's resource loader.
 *
 * Any folder/file parameter can be a full path and is relative to the current directory,
 * or the directory set by the higher variables.
 *
 * @code
 * Loader l;
 * l.setFolder("resources/");
 * l.setSubFolder<sf::Texture>("textures/");
 * thor::ResourceKey<sf::Texture> t = l.fromFile<sf::Texture>("myimage.png");
 *
 * ResourceManager r;
 * r.acquire(t); // This loads from "resources/textures/myimage.png".
 *               //                  folder subfolder file
 * @endcode
 */
class Loader : public Singleton<Loader> {
// Public functions.
public:
	/**
	 * Sets the general resource folder path.
	 */
	inline void setFolder(const std::string& folder) {mFolder = folder;};

	/**
	 * Sets the resource subfolder for the specific type.
	 */
	template <typename R> void
	setSubFolder(std::string path) {
		getLoader<R>()->setSubFolder(path);
	}

	/**
	 * Loads a resource from a file, looking in the specific resource folder for this type.
	 */
	template <typename T> thor::ResourceKey<T>
	fromFile(const std::string& file) {
		return static_cast<SpecificLoader<T>* >(getLoader<T>().get())->fromFile(mFolder, file);
	}

// Private types.
private:
	/**
	 * We need this to save templates of different types in the same container.
	 */
	class LoaderBase {
	public:
		virtual void setSubFolder(const std::string& path) = 0;
	};

	/**
	 * This class forwards the loading of each individual type to Thor.
	 */
	template <typename T>
	class SpecificLoader : public LoaderBase {
	public:
		/**
		 * Sets the subfolder for the current type.
		 */
		void
		setSubFolder(const std::string& path) {
			mSubfolder = path;
		}

		/**
		 * Loads a resource from file via Thor.
		 *
		 * @param folder The general resource folder
		 * @param file Path/name of the file within the resource subfolder.
		 */
		thor::ResourceKey<T>
		fromFile(const std::string& folder, const std::string& file) {
			return thor::Resources::fromFile<T>(folder + mSubfolder + file);
		}

	private:
		std::string mSubfolder;
	};

// Private functions.
private:
	/**
	 * For Singleton behaviour.
	 */
	Loader() = default;
	friend class Singleton<Loader>;

	/**
	 * Gets the correct loader for each template type, creates it if it does not exist.
	 */
	template <typename T> std::unique_ptr<LoaderBase>&
	getLoader() {
		auto loader = mLoaders.find(typeid(T));
		if (loader != mLoaders.end()) {
			return static_cast<std::unique_ptr<LoaderBase>&>(loader->second);
		}
		else {
			return (*mLoaders.insert(std::pair<std::type_index, std::unique_ptr<LoaderBase> >
					(typeid(T), std::unique_ptr<LoaderBase>(new SpecificLoader<T>))).first).second;
		}
	};

// Private variables.
private:
	std::string mFolder;
	std::map<std::type_index, std::unique_ptr<LoaderBase> > mLoaders;
};

#endif /* DG_LOADER_H_ */
