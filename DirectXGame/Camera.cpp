#include "Camera.h"

#include "EngineTime.h"
#include "InputSystem.h"

Camera::Camera(std::string name, ObjectTypes type) : AGameObject(name, type)
{

}

Camera::~Camera()
{
}

void Camera::Update(float deltaTime, AppWindow* app_window)
{
}

Matrix4x4 Camera::GetCamViewMatrix()
{
	return m_matrix;
}

void Camera::UpdateViewMatrix()
{
	// creating the camera matrix
	Matrix4x4 world_cam;
	world_cam.setIdentity();
	Matrix4x4 temp;
	temp.setIdentity();
	// set the transform rotation X of the object
	temp.setIdentity();
	temp.setRotationX(GetLocalRotation().m_x);
	// make the object relative to the camera
	world_cam *= temp;
	// set the transform rotation Y of the object
	temp.setIdentity();
	temp.setRotationY(GetLocalRotation().m_y);
	// make the object relative to the camera
	world_cam *= temp;
	
	// convert camera matrix to view matrix
	world_cam.inverse();
	m_matrix = world_cam;
}

void Camera::onKeyDown(int key)
{
	Vector3D localPos = this->GetLocalPosition();
	float x = localPos.m_x;
	float y = localPos.m_y;
	float z = localPos.m_z;
	if (key == 'W')
	{
		if (this->mouseDown) {
			y += EngineTime::getDeltaTime() * NAVIGATE_SPEED;
		}
		else {
			z += EngineTime::getDeltaTime() * NAVIGATE_SPEED;
		}
		SetPosition(x, y, z);
		m_forward = 1.0f * EngineTime::getDeltaTime() * NAVIGATE_SPEED;
		UpdateViewMatrix();
	}
	else if (key == 'S')
	{
		//m_rot_x -= 0.707f * m_delta_time;
		m_forward = -1.0f * EngineTime::getDeltaTime() * NAVIGATE_SPEED;
	}
	else if (key == 'A')
	{
		//m_rot_y += 0.707f * m_delta_time;
		m_rightward = -1.0f * EngineTime::getDeltaTime() * NAVIGATE_SPEED;
	}
	else if (key == 'D')
	{
		//m_rot_y -= 0.707f * m_delta_time;
		m_rightward = 1.0f * EngineTime::getDeltaTime() * NAVIGATE_SPEED;
	}
}

void Camera::onKeyUp(int key)
{
}

void Camera::onMouseMove(const Point& mouse_pos)
{
	if (this->mouseDown) {
		Vector3D localRot = GetLocalRotation();
		float x = localRot.m_x;
		float y = localRot.m_y;
		float z = localRot.m_z;

		float speed = 0.005f;
		x += mouse_pos.m_y * speed;
		y += mouse_pos.m_x * speed;

		SetRotation(x, y, z);
		UpdateViewMatrix();
	}
}

void Camera::onRightMouseDown(const Point& delta_mouse_pos)
{
	mouseDown = true;
}

void Camera::onRightMouseUp(const Point& delta_mouse_pos)
{
	mouseDown = false;
}
