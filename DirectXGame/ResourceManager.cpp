#include "ResourceManager.h"

#if __cplusplus <= 201402L 
#define _SILENCE_EXPERIMENTAL_FILESYSTEM_DEPRECATION_WARNING
#include <experimental/filesystem>
#endif

#if __cplusplus >= 201703L
#include <filesystem>
#endif

ResourceManager::ResourceManager()
{
}


ResourceManager::~ResourceManager()
{
}

ResourcePtr ResourceManager::createResourceFromFile(const wchar_t* file_path)
{
	// transforms the passed file_path to absolute; ensures a unique key for each reserves
#if (_MSC_VER >= 1900 && _MSC_VER <= 1916)  || ( _MSC_VER >= 1920 && __cplusplus <= 201402L) 
	std::wstring full_path = std::experimental::filesystem::absolute(file_path);
#endif

#if _MSC_VER >= 1920 && __cplusplus > 201402L 
	std::wstring full_path = std::filesystem::absolute(file_path);
#endif

	// find the recently transformed path from the unordered map
	auto it = m_map_resources.find(full_path);
	// if found, then we return the resourcePtr of that path
	if (it != m_map_resources.end())
		return it->second;

	// if not found, then we need to allocate it for the first time
	Resource* raw_res = this->createResourceFromFileConcrete(full_path.c_str());
	if (raw_res)
	{
		// copy constructor; passed the reference to the smart pointer
		ResourcePtr res(raw_res);
		m_map_resources[full_path] = res;
		return res;
	}

	return nullptr;
}

