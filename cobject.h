#pragma once

#include "framework.h"
#define DegreeToRadian(degree) ((degree) * PI / 180)
#define PI 3.141592653589793
#define HALF 16383.5
#define MAX 32767.0

class CObject
{
protected:
	double cx, cy;
	double speed;
	double va, vb;
public:
	CObject(POINT p);
	~CObject();
	void Update();
	virtual void Draw(HDC hdc) = 0;
	virtual bool Collision() = 0;
	//SetPosition();
};

class CCircle : public CObject
{
private :
	double radius;
public:
	CCircle(POINT p);
	void Draw(HDC hdc) override;
	bool Collision() override;
};

class CStar : public CObject
{
private:
	double radius;
public:
	CStar(POINT p);
	void Draw(HDC hdc) override;
	bool Collision() override;
};

class CRectangle : public CObject
{
private:
	double width, height;
public:
	CRectangle(POINT p);
	void Draw(HDC hdc) override;
	bool Collision() override;
};

