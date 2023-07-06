#pragma once
#include "framework.h"

class CObject
{
protected:
	POINT center;
	double speed;
	double xdirection;
	double ydirection;
	int type;

public:
	virtual void Update(RECT* rectView) = 0;
	virtual void Draw(HDC hdc) = 0;
	virtual BOOL Collision() = 0;
};

