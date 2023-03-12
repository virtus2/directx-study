#include "CameraPosition.h"

CameraPosition::CameraPosition()
{
	frameTime = 0.0f;
	rotationY = 0.0f;
	leftTurnSpeed = 0.0f;
	rightTurnSpeed = 0.0f;
}

CameraPosition::CameraPosition(const CameraPosition&)
{
}

CameraPosition::~CameraPosition()
{
}

void CameraPosition::SetFrameRate(float time)
{
	frameTime = time;
}

void CameraPosition::GetRotation(float& y)
{
	y = rotationY;
}

void CameraPosition::TurnLeft(bool keydown)
{
	if(keydown)
	{
		leftTurnSpeed += frameTime * 0.01f;
		if(leftTurnSpeed > frameTime * 0.15f)
		{
			leftTurnSpeed = frameTime * 0.15f;
		}
	}
	else
	{
		leftTurnSpeed -= frameTime * 0.005f;
		if (leftTurnSpeed < 0.0f)
		{
			leftTurnSpeed = 0.0f;
		}
	}

	rotationY -= leftTurnSpeed;
	if(rotationY < 0.0f)
	{
		rotationY += 360.0f;
	}
}

void CameraPosition::TurnRight(bool keydown)
{
	if (keydown)
	{
		rightTurnSpeed += frameTime * 0.01f;
		if (rightTurnSpeed > frameTime * 0.15f)
		{
			rightTurnSpeed = frameTime * 0.15f;
		}
	}
	else
	{
		rightTurnSpeed -= frameTime * 0.005f;
		if (rightTurnSpeed < 0.0f)
		{
			rightTurnSpeed = 0.0f;
		}
	}

	rotationY += rightTurnSpeed;
	if (rotationY < 0.0f)
	{
		rotationY -= 360.0f;
	}
}
