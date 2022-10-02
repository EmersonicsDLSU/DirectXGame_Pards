#pragma once
#include <unordered_map>
#include <string>
#include "Prerequisites.h"
#include "Resource.h"

/* Abstract class
 * handles resources in such a way to simplify some very complex tasks such as
 * textures, static meshes, and materials.
 */
class ResourceManager
{
public:
	ResourceManager();
	virtual ~ResourceManager();

	ResourcePtr createResourceFromFile(const wchar_t* file_path);
protected:
	virtual Resource* createResourceFromFileConcrete(const wchar_t* file_path) = 0;
private:
	// map for the path and the resource reference
	std::unordered_map<std::wstring, ResourcePtr> m_map_resources;
};

