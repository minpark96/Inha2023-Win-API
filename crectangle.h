#pragma once

#include "framework.h"

#define DegreeToRadian(degree) ((degree) * PI / 180)
#define PI 3.141592653589793

class CRectangle
{
private:
	double cx, cy;
	double vx;
	double width, height;
	double left, right, top, bottom;
	int hp;
	COLORREF color;
public:
	CRectangle();
	CRectangle(double cx, double cy, int hp);
	~CRectangle();

	void Draw(HDC hdc);
	void Update(RECT* rectview);
	bool Collision(const CCircle& cir) const;

	double GetWidth() const;
	double GetHeight() const;
	COLORREF GetColor() const;
};

