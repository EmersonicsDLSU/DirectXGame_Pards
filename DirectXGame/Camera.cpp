#include "Camera.h"

#include "AppWindow.h"
#include "EngineTime.h"
#include "InputSystem.h"

Camera::Camera(std::string name, ObjectTypes type, AppWindow* app_window) : AGameObject(name, type)
{
	this->appWindow = app_window;
	m_view_cam.setTranslation(Vector3D{ 0.0f, 0.0f, 0.0f });
	//this->worldCameraMatrix.setTranslation(this->getLocalPosition());
	this->UpdateViewMatrix();
	// subscribe this class to the InputSystem
	InputSystem::get()->addListener(this);
}

Camera::~Camera()
{
	InputSystem::get()->removeListener(this);
}

void Camera::Update(float deltaTime, AppWindow* app_window)
{
}

Matrix4x4 Camera::GetCamViewMatrix()
{
	return m_view_cam;
}

void Camera::UpdateViewMatrix()
{
	Matrix4x4 world_cam; world_cam.setIdentity();
	Matrix4x4 temp; temp.setIdentity();

	Vector3D localRot = this->GetLocalRotation();

	temp.setRotationX(localRot.m_x);
	world_cam = world_cam.MultiplyTo(temp);

	temp.setRotationY(localRot.m_y);
	world_cam = world_cam.MultiplyTo(temp);

	//world_cam.setTranslation(this->GetLocalPosition());
	//world_cam = world_cam.MultiplyTo(temp);

	// moving or setting the camera position in the z or x axis
	Vector3D new_pos = m_matrix.getTranslation() + world_cam.getZDirection() * (m_forward * 1.0f);
	new_pos = new_pos + world_cam.getXDirection() * (m_rightward * 1.0f);
	temp.setTranslation(new_pos);
	world_cam = world_cam.MultiplyTo(temp);
	m_matrix = world_cam;

	// convert camera matrix to view matrix
	world_cam.inverse();
	m_view_cam = world_cam;

}

void Camera::onKeyDown(int key)
{
	Vector3D localPos = this->GetLocalPosition();
	float x = localPos.m_x;
	float y = localPos.m_y;
	float z = localPos.m_z;
	if (key == 'W')
	{
		z += EngineTime::getDeltaTime() * NAVIGATE_SPEED;
		SetPosition(x, y, z);
		UpdateViewMatrix();
		m_forward = 1.0f * EngineTime::getDeltaTime() * NAVIGATE_SPEED;
	}
	else if (key == 'S')
	{
		z -= EngineTime::getDeltaTime() * NAVIGATE_SPEED;
		SetPosition(x, y, z);
		UpdateViewMatrix();
		m_forward = -1.0f * EngineTime::getDeltaTime() * NAVIGATE_SPEED;
	}
	else if (key == 'A')
	{
		x -= EngineTime::getDeltaTime() * NAVIGATE_SPEED;
		SetPosition(x, y, z);
		UpdateViewMatrix();
		m_rightward = -1.0f * EngineTime::getDeltaTime() * NAVIGATE_SPEED;
	}
	else if (key == 'D')
	{
		x += EngineTime::getDeltaTime() * NAVIGATE_SPEED;
		SetPosition(x, y, z);
		UpdateViewMatrix();
		m_rightward = 1.0f * EngineTime::getDeltaTime() * NAVIGATE_SPEED;
	}
}

void Camera::onKeyUp(int key)
{
	m_forward = 0.0f;
	m_rightward = 0.0f;
}

void Camera::onMouseMove(const Point& mouse_pos)
{
	if (this->mouseDown) {
		// width and height of the screen
		int width = (appWindow->getClientWindowRect().right - appWindow->getClientWindowRect().left);
		int height = (appWindow->getClientWindowRect().bottom - appWindow->getClientWindowRect().top);
		
		float x = GetLocalRotation().m_x;
		float y = GetLocalRotation().m_y;
		float z = GetLocalRotation().m_z;

		float speed = 0.5f;
		x += (mouse_pos.m_y - (height / 2.0f)) * EngineTime::getDeltaTime() * speed;
		y += (mouse_pos.m_x - (width / 2.0f)) * EngineTime::getDeltaTime() * speed;
		
		InputSystem::get()->setCursorPosition(Point((int)(width / 2.0f), (int)(height / 2.0f)));

		
		SetRotation(x, y, z);
		UpdateViewMatrix();
	}
}

void Camera::onLeftMouseDown(const Point& delta_mouse_pos)
{
}

void Camera::onLeftMouseUp(const Point& delta_mouse_pos)
{
}

void Camera::onRightMouseDown(const Point& delta_mouse_pos)
{
	mouseDown = true;
}

void Camera::onRightMouseUp(const Point& delta_mouse_pos)
{
	mouseDown = false;
}
