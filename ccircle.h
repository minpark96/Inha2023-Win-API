#pragma once

#include "framework.h"

#define DegreeToRadian(degree) ((degree) * PI / 180)
#define PI 3.141592653589793
#define HALF 16383.5
#define MAX 32767.0

class CCircle
{
private:
	double cx, cy;
	double vx, vy;
	double radius;
public:
	CCircle();
	~CCircle();

	void Update(RECT* rectView);
	void Draw(HDC hdc);
	double distance(const double x, const double y) const;

	double GetCX() const;
	double GetCY() const;
	double GetRadius() const;
	void SetPosition(CCircle& other);
};

