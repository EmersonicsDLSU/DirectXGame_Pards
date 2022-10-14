#pragma once
#include <vector>

#include "Matrix4x4.h"
#include "AGameObject.h"
#include "Cube.h"
#include "Mesh.h"
#include "Prerequisites.h"

class Matrix4x4;

struct TransparencyFilterPolicy
{
private:
	Matrix4x4 _camera;
public:
	void SetCamera(const Matrix4x4& camera)
	{
		_camera = camera;
	}
	// ignore Opaque pass
	bool ShouldRender(AGameObjectPtr object)
	{
		AGameObject* raw_obj = object.get();
		if (dynamic_cast<Cube*>(raw_obj)->GetAlpha() >= 1.0f)
		{
			return false;
		}
		return true;
	}
};

struct OpaqueFilterPolicy
{
private:
	Matrix4x4 _camera;
public:
	void SetCamera(const Matrix4x4& camera)
	{
		_camera = camera;
	}
	// ignore Transparent pass
	bool ShouldRender(AGameObjectPtr object)
	{
		AGameObject* raw_obj = object.get();
		if (dynamic_cast<Cube*>(raw_obj)->GetAlpha() < 1.0f)
		{
			return false;
		}
		return true;
	}
};

struct BackToFrontPolicy
{
private:
	Matrix4x4 _camera;
public:
	void SetCamera(const Matrix4x4& camera)
	{
		_camera = camera;
	}
	std::vector<AGameObjectPtr>& sort(std::vector<AGameObjectPtr>& inputArray)
	{
		int i, j, max_idx;
		int n = inputArray.size();
		// selection sort
		for (i = 0; i < n - 1; i++)
		{

			// Find the minimum element in
			// unsorted array
			max_idx = i;
			for (j = i + 1; j < n; j++)
			{
				if (inputArray[j]->GetDistance(_camera) > inputArray[max_idx]->GetDistance(_camera))
					max_idx = j;
			}

			// Swap the found minimum element
			// with the first element
			if (max_idx != i)
				swap(inputArray[max_idx], inputArray[i]);
		}

		return inputArray;

	}
private:
	void swap(AGameObjectPtr& xp, AGameObjectPtr& yp)
	{
		AGameObjectPtr temp = xp;
		xp = yp;
		yp = temp;
	}
};

struct FrontToBackPolicy
{
private:
	Matrix4x4 _camera;
public:
	void SetCamera(const Matrix4x4& camera)
	{
		_camera = camera;
	}
	std::vector<AGameObjectPtr>& sort(std::vector<AGameObjectPtr>& inputArray)
	{
		int i, j, min_idx;
		int n = inputArray.size();
		// selection sort
		for (i = 0; i < n - 1; i++)
		{

			// Find the minimum element in
			// unsorted array
			min_idx = i;
			for (j = i + 1; j < n; j++)
			{
				if (inputArray[j]->GetDistance(_camera) < inputArray[min_idx]->GetDistance(_camera))
					min_idx = j;
			}

			// Swap the found minimum element
			// with the first element
			if (min_idx != i)
				swap(inputArray[min_idx], inputArray[i]);
		}

		return inputArray;

	}
private:
	void swap(AGameObjectPtr& xp, AGameObjectPtr& yp)
	{
		AGameObjectPtr temp = xp;
		xp = yp;
		yp = temp;
	}
};

class RenderPolicies
{
	
};