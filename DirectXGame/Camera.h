#pragma once
#include "AGameObject.h"
#include "InputListener.h"
#include "Prerequisites.h"

class Point;

class Camera : public AGameObject, public InputListener
{
public:
	Camera(std::string name, ObjectTypes type);
	~Camera();
public:
	void Update(float deltaTime, AppWindow* app_window) override;

	Matrix4x4 GetCamViewMatrix();
	void UpdateViewMatrix();

	void onKeyDown(int key) override;
	void onKeyUp(int key) override;
	void onMouseMove(const Point& mouse_pos) override;
	void onRightMouseDown(const Point& delta_mouse_pos) override;
	void onRightMouseUp(const Point& delta_mouse_pos) override;
private:
	float m_forward = 0.0f;
	float m_rightward = 0.0f;
	bool mouseDown = false;
	const float NAVIGATE_SPEED = 5.0f;
};

