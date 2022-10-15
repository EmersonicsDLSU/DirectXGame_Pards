#pragma once

class Vector3D
{
public:
	// default constructor
	Vector3D() :m_x(0), m_y(0), m_z(0)
	{
	}
	// modified valued vector
	Vector3D(float x, float y, float z) :m_x(x), m_y(y), m_z(z)
	{
	}
	// deep-copy of values
	Vector3D(const Vector3D& vector) :m_x(vector.m_x), m_y(vector.m_y), m_z(vector.m_z)
	{
	}
	// create a lerp function for the vector data type
	static Vector3D lerp(const Vector3D& start, const Vector3D& end, float delta)
	{
		Vector3D v;
		v.m_x = start.m_x * (1.0f - delta) + end.m_x * (delta);
		v.m_y = start.m_y * (1.0f - delta) + end.m_y * (delta);
		v.m_z = start.m_z * (1.0f - delta) + end.m_z * (delta);
		return v;
	}

	static Vector3D one()
	{
		Vector3D v;
		v.m_x = 1;
		v.m_y = 1;
		v.m_z = 1;
		return v;
	}

	static Vector3D zero()
	{
		Vector3D v;
		v.m_x = 0;
		v.m_y = 0;
		v.m_z = 0;
		return v;
	}

	Vector3D operator *(float num)
	{
		return Vector3D(m_x * num, m_y * num, m_z * num);
	}

	Vector3D operator +(Vector3D vec)
	{
		return Vector3D(m_x + vec.m_x, m_y + vec.m_y, m_z + vec.m_z);
	}

	~Vector3D()
	{
	}

public:
	// representation of the vector structure
	float m_x, m_y, m_z;
};