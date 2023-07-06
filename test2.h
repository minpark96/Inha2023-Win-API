#pragma once
#include "test.h"

class CCircle : public CObject
{
private:
	double radius;

public:
	CCircle();
	CCircle(POINT center, double radius);
	~CCircle();
	void Update(RECT* rectView) override;
	void Draw(HDC hdc) override;
	BOOL Collision() override;
};

