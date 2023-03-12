#ifndef _CAMERAPOSITION_H_
#define _CAMERAPOSITION_H_

#include <math.h>

class CameraPosition
{
public:
	CameraPosition();
	CameraPosition(const CameraPosition&);
	~CameraPosition();

	void SetFrameRate(float);
	void GetRotation(float&);

	void TurnLeft(bool);
	void TurnRight(bool);

private:
	float frameTime;
	float rotationY;
	float leftTurnSpeed, rightTurnSpeed;
};

#endif
