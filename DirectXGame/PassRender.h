#pragma once
#include <vector>
#include "GameObjectManager.h"
#include "RenderPolicies.h"
#include "AGameObject.h"
#include "Prerequisites.h"
#include "Matrix4x4.h"

template<typename FilteringPolicy, typename SortingPolicy>
class PassRender
{
public:
	void Render(const VertexShaderPtr& m_vs, const PixelShaderPtr& m_ps, const BlenderPtr& m_blender, const Matrix4x4& camera)
	{
		m_sortingPolicy.SetCamera(camera);
		m_filterPolicy.SetCamera(camera);

		std::vector<AGameObjectPtr>& objectsToRender = GameObjectManager::get()->getObjectList();
		std::vector<AGameObjectPtr>& sortedObjects = m_sortingPolicy.sort(objectsToRender);

		for (auto object : sortedObjects)
		{
			if (m_filterPolicy.ShouldRender(object))
				object->Draw(m_vs, m_ps, m_blender);
		}
	}
private:
	SortingPolicy m_sortingPolicy;
	FilteringPolicy m_filterPolicy;
};

