#pragma once
#include "AGameObject.h"
#include "InputListener.h"
#include "Prerequisites.h"

class Point;

class Camera : public AGameObject, public InputListener
{
public:
	Camera(std::string name, ObjectTypes type, AppWindow* app_window);
	~Camera();
public:
	void Update(float deltaTime, AppWindow* app_window) override;

	Matrix4x4 GetCamViewMatrix();
	void UpdateViewMatrix();

	void onKeyDown(int key) override;
	void onKeyUp(int key) override;
	void onMouseMove(const Point& mouse_pos) override;
	void onLeftMouseDown(const Point& delta_mouse_pos) override;
	void onLeftMouseUp(const Point& delta_mouse_pos) override;
	void onRightMouseDown(const Point& delta_mouse_pos) override;
	void onRightMouseUp(const Point& delta_mouse_pos) override;
private:
	float m_upward = 0.0f;
	float m_forward = 0.0f;
	float m_rightward = 0.0f;
	bool mouseDown = false;
	bool isMoving = false;
	const float NAVIGATE_SPEED = 2.5f;
	Matrix4x4 m_view_cam;
	AppWindow* appWindow;
};

