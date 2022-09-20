#pragma once

// interface class
class InputListener
{
public:
	InputListener(){}
	~InputListener(){}

	// KEYBOARD; pure virtual callback functions
	virtual void onKeyDown(int key) = 0;
	virtual void onKeyUp(int key) = 0;
};